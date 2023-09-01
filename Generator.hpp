/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Generator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 08:34:22 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/31 09:44:03 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Tree.hpp"
#include "TreeNode.hpp"
#include "ErrException.hpp"
#include "Tokenizer.hpp"

class	Generator {
	private:

		Generator(Generator const &gen);
		Generator	&operator=(Generator const &gen);

	public:
		Generator(std::vector<token>);
		~Generator(void);

		Tree<t_node>	generate(void);
};

std::map<std::string, std::map<std::string, std::vector<std::string> > >
