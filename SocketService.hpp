#pragma once
#include "Server.hpp" 
#include "Socket.hpp"

class SocketService {
	private:
		// variables
		Socket	socket;

		// functions
		SocketService(SocketService const &sockserv);
		SocketService const	&operator=(SocketService const &sockserv);

	public:
		// functions
		SocketService(void);
		~SocketService(void);
		void	sockBind();
		void	sockRecv();
		void	sockSend();
		void	sockAccept();
};
