/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:44:53 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/07 16:53:25 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ARequest.hpp"

std::map<int, std::string>	ARequest::errors = ARequest::initMap();

std::map<int, std::string>	ARequest::initMap(void) {
	std::map<int, std::string>	ret;

	ret[200] = ERR_200; // OK
	ret[201] = ERR_201; // created
	ret[301] = ERR_301; // perm redirect
	ret[302] = ERR_302; // temp redirect
	ret[400] = ERR_400; // bad request
	ret[404] = ERR_404; // not found
	ret[405] = ERR_405; // method not allowed
	ret[411] = ERR_411; // length required
	ret[413] = ERR_413; // request too large
	ret[500] = ERR_500; // server error
	ret[501] = ERR_501; // not implemented
	ret[505] = ERR_505; // bad version
	return (ret);
}

ARequest::ARequest(Connection *conn, std::string type): conn(conn), type(type) {
	std::string	buffer;

	buffer = this->conn->getReadBuff();
	buffer = buffer.substr(0, buffer.find(CRLF + CRLF));
	this->lines = Utils::ft_split(buffer, CRLF);
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

std::string const	&ARequest::getType(void) const {
	return (this->type);
}

void			ARequest::setErrorCode(int errorCode) {
	this->errorCode = errorCode;
}	

void			ARequest::sendRes(void) {
	this->conn->setWriteBuff(this->response);
	this->conn->setReadBuff("");
	this->conn->setAlive(false);
}

std::string		ARequest::generateError(std::map<int, std::string> errPages, std::string red) {
	std::string			path;
	std::stringstream	header;
	std::stringstream	html;

	try {
		path = errPages.at(this->errorCode);
		html << Utils::ft_readFile(path);
	}
	catch (std::out_of_range &e) {
		path = ARequest::errors.at(this->errorCode);
		html << Utils::ft_readFile(path);
	}
	catch (ErrException &e) {
		/*
		std::cout << e.what() << std::endl;
		html << Utils::ft_readFile(ERR_500);
		this->errorCode = 500;
		*/
		path = ARequest::errors.at(this->errorCode);
		html << Utils::ft_readFile(path);
	}
	header << this->generateHeader(html.str().length(), red);
	return (header.str() + html.str());
}

std::string	ARequest::generateHeader(size_t length, std::string red) const {
	std::stringstream	header;

	header << "HTTP/1.1 " << this->errorCode << " webserver" << CRLF;
	if (red == "")
		header << "Content-Length: " << length << CRLF;
	else
		header << "Location: " << red << CRLF;
	header << CRLF;
	return (header.str());
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
