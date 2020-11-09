#include "main.h"

int main(int argc, char** argv) {
	if(argc == 1) {
		std::cout << "usage:" << std::endl;
		std::cout << "  -g | --generate: Generate a random public/private key" << std::endl;
	} else {
		for(int i = 1; i < argc; i++) {
			if(std::string(argv[i]) == "-g" || std::string(argv[i]) == "--generate") {
				generatePrivateKey();
			}
		}
	}
	return 1;
}

std::string generatePrivateKey() {
	std::cout << rand(123456789) << std::endl;

	return "1";
}
