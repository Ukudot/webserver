/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tree.tpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 11:43:36 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/25 12:06:37 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "TreeNode.tpp"

template <typename T>
class Tree {
	private:
		

	public:
		Tree(void);
		~Tree(void);
		Tree(Tree const &tree);
		Tree const	&operator=(Tree const &tree);

};


