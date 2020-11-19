#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>
#include "main.h"

bool PRE::generate_keypair(std::string &result_private_key,
                           std::string &result_public_key) {
    generate_private_key(result_private_key);
    generate_public_key(result_private_key, result_public_key);
}

// Run by: anyone
bool PRE::generate_private_key(std::string &result_private_key) {
    int argc = 1;

    char *arg1 = new char[]{"generate_private_key"};

    char *argv[] = {arg1};

    result_private_key = run_python_function(argc, argv);
    return true;
}

// Run by: owner of private key
bool PRE::generate_public_key(std::string private_key,
                              std::string &result_public_key) {
    int argc = 2;
    char *arg1 = new char[]{"generate_public_key"};

    char *arg2 = new char[private_key.length() + 1];
    strcpy(arg2, private_key.c_str());

    char *argv[] = {arg1, arg2};

    result_public_key = run_python_function(argc, argv);
    return true;
}

// Run by: owner of file
bool PRE::encrypt(std::string key,
                  std::string message,
                  std::string &result_capsule,
                  std::string &result_ciphertext) {
    int argc = 3;
    char *arg1 = new char[]{"encrypt_ciphertext"};

    char *arg2 = new char[key.length() + 1];
    strcpy(arg2, key.c_str());

    char *arg3 = new char[message.length() + 1];
    strcpy(arg3, message.c_str());

    char *argv[] = {arg1, arg2, arg3};

    std::string result = run_python_function(argc, argv);
    result_ciphertext = result.substr(0, result.find(','));
    result_capsule = result.substr(result.find(',') + 1);
    return true;
}

// Run by: anyone
bool PRE::decrypt(std::string key,
                  std::string capsule,
                  std::string ciphertext,
                  std::string &result_message) {
    int argc = 4;

    char *arg1 = new char[]{"decrypt_ciphertext"};

    char *arg2 = new char[key.length() + 1];
    strcpy(arg2, key.c_str());

    char *arg3 = new char[capsule.length() + 1];
    strcpy(arg3, capsule.c_str());

    char *arg4 = new char[ciphertext.length() + 1];
    strcpy(arg4, ciphertext.c_str());

    char *argv[] = {arg1, arg2, arg3, arg4};

    result_message = run_python_function(argc, argv);

    return true;
}

bool PRE::get_reencryption_key(std::string alice_private_key,
                    std::string alice_private_signing_key,
                    std::string bob_public_key,
                    std::string &kfrag) {
    int argc = 4;

    char *arg1 = new char[]{"get_reencryption_key"};

    char *arg2 = new char[alice_private_key.length() + 1];
    strcpy(arg2, alice_private_key.c_str());

    char *arg3 = new char[alice_private_signing_key.length() + 1];
    strcpy(arg3, alice_private_signing_key.c_str());

    char *arg4 = new char[bob_public_key.length() + 1];
    strcpy(arg4, bob_public_key.c_str());

    char *argv[] = {arg1, arg2, arg3, arg4};

    kfrag = run_python_function(argc, argv);

    return true;
}

bool PRE::kfrag_decrypt(std::string alice_public_key,
                        std::string alice_public_signing_key,
                        std::string bob_private_key,
                        std::string capsule,
                        std::string kfrag,
                        std::string ciphertext,
                        std::string &result) {
    int argc = 7;

    char *arg1 = new char[]{"kfrag_decrypt"};

    char *arg2 = new char[alice_public_key.length() + 1];
    strcpy(arg2, alice_public_key.c_str());

    char *arg3 = new char[alice_public_signing_key.length() + 1];
    strcpy(arg3, alice_public_signing_key.c_str());

    char *arg4 = new char[bob_private_key.length() + 1];
    strcpy(arg4, bob_private_key.c_str());

    char *arg5 = new char[capsule.length() + 1];
    strcpy(arg5, capsule.c_str());

    char *arg6 = new char[kfrag.length() + 1];
    strcpy(arg6, kfrag.c_str());

    char *arg7 = new char[ciphertext.length() + 1];
    strcpy(arg7, ciphertext.c_str());

    char *argv[] = {arg1, arg2, arg3, arg4, arg5, arg6, arg7};

    result = run_python_function(argc, argv);

    return true;
}

