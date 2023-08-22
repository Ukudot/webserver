#pragma once
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "Macro.hpp"
#include "ErrException.hpp"

class	Socket {
	private:
		// variables 
		int					sfd;
		char				buff[BUFFSIZE];
		std::string			readBuff;
		std::string			writeBuff;

		// functions 
		Socket(Socket const &sock);
		Socket const	&operator=(Socket const &sock);

	public:
		// functions
		Socket(int fd);
		~Socket(void);

		// getters
		int				getSfd(void) const;
		const char		*getBuff(void) const;
		std::string		getReadBuff(void) const;
		std::string		getWriteBuff(void) const;
		// setters
		void	setSfd(int const &sfd);
		void	setBuff(char *readBuff);
		void	setReadBuff(std::string const &readBuff);
		void	setWriteBuff(std::string const &writeBuff);
};
