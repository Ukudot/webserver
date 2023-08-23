#pragma once
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <string.h>
#include "Macro.hpp"
#include "ErrException.hpp"

class ServSocket {
	private:
		struct addrinfo		addrInfo;
		struct pollfd		pollfds[BACKLOG];
		int					npoll;
		int					sfd;
		
	public:
		ServSocket(void);
		ServSocket(struct addrinfo addrInfo);
		~ServSocket(void);
		ServSocket(ServSocket const &ssock);
		ServSocket	&operator=(ServSocket const & ssock);

		// getters
		struct addrinfo		getAddrInfo(void) const;
		struct pollfd const		*getPollfds(void) const;
		int					getNpoll(void) const;
		int					getSfd(void) const;
};
