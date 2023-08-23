/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 09:37:05 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/23 12:18:58 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include <ctime>
#include "Macro.hpp"

class Utils
{
	private:
		Utils(void);
		~Utils(void);
	public:
		static std::vector<std::string>	ft_split(std::string str, std::string del);
		static std::string				*ft_strdup(const char *c);
		static std::string				ft_toupper(std::string str);
		static std::string				ft_tolower(std::string str);
		static long						ft_gettime(void);
		static bool						ft_isdigit(std::string str);
		static void						*ft_memset(void *b, int c, size_t n);
		static int						ft_strcmp(const char *s1, const char *s2);
		template <typename T>
		static std::string				ft_itoa(T num);
		
};

template <typename T>
std::string	Utils::ft_itoa(T num)
{
	std::ostringstream ss;
	ss << num;
	return (ss.str());
}
