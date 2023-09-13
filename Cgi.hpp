#pragma once
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include "Macro.hpp"
#include "Server.hpp"
#include "TreeNode.hpp"

class	Cgi {
	private:
		char				**newEnv;
		std::string			path;
		std::string			pathInfo;
		TreeNode<t_node>	*loc;
		t_cgi				cgi;
		pid_t				pid;
		int					errorCode;

		Cgi(Cgi const &cgi);
		Cgi	&operator=(Cgi const &cgi);
		void	formatEnv(std::string env);
		void	deleteMat(void **mat);
		long	timeout(int &wstatus);
		void	launchCgi(int *fds);


	public:
		Cgi(TreeNode<t_node> *loc, t_cgi &cgi, std::string env);
		~Cgi(void);

		int	const 	&getErrorCode(void) const;
		void		setPathInfo(std::string pathInfo);
		std::string	execCgi(void);
};
