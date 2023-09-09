#include <vector>
#include <iostream>
#include <string>
#include "ServSocket.hpp"
#include "Connection.hpp"

int main(void) {
	std::vector<Connection *> conns;
	ServSocket	sock;
	std::string str = "";
	std::string body = "";

	while (42) {
		if (!sock.spoll())
			continue;
		conns = sock.getConns(true);
		for (size_t i = 0; i < conns.size(); i++) {
			std::cout << conns[i]->getReadBuff() << std::endl;
			std::cout << "Response: ";
			std::getline(std::cin, str);
			str += std::string("\r\n");
			//str += std::string("Content-Length: 13\r\n");
			str += std::string("Location: http://www.w3.org/pub/WWW/People.html\r\n");
			str += std::string("\r\n");
			//body = std::string("<h1>ciao</h1>");
			conns[i]->setWriteBuff(str + body);
			conns[i]->setReadBuff("");
		}
		sock.pushBuffers();
	}
	return (0);
}
