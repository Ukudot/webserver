#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#define	PORT 8000
#define BACKLOG 10
#define BUFFSIZE 1000
#define IP "10.12.3.3"

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
