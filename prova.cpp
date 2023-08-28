#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Optional.hpp"

int main(int argcm, char *argv[]) {
	std::ifstream		ifs;
	std::stringstream	sbuff;
	std::string			buff;

	enum class	tokenType {stm, word, path, ip, url, semi, int_lit, open_cbr, close_cbr};

	struct token {
		tokenType				type;
		Optional<std::string>	value;
	};

	// convert a file into a string
	ifs.open(argv[1], std::ifstream::in);
	if (!ifs.is_open() || !ifs.good()) {
		std::cerr << "there was a problem with passed file" << std::endl;
		exit(1);
	}
	sbuff << ifs.rdbuf();
	ifs.close();
	buff = sbuff.str();
	std::cout << buff << std::endl;

	//tokenize
	std::stringstream	tmp;
	token				tmpToken;
	std::vector<token>	tokens;
	int					line = 1;


	for (size_t i = 0; i < buff.length(); i++) {
		if (std::isalpha(buff[i])) {
			tmp << buff[i];
			i++;
			tmpToken.type = tokenType::word;
			if (buff[i - 1] == 'h' && buff.substr(i - 1, 7) == "http://")
				tmpToken.type = tokenType::url;
			while (i < buff.length() && (std::isalnum(buff[i]) || std::string("/-._").find(buff[i]) != std::string::npos)
					|| (tmpToken.type == tokenType::url && buff[i] == ':')) {
				tmp << buff[i];
				i++;
			}
			tmpToken.value = Optional<std::string>(tmp.str());
			tmp.str("");
			tmp.clear();
			tokens.push_back(tmpToken);
			i--;
			continue;
		}
		else if (buff[i] == '/') {
			tmp << buff[i];
			i++;
			while (i < buff.length() && (std::isalnum(buff[i]) || std::string("/-._").find(buff[i]) != std::string::npos)) {
				tmp << buff[i];
				i++;
			}
			tmpToken.type = tokenType::path;
			tmpToken.value = Optional<std::string>(tmp.str());
			tmp.str("");
			tmp.clear();
			tokens.push_back(tmpToken);
			i--;
			continue;
		}
		else if (std::isdigit(buff[i])) {
			tmp << buff[i];
			i++;
			tmpToken.type = tokenType::int_lit;
			for (size_t j = 0; i + j < buff.length() && (std::isdigit(buff[i + j])
						|| std::string(".:").find(buff[i + j]) != std::string::npos); j++) {
				if (std::string(".:").find(buff[i + j]))
					tmpToken.type = tokenType::ip;
			}
			while (i < buff.length() && (std::isdigit(buff[i])
						|| std::string(".:").find(buff[i]) != std::string::npos)) {
				tmp << buff[i];
				i++;
			}
			tmpToken.value = Optional<std::string>(tmp.str());
			tmp.str("");
			tmp.clear();
			tokens.push_back(tmpToken);
			i--;
			continue;
		}
		else if (std::isspace(buff[i])) {
			while (std::isspace(buff[i])) {
				if (buff[i] == '\n')
					line++;
				i++;
			}
			i--;
			continue;
		}
		else if (buff[i] == ';') {
			i++;
			if (buff[i] != '\n') {
				std::cout << "Unrecognize char: " << buff[i] << " at line " << line << std::endl;
				std::cout << "hint: " << buff.substr(i - 2, 5) << std::endl;
				exit(1);
			}
			line++;
			tmpToken.type = tokenType::semi;
			tmpToken.value = Optional<std::string>();
			tokens.push_back(tmpToken);
			continue;
		}
		else if (buff[i] == '{') {
			tmpToken.type = tokenType::open_cbr;
			tmpToken.value = Optional<std::string>();
			tokens.push_back(tmpToken);
			continue;
		}
		else if (buff[i] == '}') {
			tmpToken.type = tokenType::close_cbr;
			tmpToken.value = Optional<std::string>();
			tokens.push_back(tmpToken);
			continue;
		}
		else {
			std::cerr << "Unrecognize char: " << buff[i] << " at line " << line << std::endl;
			std::cerr << "hint: " << buff.substr(i - 2, 5) << std::endl;
			exit(1);
		}
	}

	for (size_t i = 0; i < tokens.size(); i++)
		if (tokens[i].value.hasValue())
			std::cout << tokens[i].value.value() << std::endl;

	// parser

	for (size_t i = 0; i < tokens.size(); i++) {
		if (!tokens[i].value.hasValue() || tokens[i].value.value() != "server"
				|| tokens[i + 1].type != tokenType::open_cbr) {
			std::cout << "Bad server syntax" << std::endl;
			exit(1);
		}
		i += 2;
		while (i < tokens.size()) {
			if (!tokens[i].value.hasValue() && tokens[i].type != tokenType::close_cbr) {
				std::cout << "Bad syntax: no value" << std::endl;
				exit(1);
			}
			else if (tokens[i].type == tokenType::close_cbr) {
				break ;
			}
			else if (i + 2 < tokens.size()
					&& tokens[i].value.value() == "location"
					&& tokens[i + 1].type == tokenType::path
					&& tokens[i + 2].type == tokenType::open_cbr) {
				std::cout << "location found with path: " << tokens[i + 1].value.value() << std::endl;
				i += 2;
				while (i < tokens.size() && tokens[i].type != tokenType::close_cbr)
					i++;
				if (tokens[i].type != tokenType::close_cbr) {
					std::cout << "Bad syntax: no '}' after location" << std::endl;
					exit(1);
				}
			}
			else if (tokens[i].value.value() == "server_name") {
				std::cout << tokens[i].value.value() << ":";
				i++;
				while (i < tokens.size() && tokens[i].type == tokenType::word) {
					std::cout << " " << tokens[i].value.value();
					i++;
				}
				std::cout << std::endl;
				if (tokens[i].type != tokenType::semi) {
					std::cout << "Bad syntax: no ';' after server_name" << std::endl;
					exit(1);
				}
			}
			else if (i + 2 < tokens.size()
					&& tokens[i].value.value() == "listen"
					&& (tokens[i + 1].type == tokenType::ip || tokens[i + 1].type == tokenType::int_lit)) {
				if (tokens[i + 2].type != tokenType::semi) {
					std::cout << "Bad syntax: no ';' after listen" << std::endl;
					exit(1);
				}
				std::cout << "listen: " << tokens[i + 1].value.value() << std::endl;
				i += 2;
			}
			else {
				std::cout << "Invalid statement" << std::endl;
				while (i < tokens.size() && tokens[i].type != tokenType::semi)
					i++;
				//exit(1);
			}
			i++;
		}
	}
	return (0);
}