/*
 * format of argv: function_name, arg1, arg2, ...
 */
std::string PRE::run_python_function(int argc, char **argv) {
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    Py_Initialize();

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
//    PyRun_SimpleString("print(sys.path)");

    pName = PyUnicode_DecodeFSDefault("pyUmbralInterface");
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, argv[0]);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(argc - 1);
            for (i = 1; i < argc; ++i) {
                pValue = PyUnicode_FromFormat(argv[i], "s");

                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return nullptr;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i - 1, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
//                printf("Result of call: %ld\n", PyLong_AsLong(pValue));
                std::string result = PyUnicode_AsUTF8(pValue);
                Py_DECREF(pValue);
                return result;
            } else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr, "Call failed\n");
                return nullptr;
            }
        } else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", argv[0]);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        fprintf(stderr, "Failed to load the pyUmbralInterface.py interface\n");
        return nullptr;
    }
    if (Py_FinalizeEx() < 0) {
        return nullptr;
    }
    return nullptr;
}

void message_help() {
    std::cout << "Commands:\n";
    std::cout << "  --generate_private_key\n";
    std::cout << "  --generate_public_key [private_key]\n";
    std::cout << "  --generate_keypair\n";
    std::cout << "  --encrypt [key] [message]\n";
    std::cout << "  --decrypt [key] [encrypted message]\n";
    std::cout << "  --get_reencryption_key [alice private key] [alice private signing key] [bob public key]\n";
    std::cout << "  --kfrag_decrypt [alice pub key] [alice pub signing key] [bob private key] [capsule] [kfrag] [ciphertext]\n";
    std::cout << "  --run_test\n";
}

