/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:12:56 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/09 10:29:04 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include "Connection.hpp"
#include "TreeNode.hpp"
#include "Utils.hpp"
#include "ErrException.hpp"

#define CRLF std::string("\r\n")
#define NPOS std::string::npos
#define ERR_200 std::string("www/errors/error200.html")
#define ERR_201 std::string("www/errors/error201.html")
#define ERR_301 std::string("www/errors/error301.html")
#define ERR_302 std::string("www/errors/error302.html")
#define ERR_400 std::string("www/errors/error400.html")
#define ERR_404 std::string("www/errors/error404.html")
#define ERR_405 std::string("www/errors/error405.html")
#define ERR_411 std::string("www/errors/error411.html")
#define ERR_413 std::string("www/errors/error413.html")
#define ERR_500 std::string("www/errors/error500.html")
#define ERR_501 std::string("www/errors/error501.html")
#define ERR_505 std::string("www/errors/error505.html")

class	ARequest {
	protected:
		// static vars
		static std::map<int, std::string>	errors;

		// vars
		Connection					*conn;
		std::string					path;
		std::string					env;
		std::string					type;
		std::string					host;
		std::string 				response;
		std::vector<std::string>	lines;
		int							errorCode;
	
		// funcs
		TreeNode<t_node>	*findLocation(TreeNode<t_node> *config) const;
		std::string			generateError(std::map<int, std::string> errPages = ARequest::errors, std::string red = "");
		std::string			generateHeader(size_t length, std::string red) const;

	public:

		ARequest(Connection *conn, std::string type);
		virtual ~ARequest(void);
		ARequest(ARequest const &req);
		ARequest	&operator=(ARequest const &req);

		// static funcs
		static std::map<int, std::string>	initMap(void);

		// funcs
		std::string const	&getType(void) const;
		void				setErrorCode(int errorCode);
		virtual void		getInfo(void) = 0;
		virtual void		createRes(TreeNode<t_node> *config) = 0;
		void				sendRes(void);
};
