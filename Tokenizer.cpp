/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 15:35:52 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/29 10:56:27 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tokenizer.hpp"
#include "ErrException.hpp"
#include "Utils.hpp"
#define NPOS std::string::npos
#define DEL std::string(" \t\n;")

Tokenizer::Tokenizer(std::string const &buffer): buffer(buffer) {
	this->index = 0;
	this->line = 1;
	this->eof = false;
}

Tokenizer::Tokenizer(Tokenizer const &tok) {
	*this = tok;	
}

Tokenizer	&Tokenizer::operator=(Tokenizer const &tok) {
	if (this == &tok)
		return (*this);
	this->buffer = tok.buffer;
	this->index = tok.index;
	this->tokens = tok.tokens;
	this->tmpValue.str("");
	this->tmpValue.clear();
	this->tmpValue.str(tok.tmpValue.str());
	this->tmpToken = tok.tmpToken;
	this->line = tok.line;
	this->eof = tok.eof;
	return (*this);
}

Tokenizer::~Tokenizer(void) {
	return ;
}

char				Tokenizer::peek(size_t offset) const {
	if (this->eof)
		return ('\0');
	return (this->buffer.at(this->index + offset));
}

std::string			Tokenizer::peekSubstr(size_t offset, size_t len) const {
	if (this->eof)
		return ("");
	return (this->buffer.substr(this->index + offset, len));
}

char				Tokenizer::consume(size_t offset) {
	if (this->index + offset + 1 >= this->buffer.length())
		this->eof = true;
	if (this->eof)
		return ('\0');
	this->index += offset;
//	std::cout << "consume char: " << (int) this->peek() << std::endl;
	return (this->buffer.at(this->index++));
}

std::vector<token>	Tokenizer::tokenize(void) {
	while (this->peek()) {
		while (std::isspace(this->peek())) {
			if (this->peek() == '\n') {
				this->line++;
//				std::cout << "DEBUG: " << this->line << std::endl;
			}
			this->consume();
		}
//		std::cout << "current char: " << this->peek() << std::endl;
		if (this->peek() == 'h' && this->peekSubstr(0, 7) == "http://")
			this->tokenWithValue(url);
		else if (std::isalpha(this->peek()))
			this->tokenWithValue(word);
		else if (this->peek() == '/')
			this->tokenWithValue(path);
		else if (std::isdigit(this->peek())) {
			for (size_t i = 0; DEL.find(this->peek(i)) == NPOS; i++) {
				if (this->peek(i) == '.') {
					this->tokenWithValue(ip);
					break ;
				}
				else if (!std::isdigit(this->peek(i))){
					throw (ErrException("Error: bad number"));
				}
			}
			if (DEL.find(this->peek()) == NPOS) {
				this->tokenWithValue(int_lit);
			}
		}
		else if (this->peek() == ';')
			this->tokenWoutValue(semi);
		else if (this->peek() == '{')
			this->tokenWoutValue(open_cbr);
		else if (this->peek() == '}')
			this->tokenWoutValue(close_cbr);
		else if (this->peek() == '#')
			while (this->peek() != '\n')
				this->consume();
		else if (this->peek() != 0)
			throw (ErrException("Error: invalid charachter"));
	}
	return (this->tokens);
}

void	Tokenizer::tokenWithValue(tokenType type) {
	while (this->peek() && DEL.find(this->peek()) == NPOS)
		this->tmpValue << this->consume();
	this->tmpToken.type = type;
	this->tmpToken.value = this->tmpValue.str();
	this->tokens.push_back(this->tmpToken);
	this->tmpValue.str("");
	this->tmpValue.clear();
}

void	Tokenizer::tokenWoutValue(tokenType type) {
	this->consume();
	if (type == semi) {
		while (std::isspace(this->peek()) && this->peek() != '\n')
			this->consume();
		if (this->peek() != '\n')
			throw (ErrException("Error: no newline after ';'"));
		this->consume();
		this->line++;
	}
	this->tmpToken.type = type;
	this->tmpToken.value = Optional<std::string>();
	this->tokens.push_back(this->tmpToken);
}

size_t	Tokenizer::getNoLine(void) const {
	return (this->line);
}

std::string	Tokenizer::getLine(void) {
	std::stringstream	ss;
	size_t				error;

	error = this->index;
	while (this->index != 1 && this->buffer[this->index - 1] != '\n')
		this->index--;
	for (size_t i = 0; this->peek(i) && this->peek(i) != '\n'; i++) {
		if (this->index + i == error) {
			ss << "\033[1;5;31m";
			while (this->peek(i) && DEL.find(this->peek(i)) == NPOS) {
				ss << this->peek(i);
				i++;
			}
			ss << "\033[0m";
		}
		else
			ss << this->peek(i);
	}
	return (ss.str());

}
