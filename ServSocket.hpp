#pragma once
#include "Macro.hpp"

class ServSocket {
	private:
		struct addrinfo		addrInfo;
		struct pollfd		pollfds[BACKLOG];
		int					npoll;
		int					sfd;
		
	public:
		ServSocket(void);
		ServSocket(struct addrInfo addrInfo);
		~ServSocket(void);
		ServSocket(ServSocket const &ssock);
		ServSocket	&operator=(ServSocket const & ssock);

		// getters
		struct addrinfo		getAddrInfo(void) const;
		struct pollfd		*getPollfds(void) const;
		int					getNpoll(void) const;
		int					getSfd(void) const;
};
