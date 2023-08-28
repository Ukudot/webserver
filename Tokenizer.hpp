/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 15:35:52 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/28 16:17:36 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Optional.hpp"

enum class	tokenType {stm, word, path, ip, url, semi, int_lit, open_cbr, close_cbr};

typedef struct s_token {
	tokenType				type;
	Optional<std::string>	value;
}	token;

class	Tokenizer {
	private:
		std::string			buffer;
		size_t				index = 0;
		std::vector<token>	tokens;
		std::stringstream	tmpValue;
		token				tmpToken;
		size_t				line;

		Tokenizer(Tokenizer const &tok);
		Tokenizer	&operator=(Tokenizer const &tok);

	public:
		Tokenizer(std::string const &buffer);
		~Tokenizer(void);

		char				peek(size_t offset = 0) const;
		char				peekSubstr(size_t offset = 0, size_t len) const;
		char				consume(size_t offset = 0);
		std::vector<token>	tokenize(void);
};
