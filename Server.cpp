/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 10:05:23 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/04 14:42:51 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::vector<Server *>	Server::servers;

Server::Server(TreeNode<t_node>	*config): config(config) {
	std::map<int, std::string>::const_iterator	ite;

	for (ite = this->config->getData().ports.begin(); ite != this->config->getData().ports.end(); ite++)
		this->sockets.push_back(new ServSocket(ite->second, ite->first));
	if (!std::distance(this->config->getData().ports.begin(), ite))
		this->sockets.push_back(new ServSocket());
	Server::servers.push_back(this);
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

	for (ite = this->sockets.begin(); ite != this->socket.end(); ite++) {

	}
}

void	Server::polls(void) {
	std::vector<Server *>::iterator	ite;

	for (ite = Server::servers.begin(); ite != Server::servers.end(); ite++) {
		try {
			(*ite)->interpret();
		}
		catch (std::exception &e) {
			std::cout << e.what() << std::endl;
		}
	}
}
