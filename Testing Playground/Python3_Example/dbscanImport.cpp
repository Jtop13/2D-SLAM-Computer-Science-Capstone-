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
    PyObject* module = PyImport_ImportModule("plot_dbscan");
    if (module == NULL) {
        PyErr_Print();
        std::cerr << "Failed to import Python module." << std::endl;
        return 1;
    }

    // Get the function that performs the machine learning tasks
    PyObject* function = PyObject_GetAttrString(module, "mydbscan");
    if (function == NULL) {
        PyErr_Print();
        std::cerr << "Failed to get Python function." << std::endl;
        return 1;
    }

    // Create a Python object that represents the data file
    PyObject* data_file = PyUnicode_FromString("bestMap.tmp");

    // Call the Python function with the data file as an argument
    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, data_file);
    PyObject* result = PyObject_CallObject(function, args);
    Py_DECREF(args);

    // Check if the call to the Python function succeeded
    if (result == NULL) {
        PyErr_Print();
        std::cerr << "Failed to call Python function." << std::endl;
        return 1;
    }

    // Extract the mean value from the result
    double mean = PyFloat_AsDouble(result);

    // Print out the mean value
    std::cout << "Mean: " << mean << std::endl;
	
	std::string text;
	
	std::cout << "Enter letter then press enter to quit: ";
	std::cin >> text;

    // Clean up
    Py_DECREF(result);
    Py_DECREF(module);
    Py_DECREF(function);
    Py_Finalize();

    return 0;
}
