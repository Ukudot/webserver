/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:44:53 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/05 14:40:22 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ARequest.hpp"
#define CRLF std::string("\n\r")

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
