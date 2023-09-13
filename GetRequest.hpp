/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:12:56 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/11 14:06:20 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include "ARequest.hpp"
#include "Cgi.hpp"
#include "Server.hpp"

#define CGI_TIMEOUT 1
#define	PHP "/usr/bin/php"
#define	BASH "/bin/bash"
#define PYTHON "/usr/bin/python3"
// autoindex
#define AUTOIN_HEAD "<!DOCTYPE html>\n<html>\n\t<head>\n\t</head>\n"
#define AUTOIN_BODY(path) "\t<body>\n\t\t<h1 style=\"font-family:arial;\"> Index of " << path << "</h1>\n\t\t<hr>\n\t\t<div style=\"display:flex; flex-direction:column; font-family:arial;\">\n"
#define AUTOIN_CAT "\t\t\t<div style=\"display:flex; font-weight:900; color:black; justify-content:start\">\n\t\t\t\t<span style=\"width:15vw;\">" << "FILE NAME" << "</span>\n\t\t\t\t<span style=\"width: 15vw;\">"<< "FILE SIZE" <<"</span>\n\t\t\t\t<span style=\"width: 15vw;\">"<< "LAST MODIFICATION" <<"</span>\n\t\t\t</div>\n"
#define AUTOIN_LINK(url, name, css) std::string("<a href=\"http://") + url + "\" style=\"width:15vw; " + css + "\">" + name + "</a>"
#define AUTOIN_LINE(link, dim, date) "\t\t\t<div style=\"display:flex; justify-content:start\">\n\t\t\t\t" << link << "\n\t\t\t\t<span style=\"width: 15vw;\">" << dim << "</span>\n\t\t\t\t<span style=\"width: 15vw;\">" << date << "</span>\n\t\t\t</div>\n"
#define AUTOIN_DIR(url, name) AUTOIN_LINK(url + "/", name, "color:blue;")
#define AUTOIN_FILE(url, name) AUTOIN_LINK(url, name, "color:black; text-decoration:none;")
#define AUTOIN_FOOTER "\t\t</div>\n\t\t<hr>\n\t</body>\n</html>"

typedef struct s_file {
    std::string     name;
    unsigned char   type;
	std::string		date;
	size_t			dim;
    s_file() {
        this->name = "";
        this->type = 0;
    }
    s_file(std::string name, unsigned char type): name(name), type(type) {;}
}   t_file;

class	GetRequest: public ARequest {
	private:

		void	doAutoindex(TreeNode<t_node> *loc, std::string path);
		char	**updateEnvp(void);
		bool	reds(TreeNode<t_node> *config, TreeNode<t_node> *loc, std::string tmpPath);
		bool	cgi(TreeNode<t_node> *loc, std::string tmpPath);

		static bool	cmp(t_file const &f1, t_file const &f2);
		static bool	getFileInfo(std::string path, t_file &file);

	public:

		GetRequest(Connection *conn);
		~GetRequest(void);
		GetRequest(GetRequest const &req);
		GetRequest	&operator=(GetRequest const &req);

		virtual void	getInfo(void);
		virtual void	createRes(TreeNode<t_node> *config);
};
