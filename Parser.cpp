/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:59:46 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/29 16:52:36 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(std::vector<token> &tokens): tokens(tokens) {
	this->index = 0;
	this->end = false;
	this->parsers["server_name"] = &Parser::parserSname;
	this->parsers["listen"] = &Parser::parserListen;
	this->parsers["root"] = &Parser::parserRoot;
	this->parsers["upload"] = &Parser::parserUpload;
	this->parsers["err_page"] = &Parser::parserErrPage;
	this->parsers["max_body_size"] = &Parser::parserMaxBody;
	this->parsers["methods"] = &Parser::parserMethods;
	this->parsers["prohibit_methods"] = &Parser::parserPrMethods;
	this->parsers["rewrite"] = &Parser::parserRewrite;
	this->parsers["cgi"] = &Parser::parserCgi;
	this->parsers["cgi-bin"] = &Parser::parserCgiBin;
	this->parsers["index"] = &Parser::parserIndex;
	this->parsers["autoindex"] = &Parser::parserAutoindex;
	this->parsers["location"] = &Parser::parserLocation;
}

Parser::~Parser(void) {
	return ;
}

Parser::Parser(Parser const &par) {
	*this = par;
}

Parser	&Parser::operator=(Parser const &par) {
	if (this == &par)
		return (*this);
	this->tokens = par.tokens;
	this->index = par.index;
	this->end = par.end;
	return (*this);
}

token	Parser::peek(size_t offset) {
	if (this->end)
		return (this->tokens.at(this->index));
	return (this->tokens.at(this->index + offset));
}

token	Parser::consume(size_t offset) {
	if (this->index + offset + 1 >= this->tokens.size())
		this->end = true;
	if (this->end)
		return (this->tokens.at(this->index));
	this->index += offset;
	return (this->tokens.at(this->index++));
}


void	Parser::parse(void) {
	while (this->peek().type != tnull) {
		if (!this->peek().value.hasValue()
				|| this->peek().value.value() != "server"
				|| this->peek(1).type != open_cbr)
			throw (ErrException("Missing '{' after server"));
		this->consume(1);
		if (this->peek().type == close_cbr)
			throw (ErrException("Empty server"));
		while (this->peek().type != tnull && this->peek().type != close_cbr) {
			if (this->peek().type != word)
				throw (ErrException("Invalid statement in server"));
			(this->*(this->parsers.at(this->peek().value.value())))();
		}
		if (this->peek().type == tnull)
			throw (ErrException("Unclosed brackets in server"));
	}
}

void	Parser::parserSname(void) {
	this->consume();
	if (this->peek().type != word)
		throw (ErrException("Missing value in server_name"));
	while (this->peek().type == word)
		this->consume();
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after server_name"));
	this->consume();
};

void	Parser::parserListen(void) {
	this->consume();
	if (this->peek().type != ip && this->peek().type != int_lit)
		throw (ErrException("Invalid value in listen"));
	else if (this->peek().type == ip && !checkIp(this->peek().value.value())) // checkIp
		throw (ErrException("Invalid ip format in listen"));
	this->consume();
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after listen"));
	this->consume();
}

void	Parser::parserRoot(void) {
	std::ofstream	test;

	this->consume();
	if (this->peek().type != path)
		throw (ErrException("Invalid path in root"));
	test.open((this->peek().value.value() + std::string("/.test")).c_str(), std::ofstream::out);
	if (!test.is_open())
		throw (ErrException("Invalid path in root"));
	remove((this->peek().value.value() + std::string("/.test")).c_str());
	this->consume();
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after root"));
	this->consume();
}

void	Parser::parserUpload(void) {
	std::ofstream	test;

	this->consume();
	if (this->peek().type != path)
		throw (ErrException("Invalid path in upload"));
	test.open((this->peek().value.value() + std::string("/.test")).c_str(), std::ofstream::out);
	if (!test.is_open())
		throw (ErrException("Invalid path in upload"));
	remove((this->peek().value.value() + std::string("/.test")).c_str());
	this->consume();
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after upload"));
	this->consume();
}

void	Parser::parserErrPage(void) {
	std::ifstream	test;

	this->consume();
	if (this->peek().type != int_lit
			|| std::atoi(this->peek().value.value().c_str()) > 499
			|| std::atoi(this->peek().value.value().c_str()) < 400)
		throw (ErrException("Invalid error_code in err_page"));
	this->consume();
	if (this->peek().type != path) // il controllo del path verrá fatto successivamente
		throw (ErrException("Invalid path in err_page"));
	this->consume();
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after err_page"));
	this->consume();
}

void	Parser::parserMaxBody(void) {
	this->consume();
	if (this->peek().type != int_lit
			|| this->peek().value.value().length() >= 10)
		throw (ErrException("Invalid size in max_body_size"));
	this->consume();
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after max_body_size"));
	this->consume();
}

