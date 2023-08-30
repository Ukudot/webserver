/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:51:33 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/30 15:58:45 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServSocket.hpp"
#include "TreeNode.hpp"
#include "Tree.hpp"

/*
int	main(void)
{
	struct sockaddr_in	addr;
	int					addrlen = sizeof(struct sockaddr);
	int					sfd;
	int					on = 1;
	char				buff[BUFFSIZE];
	int					fds[BACKLOG];

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0 || setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on)))
		return (1);
	if (bind(sfd,(struct sockaddr *) &addr, sizeof(addr)))
		return (2);
	if (listen(sfd, BACKLOG))
		return (3);
	on = 0;
	while (42)
	{
		std::cout << "before accepted" << std::endl;
		if ((fds[on] = accept(sfd, (struct sockaddr *) &addr, (socklen_t *) &addrlen)) == -1)
			continue ;
		std::cout << "accepted" << std::endl;
		on++;
		for (int i = 0; i < on; i++)
		{
			memset((void *) buff, 0, BUFFSIZE);
			recv(fds[i], buff, BUFFSIZE, 0);
			std::cout << "fds[" << i << "]: " << buff << std::endl;
		}
	}
}
*/

int main(void) {
	ServSocket					ssock;
	std::vector<Connection *>	conns;
	bool						end = true;

	while (end) {
		if (!ssock.spoll())
			continue ;
		conns = ssock.getConns(true);
		for (size_t i = 0; i < conns.size(); i++) {
			std::cout << "Conn [" << conns[i]->getFd() << "] :" << conns[i]->getReadBuff() << std::endl;
			if (conns[i]->getReadBuff() == "exit\n") {
				end = false;
				break;
			}
			conns[i]->setWriteBuff(conns[i]->getReadBuff());
			conns[i]->setReadBuff("");
		}
		ssock.pushBuffers();
	}
	ssock.closeSfd();
//	TreeNode<int>	*node;
//	TreeNode<int>	*node2;
//	TreeNode<int>	*node3;
//	TreeNode<int>	*node4;

//	node = new TreeNode<int>("base", 0);
//	node2 = new TreeNode<int>("2level", 0);
//	node3 = new TreeNode<int>("2level-par", 0);
//	node4 = new TreeNode<int>("3level", 0);

//	node->add(new TreeNode<int>("H", 0));
//	node->add(new TreeNode<int>("A", 0));
//	node->add(new TreeNode<int>("E", 0));
//	node->add(node2);
//	node->add(node3);
//	node2->add(new TreeNode<int>("B", 0));
//	node2->add(new TreeNode<int>("F", 0));
//	node2->add(new TreeNode<int>("C", 0));
//	node3->add(new TreeNode<int>("G", 0));
//	node3->add(new TreeNode<int>("D", 0));
//	node2->add(node4);
//	node4->add(new TreeNode<int>("Z", 0));
//	node4->add(new TreeNode<int>("Y", 0));

//	for (int i = 0; i < 8; i++)
//		std::cout << node.next[i]->getName() << std::endl;
	
//	Tree<int>	tree(node);
//	tree.print(NULL, "root");
//	std::vector<std::string>	names = {"2level", "3level", "Z"};
//	TreeNode<int> const	*search = tree.getNode(names);
//	std::cout << search->getName() << std::endl;
//	names.erase(names.begin() + 1);
//	search = tree.getNode(names);
//	if (!search)
//		std::cout << "funziona" << std::endl;
//	else
//		std::cout << "non funziona" << std::endl;
	return (0);
}
