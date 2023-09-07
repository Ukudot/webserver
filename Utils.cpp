/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 10:46:40 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/07 15:28:59 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "ErrException.hpp"

std::vector<std::string>	Utils::ft_split(std::string str, std::string del)
{
	std::vector<std::string>	splitted;
	int							i = 0;
	int							j = 0;

	while (i < (int) str.size())
	{
		while (str.substr(j, del.size()) == del)
			j += del.size();
		i = str.find(del, j);
		if (i == (int) NPOS)
		{
			if (str.substr(j).size())
				splitted.push_back(str.substr(j));
			break ;
		}
		splitted.push_back(str.substr(j, i - j));
		j = i + del.size();
	}
	return (splitted);
}

std::string	*Utils::ft_strdup(const char *str)
{
	std::string	*copy;

	copy = new std::string(str);
	return (copy);
}

std::string	Utils::ft_toupper(std::string str)
{
	for (int i = 0; i < (int) str.size(); i++)
		str[i] = std::toupper(str[i]);
	return (str);
}

std::string	Utils::ft_tolower(std::string str)
{
	for (int i = 0; i < (int) str.size(); i++)
		str[i] = std::tolower(str[i]);
	return (str);
}

long	Utils::ft_gettime(void)
{
	time_t	now;

	std::time(&now);
	return (((long)((int) now)));
}

bool	Utils::ft_isdigit(std::string str)
{
	for (int i = 0; i < (int) str.length(); i++)
		if (!std::isdigit(str[i]))
			return (false);
	return (true);
}

void	*Utils::ft_memset(void *b, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;

	str = (unsigned char *) b;
	i = 0;
	while (i < n)
		str[i++] = c;
	return (b);
}

int	Utils::ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

std::string		Utils::ft_readFile(std::string file) {
    std::stringstream	read;
    std::ifstream       ifs;

    ifs.open(file.c_str(), std::ifstream::in);
    if (!ifs.is_open() || !ifs.good())
        throw (ErrException("Error file not found"));
    read << ifs.rdbuf();
    ifs.close();
	return (read.str());
}
