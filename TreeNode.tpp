/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreeNode.tpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 12:06:11 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/25 12:30:07 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <map>

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

template <typename T>
TreeNode<T> const	*TreeNode<T>::search(std::string name) const {
	
	for (size_t i = this->next.size() / 2; this->next[i]->getName() != name;) {
		if (name.compare(this->next[i]->getName()) > 0) {
			if ()
		}
	}

	for (size_t i = 0; i < this->next.size(); i++)
		if (this->next[i]->getName() == name)
			return (this->next[i]);
	
}

template <typename T>
void			TreeNode<T>::add(TreeNode *node);