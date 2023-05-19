# :no_entry:[DEPRECATED]
In order to run python within this system:

The header file must contain:
#include <python3.5m/Python.h>

Compiling must look like(embedding is main cpp):
g++ -o embedding embedding.cpp $(/usr/bin/python3.5m-config --ldflags)
