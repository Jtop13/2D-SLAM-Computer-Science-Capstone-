#include <python3.5m/Python.h>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <string>

bool keep_running = true;

int main() {
    // Initialize the Python interpreter
    Py_Initialize();

    // Add the current directory to the Python module search path
    PyObject* sysPath = PySys_GetObject((char*)"path");
    PyObject* currentDir = PyUnicode_FromString(".");
    PyList_Append(sysPath, currentDir);
    Py_DECREF(currentDir);

    // Import the Python script that performs the machine learning tasks
    PyObject* module = PyImport_ImportModule("simpleLines");
    if (module == NULL) {
        PyErr_Print();
        std::cerr << "Failed to import Python module." << std::endl;
        return 1;
    }

    // Get the function that performs the machine learning tasks
    PyObject* function = PyObject_GetAttrString(module, "simpleLines");
    if (function == NULL) {
        PyErr_Print();
        std::cerr << "Failed to get Python function." << std::endl;
        return 1;
    }

    // Set up the loop to call the Python function multiple times
    std::string text;
    while (keep_running) {
		std::cout << "Start Loop Error";
        // Create a Python object that represents the data file
        PyObject* data_file = PyUnicode_FromString("juliaRogers.tmp");

        // Call the Python function with the data file as an argument
        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, data_file);

        PyObject* result = PyObject_CallObject(function, args);

        if (result == NULL) {
            PyErr_Print();
            std::cerr << "Failed to call Python function." << std::endl;
            return 1;
        }

        //Py_DECREF(result);
        //Py_DECREF(data_file);  // Decrement the reference count for the data_file object
        //Py_DECREF(args);

        /*std::cout << "Enter 'q' then press enter to quit, or any other letter to run again: ";
        std::cin >> text;

        if (text == "q" || text == "Q") {
            keep_running = false;
        }

		std::cout << "End Loop Error";*/
    }

	std::cout << "Out of Loop Error";

    // Clean up
    Py_DECREF(module);
    Py_DECREF(function);
    Py_Finalize();

    return 0;
}


