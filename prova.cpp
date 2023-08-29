#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Tokenizer.hpp"
#include "Optional.hpp"
#include "Parser.hpp"

int main(int argc, char *argv[]) {
	std::ifstream		ifs;
	std::stringstream	sbuff;
	std::string			buff;


	if (argc != 2) {
		std::cerr << "Ussage error" << std::endl;
		return (1);
	}
	// convert a file into a string
	ifs.open(argv[1], std::ifstream::in);
	if (!ifs.is_open() || !ifs.good()) {
		std::cerr << "there was a problem with passed file" << std::endl;
		return (1);
	}
	sbuff << ifs.rdbuf();
	ifs.close();
	buff = sbuff.str();
	std::cout << buff << std::endl;

	// new tokenizer
	std::vector<token>	tokens;
	Tokenizer			tokenizer(buff);

	try {
		tokens = tokenizer.tokenize();
	}
	catch (std::exception &e) {
		std::cout << "Error at line: " << tokenizer.getNoLine() << std::endl;
		std::cout << "line: " << tokenizer.getLine() << std::endl;
		std::cout << e.what() << std::endl;
	}
	for (size_t i = 0; i < tokens.size(); i++ )
		if (tokens[i].value.hasValue())
			std::cout << tokens[i].value.value() << std::endl;

	// parser
/*
	for (size_t i = 0; i < tokens.size(); i++) {
		if (!tokens[i].value.hasValue() || tokens[i].value.value() != "server"
				|| tokens[i + 1].type != open_cbr) {
			std::cout << "Bad server syntax" << std::endl;
			return (1);
		}
		i += 2;
		while (i < tokens.size()) {
			if (!tokens[i].value.hasValue() && tokens[i].type != close_cbr) {
				std::cout << "Bad syntax: no value" << std::endl;
				return (1);
			}
			else if (tokens[i].type == close_cbr) {
				break ;
			}
			else if (i + 2 < tokens.size()
					&& tokens[i].value.value() == "location"
					&& tokens[i + 1].type == path
					&& tokens[i + 2].type == open_cbr) {
				std::cout << "location found with path: " << tokens[i + 1].value.value() << std::endl;
				i += 2;
				while (i < tokens.size() && tokens[i].type != close_cbr)
					i++;
				if (tokens[i].type != close_cbr) {
					std::cout << "Bad syntax: no '}' after location" << std::endl;
					return (1);
				}
			}
			else if (tokens[i].value.value() == "server_name") {
				std::cout << tokens[i].value.value() << ":";
				i++;
				while (i < tokens.size() && tokens[i].type == word) {
					std::cout << " " << tokens[i].value.value();
					i++;
				}
				std::cout << std::endl;
				if (tokens[i].type != semi) {
					std::cout << "Bad syntax: no ';' after server_name" << std::endl;
					return (1);
				}
			}
			else if (i + 2 < tokens.size()
					&& tokens[i].value.value() == "listen"
					&& (tokens[i + 1].type == ip || tokens[i + 1].type == int_lit)) {
				if (tokens[i + 2].type != semi) {
					std::cout << "Bad syntax: no ';' after listen" << std::endl;
					return (1);
				}
				std::cout << "listen: " << tokens[i + 1].value.value() << std::endl;
				i += 2;
			}
			else {
				std::cout << "Invalid statement" << std::endl;
				while (i < tokens.size() && tokens[i].type != semi)
					i++;
				//return (1);
			}
			i++;
		}
	}
*/
	// new parser
	Parser	parser(tokens);

	try {
		parser.parse();
	}
	catch (std::exception &e){
		std::cout << e.what() << std::endl;
	}
	return (0);
}
