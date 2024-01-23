#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Include this for automatic STL container conversions
#include "json_reader.h"

namespace py = pybind11;

PYBIND11_MODULE(json_reader_module, m) {
    m.def("read_json_files", &read_json_files, "A function that reads multiple JSON files and returns their contents as a Python dictionary");
}