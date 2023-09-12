#include <iostream>

int main(int argc, char *argv[], char *envp[]) {
	for (int i = 0; envp[i]; i++) {
		std::cout << envp[i] << std::endl;
	}
	return (0);
}
