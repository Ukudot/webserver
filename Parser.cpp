/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:59:46 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/31 16:55:24 by gpanico          ###   ########.fr       */
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
	this->parsers["error_page"] = &Parser::parserErrPage;
	this->parsers["client_max_body_size"] = &Parser::parserMaxBody;
	this->parsers["allow_methods"] = &Parser::parserMethods;
	this->parsers["prohibit_methods"] = &Parser::parserPrMethods;
	this->parsers["rewrite"] = &Parser::parserRewrite;
	this->parsers["cgi"] = &Parser::parserCgi;
	this->parsers["cgi-bin"] = &Parser::parserCgiBin;
	this->parsers["index"] = &Parser::parserIndex;
	this->parsers["autoindex"] = &Parser::parserAutoindex;
	this->parsers["location"] = &Parser::parserLocation;
}

Parser::~Parser(void) {
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
	this->nodes = par.nodes;
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


std::vector<TreeNode<t_node> *>	Parser::parse(void) {
	t_node data, empty;
	
	while (this->peek().type != tnull) {
		if (!this->peek().value.hasValue()
				|| this->peek().value.value() != "server"
				|| this->peek(1).type != open_cbr)
			throw (ErrException("Missing '{' after server"));
		this->consume(1);
		if (this->peek().type == close_cbr)
			throw (ErrException("Empty server"));
		data = empty;
		this->nodes.push_back(new TreeNode<t_node>());
		while (this->peek().type != tnull && this->peek().type != close_cbr) {
			std::cout << "DEBUG>> " << this->peek().value.value() << std::endl;
			if (this->peek().type != word)
				throw (ErrException("Invalid statement in server"));
			(this->*(this->parsers.at(this->peek().value.value())))(data);
		}
		if (this->peek().type == tnull)
			throw (ErrException("Unclosed brackets in server"));
		this->nodes.back()->setData(data);
		this->consume();
	}
	return (this->nodes);
}

void	Parser::parserSname(t_node &data) {
	this->consume();
	if (this->peek().type != word)
		throw (ErrException("Missing value in server_name"));
	while (this->peek().type == word)
		data.servNames.push_back(this->consume().value.value());
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after server_name"));
	this->consume();
};

void	Parser::parserListen(t_node &data) {
	std::string	value;

	this->consume();
	if (this->peek().type != ip && this->peek().type != int_lit)
		throw (ErrException("Invalid value in listen"));
	else if (this->peek().type == ip && !checkIp(this->peek().value.value()))
		throw (ErrException("Invalid ip format in listen"));
	value = this->peek().value.value();
	if (this->peek().type == int_lit)
		data.port = std::atoi(value.c_str());
	else {
		data.host = value.substr(0, value.find(':'));
		data.port = std::atoi(value.substr(value.find(':') + 1).c_str());
	}
	this->consume();
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after listen"));
	this->consume();
}

void	Parser::parserRoot(t_node &data) {
	this->consume();
	if (this->peek().type != path)
		throw (ErrException("Invalid path in root"));
	data.root = this->consume().value.value();
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after root"));
	this->consume();
}

void	Parser::parserUpload(t_node &data) {
	this->consume();
	if (this->peek().type != path)
		throw (ErrException("Invalid path in upload"));
	data.upload = this->consume().value.value();
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after upload"));
	this->consume();
}

void	Parser::parserErrPage(t_node &data) {
	int	code;

	this->consume();
	if (this->peek().type != int_lit
			|| std::atoi(this->peek().value.value().c_str()) > 599
			|| std::atoi(this->peek().value.value().c_str()) < 400)
		throw (ErrException("Invalid error_code in err_page"));
	code = std::atoi(this->consume().value.value().c_str());
	if (this->peek().type != path)
		throw (ErrException("Invalid path in err_page"));
	data.errPages[code] = this->consume().value.value();
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after err_page"));
	this->consume();
}

void	Parser::parserMaxBody(t_node &data) {
	this->consume();
	if (this->peek().type != int_lit
			|| this->peek().value.value().length() >= 10)
		throw (ErrException("Invalid size in max_body_size"));
	data.maxBodySize = std::atoi(this->consume().value.value().c_str());
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after max_body_size"));
	this->consume();
}

void	Parser::parserMethods(t_node &data) {
	std::string	value;

	this->consume();
	if (this->peek().type != word)
		throw (ErrException("Invalid method in methods"));
	while (this->peek().type == word) {
		value = this->consume().value.value();
		if (value == "GET")
			data.methods |= 1;
		else if (value == "POST")
			data.methods |= 2;
		else if (value == "DELETE")
			data.methods |= 4;
		else
			throw (ErrException("Unknown method in methods"));
	}
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after methods"));
	this->consume();
}

void	Parser::parserPrMethods(t_node &data) {
	std::string	value;

	this->consume();
	if (this->peek().type != word)
		throw (ErrException("Invalid method in prohibit_methods"));
	while (this->peek().type == word) {
		value = this->consume().value.value();
		if (value == "GET")
			data.methods &= ~1;
		else if (value == "POST")
			data.methods &= ~2;
		else if (value == "DELETE")
			data.methods &= ~4;
		else
			throw (ErrException("Unknown method in methods"));
	}
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after prohibit_methods"));
	this->consume();
}

void	Parser::parserRewrite(t_node &data) {
	t_redirect		red;
	std::string		value;

	this->consume();
	if (this->peek().type != path)
		throw (ErrException("Invalid path in rewrite"));
	red.src = this->consume().value.value();
	if (this->peek().type != path && this->peek().type != url)
		throw (ErrException("Invalid param in rewrite"));
	red.dst = this->consume().value.value();
	if (this->peek().type != word)
		throw (ErrException("Invalid method in prohibit_methods"));
	value = this->consume().value.value();
	if (value != "permanent" && value != "redirect")
		throw (ErrException("Unkonwn mode in prohibit_methods"));
	red.type = value.at(0);
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after rewrite"));
	this->consume();
	data.redirections.push_back(red);
}

void	Parser::parserCgi(t_node &data) {
	t_cgi		cgi;
	std::string	value;

	this->consume();
	if (this->peek().type != word)
		throw (ErrException("Invalid param in cgi"));
	value = this->consume().value.value();
	if (value != "python" && value != "exe" && value != "bash" && value != "php")
		throw (ErrException("Unknown type in cgi"));
	cgi.type = value;
	if (this->peek().type != word)
		throw (ErrException("Invalid param in cgi"));
	cgi.eName = this->consume().value.value();
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after cgi"));
	this->consume();
	data.cgis.push_back(cgi);
}

void	Parser::parserCgiBin(t_node &data) {
	this->consume();
	if (this->peek().type != path)
		throw (ErrException("Invalid path in cgi-bin"));
	data.cgiBin = this->consume().value.value();
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after cgi-bin"));
	this->consume();
}

void	Parser::parserIndex(t_node &data) {
	this->consume();
	if (this->peek().type != word)
		throw (ErrException("Invalid param in index"));
	while (this->peek().type == word)
		data.index.push_back(this->consume().value.value());
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after index"));
	this->consume();
}

void	Parser::parserAutoindex(t_node &data) {
	std::string	value;

	this->consume();
	if (this->peek().type != word)
		throw (ErrException("Invalid param in autoindex"));
	value = this->consume().value.value();
	if (value != "on" && value != "off")
		throw (ErrException("Unknown mode in autoindex"));
	data.autoindex = (value == "on") ? true : false;
	if (this->peek().type != semi)
		throw (ErrException("Missing ';' after autoindex"));
	this->consume();
}

void	Parser::parserLocation(t_node &data) {
	std::string			value;

	(void) data;
	this->consume();
	if (this->peek().type != path
			|| this->peek(1).type != open_cbr)
		throw (ErrException("Missing '{' after location"));
	value = this->consume().value.value();
	this->consume();
	if (this->peek().type == close_cbr)
		throw (ErrException("Empty location"));
	t_node data_location;
	this->nodes.back()->add(new TreeNode<t_node>(value, data_location));
	while (this->peek().type != tnull && this->peek().type != close_cbr) {
		if (this->peek().type != word || this->peek().value.value() == "location")
			throw (ErrException("Invalid statement in location"));
		(this->*(this->parsers.at(this->peek().value.value())))(data_location);
	}
	if (this->peek().type == tnull)
		throw (ErrException("Unclosed brackets in location"));
	this->consume();
	this->nodes.back()->setData(data_location);
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
