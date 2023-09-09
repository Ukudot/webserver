/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 10:05:23 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/08 15:06:53 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "TreeNode.hpp"
#include "Tokenizer.hpp"
#include "Parser.hpp"
#include "ServSocket.hpp"
#include "ARequest.hpp"
#include "GetRequest.hpp"
#include "Macro.hpp"

class	Server {
	private:

		// vars
		std::vector<ServSocket *>	sockets;
		TreeNode<t_node>			*config;

		// functions
		Server(Server const &serv);
		Server	&operator=(Server const &serv);

	public:

		// static vars
		static std::vector<Server *>	servers;
		static std::vector<char *>		envp;

		Server(TreeNode<t_node>	*config);
		~Server(void);

		// functions
		void		interpret(void);
		ARequest	*readConn(Connection *conn);

		// static functions
		static void	polls(void);
};
