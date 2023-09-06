/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:12:56 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/06 11:59:13 by gpanico          ###   ########.fr       */
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

class	ARequest {
	protected:
		// static vars
		static std::map<int, std::string>	errors;

		// vars
		Connection					*conn;
		std::string					path;
		std::string					type;
		std::string					host;
		std::string 				response;
		std::vector<std::string>	lines;
		int							errorCode;
	
	public:
		ARequest(Connection *conn, std::string type);
		virtual ~ARequest(void);
		ARequest(ARequest const &req);
		ARequest	&operator=(ARequest const &req);

		// static funcs
		static std::map<int, std:::string>	ARequest::initMap(void);

		// funcs
		virtual void		getInfo(void) = 0;
		virtual void		createRes(TreeNode *config) = 0;
		void				sendRes(void);
		std::string			generateError(std::map<int, std::string>);
		std::stringstream	generateHeader(void) const;
		TreeNode<t_node>	*findLocation(TreeNode<t_node> *config) const;
};
