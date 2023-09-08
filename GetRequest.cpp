/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:12:56 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/08 10:52:51 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetRequest.hpp"

GetRequest::GetRequest(Connection *conn): ARequest(conn, "GET") {}

GetRequest::~GetRequest(void) {}

GetRequest::GetRequest(GetRequest const &req): ARequest(req) {
	*this = req;
}

GetRequest	&GetRequest::operator=(GetRequest const &req) {
	if (this == &req)
		return (*this);
	this->conn = req.conn;
	this->type = req.type;
	this->path = req.path;
	this->env = req.env;
	this->host = req.host;
	return (*this);
}

void	GetRequest::getInfo(void) {
	std::vector<std::string>			line;
	std::vector<std::string>::iterator	ite;

	line = Utils::ft_split(this->lines[0], " ");
	this->path = line[1].substr(0, line[1].find("?"));
	this->env = line[1].substr(this->path.length() + 1);
	this->errorCode = 400;
	for (ite = this->lines.begin(); ite != this->lines.end(); ite++) {
		if ((*ite).substr(0, (*ite).find(":")) == "Host") {
			this->errorCode = 200;
			DEBUG(PURPLE + "Host found" + RESET);
			break ;
		}
	}
}

void	GetRequest::createRes(TreeNode<t_node> *config) {
	std::stringstream	html;
	TreeNode<t_node>	*loc;
	std::string			tmpPath = this->path;

	if (this->errorCode != 200) {
		this->response = this->generateError(config->getData().errPages);
		return ;
	}
	loc = this->findLocation(config);
	if (loc->getName() != "")
		tmpPath = tmpPath.substr(loc->getName().length());
	if (this->env != "") {
		std::string	newEnv;

		newEnv = Server::env + this->varPars(this->env);

		
	} // todo cgi
	for (size_t i = 0; i < loc->getData().redirections.size(); i++) {
		if (tmpPath == loc->getData().redirections[i].src) {
			this->errorCode = loc->getData().redirections[i].type == 'r' ? 302 : 301;
			this->response = this->generateError(config->getData().errPages, loc->getData().redirections[i].dst);
			return ;
		}
	}
	if (loc->getData().root == "") {
		this->errorCode = 404;
		this->response = this->generateError();
		return ;
	}
	tmpPath = loc->getData().root + tmpPath;
	try {
		html << Utils::ft_readFile(tmpPath);
	}
	catch (ErrException &e) {
		this->errorCode = 404;
		this->response = this->generateError(loc->getData().errPages);
		return ;
	}
	this->response = generateHeader(html.str().length(), "") + html.str();
	DEBUG(CYAN + "response created" + RESET);
}
