#ifndef JSON_READER_H
#define JSON_READER_H

#include <string>
#include <vector>
#include <pybind11/pybind11.h>

pybind11::dict read_json_files(const std::vector<std::string>& file_paths);

#endif // JSON_READER_H
