#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Tokenizer.hpp"
#include "Optional.hpp"
#include "Parser.hpp"
#include "TreeNode.hpp"
#include "Tree.hpp"

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

	// new parser
	Parser	parser(tokens);
	std::vector<TreeNode<t_node> *>	nodes;
	Tree<t_node>					*tree;

	try {
		nodes = parser.parse();
	}
	catch (std::exception &e){
		std::cout << e.what() << std::endl;
	}

	for (size_t i = 0; i < tokens.size(); i++ )
		if (tokens[i].value.hasValue())
			std::cout << tokens[i].value.value() << std::endl;

	for (size_t i = 0; i < nodes.size(); i++) {
		tree = new Tree<t_node>(nodes[i]);
		tree->print(NULL, "server");
		delete (tree);
	}

	return (0);
}
