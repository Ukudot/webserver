/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreeNode.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 12:06:11 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/30 15:31:24 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <map>
#include "ErrException.hpp"

typedef struct s_cgi {
	std::string	type;
	std::string	eName;
}	t_cgi;

typedef struct s_redirect {
	std::string	src;
	std::string	dst;
	char		type;
}	t_redirect;

typedef struct s_node {
	std::vector<std::string>	servNames;
	std::string					host;
	int							port;
	std::string					root;
	std::map<int, std::string>	errPages;
	int							maxBodySize;
	short						methods;
	std::string					upload;
	bool						autoindex;
	std::vector<std::string>	index;
	std::vector<t_redirect>		redirections;
	std::vector<t_cgi>			cgis;
	std::string					cgiBin;
}	t_node;

template <typename T>
class TreeNode {
	private:
		std::string				name;
		T						data;
		std::vector<TreeNode *>	next;

		// functions
		size_t	getIndexByName(std::string name) const;
		size_t	getIndexToInsert(std::string name) const;

	public:
		TreeNode(std::string name, T data);
		~TreeNode(void);
		TreeNode(TreeNode const &trn);
		TreeNode const	&operator=(TreeNode const &trn);
		
		// getters
		std::string const				&getName(void) const;
		T const							&getData(void) const;
		std::vector<TreeNode *> const	&getNext(void) const;

		// functions
		TreeNode const	*search(std::string name) const;
		void			add(TreeNode *node);
};

template <typename T>
TreeNode<T>::TreeNode(std::string name, T data): name(name), data(data) {
	return ;
}

template <typename T>
TreeNode<T>::~TreeNode(void) {
	for (size_t i = 0; i < this->next.size(); i++)
		delete this->next[i];
	this->next.clear();
}

template <typename T>
TreeNode<T>::TreeNode(TreeNode<T> const &trn) {
	*this = trn;
}

template <typename T>
TreeNode<T> const	&TreeNode<T>::operator=(TreeNode<T> const &trn) {
	if (this == &trn)
		return (*this);
	this->name = trn.getName();
	this->data = trn.getData();
	for (size_t i = 0; i < this->next.size(); i++)
		delete this->next[i];
	this->next.clear();
	this->next = trn.getNext();
	return (*this);
}

template <typename T>
std::string const				&TreeNode<T>::getName(void) const {
	return (this->name);
}

template <typename T>
T const							&TreeNode<T>::getData(void) const {
	return (this->data);
}

template <typename T>
std::vector<TreeNode<T> *> const	&TreeNode<T>::getNext(void) const {
	return (this->next);
}

// #####################################################################################

template <typename T>
size_t	TreeNode<T>::getIndexByName(std::string name) const {
	size_t	start, end, i;

	start = 0;
	end = this->next.size() - 1;
	if (name == this->next[end]->getName())
		return (end);
	while (true) {
		i = (start + end) / 2;
		if (this->next[i]->getName() == name)
			return (i);
		if (i == start || i == end)
			break ;
		if (name.compare(this->next[i]->getName()) > 0)
			start = i;
		else
			end = i;
	}
	return (-1);
}

template <typename T>
TreeNode<T> const	*TreeNode<T>::search(std::string name) const {
	int	i;

	i = this->getIndexByName(name);
	if (i == -1)
		return (NULL);
	return (this->next[i]);
}

/*
std::cout << "before[" << i - 1 << "]: " << this->next[i - 1]->getName()
<< " present [" << i << "]: " << this->next[i]->getName()
<< " after[" << i + 1 << "]: " << this->next[i + 1]->getName() << std::endl;
std::cout << "before[" << i - 1 << "]: " << name.compare(this->next[i - 1]->getName())
<< " present [" << i << "]: " << name.compare(this->next[i]->getName())
<< " after[" << i + 1 << "]: " << name.compare(this->next[i + 1]->getName()) << std::endl;
std::cout << std::endl;
*/
template <typename T>
size_t	TreeNode<T>::getIndexToInsert(std::string name) const {
	size_t	start, end, i;

	start = 0;
	end = this->next.size() - 1;
	if (this->next.size() == 0 || name.compare(this->next[0]->getName()) < 0)
		return (0);
	else if (name.compare(this->next[end]->getName()) > 0)
		return (end + 1);
	while (true) {
		i = (start + end) / 2;
		if (this->next[i]->getName() == name)
			return (i);
		if (name.compare(this->next[i]->getName()) < 0
				&& (i > 0 && name.compare(this->next[i - 1]->getName()) > 0))
			return (i);
		else if (name.compare(this->next[i]->getName()) > 0
				&& (i < this->next.size() && name.compare(this->next[i + 1]->getName()) < 0))
			return (i + 1);
		if (i == start || i == end)
			break ;
		if (name.compare(this->next[i]->getName()) > 0)
			start = i;
		else
			end = i;
	}
	return (this->next.size() + 1);
}

template <typename T>
void			TreeNode<T>::add(TreeNode<T> *node) {
	size_t	index;

	index = this->getIndexToInsert(node->getName());
	if (index > this->next.size()) {
		std::cerr << "invalid index in 'add'" << std::endl;
		std::cerr << "trying to insert '" << node->getName() << "'" << std::endl;
		std::cerr << "vect:";
		for (size_t i = 0; i < this->next.size(); i++)
			std::cerr << " " << this->next[i]->getName();
		std::cerr << std::endl;
		throw (ErrException("TreeNode::add() failed"));
	}
	// si puó splittare
	this->next.insert(this->next.begin() + index, node);
}
