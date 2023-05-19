#include <python3.5m/Python.h>

int main() {
    // Initialize the Python interpreter
    Py_Initialize();

    FILE* file = fopen("hello.py", "r");
    PyRun_SimpleFile(file, "hello.py");
    fclose(file);

    Py_Finalize();
   
    return 0;
}
