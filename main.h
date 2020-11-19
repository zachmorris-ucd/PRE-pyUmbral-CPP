#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>

// int main(int argc, char** argv);
// std::string generatePrivateKey();

class PRE {
 public:
  bool generate_keypair(std::string &result_private_key, std::string &result_public_key);
  bool generate_private_key(std::string &result_private_key);
  bool generate_public_key(std::string private_key,
                           std::string &result_public_key);
  bool encrypt(std::string key,
               std::string message,
               std::string &result_capsule,
               std::string &result_ciphertext);
  bool decrypt(std::string key,
               std::string capsule,
               std::string ciphertext,
               std::string &result_message);
  bool get_reencryption_key(std::string alice_private_key,
                       std::string alice_private_signing_key,
                       std::string bob_public_key,
                       std::string &kfrag);
  bool kfrag_decrypt(std::string alice_public_key,
                     std::string alice_public_signing_key,
                     std::string bob_private_key,
                     std::string capsule,
                     std::string kfrag,
                     std::string ciphertext,
                     std::string &result);

 private:
  std::string run_python_function(int argc, char *argv[]);
};

#endif //MAIN_H
