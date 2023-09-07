/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServSocket.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 13:56:44 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/07 16:51:41 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServSocket.hpp"

void	ServSocket::createSocket(struct addrinfo *res) {
	int	on = 1;

	this->sfd = 0;
	for (; res; res = res->ai_next) {
		this->sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (this->sfd == -1)
			continue ;
		if (setsockopt(this->sfd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on)))
			throw ErrException("setsockopt() failed");
		if (bind(this->sfd, res->ai_addr, res->ai_addrlen)) {
			close(this->sfd);
			continue ;
		}
		break ;
	}
	if (!res)
		throw ErrException("Cannot create socket");
	if (listen(this->sfd, BACKLOG))
		throw ErrException("listen() failed");
}

ServSocket::ServSocket(std::string ip, int port) {
	struct addrinfo	hints, *res;

	this->sfd = -1;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	if (getaddrinfo(ip.c_str(), Utils::ft_itoa(port).c_str(), &hints, &res))
		throw (ErrException("getaddrinfo() failed"));
	try {
		*this  = ServSocket(res);
		freeaddrinfo(res);
	}
	catch (ErrException &e) {
		freeaddrinfo(res);
		throw (ErrException(e.what()));
	}
}

ServSocket::ServSocket(struct addrinfo *res) {
	memset(this->buff, 0, BUFFSIZE);
	this->createSocket(res);
	memset(this->pollfds, 0, sizeof(this->pollfds));
	this->pollfds[0].fd = this->sfd;
	this->pollfds[0].events = POLLIN;
	this->npoll = 1;
	this->toClean = false;
	DEBUG("socket created");
}

ServSocket::~ServSocket(void) {
	std::vector<Connection *>::iterator	ite;
	for (ite = this->conns.begin(); ite != this->conns.end(); ite++)
		delete (*ite);
	this->conns.clear();
}

ServSocket::ServSocket(ServSocket const &ssock) {
	*this = ssock;
}

ServSocket	&ServSocket::operator=(ServSocket const & ssock) {
	std::vector<Connection *>::iterator	ite;

	if (this == &ssock)
		return (*this);
	if (this->sfd != -1)
		close(this->sfd);
	this->sfd = ssock.getSfd();
	memcpy((void *) this->pollfds, (void *) ssock.getPollfds(), sizeof(pollfd) * BACKLOG);
	this->npoll = ssock.getNpoll();
	memcpy((void *) this->buff, (void *) ssock.getBuff(), BUFFSIZE);
	for (ite = this->conns.begin(); ite != this->conns.end(); ite++)
		delete (*ite);
	this->conns = ssock.getConns(false);
	this->toClean = ssock.getToClean();
	return (*this);
}

struct pollfd const	*ServSocket::getPollfds(void) const {
	return (this->pollfds);
}

int	ServSocket::getNpoll(void) const {
	return (this->npoll);
}

int	ServSocket::getSfd(void) const {
	return (this->sfd);
}

char const	*ServSocket::getBuff(void) const {
	return (this->buff);
}

bool	ServSocket::getToClean(void) const {
	return (this->toClean);
}

std::vector<Connection *>	ServSocket::getConns(bool active) const {
	std::vector<Connection *>::const_iterator	ite;
	std::vector<Connection *>					ret;

	if (!active)
		return (this->conns);
	for (ite = this->conns.begin(); ite != this->conns.end(); ite++) {
		if ((*ite)->getReadBuff() != "")
			ret.push_back(*ite);
	}
	return (ret);
}

Connection	*ServSocket::getConnByFd(int fd) const {
	std::vector<Connection *>::const_iterator	ite;

	for (ite = this->conns.begin(); ite != this->conns.end(); ite++)
		if ((*ite)->getFd() == fd)
			return (*ite);
	return (NULL);
}

int	ServSocket::findConnByFd(int fd) const {
	std::vector<Connection *>::const_iterator	ite;

	for (ite = this->conns.begin(); ite != this->conns.end(); ite++)
		if ((*ite)->getFd() == fd)
			return (std::distance(this->conns.begin(), ite));
	return (-1);
}

