/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:12:18 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/13 14:12:21 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
#define QUERY_STRING(env) std::string(std::string("QUERY_STRING=") + env).c_str()
#define PATH_INFO(path) std::string(std::string("PATH_INFO=") + path).c_str()
#define	PHP "/usr/bin/php"
#define	BASH "/bin/bash"
#define PYTHON "/usr/bin/python3"
#define CGI_TIMEOUT 1

Cgi::Cgi(TreeNode<t_node> *loc, t_cgi &cgi, std::string env) {
	this->loc = loc;
	this->cgi = cgi;
	this->path = loc->getData().cgiBin + "/" + this->cgi.eName;
	this->pathInfo = "";
	this->formatEnv(env);
	this->pid = 0;
	this->errorCode = 200;
}

Cgi::Cgi(Cgi const &cgi) {
	*this = cgi;	
}

Cgi	&Cgi::operator=(Cgi const &cgi) {
	if (this == &cgi)
		return (*this);
	this->deleteMat((void **) this->newEnv);
	this->newEnv = cgi.newEnv;
	this->pathInfo = cgi.pathInfo;
	this->loc = cgi.loc;
	this->cgi = cgi.cgi;
	this->pid = cgi.pid;
	this->errorCode = cgi.errorCode;
	return (*this);
}

Cgi::~Cgi(void) {;}

void	Cgi::formatEnv(std::string env) {
	size_t						i = 0;

	this->newEnv = new char*[Server::envp.size() + 3];
	for (; i < Server::envp.size(); i++)
		this->newEnv[i] = Server::envp[i];
	this->newEnv[i] = strdup(QUERY_STRING(env));
	this->newEnv[i + 1] = strdup(PATH_INFO(this->pathInfo));
	this->newEnv[i + 2] = NULL;
}

void	Cgi::deleteMat(void **mat) {
	for (int i = 0; mat[i]; i++) {
		free(mat[i]);
	}
	delete [] mat;
}

int	const 	&Cgi::getErrorCode(void) const {
	return (this->errorCode);
}

void	Cgi::setPathInfo(std::string pathInfo) {
	this->pathInfo = pathInfo;
}

long	Cgi::timeout(int &wstatus) {
	long	time;
	int		ret;

	time = Utils::ft_gettime();
	while (Utils::ft_gettime() - time < CGI_TIMEOUT) {
		ret = waitpid(this->pid, &wstatus, WNOHANG);
		if (ret == -1) {
			this->errorCode = 177;
			break ;
		}
		else if (ret && WIFEXITED(wstatus)) {
			wstatus = WEXITSTATUS(wstatus);
			time = -1;
			break ;
		}
	}
	return (time);
}

std::string	Cgi::execCgi(void) {
	int							fds[2];
	char						buff[1];
	std::stringstream			output;
	int							wstatus = 0;

	DEBUG(PURPLE + "executing cgi" + RESET);
	pipe(fds);
	this->launchCgi(fds);
	if (this->timeout(wstatus) != -1) {
		DEBUG(RED + "Server error" + RESET);
		this->errorCode = 500;
	}
	else if (!wstatus) {
		DEBUG(RED + "nothing to report" + RESET);
		while (read(fds[0], buff, 1) == 1)
			output << *buff;
		close(fds[0]);
		return (output.str());
	}
	else if (wstatus == 177 || wstatus == 2) {
		DEBUG(RED + "404" + RESET);
		this->errorCode = 404;
	}
	else {
		DEBUG(RED + "500" + RESET);
		this->errorCode = 500;
	}
	//this->response = this->generateError(loc->getData().errPages);
	return ("");
}

void	Cgi::launchCgi(int *fds) {
	char	**argv;

	this->pid = fork();
	if (!this->pid) {
		dup2(fds[1], 1);
		close(fds[0]);
		if (this->cgi.type == "exe") {
			argv = new char*[2];
			if (this->path.rfind("/") == NPOS)
				argv[0] = strdup(this->path.c_str());
			else
				argv[0] = strdup(this->path.substr(this->path.rfind("/") + 1).c_str());
			argv[1] = NULL;
			execve(this->path.c_str(), argv, this->newEnv);
			this->deleteMat((void **) argv);
			this->deleteMat((void **) this->newEnv);
			exit(177);
		}
		argv = new char*[3];
		argv[0] = strdup(this->cgi.type.c_str());
		argv[1] = strdup(this->path.c_str());
		argv[2] = NULL;
		if (cgi.type == "bash")
			execve(BASH, argv, this->newEnv);
		else if (cgi.type == "php")
			execve(PHP, argv, this->newEnv);
		else
			execve(PYTHON, argv, this->newEnv);
		this->deleteMat((void **) argv);
		this->deleteMat((void **) this->newEnv);
		exit(177);
	}
	std::cout << RED << "pid_t: " << this->pid << RESET << std::endl;
	close(fds[1]);
}
