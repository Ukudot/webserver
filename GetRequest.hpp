/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:12:56 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/07 09:47:53 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ARequest.hpp"

class	GetRequest: public ARequest {
	public:
		GetRequest(Connection *conn);
		~GetRequest(void);
		GetRequest(GetRequest const &req);
		GetRequest	&operator=(GetRequest const &req);

		virtual void	getInfo(void);
		virtual void	createRes(TreeMode<t_node> *config);
};
