/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 15:35:52 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/29 10:34:32 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Optional.hpp"

enum	tokenType {stm, word, path, ip, url, semi, int_lit, open_cbr, close_cbr};

typedef struct s_token {
	tokenType				type;
	Optional<std::string>	value;
}	token;

class	Tokenizer {
	private:
		std::string			buffer;
		size_t				index;
		std::vector<token>	tokens;
		std::stringstream	tmpValue;
		token				tmpToken;
		size_t				line;
		bool				eof;

		Tokenizer(Tokenizer const &tok);
		Tokenizer	&operator=(Tokenizer const &tok);

		void		tokenWithValue(tokenType type);
		void		tokenWoutValue(tokenType type);
		char		peek(size_t offset = 0) const;
		std::string	peekSubstr(size_t offset = 0, size_t len = 1) const;
		char		consume(size_t offset = 0);

	public:
		Tokenizer(std::string const &buffer);
		~Tokenizer(void);

		size_t				getNoLine(void) const;
		std::string			getLine(void);
		std::vector<token>	tokenize(void);
};
