#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>
#include "main.h"

std::string PRE::generate_private_key() {

}

std::string PRE::run_python_function(int argc, char **argv) {
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

//    if (argc < 3) {
//        fprintf(stderr,"Usage: call pythonfile funcname -[s: str, b: bytearray] [args]\n");
//        return nullptr;
//    }

    Py_Initialize();

    PyRun_SimpleString("import  sys");
    PyRun_SimpleString("sys.path.append('./')");
    PyRun_SimpleString("print(sys.path)");

    pName = PyUnicode_DecodeFSDefault(argv[1]);
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, argv[2]);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(argc - 3);
            for (i = 0; i < argc - 3; ++i) {

                pValue = PyUnicode_FromFormat(argv[i+3], "s");
//                pValue = PyLong_FromLong(atoi(argv[i + 3]));
//                if(i == 0) {
//                    pValue = PyBytes_FromStringAndSize(argv[i + 3],
//                                                           std::string(argv[i
//                                                               + 3]).length());
//                } else {
//                    pValue = PyUnicode_FromString(argv[i+3]);
//                }
//                pValue = argv[i+3];
//                pValue = Py_BuildValue('s', argv[i+3]);
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return nullptr;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
//                printf("Result of call: %ld\n", PyLong_AsLong(pValue));
                std::string result = PyUnicode_AsUTF8(pValue);
                Py_DECREF(pValue);
                return result;
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return nullptr;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
        return nullptr;
    }
    if (Py_FinalizeEx() < 0) {
        return nullptr;
    }
}

int main(int argc, char *argv[])
{
//    Py_Initialize();
//    std::cout << "Here --1\n";
//
//    // Import the current path
//    PyRun_SimpleString("import  sys");
//    PyRun_SimpleString("sys.path.append('./')");
//    PyRun_SimpleString("print(sys.path)");
//
//    std::cout << "Here 1" << std::endl;
//    PyObject *pName, *pModule, *pFunc, *pArgs, *pValue;
//    std::cout << "Here 2" << std::endl;
//    pName = PyUnicode_FromString((char*)"script");
//    std::cout << "Here 3" << std::endl;
//    pModule = PyImport_Import(pName);
//    std::cout << "Here 4" << std::endl;
//    pFunc = PyObject_GetAttrString(pModule, (char*)"test");
//    std::cout << "Here 5" << std::endl;
//    pArgs = PyTuple_Pack(1, PyUnicode_FromString((char*)"Greg"));
//    std::cout << "Here 6" << std::endl;
//    pValue = PyObject_CallObject(pFunc, pArgs);
//    std::cout << "Here 7" << std::endl;
//    auto result = _PyUnicode_AsString(pValue);
//    std::cout << result << std::endl;

    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    if (argc < 3) {
        fprintf(stderr,"Usage: call pythonfile funcname -[s: str, b: bytearray] [args]\n");
        return 1;
    }

    Py_Initialize();

    PyRun_SimpleString("import  sys");
    PyRun_SimpleString("sys.path.append('./')");
    PyRun_SimpleString("print(sys.path)");

    pName = PyUnicode_DecodeFSDefault(argv[1]);
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, argv[2]);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(argc - 3);
            for (i = 0; i < argc - 3; ++i) {

                pValue = PyUnicode_FromFormat(argv[i+3], "s");
//                pValue = PyLong_FromLong(atoi(argv[i + 3]));
//                if(i == 0) {
//                    pValue = PyBytes_FromStringAndSize(argv[i + 3],
//                                                           std::string(argv[i
//                                                               + 3]).length());
//                } else {
//                    pValue = PyUnicode_FromString(argv[i+3]);
//                }
//                pValue = argv[i+3];
//                pValue = Py_BuildValue('s', argv[i+3]);
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
//                printf("Result of call: %ld\n", PyLong_AsLong(pValue));
                std::string result = PyUnicode_AsUTF8(pValue);
                Py_DECREF(pValue);
                std::cout << "Result of call:: " << PyUnicode_AsUTF8(pValue) << std::endl;
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
        return 1;
    }
    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    return 0;
}
