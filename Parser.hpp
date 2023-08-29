/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:59:46 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/29 16:10:57 by gpanico          ###   ########.fr       */
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

class	Parser {

	typedef void (Parser::*f)(void);

	private:
		std::vector<token>			tokens;
		size_t						index;
		bool						end;
		std::map<std::string, f>	parsers;
		
		Parser(Parser const &par);
		Parser	&operator=(Parser const &par);

		token	peek(size_t offset = 0);
		token	consume(size_t offset = 0);

		void	parserSname(void);
		void	parserListen(void);
		void	parserRoot(void);
		void	parserUpload(void);
		void	parserErrPage(void);
		void	parserMaxBody(void);
		void	parserMethods(void);
		void	parserPrMethods(void);
		void	parserRewrite(void);
		void	parserCgi(void);
		void	parserCgiBin(void);
		void	parserIndex(void);
		void	parserAutoindex(void);
		void	parserLocation(void);
	public:
		Parser(std::vector<token> &tokens);
		~Parser(void);
		
		void	parse(void);
};
