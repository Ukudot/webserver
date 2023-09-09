/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tree.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 11:43:36 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/30 15:31:41 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "TreeNode.hpp"
#include "ErrException.hpp"

template <typename T>
class Tree {
	private:
		TreeNode<T>	*root;

		Tree(Tree const &tree);
		Tree const	&operator=(Tree const &tree);

	public:
		Tree(void);
		Tree(TreeNode<T> *root);
		~Tree(void);

		// functions
		TreeNode<T> *getRoot(void) const;
		TreeNode<T> const *getNode(std::vector<std::string> &names) const;
		void		addNode(TreeNode<T> *node, std::vector<std::string> const &names);
		void		print(TreeNode<T> *node, std::string const &name) const;
};

template <typename T>
Tree<T>::Tree(void): root(NULL) {}

template <typename T>
Tree<T>::Tree(TreeNode<T> *root): root(root) {}

template <typename T>
Tree<T>::~Tree(void) {
	delete (root);
}

template <typename T>
Tree<T>::Tree(Tree<T> const &tree) {
	*this = tree;
}

template <typename T>
Tree<T> const	&Tree<T>::operator=(Tree<T> const &tree) {
	(void) tree;
	return (*this);
}

template <typename T>
TreeNode<T>	*Tree<T>::getRoot(void) const {
	return (this->root);
}

template <typename T>
TreeNode<T>	const *Tree<T>::getNode(std::vector<std::string> &names) const {
	TreeNode<T>	const *node;
	std::vector<std::string>::iterator	ite;

	ite = names.begin();
	for (node = this->root; node && ite != names.end(); ite++) {
		node = node->search(*(ite));
	}
	return (node);
}

template <typename T>
void		Tree<T>::addNode(TreeNode<T> *node, std::vector<std::string> const &names) {
	TreeNode<T>	*current;

	current = this->getNode(names);
	if (!current)
		throw (ErrException("Tree::addNode() failed"));
	current->add(node);
}
template <typename T>
void	Tree<T>::print(TreeNode<T> *node, std::string const &name) const {
	std::vector<TreeNode<T> *>	nodes;

	if (!node) {
		node = this->root;
		this->root->setName(name);
	}
	nodes = node->getNext();
	for (size_t i = 0; i < nodes.size(); i++)
		this->print(nodes[i], node->getName());
	std::cout << "parent: " << name << " -> " << node->getName() << std::endl;

}