bool	ServSocket::spoll(void) {
	Connection	*conn;
	int			rc;

	DEBUG("Polling...");
	rc = poll(this->pollfds, this->npoll, TIMEOUT);
//	std::cout << this->pollfds[0].revents <<std::endl;
	if (rc == -1)
		throw ErrException("poll() failed");
	else if (rc == 0)
		return (false);
	if (this->pollfds[0].revents == POLLIN && this->npoll < BACKLOG)
		this->newConn();
	for (int i = 1; i < this->npoll; i++) {
//		std::cout << this->pollfds[1].revents <<std::endl;
		if (this->pollfds[i].fd == -1) {
			this->toClean = true;
			ERROR("fd set to -1");
			continue ;
		}
		conn = this->getConnByFd(this->pollfds[i].fd);
		if (!conn) {
			this->pollfds[i].fd = -1;
			this->toClean = true;
			ERROR("no connection found");
			continue ;
		}
		if (this->pollfds[i].revents == POLLIN)
			this->srecv(conn, i);
		else if (this->pollfds[i].revents == POLLOUT && conn->getWriteBuff() != "")
			this->ssend(conn, i);
		else if (this->pollfds[i].revents & !POLLOUT & !POLLIN) {
//			std::cout << this->pollfds[i].revents <<std::endl;
			this->toClean = true;
			this->pollfds[i].revents = POLLERR;
		}
	}
	if (this->toClean) {
		this->clean();
		this->shrink();
		this->toClean = false;
	}
	return (true);
}

void	ServSocket::srecv(Connection *conn, int i) {
	int	rc;

	DEBUG("Connection receiving data");
	memset(this->buff, 0, BUFFSIZE);
	rc = recv(conn->getFd(), this->buff, BUFFSIZE, MSG_DONTWAIT);
	if (rc == -1)
		throw ErrException("recv() failed");
	else if (rc == 0) {
		this->toClean = true;
		this->pollfds[i].revents = POLLERR;
	}
	else
		conn->setReadBuff(conn->getReadBuff() + std::string(this->buff));
}

void	ServSocket::ssend(Connection *conn, int i) {
	int	rc;

	DEBUG("Connection sending data");
	rc = send(conn->getFd(), conn->getWriteBuff().c_str(), conn->getWriteBuff().size(), MSG_DONTWAIT);
	if (rc == -1)
		throw ErrException("send() failed");
	else if (rc == 0) {
		this->toClean = true;
		this->pollfds[i].revents = POLLERR;
	}
	else if (rc < (int) conn->getWriteBuff().size())
		conn->setWriteBuff(conn->getWriteBuff().substr(rc));
	else {
		conn->setWriteBuff("");
		if (!conn->getAlive()) {
			this->pollfds[i].revents = POLLERR;
			this->toClean = true;
		}
		this->pollfds[i].events = POLLIN;
		DEBUG(RED + "sent all" + RESET);
	}
}

void	ServSocket::newConn(void) {
	Connection		*conn;
	struct sockaddr	*connAddr = NULL;
	socklen_t		addrlen;

	this->pollfds[this->npoll].fd = accept(this->sfd, connAddr, &addrlen);
	if (this->pollfds[this->npoll].fd == -1)
		throw ErrException("accept() failed");
	conn = new Connection(this->pollfds[this->npoll].fd, connAddr, addrlen);
	this->conns.push_back(conn);
	this->pollfds[this->npoll].events = POLLIN;
	this->npoll++;
	DEBUG("new connection created");
}

void	ServSocket::clean(void) {
	int	pos;

	DEBUG("cleaning");
	for (int i = 1; i < this->npoll; i++) {
		if (this->pollfds[i].revents == POLLERR) {
			pos = this->findConnByFd(this->pollfds[i].fd);
			if (pos == -1)
				ERROR("clean didn't find connection");
			delete this->conns[pos];
			this->conns.erase(this->conns.begin() + pos);
			this->pollfds[i].fd = -1;
		}
	}
}

void	ServSocket::shrink(void) {
	for (int i = 1; i < this->npoll - 1; i++) {
		if (this->pollfds[i].fd == -1) {
			for (int j = i; j < this->npoll - 1; j++)
				this->pollfds[j] = this->pollfds[j + 1];
			i--;
			this->npoll--;
		}
	}
	if (this->pollfds[this->npoll - 1].fd == -1)
		memset((void *) &this->pollfds[--this->npoll], 0, sizeof(struct pollfd));
}

void	ServSocket::closeSfd(void) {
	close(this->sfd);
}

void	ServSocket::pushBuffers(void) {
	Connection	*conn;

	for (int i = 1; i < this->npoll; i++) {
		conn = this->getConnByFd(this->pollfds[i].fd);
		if (conn->getWriteBuff() != "")
			this->pollfds[i].events = POLLOUT;
	}
}
