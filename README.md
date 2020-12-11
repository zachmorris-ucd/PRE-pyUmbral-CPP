This adapts the pyUmbral threshold re-encryption (very similar to proxy re-encryption) into a C++ interface so that it can be used with C++. This is not an entire adaptation of pyUmbral to C++, it just serves as an example of proxy re-encryption in C++. U

The goal of this will be to fit the functionalities from pyUmbralInterface.py and main.cpp into the ResilientDB network. We built this entire program just to allow the code to be implemented into ResilientDB, but fell short because we struggled to get python to run on our docker implementation of ResilientDB.

## Requirements
CMake, Make, Python3

# Usage

### Clone the repository
```
$ git clone https://github.com/zachmorris-ucd/PRE-pyUmbral-CPP.git
$ cd PRE-pyUmbral-CPP/
```

### Install pyUmbral
```
$ pip3 install umbral
```

### Confirm python requirements are installed
```
$ python3  
>>> import pyUmbralInterface  
>>> pyUmbralInterface.run_test()
```

You should get the output
```
Plain text: b'Proxy Re-encryption is cool!'
bob_cleartext: b'Proxy Re-encryption is cool!'
```

### Build the Executable
```
make clean
make
```

### Test/Run the Executable
```
./pre.out --run_test
```

### Experiment With All The Features
All of these commands are the same commands run when you do ./pre.out --run_test, you just don't have to remember all the values.
```
./pre.out // List all the commands
./pre.out --generate_keypair // Generate a public and private key - 3 are needed for proxy re encryptioon

./pre.out --encrypt ...
./pre.out --get_reencryption_key ...
./pre.out --kfrag_decrypt
```


