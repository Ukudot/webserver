/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macro.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 11:36:59 by gpanico           #+#    #+#             */
/*   Updated: 2023/09/07 16:33:22 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// utils
#define BUFFSIZE 1000
#define PORT 8123
#define BACKLOG 10
#define IP "10.12.3.3"
#define TIMEOUT 100
#define NPOS std::string::npos
#define CRLF std::string("\r\n")

// debug 
#define DEBUG_FLAG 1
#define DEBUG(str) do {\
	if (DEBUG_FLAG) \
		std::cout << "DEBUG LOG >> " << str << std::endl; \
	} while (0) 

// colors
#define RESET std::string("\033[0m")
#define GREEN std::string("\033[1;32m")
#define RED std::string("\033[1;31m")
#define CYAN std::string("\033[1;36m")
#define PURPLE std::string("\033[1;35m")

// error 
#define ERROR(str) do { std::cerr << "ERROR LOG >> " << str << std::endl; } while (0) 
