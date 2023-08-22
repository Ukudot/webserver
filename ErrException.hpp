/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrException.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 11:29:20 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/11 15:15:00 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string.h>
#include <string>
#include "Utils.hpp"

class	ErrException: public std::exception
{
	private:
		std::string	*_message;

	public:
		ErrException(const char * message) {
			this->_message = Utils::ft_strdup(message);
		}
		virtual	~ErrException() throw() {
			delete this->_message;
		}
		virtual const char * what() const throw() {
			return (this->_message->c_str());
		}
};
