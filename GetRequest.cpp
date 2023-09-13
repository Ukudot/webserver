/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:12:56 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/13 09:37:10 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetRequest.hpp"

#define	PHP "/usr/bin/php"
#define	BASH "/bin/bash"
#define PYTHON "/usr/bin/python3"
#define CGI_TIMEOUT 1

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
	DEBUG(RED + "path created" + RESET);
	this->env = line[1].substr(this->path.length());
	if (this->env[0] == '?')
		this->env = this->env.substr(1);
	DEBUG(RED + "env created" + RESET);
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
	DEBUG(GREEN + "tmpPath: " + tmpPath + RESET);
	// cgis && reds
	if (this->cgi(loc, tmpPath) || this->reds(config, loc, tmpPath))
		return ;
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

bool	GetRequest::reds(TreeNode<t_node> *config, TreeNode<t_node> *loc, std::string tmpPath) {
	for (size_t i = 0; i < loc->getData().redirections.size(); i++) {
		if (tmpPath == loc->getData().redirections[i].src) {
			this->errorCode = loc->getData().redirections[i].type == 'r' ? 302 : 301;
			this->response = this->generateError(config->getData().errPages, loc->getData().redirections[i].dst);
			return (true);
		}
	}
	return (false);
}

bool	GetRequest::cgi(TreeNode<t_node> *loc, std::string tmpPath) {
	std::string	cgiName;
	size_t		cgiPos;
	Cgi			*cgi;
	std::string	output;

	//cgiName = this->path.substr(this->path.rfind("/") + 1);
	
	for (std::vector<t_cgi>::iterator ite = loc->getData().cgis.begin(); ite != loc->getData().cgis.end(); ite++) {
		DEBUG(PURPLE + "cgiName: " + cgiName + RESET);
		DEBUG(PURPLE + "tmpPath: " + tmpPath + RESET);
		DEBUG(PURPLE + "path: " + this->path + RESET);
		DEBUG(PURPLE + "eName: " + (*ite).eName + RESET);
		//if ((*ite).eName == cgiName) {
		cgiPos = this->path.find((*ite).eName);
		std::cout << ">> cgiPos: " << cgiPos << std::endl;
		if (cgiPos == 1) {
			DEBUG(RED + "cgi found" + RESET);
			DEBUG(RED + "cgi path: " + this->path + RESET);
			DEBUG(RED + "cgi string query: " + this->env + RESET);
			cgi = new Cgi(loc, (*ite), this->env);
			cgi->setPathInfo(this->path.substr((*ite).eName.length() + 1, this->path.find("?") - (*ite).eName.length()));
			output = cgi->execCgi();
			DEBUG(GREEN + "cgi output: " + output + RESET);
			std::cout << GREEN << "cgi errorCode: " << cgi->getErrorCode() << RESET << std::endl;
			if (output == "") {
				this->errorCode = cgi->getErrorCode();
				this->response = this->generateError(loc->getData().errPages);
			}
			else {
				this->response = this->generateHeader(output.length(), "") + output;

			}
			return (true);
		}
	}
	DEBUG(RED + "cgi not found" + RESET);
	return (false);
}
