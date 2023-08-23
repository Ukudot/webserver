/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServSocket.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 13:57:03 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/23 16:43:50 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <string.h>
#include "Macro.hpp"
#include "ErrException.hpp"
#include "Connection.hpp"

class Connection;

class ServSocket {
	private:
		struct addrinfo				addrInfo;
		struct pollfd				pollfds[BACKLOG];
		int							npoll;
		int							sfd;
		char						buff[BUFFSIZE];
		bool						toClean;
		std::vector<Connection *>	conns;

		// functions
		void	srecv(Connection *conn, int i);
		void	ssend(Connection *conn, int i);
		void	newConn(void);
		
	public:
		ServSocket(void);
		ServSocket(struct addrinfo addrInfo);
		~ServSocket(void);
		ServSocket(ServSocket const &ssock);
		ServSocket	&operator=(ServSocket const & ssock);

		// getters
		struct addrinfo				getAddrInfo(void) const;
		struct pollfd const			*getPollfds(void) const;
		int							getNpoll(void) const;
		int							getSfd(void) const;
		char const					*getBuff(void) const;
		bool						getToClean(void) const;
		std::vector<Connection *>	getConns(bool active) const;
		Connection					*getConnByFd(int fd) const;
		int							findConnByFd(int fd) const;

		// functions
		bool	spoll(void);
		void	clean(void);
		void	shrink(void);
};
