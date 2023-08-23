#include "ServSocket.hpp"

ServSocket::ServSocket(void) {
	struct sockaddr_in	*addr = new struct sockaddr_in;

	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = INADDR_ANY;
	addr->sin_port = htons(PORT);
	this->addrInfo.ai_flags = AI_PASSIVE;
	this->addrInfo.ai_family = AF_UNSPEC;
	this->addrInfo.ai_socktype = SOCK_STREAM;
	this->addrInfo.ai_protocol = 0;
	this->addrInfo.ai_addrlen = sizeof(struct sockaddr);
	this->addrInfo.ai_canonname = NULL;
	this->addrInfo.ai_addr = addr;
	this->addrInfo.ai_next = NULL;
	*this = ServSocket(this->addrInfo);
}

ServSocket::ServSocket(struct addrinfo addrInfo): addrInfo(addrInfo) {
	this->sfd = socket()
}

ServSocket::~ServSocket(void) {

}

ServSocket::ServSocket(ServSocket const &ssock) {
	*this = ssock;
}

ServSocket	&ServSocket::operator=(ServSocket const & ssock) {
	if (this == &ssock)
		return (*this);
	this->sfd = ssock.getSfd();
	this->pollfds = ssock.getPollfds();
	this->npoll = ssock.getNpoll();
	this->addrInfo = ssock.getAddrInfo();
	return (*this);
}

struct addrInfo		ServSocket::getAddrInfo(void) const {
	return (this->addrInfo);
}

struct pollfd		*ServSocket::getPollfds(void) const {
	return (this->pollfds);
}

int					ServSocket::getNpoll(void) const {
	return (this->npoll);
}

int					ServSocket::getSfd(void) const {
	return (this->sfd);
}
