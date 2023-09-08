/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:12:56 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/08 16:51:42 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetRequest.hpp"

#define	PHP "/usr/bin/php"
#define	BASH "/bin/bash"
#define PYTHON "/usr/bin/python3"
#define CGI_TIMEOUT 10

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
	{
		std::string	cgiName;

		cgiName = this->path.substr(this->path.rfind("/"));
		for (std::vector<t_cgi>::iterator ite = loc->getData().cgis.begin(); ite != loc->getData().cgis.end(); ite++) {
			if ((*ite).eName == cgiName) {
				this->execCgi(loc, (*ite), loc->getData().cgiBin + tmpPath);
				return ;
			}
		}
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

void	GetRequest::execCgi(TreeNode<t_node> *loc, t_cgi &cgi, std::string path) {
	pid_t						pid;
	char						**argv;
	char						**envp;
	std::vector<std::string>	splittedEnv;
	size_t						i = 0;
	int							fds[2];
	char						buff[1];
	std::stringstream			output;
	int							wstatus;


	pipe(fds);
	pid = fork();
	if (!pid) {
		dup2(fds[1], 1);
		close(fds[0]);
		{
			splittedEnv = Utils::ft_split(this->env, "&");
			envp = new char*[splittedEnv.size() + Server::envp.size() + 1];
			for (; i < Server::envp.size(); i++)
				envp[i] = Server::envp[i];
			for (size_t j = 0; j < splittedEnv.size(); j++, i++)
				envp[i] = strdup(splittedEnv[j].c_str());
			envp[i] = NULL;
		}
		if (cgi.type == "exe") {
			argv = new char*[2];
			if (path.rfind("/") == NPOS)
				argv[0] = strdup(path.c_str());
			else
				argv[0] = strdup(path.substr(path.rfind("/") + 1).c_str());
			argv[1] = NULL;
			execve(path.c_str(), argv, envp);
			exit(10);
		}
		argv = new char*[3];
		argv[0] = strdup(cgi.type.c_str());
		argv[1] = strdup(path.c_str());
		argv[2] = NULL;
		if (cgi.type == "bash")
			execve(BASH, argv, envp);
		else if (cgi.type == "php")
			execve(PHP, argv, envp);
		else
			execve(PYTHON, argv, envp);
		exit(177);
	}
	close(fds[1]);
	long	time = Utils::ft_gettime();
	while (Utils::ft_gettime() - time < CGI_TIMEOUT) {
		waitpid(pid, &wstatus, WNOHANG);
		if (WIFEXITED(wstatus)) {
			wstatus = WEXITSTATUS(wstatus);
			time = -1;
			break;
		}
	}
	if (time != -1)
		this->errorCode = 500;
	else if (!wstatus) {
		while (read(fds[0], buff, 1) == 1)
			output << *buff;
		close(fds[0]);
		this->response = generateHeader(output.str().length(), "") + output.str();
		return ;
	}
	else if (wstatus == 177)
		this->errorCode = 404;
	else
		this->errorCode = 500;
	this->response = this->generateError(loc->getData().errPages);
}
