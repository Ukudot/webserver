/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 10:05:23 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/11 15:43:36 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::vector<Server *>	Server::servers;

std::vector<char *>	Server::envp;

Server::Server(TreeNode<t_node>	*config): config(config) {
	std::map<int, std::string>::iterator	ite;

	for (ite = this->config->getData().ports.begin(); ite != this->config->getData().ports.end(); ite++) {
		this->sockets.push_back(new ServSocket(ite->second, ite->first));
	}
	if (!std::distance(this->config->getData().ports.begin(), ite))
		this->sockets.push_back(new ServSocket());
	Server::servers.push_back(this);
	DEBUG(GREY + "server created" + RESET);
}

Server::~Server(void) {
	delete config;
	for (std::vector<ServSocket *>::iterator ite = this->sockets.begin(); ite != this->sockets.end(); ite++)
		delete *ite;
	this->sockets.clear();
	for (std::vector<Server *>::iterator begin = Server::servers.begin(); begin != Server::servers.end(); begin++)
		if (this == *begin) {
			Server::servers.erase(begin);
			break ;
		}
}

Server::Server(Server const &serv) {
	*this = serv;
}

Server	&Server::operator=(Server const &serv) {
	if (this == &serv)
		return (*this);
	this->servers = serv.servers;
	this->sockets = serv.sockets;
	this->config = serv.config;
	return (*this);
}

void	Server::interpret(void) {
	std::vector<ServSocket *>::iterator	ite;
	std::vector<Connection *>::iterator	iteCon;
	std::vector<Connection *>			conns;
	std::vector<ARequest *>				reqs;
	ARequest							*request;

	for (ite = this->sockets.begin(); ite != this->sockets.end(); ite++) {
		if (!(*ite)->spoll())
			continue ;
		conns = (*ite)->getConns(true);

		for (iteCon = conns.begin(); iteCon != conns.end(); iteCon++) {
			// gestire richieste post
			request = this->readConn((*iteCon));
			if (!request)
				continue ;
			//if (request->getType() == "post")
			//	this->reqs.push_back();
			request->getInfo();
			DEBUG(GREY + "info completed" + RESET);
			request->createRes(this->config);
			DEBUG(GREY + "create completed" + RESET);
			request->sendRes();
			delete request;
			DEBUG(GREY + "richiesta evasa" + RESET);
		}
		(*ite)->pushBuffers();
	}
}

void	Server::polls(void) {
	std::vector<Server *>::iterator	ite;

	for (ite = Server::servers.begin(); ite != Server::servers.end(); ite++) {
		try {
			(*ite)->interpret();
			// DEBUG("...interpreted");
		}
		catch (std::exception &e) {
			std::cout << e.what() << std::endl;
		}
	}
}

ARequest	*Server::readConn(Connection *conn) {
	ARequest 	*request;
	std::string	req;
	std::string	tmp;

	req = conn->getReadBuff();
	while (req.find(CRLF) == 0)
		req = req.substr(2);
	conn->setReadBuff(req);
	if (req.find(CRLF + CRLF) == NPOS)
		return (NULL);
	req = req.substr(0, req.find(CRLF + CRLF));
	DEBUG(GREEN + "\n" + req + RESET);
	tmp = req.substr(0, req.find(CRLF));
	for (int i = 0; i < 2; i++) {
		if (tmp.find(" ") == NPOS) {
			request = new GetRequest(conn);
			request->setErrorCode(400);
			DEBUG(RED + "space missing" + RESET);
			return (request);
		}
		tmp = tmp.substr(tmp.find(" ") + 1);
	}
	if (tmp.find(" ") != NPOS || tmp.find("HTTP/") == NPOS) {
		request = new GetRequest(conn);
		request->setErrorCode(400);
		DEBUG(RED + "too many spaces" + RESET);
	}
	else if (tmp.find("HTTP/1.1") == NPOS) {
		request = new GetRequest(conn);
		request->setErrorCode(505);
		DEBUG(RED + "mbad version" + RESET);
	}
	else if (req.find("GET") == 0)
		request = new GetRequest(conn);
	else if (req.find("POST") == 0)
		; //request = new PostRequest(conn);
	else if (req.find("DELETE") == 0)
		; //request = new DeleteRequest(conn);
	else {
		request = new GetRequest(conn);
		request->setErrorCode(501);
	}
	DEBUG(PURPLE + "specific request created" + RESET);
	return (request);
}

