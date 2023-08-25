/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macro.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 11:36:59 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/25 08:35:53 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// utils
#define BUFFSIZE 1000
#define PORT 8123
#define BACKLOG 10
#define IP "10.12.3.3"
#define TIMEOUT 10000
#define NPOS std::string::npos

// debug 
#define DEBUG_FLAG 1
#define DEBUG(str) do {\
	if (DEBUG_FLAG) \
		std::cout << "DEBUG LOG >> " << str << std::endl; \
	} while (0) 

// error 
#define ERROR(str) do { std::cerr << "ERROR LOG >> " << str << std::endl; } while (0) 