int main(int argc, char *argv[]) {
    PRE pre = PRE();

    if (argc == 1) {
        message_help();
        return 0;
    } else {
        std::string command(argv[1]);
        if (command == "--generate_private_key") {
            std::string priv_key_result("");
            pre.generate_private_key(priv_key_result);
            std::cout << "Private Key: " << priv_key_result << std::endl;
            return 0;
        } else if (command == "--generate_public_key") {
            if (argc <= 2) {
                std::cout << "Usage: --generate_public_key [private_key]\n";
                return 0;
            }

            std::string pub_key_result("");
            pre.generate_public_key(std::string(argv[2]), pub_key_result);
            std::cout << "Public Key: " << pub_key_result << std::endl;
            return 0;
        } else if (command == "--generate_keypair") {
            std::string priv_key_result, pub_key_result;
            pre.generate_keypair(priv_key_result, pub_key_result);
            std::cout << "Private Key: " << priv_key_result << std::endl;
            std::cout << "Public Key: " << pub_key_result << std::endl;
        } else if (command == "--encrypt") {
            if (argc <= 3) {
                std::cout << "Usage: --encrypt [key] [message]\n";
                return 0;
            }

            std::string capsule("");
            std::string cipher_text("");
            pre.encrypt(std::string(argv[2]),
                        std::string(argv[3]),
                        capsule,
                        cipher_text);

            std::cout << "Capsule: " << capsule << std::endl;
            std::cout << "Cipher Text: " << cipher_text << std::endl;
        } else if (command == "--decrypt") {
            if (argc <= 4) {
                std::cout << "Usage: --decrypt [key] [capsule] [ciphertext]\n";
                return 0;
            }

            std::string message("");
            pre.decrypt(std::string(argv[2]),
                        std::string(argv[3]),
                        std::string(argv[4]),
                        message);
            std::cout << "Decrypted Message: " << message << std::endl;
        } else if (command == "--get_reencryption_key") {
            if (argc <= 4) {
                std::cout
                    << "Usage: --get_reencryption_key [alice private key] [alice private signing key] [bob public key]\n";
                return 0;
            }

            std::string kfrag("");
            pre.get_reencryption_key(std::string(argv[2]),
                          std::string(argv[3]),
                          std::string(argv[4]),
                          kfrag);
            std::cout << "Re encryption key (KFrag): " << kfrag << std::endl;
        } else if (command == "--kfrag_decrypt") {
            if (argc <= 4) {
                std::cout
                    << "Usage: --kfrag_decrypt [alice pub key] [alice pub signing key] [bob private key] [capsule] [kfrag] [ciphertext]\n";
                return 0;
            }

            std::string res("");
            pre.kfrag_decrypt(std::string(argv[2]),
                              std::string(argv[3]),
                              std::string(argv[4]),
                              std::string(argv[5]),
                              std::string(argv[6]),
                              std::string(argv[7]),
                              res);
            std::cout << "Decrypted Message: " << res << std::endl;
        } else if(command == "--run_test") {
            std::string alice_priv_key, alice_pub_key;
            std::string alice_priv_signing_key, alice_pub_signing_key;
            std::string bob_priv_key, bob_pub_key;

            std::string capsule;
            std::string kfrag;
            std::string ciphertext;
            std::string resulttext;

            std::string origmessage("This is the FIRST message to be encrypted.");
            std::string nextmessage("This is the SECOND message to be encrypted");

            // Generate public/private keys for Alice, Bob, and Alice's signer.
            pre.generate_keypair(alice_priv_key, alice_pub_key);
            pre.generate_keypair(alice_priv_signing_key, alice_pub_signing_key);
            pre.generate_keypair(bob_priv_key, bob_pub_key);

            std::cout << "Alice's FIRST message:\n";
            std::cout << "  " << origmessage << std::endl << std::endl;
            std::cout << "Alice's SECOND message:\n";
            std::cout << "  " << nextmessage << std::endl << std::endl;

            std::cout << "Alice:\n";
            std::cout << "  Private key: " << alice_priv_key << std::endl;
            std::cout << "  Public key: " << alice_pub_key << std::endl;
            std::cout << std::endl;
            std::cout << "Alice Signing:\n";
            std::cout << "  Private key: " << alice_priv_signing_key << std::endl;
            std::cout << "  Public key: " << alice_pub_signing_key << std::endl;
            std::cout << std::endl;
            std::cout << "Bob:\n";
            std::cout << "  Private key: " << bob_priv_key << std::endl;
            std::cout << "  Public key: " << bob_pub_key << std::endl << std::endl;

            // Generate a re-encryption key, or KFrag, using Alice's private
            // key, Alice's Signer's private key, and Bob's public key.
            pre.get_reencryption_key(alice_priv_key, alice_priv_signing_key, bob_pub_key, kfrag);

            std::cout << "Re-Encryption Key (KFrag):\n";
            std::cout << "  " << kfrag << std::endl << std::endl;

            // Encrypt the cipher text using Alice's public key.
            pre.encrypt(alice_pub_key, origmessage, capsule, ciphertext);

            std::cout << "MESSAGE ONE\n";
            std::cout << "  Capsule:\n";
            std::cout << "    " << capsule << std::endl << std::endl;
            std::cout << "  Encrypted Text:\n";
            std::cout << "    " << ciphertext << std::endl << std::endl;

            // Re-encrypt the ciphertext using the KFrag (re-encryption key) and
            // then decrypt the re-encrypted ciphertext using Bob's private key.
            pre.kfrag_decrypt(alice_pub_key, alice_pub_signing_key, bob_priv_key, capsule, kfrag, ciphertext, resulttext);

            std::cout << "  Alice's Original Message Decrypted by Bob:\n";
            std::cout << "    " << resulttext << std::endl << std::endl;

            pre.encrypt(alice_pub_key, nextmessage, capsule, ciphertext);

            // The point of doing this a second time is to show that the
            // re-encryption key (KFrag) can be used to convert anything
            // encrypted by Alice's public key to be used by Bob.
            std::cout << "MESSAGE TWO\n";
            std::cout << "  Capsule:\n";
            std::cout << "    " << capsule << std::endl << std::endl;
            std::cout << "  Encrypted Text:\n";
            std::cout << "    " << ciphertext << std::endl << std::endl;

            pre.kfrag_decrypt(alice_pub_key, alice_pub_signing_key, bob_priv_key, capsule, kfrag, ciphertext, resulttext);
            std::cout << "  Alice's Original Message Decrypted by Bob:\n";
            std::cout << "    " << resulttext << std::endl << std::endl;
        } else {
            std::cout << std::endl << command << " is not a valid command.\n\n";
            message_help();
        }
    }

    return 0;
}
