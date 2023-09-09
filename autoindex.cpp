/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 10:32:30 by adi-stef          #+#    #+#             */
/*   Updated: 2023/09/09 14:31:11 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sstream>
#include <iostream>
#include "Macro.hpp"
#define PATH std::string("/home/aldisti/")

#include <vector>
#include <dirent.h>

#define DIR_TEMPLATE(url, dirName) std::string("<h2><a href=\"") + url + "\">" + dirName + "</a></h2>"
#define FILE_TEMPLATE(fileName) std::string("<h2>") + fileName + "</h2>"
#define URL_TEMPLATE(host, path) std::string("http://") + host + path

typedef struct s_file {
    std::string     name;
    unsigned char   type;
    s_file() {
        this->name = "";
        this->type = 0;
    }
    s_file(std::string name, unsigned char type): name(name), type(type) {;}
}   t_file;

int main(void) {
    DIR                 *dir;
    struct dirent       *entry;
    std::vector<t_file> files;

    if ((dir = opendir(PATH.c_str())) == NULL)
        std::cerr << "cannot open dir" << std::endl;
    else {
        while ((entry = readdir(dir)) != NULL) {
            if ((entry->d_type == DT_DIR || entry->d_type == DT_REG)
                && (entry->d_name[0] != '.' || !entry->d_name[1]
                || (entry->d_name[1] == '.' && !entry->d_name[2])))
                files.push_back(t_file(entry->d_name, entry->d_type));
        }
        closedir(dir);
    }

    std::string output = "";

    for (size_t i = 0; i < files.size(); i++) {
        if (files[i].type == DT_DIR) {
            output += DIR_TEMPLATE(URL_TEMPLATE("localhost:8000", PATH + "/" + files[i].name), files[i].name) + "\n";
        }
        else
            output += FILE_TEMPLATE(files[i].name) + "\n";
        std::cout << files[i].name << std::endl;
    }
    // std::cout << output << std::endl;
}
