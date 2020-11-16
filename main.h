#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <iostream>

// int main(int argc, char** argv);
// std::string generatePrivateKey();

class PRE {
 public:
  std::string generate_private_key();
  std::string generate_public_key(std::string private_key);
 private:
  std::string run_python_function(int argc, char* argv[]);
};

#endif //MAIN_H
