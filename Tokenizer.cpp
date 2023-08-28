/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 15:35:52 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/28 16:50:59 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tokenizer.hpp"
#define NPOS std::string::npos
#define DEL std::string(" \t;")

Tokenizer::Tokenizer(std::string const &buffer): buffer(buffer) {
	return ;
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
	this->tmpValue = tok.tmpValue;
	this->tmpToken = tok.tmpToken;
	this->line = tok.line;
	return (*this);
}

Tokenizer::~Tokenizer(void) {
	return ;
}

char				Tokenizer::peek(size_t offset = 0) const {
	return (this->buffer.at(this->index + offset));
}

std::string			Tokenizer::peekSubstr(size_t offset = 0, size_t len) const {
	return (this->buffer.substr(this->index + offset, len));
}

char				Tokenizer::consume(size_t offset = 0) {
	this->index += offset;
	return (this->buffer.at(this->index++));
}

std::vector<token>	Tokenizer::tokenize(void) {
	while (this->peek()) {
		while (std::isspace(this->peek())) {
			if (this->peek(i) == '\n')
				this->line++;
			this->consume();
		}
		if (this->peek() == 'h' && this->peekSubstr(0, 7) == "http://")
			this->tokenUrl();
		else if (std::isalpha(this->peek()))
			this->tokenWord();
		else if (this->peek() == '/')
			this->tokenPath();
		else if (std::isdigit(this->peek())) {
			for (size_t i = 0; DEL.find(this->peek(i)) != NPOS; i++) {
				if (this->peek(i) == '.') {
					this->tokenIp();
					break ;
				}
				else if (!std::isdigit(this->peek(i))){
					throw (ErrException("Error: bad number"))
				}
			}
			if (DEL.find(this->peek(i)) != NPOS) {
				this->tokenInt();
			}
		}
		else if (this->peek() == ';')
			this->tokenSemi();
		else if (this->peek() == '{')
			this->tokenOpencbr();
		else if (this->peek() == '}')
			this->tokenClosecbr();
		else
			throw (ErrException("Error: invalid charachter"));
	}
}

void	Tokenizer::tokenUrl(void) {
	while (DEL.find(this->peek()) != NPOS) // to check
		this->tmpValue << this->consume();
	this->tokens.push_back({.type = tokenType::url, .value = this->tmpValue.str()});
	this->tmpValue.str("");
	this->tmpClear.str("");
}

void	Tokenizer::tokenWord(void) {
	while (DEL.find(this->peek()) != NPOS) // to check
		this->tmpValue << this->consume();
	this->tokens.push_back({.type = tokenType::word, .value = this->tmpValue.str()});
	this->tmpValue.str("");
	this->tmpClear.str("");
}

void	Tokenizer::tokenPath(void) {
	while (DEL.find(this->peek()) != NPOS) // to check
		this->tmpValue << this->consume();
	this->tokens.push_back({.type = tokenType::path, .value = this->tmpValue.str()});
	this->tmpValue.str("");
	this->tmpClear.str("");
}

void	Tokenizer::tokenIp(void) {
	while (DEL.find(this->peek()) != NPOS) // to check
		this->tmpValue << this->consume();
	this->tokens.push_back({.type = tokenType::ip, .value = this->tmpValue.str()});
	this->tmpValue.str("");
	this->tmpClear.str("");
}

void	Tokenizer::tokenInt(void) {
	while (DEL.find(this->peek()) != NPOS) // to check
		this->tmpValue << this->consume();
	this->tokens.push_back({.type = tokenType::int_lit, .value = this->tmpValue.str()});
	this->tmpValue.str("");
	this->tmpClear.str("");
}

void	Tokenizer::tokenSemi(void) {
	this->consume();
	this->tokens.push_back({.type = tokenType::semi);
}

void	Tokenizer::tokenOpencbr(void) {
	this->consume();
	this->tokens.push_back({.type = tokenType::open_cbr);
}

void	Tokenizer::tokenClosecbr(void) {
	this->consume();
	this->tokens.push_back({.type = tokenType::close_cbr);
}

// a better solution could be
void	Tokenizer::tokenWithValue(tokenType type) {
	while (DEL.find(this->peek()) != NPOS) // to check
		this->tmpValue << this->consume();
	this->tokens.push_back({.type = type, .value = this->tmpValue.str()});
	this->tmpValue.str("");
	this->tmpClear.str("");
}

void	Tokenizer::tokenWoutValue(tokenType type) {
	this->consume();
	if (type == tokenType::semi) {
		if (this->peek() != '\n')
			throw (ErrException("Error: no newline after ';'"));
		this->consume();
		this->line++;
	}
	this->tokens.push_back({.type = type});
}

size_t	Tokenizer::getNoLine(void) const {
	return (this->line);
}
