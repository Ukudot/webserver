/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tree.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 11:43:36 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/30 12:22:42 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "TreeNode.tpp"
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
		TreeNode<T>	*getRoot(void) const;
		TreeNode<T>	*getNode(std::string const &name) const;
		void		addNode(TreeNode<T> *node, std::string const &name);
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
Tree const	&Tree<T>::operator=(Tree<T> const &tree) {}

template <typename T>
TreeNode<T>	*Tree<T>::getRoot(void) const {
	return (this->root);
}

template <typename T>
TreeNode<T>	*Tree<T>::getNode(std::vector<std::string> const &names) const {
	TreeNode<T>	*node;
	std::vector<std::string>::iterator	ite;

	ite = names.begin();
	node = this->root;
	while (node && ite != names.end())
		node = node.search(*(ite++));
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
