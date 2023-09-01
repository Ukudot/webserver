/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:59:46 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/31 15:03:52 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <map>
#include "Tokenizer.hpp"
#include "ErrException.hpp"
#include "TreeNode.hpp"

class	Parser {

	typedef void (Parser::*f)(t_node &data);

	private:
		std::vector<token>				tokens;
		size_t							index;
		bool							end;
		std::map<std::string, f>		parsers;
		std::vector<TreeNode<t_node> *>	nodes;

		
		Parser(Parser const &par);
		Parser	&operator=(Parser const &par);

		token	peek(size_t offset = 0);
		token	consume(size_t offset = 0);

		void	parserSname(t_node &data);
		void	parserListen(t_node &data);
		void	parserRoot(t_node &data);
		void	parserUpload(t_node &data);
		void	parserErrPage(t_node &data);
		void	parserMaxBody(t_node &data);
		void	parserMethods(t_node &data);
		void	parserPrMethods(t_node &data);
		void	parserRewrite(t_node &data);
		void	parserCgi(t_node &data);
		void	parserCgiBin(t_node &data);
		void	parserIndex(t_node &data);
		void	parserAutoindex(t_node &data);
		void	parserLocation(t_node &data);

		bool	checkIp(std::string);
	public:
		Parser(std::vector<token> &tokens);
		~Parser(void);
		
		std::vector<TreeNode<t_node> *>	parse(void);
};
