/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:12:56 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/11 14:06:20 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ARequest.hpp"
#include "Server.hpp"

class	GetRequest: public ARequest {
	private:

		char	**updateEnvp(void);
		bool	reds(TreeNode<t_node> *config, TreeNode<t_node> *loc, std::string tmpPath);
		bool	cgi(TreeNode<t_node> *loc, std::string tmpPath);
		void	execCgi(TreeNode<t_node> *loc, t_cgi &cgi, std::string path);
		pid_t	launchCgi(int *fds, t_cgi &cgi, std::string path);
		void	deleteMat(void **mat);

	public:

		GetRequest(Connection *conn);
		~GetRequest(void);
		GetRequest(GetRequest const &req);
		GetRequest	&operator=(GetRequest const &req);

		virtual void	getInfo(void);
		virtual void	createRes(TreeNode<t_node> *config);
};
