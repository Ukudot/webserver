/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:12:56 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/09 15:05:57 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <vector>
#include <algorithm>
#include "ARequest.hpp"
#include "Server.hpp"

#define CGI_TIMEOUT 1
#define	PHP "/usr/bin/php"
#define	BASH "/bin/bash"
#define PYTHON "/usr/bin/python3"
// autoindex
#define AUTOIN_FONT "h3"
#define AUTOIN_HEAD "<!DOCTYPE html>\n<html>\n<head>\n</head>\n"
#define AUTOIN_BODY "<body>\n<h1>Autoindex</h1>\n<" << AUTOIN_FONT << ">\n"
#define AUTOIN_LINK(url, name, css) "<a href=\"http://" << url << "\" style=\"" << css << "\">" << name << "</a><br>\n"
#define AUTOIN_DIR(url, name) AUTOIN_LINK(url + "/", name, "color:blue; text-decoration:none;")
#define AUTOIN_FILE(url, name) AUTOIN_LINK(url, name, "color:black; text-decoration:none;")
#define AUTOIN_FOOTER "</" << AUTOIN_FONT << ">\n</body>\n</html>"

typedef struct s_file {
    std::string     name;
    unsigned char   type;
    s_file() {
        this->name = "";
        this->type = 0;
    }
    s_file(std::string name, unsigned char type): name(name), type(type) {;}
}   t_file;

class	GetRequest: public ARequest {
	private:

		void	execCgi(TreeNode<t_node> *loc, t_cgi &cgi, std::string path);
		void	doAutoindex(TreeNode<t_node> *loc, std::string path);

	public:

		GetRequest(Connection *conn);
		~GetRequest(void);
		GetRequest(GetRequest const &req);
		GetRequest	&operator=(GetRequest const &req);

		virtual void	getInfo(void);
		virtual void	createRes(TreeNode<t_node> *config);
};
