/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 13:45:08 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/29 14:41:08 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>

int	search(std::vector<std::string> vect, std::string name) {
	size_t	start, end, i;

	start = 0;
	end = vect.size() - 1;
	if (vect.size() == 0 || name.compare(vect[0]) < 0)
		return (0);
	else if (name.compare(vect[end]) > 0)
		return (end + 1);
	while (true) {
		i = (start + end) / 2;
		if (vect[i] == name)
			return (i);
		if (i == start || i == end)
			break ;
		std::cout << "before[" << i - 1 << "]: " << vect[i - 1]
		<< " present [" << i << "]: " << vect[i]
		<< " after[" << i + 1 << "]: " << vect[i + 1] << std::endl;
		std::cout << "before[" << i - 1 << "]: " << name.compare(vect[i - 1])
		<< " present [" << i << "]: " << name.compare(vect[i])
		<< " after[" << i + 1 << "]: " << name.compare(vect[i + 1]) << std::endl;
		std::cout << std::endl;
		if (name.compare(vect[i]) < 0
				&& (i > 0 && name.compare(vect[i - 1]) > 0))
			return (i);
		else if (name.compare(vect[i]) > 0
				&& (i < vect.size() && name.compare(vect[i + 1]) < 0))
			return (i + 1);
		if (name.compare(vect[i]) > 0)
			start = i;
		else
			end = i;
	}
	return (-1);
}

int	main(void) {
	std::vector<std::string>	vect;
	std::string					tmp = "000";
	int							index;

	// for (int a = 65; a < 68; a++)
	// 	for (int b = 70; b < 73; b++)
	// 		for (int c = 75; c < 78; c++) {
	// 			tmp[0] = (char) a;
	// 			tmp[1] = (char) b;
	// 			tmp[2] = (char) c;
	// 			vect.push_back(tmp);
	// 		}
	// vect.push_back("A");
	// vect.push_back("B");
	// vect.push_back("C");
	// vect.push_back("D");
	// vect.push_back("E");
	// vect.push_back("F");
	// vect.push_back("G");
	// vect.push_back("H");
	// vect.push_back("I");
	// vect.push_back("L");
	// vect.push_back("M");
	// vect.push_back("N");
	#define S std::string("M")
	for (int i = 0; i < (int) vect.size(); i++)
		std::cout << vect[i] << " ";
	std::cout << std::endl;
	std::cout << "s: " << vect.size() << std::endl;
	index = search(vect, S);
	std::cout << index << std::endl;
	if (index >= 0)
		vect.insert(vect.begin() + index, S);
	// vect.push_back("aaa");
	// vect.push_back("bbb");
	// vect.push_back("ccc");
	// vect.insert(vect.begin() + 1, "ddd");
	for (int i = 0; i < (int) vect.size(); i++)
		std::cout << vect[i] << " ";
	return (0);
}
