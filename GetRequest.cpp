/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:12:56 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/11 14:54:01 by adi-stef         ###   ########.fr       */
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
	DEBUG(RED + "path: |" + this->path + "|" + RESET);
	this->env = line[1].substr(this->path.length());
	if (this->env[0] == '?')
		this->env = this->env.substr(1);
	this->errorCode = 400;
	for (ite = this->lines.begin(); ite != this->lines.end(); ite++) {
		if ((*ite).substr(0, (*ite).find(":")) == "Host") {
			this->errorCode = 200;
			this->host = (*ite).substr((*ite).find(" ") + 1);
			DEBUG(PURPLE + "host: |" + this->host + "|" + RESET);
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
		cgiName = cgiName[0] == '/' ? cgiName.substr(1) : cgiName;
		for (std::vector<t_cgi>::iterator ite = loc->getData().cgis.begin(); ite != loc->getData().cgis.end(); ite++) {
			if ((*ite).eName == cgiName) {
				DEBUG(RED + "cgi name: " + cgiName + RESET);
				this->execCgi(loc, (*ite), loc->getData().cgiBin + tmpPath);
				return ;
			}
		}
	}

	for (size_t i = 0; i < loc->getData().redirections.size(); i++) {
		if (tmpPath == loc->getData().redirections[i].src) {
			this->errorCode = loc->getData().redirections[i].type == 'r' ? 302 : 301;
			this->response = this->generateError(config->getData().errPages, loc->getData().redirections[i].dst);
			DEBUG(RED + "rewrite done to: " + loc->getData().redirections[i].dst + RESET);
			return ;
		}
	}
	if (loc->getData().root == "") {
		this->errorCode = 404;
		this->response = this->generateError();
		return ;
	}
	if (opendir((loc->getData().root + tmpPath).c_str()) && loc->getData().autoindex) { // needs review
		if (tmpPath[tmpPath.size() - 1] != '/')
			tmpPath += "/";
		this->doAutoindex(loc, tmpPath);
		DEBUG(RED + "autoindex done" + RESET);
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
	DEBUG(CYAN + "GET response created" + RESET);
}

bool	GetRequest::cmp(t_file const &f1, t_file const &f2) {
   return (f1.name.compare(f2.name) < 0);
}

bool	GetRequest::getFileInfo(std::string path, t_file &file) {
	struct stat			data;
	struct tm			*lt = NULL;
	std::stringstream	date;

	if (stat(path.c_str(), &data))
		return (false);
	file.dim = (size_t) data.st_size;
	lt = localtime(&(data.st_mtim.tv_sec));
	date << lt->tm_mday << " " << lt->tm_mon << " " << (lt->tm_year + 1900);
	date << " " << lt->tm_hour << ":" << lt->tm_min << ":" << lt->tm_sec;
	file.date = date.str();
	return (true);
}

void	GetRequest::doAutoindex(TreeNode<t_node> *loc, std::string path) {
	DIR					*dir;
	struct dirent       *entry;
    std::vector<t_file> files;
	std::stringstream   html;
	std::string			url;

	if ((dir = opendir((loc->getData().root + path).c_str())) == NULL) { // trys to open the folder
		this->errorCode = 404;
		this->response = this->generateError(loc->getData().errPages);
		return ;
	}
	DEBUG(BLUE + path + RESET);
	while ((entry = readdir(dir)) != NULL)
		if ((entry->d_type == DT_DIR || entry->d_type == DT_REG) // gets only directories or files
			&& (entry->d_name[0] != '.' || !entry->d_name[1] // removes all hidden files and folders
			|| (entry->d_name[1] == '.' && !entry->d_name[2])))
			files.push_back(t_file(entry->d_name, entry->d_type));
	closedir(dir);

	std::sort(files.begin(), files.end(), cmp); // sorts the file in ascending order

	html << AUTOIN_HEAD << AUTOIN_BODY (path) << AUTOIN_CAT;
	for (std::vector<t_file>::iterator ite = files.begin(); ite != files.end(); ite++) {
		url = this->host + loc->getName() + path + (*ite).name;
		if (!GetRequest::getFileInfo(loc->getData().root + path + (*ite).name, *ite)) {
			this->errorCode = 500;
			this->response = this->generateError(loc->getData().errPages);
			return ;
		}
        if ((*ite).type == DT_DIR)
            html << AUTOIN_LINE(AUTOIN_DIR(url, (*ite).name), (*ite).dim, (*ite).date);
        else
			html << AUTOIN_LINE(AUTOIN_FILE(url, (*ite).name), (*ite).dim, (*ite).date);
	}
	html << AUTOIN_FOOTER;

	this->response = generateHeader(html.str().length(), "") + html.str();
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


	DEBUG(RED + "executing cgi: " + path + RESET);
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
		std::cout << "time: " << Utils::ft_gettime() - time << std::endl;
	}
	if (time != -1) {
		DEBUG(RED + "Timeout" + RESET);
		this->errorCode = 500;
	}
	else if (!wstatus) {
		while (read(fds[0], buff, 1) == 1)
			output << *buff;
		close(fds[0]);
		this->response = generateHeader(output.str().length(), "") + output.str();
		return ;
	}
	else if (wstatus == 177) {
		DEBUG(RED + "Not Found" + RESET);
		this->errorCode = 404;
	}
	else {
		DEBUG(RED + "Error in exec" + RESET);
		this->errorCode = 500;
	}
	this->response = this->generateError(loc->getData().errPages);
}
