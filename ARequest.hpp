/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:12:56 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/05 14:39:46 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <vector>
#include "Connection.hpp"
#include "ErrException.hpp"

class	ARequest {
	protected:
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

		virtual void	getInfo(void) = 0;
		virtual void	createRes(void) = 0;
		void			sendRes(void);
};
