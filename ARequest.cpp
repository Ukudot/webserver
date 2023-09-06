/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:44:53 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/06 12:36:01 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ARequest.hpp"
#define CRLF std::string("\n\r")
#define NPOS std::string::npos
#define ERR_200 std::string("errors/error200.html")
#define ERR_201 std::string("errors/error201.html")
#define ERR_301 std::string("errors/error301.html")
#define ERR_302 std::string("errors/error302.html")
#define ERR_400 std::string("errors/error400.html")
#define ERR_404 std::string("errors/error404.html")
#define ERR_405 std::string("errors/error405.html")
#define ERR_411 std::string("errors/error411.html")
#define ERR_413 std::string("errors/error413.html")
#define ERR_500 std::string("errors/error500.html")
#define ERR_501 std::string("errors/error501.html")
#define ERR_505 std::string("errors/error505.html")


std::map<int, std::string>	ARequest::errors = ARequest::initMap();

std::map<int, std::string>	ARequest::initMap(void) {
	std::map<int, std::string>	ret;

	ret[200] = ERR_200;
	ret[201] = ERR_201;
	ret[301] = ERR_301;
	ret[302] = ERR_302;
	ret[400] = ERR_400;
	ret[404] = ERR_404;
	ret[405] = ERR_405;
	ret[411] = ERR_411;
	ret[413] = ERR_413;
	ret[500] = ERR_500;
	ret[501] = ERR_501;
	ret[505] = ERR_505;
	return (ret);
}

ARequest::ARequest(Connection *conn, std::string type): conn(conn), type(type) {
	this->lines = ft_split(this->conn->getReadBuff(), CRLF);
	this->errorCode = 200;
}

ARequest::~ARequest(void) {}

ARequest::ARequest(ARequest const &req) {
	*this = req;
}

ARequest	&ARequest::operator=(ARequest const &req) {
	if (this == &req)
		return (*this);
	this->conn = req.conn;
	this->type = req.type;
	this->path = req.path;
	this->host = req.host;
	return (*this);
}

void			ARequest::sendRes(void) {
	this->conn->setWriteBuff(this->response);
	this->conn->setAlive(false);
}

std::string		ARequest::generateError(std::map<int, std::string> errPages) {
	std::string			path;
	std::stringstream	header;
	std::stringstream	html;

	try {
		path = errPages.at(this->errorCode);
		html = Utils::ft_readFile(path);
	}
	catch (std::out_of_range &e) {
		path = ARequest::errors.at(this->errorCode);
		html = Utils::ft_readFile(path);
	}
	catch (ErrException &e) {
		html = Utils::ft_readFile(ERR_500);
		this->errorCode = 500;
	}
	header = this->generateHeader();
	return (html.str());
}

std::stringstream	ARequest::generateHeader(void) const {
	std::stringstream	header;


}

TreeNode<t_node>		*ARequest::findLocation(TreeNode<t_node> *config) const {
	TreeNode<t_node>	*ret = NULL;
	std::string			tmpPath = this->path;

	ret = config->search(tmpPath);
	while (!ret && tmpPath.find("/") != NPOS) {
		tmpPath = tmpPath.substr(0, tmpPath.rfind("/"));
		ret = config->search(tmpPath);
	}
	if (ret)
		return (ret);
	return (config);
}
