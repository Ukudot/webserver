#include "ServSocket.hpp"

ServSocket::ServSocket(void) {
	struct sockaddr_in	*addr = new struct sockaddr_in;

	this->sfd = -1;
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = INADDR_ANY;
	addr->sin_port = htons(PORT);
	this->addrInfo.ai_flags = AI_PASSIVE;
	this->addrInfo.ai_family = AF_UNSPEC;
	this->addrInfo.ai_socktype = SOCK_STREAM;
	this->addrInfo.ai_protocol = 0;
	this->addrInfo.ai_addrlen = sizeof(struct sockaddr);
	this->addrInfo.ai_canonname = NULL;
	this->addrInfo.ai_addr = (struct sockaddr *) addr;
	this->addrInfo.ai_next = NULL;
	*this = ServSocket(this->addrInfo);
}

ServSocket::ServSocket(struct addrinfo addrInfo): addrInfo(addrInfo) {
	int	on = 1;
	struct sockaddr_in	*addr = new struct sockaddr_in;

	memcpy((void *) addr, (void *) this->addrInfo.ai_addr, this->addrInfo.ai_addrlen);
	this->addrInfo.ai_addr = (struct sockaddr *) addr;
	this->sfd = socket(((struct sockaddr_in *) this->addrInfo.ai_addr)->sin_family,
			this->addrInfo.ai_socktype, this->addrInfo.ai_protocol);
	if (this->sfd == -1)
		throw ErrException("socket() failed");
	if (setsockopt(this->sfd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on)))
		throw ErrException("setsockopt() failed");
	if (bind(this->sfd, (struct sockaddr *) this->addrInfo.ai_addr, this->addrInfo.ai_addrlen))
		throw ErrException("bind() failed");
	if (listen(this->sfd, BACKLOG))
		throw ErrException("listen() failed");
	DEBUG("socket created");
}

ServSocket::~ServSocket(void) {
	delete this->addrInfo.ai_addr;
	if (this->sfd != -1)
		close(this->sfd);
}

ServSocket::ServSocket(ServSocket const &ssock) {
	*this = ssock;
}

ServSocket	&ServSocket::operator=(ServSocket const & ssock) {
	struct sockaddr_in	*tmpAddr;

	if (this == &ssock)
		return (*this);
	tmpAddr = (struct sockaddr_in *) this->addrInfo.ai_addr;
	if (this->sfd != -1)
		close(this->sfd);
	this->sfd = ssock.getSfd();
	memcpy((void *) this->pollfds, (void *) ssock.getPollfds(), sizeof(pollfd) * BACKLOG);
	this->npoll = ssock.getNpoll();
	this->addrInfo = ssock.getAddrInfo();
	this->addrInfo.ai_addr = (struct sockaddr *) tmpAddr;
	memcpy((void *) this->addrInfo.ai_addr, (void *) ssock.getAddrInfo().ai_addr, this->addrInfo.ai_addrlen);
	return (*this);
}

struct addrinfo		ServSocket::getAddrInfo(void) const {
	return (this->addrInfo);
}

struct pollfd const		*ServSocket::getPollfds(void) const {
	return (this->pollfds);
}

int					ServSocket::getNpoll(void) const {
	return (this->npoll);
}

int					ServSocket::getSfd(void) const {
	return (this->sfd);
}
