/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 13:54:29 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/09 15:27:27 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

Connection::Connection(int fd, struct sockaddr *addr, socklen_t addrlen): fd(fd), addr(addr), addrlen(addrlen) {
	this->readBuff = "";
	this->writeBuff = "";
	this->alive = true;
}

Connection::~Connection(void) {
	DEBUG(RED + "Connection closed" + RESET);
	if (this->fd != -1)
		close(this->fd);
}

Connection::Connection(Connection const &conn) {
	*this = conn;
}

Connection const	&Connection::operator=(Connection const &conn) {
	if (this == &conn)
		return (*this);
	this->fd = conn.getFd();
	this->readBuff = conn.getReadBuff();
	this->writeBuff = conn.getWriteBuff();
	this->addr = conn.getAddr();
	this->addrlen = conn.getAddrlen();
	this->alive = conn.alive;
	return (*this);
}

int				Connection::getFd(void) const {
	return (this->fd);
}

std::string 	Connection::getReadBuff(void) const {
	return (this->readBuff);
}

std::string		Connection::getWriteBuff(void) const {
	return (this->writeBuff);
}

struct sockaddr	*Connection::getAddr(void) const {
	return (this->addr);
}

socklen_t	Connection::getAddrlen(void) const {
	return (this->addrlen);
}

bool	Connection::getAlive(void) const {
	return (this->alive);
}

void	Connection::setFd(int const &fd) {
	this->fd = fd;
}

void	Connection::setReadBuff(std::string const &readBuff) {
	this->readBuff = readBuff;
}

void	Connection::setWriteBuff(std::string const &writeBuff) {
	this->writeBuff = writeBuff;
}

void	Connection::setAlive(bool alive) {
	this->alive = alive;
}
