This adapts the pyUmbral threshold re-encryption (very similar to proxy re-encryption) into a C++ interface so that it can be used with C++. This is not an entire adaptation of pyUmbral to C++, it just serves as an example of proxy re-encryption in C++. 

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
```rm -rf cmake-build-debug/
cmake -S. -Bcmake-build-debug
make --directory cmake-build-debug
```

### Test/Run the Executable
```
./cmake-build-debug/PRE_CPP --run_test
```

