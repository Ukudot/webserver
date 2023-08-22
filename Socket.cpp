#include "Socket.hpp"

Socket::Socket(int sfd) {
	this->sfd = sfd;
	memset(this->buff, 0, BUFFSIZE);
	this->readBuff = "";
	this->writeBuff = "";
}

Socket::~Socket(void) {
	close(this->sfd);
}

Socket::Socket(Socket const &sock) {
	*this = sock;
}

Socket const	&Socket::operator=(Socket const &sock) {
	if (this == &sock)
		return (*this);
	this->sfd = sock.getSfd();
	strncpy(this->buff, sock.getBuff(), BUFFSIZE);
	this->readBuff = sock.getReadBuff();
	this->writeBuff = sock.getWriteBuff();
	return (*this);
}

int				Socket::getSfd(void) const {
	return (this->sfd);
}

const char		*Socket::getBuff(void) const {
	return (this->buff);
}

std::string 	Socket::getReadBuff(void) const {
	return (this->readBuff);
}

std::string		Socket::getWriteBuff(void) const {
	return (this->writeBuff);
}

void	Socket::setSfd(int const &sfd) {
	this->sfd = sfd;
}

void	Socket::setBuff(char *buff) {
	strncpy(this->buff, buff, BUFFSIZE);
}

void	Socket::setReadBuff(std::string const &readBuff) {
	this->readBuff = readBuff;
}

void	Socket::setWriteBuff(std::string const &writeBuff) {
	this->writeBuff = writeBuff;
}
