/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 13:41:43 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/04 13:59:49 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "ServSocket.hpp"

class	Connection {
	private:
		// variables 
		int					fd;
		std::string			readBuff;
		std::string			writeBuff;
		struct sockaddr		*addr;
		socklen_t			addrlen;
		bool				alive;

		// functions 
		Connection(Connection const &conn);
		Connection const	&operator=(Connection const &conn);

	public:
		// functions
		Connection(int fd, struct sockaddr *addr, socklen_t addrlen);
		~Connection(void);

		// getters
		int				getFd(void) const;
		std::string		getReadBuff(void) const;
		std::string		getWriteBuff(void) const;
		struct sockaddr	*getAddr(void) const;
		socklen_t		getAddrlen(void) const;
		bool			getAlive(void) const;
		// setters
		void	setFd(int const &fd);
		void	setReadBuff(std::string const &readBuff);
		void	setWriteBuff(std::string const &writeBuff);
		void	setAlive(bool alive);
};
