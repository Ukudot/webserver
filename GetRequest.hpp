/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:12:56 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/07 15:15:12 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ARequest.hpp"

class	GetRequest: public ARequest {
	public:
		GetRequest(Connection *conn);
		~GetRequest(void);
		GetRequest(GetRequest const &req);
		GetRequest	&operator=(GetRequest const &req);

		virtual void	getInfo(void);
		virtual void	createRes(TreeNode<t_node> *config);
};
