/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 13:45:08 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/25 16:57:05 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>

std::string	search(std::vector<std::string> vect, std::string name) {
	size_t	start, end, i;
	int		c = 0;

	start = 0;
	end = vect.size() - 1;
	while (true) {
		i = (start + end) / 2;
		std::cout << "s: " << start << " e: " << end << " i: " << i << std::endl;
		if (vect[i] == name)
			return (vect[i]);
		if (i == start || i == end)
			break ;
		if (name.compare(vect[i]) > 0)
			start = i;
		else
			end = i;
	}
	return (std::to_string(i));
}

int	main(void) {
	std::vector<std::string>	vect;
	std::string					tmp = "000";

	for (int a = 65; a < 68; a++)
		for (int b = 70; b < 73; b++)
			for (int c = 75; c < 78; c++) {
				tmp[0] = (char) a;
				tmp[1] = (char) b;
				tmp[2] = (char) c;
				vect.push_back(tmp);
			}
	for (int i = 0; i < (int) vect.size(); i++)
		std::cout << vect[i] << " ";
	std::cout << std::endl;
	std::cout << "s: " << vect.size() << std::endl;
	std::cout << "|" << (search(vect, "BAJ")) << "|" << std::endl;
	// vect.push_back("aaa");
	// vect.push_back("bbb");
	// vect.push_back("ccc");
	// vect.insert(vect.begin() + 1, "ddd");
	// for (int i = 0; i < (int) vect.size(); i++)
	// 	std::cout << vect[i] << std::endl;
	return (0);
}