void	Parser::parserMethods(void) {
	std::string	value;

	this->consume();
	if (this->peek().type != word)
		throw (ErrException("Invalid method in methods"));
	while (this->peek().type != word) {
		value = this->consume().value.value();
		if (value != "GET" && value != "POST" && value != "DELETE")
			throw (ErrException("Unknown method in methods"));
	}
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after methods"));
	this->consume();
}

void	Parser::parserPrMethods(void) {
	std::string	value;

	this->consume();
	if (this->peek().type != word)
		throw (ErrException("Invalid method in prohibit_methods"));
	while (this->peek().type != word) {
		value = this->consume().value.value();
		if (value != "GET" && value != "POST" && value != "DELETE")
			throw (ErrException("Unknown method in prohibit_methods"));
	}
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after prohibit_methods"));
	this->consume();
}

void	Parser::parserRewrite(void) {
	std::ifstream	test;
	std::string		value;

	this->consume();
	if (this->peek().type != path) // il controllo del path verrá fatto successivamente
		throw (ErrException("Invalid path in rewrite"));
	this->consume();
	if (this->peek().type != path && this->peek().type != url) // il controllo del path verrá fatto successivamente
		throw (ErrException("Invalid param in rewrite"));
	this->consume();
	if (this->peek().type != word)
		throw (ErrException("Invalid method in prohibit_methods"));
	value = this->consume().value.value();
	if (value != "permanent" && value != "redirect")
		throw (ErrException("Unkonwn mode in prohibit_methods"));
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after rewrite"));
	this->consume();
}

void	Parser::parserCgi(void) {
	std::string	value;

	this->consume();
	if (this->peek().type != word)
		throw (ErrException("Invalid param in cgi"));
	value = this->consume().value.value();
	if (value != "python" && value != "exe" && value != "bash" && value != "php")
		throw (ErrException("Unknown type in cgi"));
	if (this->peek().type != word) // il controllo del path verrá fatto successivamente
		throw (ErrException("Invalid param in cgi"));
	this->consume();
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after cgi"));
	this->consume();
}

void	Parser::parserCgiBin(void) {
	std::ofstream	test;

	this->consume();
	if (this->peek().type != path)
		throw (ErrException("Invalid path in cgi-bin"));
	test.open((this->peek().value.value() + std::string("/.test")).c_str(), std::ofstream::out);
	if (!test.is_open())
		throw (ErrException("Invalid path in cgi-bin"));
	remove((this->peek().value.value() + std::string("/.test")).c_str());
	this->consume();
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after cgi-bin"));
	this->consume();
}

void	Parser::parserIndex(void) {
	this->consume();
	if (this->peek().type != word) // il controllo del path verrá fatto successivamente
		throw (ErrException("Invalid param in index"));
	this->consume();
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after index"));
	this->consume();
}

void	Parser::parserAutoindex(void) {
	std::string	value;

	this->consume();
	if (this->peek().type != word) // il controllo del path verrá fatto successivamente
		throw (ErrException("Invalid param in autoindex"));
	value = this->consume().value.value();
	if (value != "on" || value != "off")
		throw (ErrException("Unknown mode in autoindex"));
	value = this->consume().value.value();
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after autoindex"));
	this->consume();
}

void	Parser::parserLocation(void) {
	this->consume();
	if (this->peek().type != path // il controllo del path verrá fatto successivamente
			|| this->peek(1).type != open_cbr)
		throw (ErrException("Missing '{' after location"));
	this->consume(1);
	if (this->peek().type == close_cbr)
		throw (ErrException("Empty location"));
	while (this->peek().type != tnull && this->peek().type != close_cbr) {
		if (this->peek().type != word)
			throw (ErrException("Invalid statement in location"));
		(this->*(this->parsers.at(this->peek().value.value())))();
	}
	if (this->peek().type == tnull)
		throw (ErrException("Unclosed brackets in server"));
}

bool	Parser::checkIp(std::string ip) {
	int					dot_counter = 0;
	int					num;
	std::stringstream	sbuff;

	for (size_t i = 0; i < ip.length() ; i++) {
		if (!std::isdigit(ip[i]) && ip[i] != '.' && ip[i] != ':')
			return (false);
		else if ((dot_counter < 3 && ip[i] == '.')
				|| (dot_counter == 3 && ip[i] == ':')) {
			num = std::atoi(sbuff.str().c_str());
			if (sbuff.str().length() > 3 || num > 255)
				return (false);
			dot_counter++;
			sbuff.str("");
			sbuff.clear();
		}
		else if (std::isdigit(ip[i]))
			sbuff << ip[i];
		else
			return (false);
	}
	return (true);
}
