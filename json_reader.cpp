#include "json_reader.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>
#include <thread>
#include <mutex>  // Include the mutex header
#include <iostream>

namespace py = pybind11;
using json = nlohmann::json;

py::object json_to_py(const json& j) {
    if (j.is_null()) {
        return py::none();
    } else if (j.is_boolean()) {
        return py::bool_(j.get<bool>());
    } else if (j.is_number()) {
        return py::float_(j.get<double>());
    } else if (j.is_string()) {
        return py::str(j.get<std::string>());
    } else if (j.is_array()) {
        py::list py_list;
        for (const auto& item : j) {
            py_list.append(json_to_py(item));
        }
        return py_list;
    } else if (j.is_object()) {
        py::dict py_dict;
        for (const auto& item : j.items()) {
            py_dict[py::str(item.key())] = json_to_py(item.value());
        }
        return py_dict;
    }
    throw std::runtime_error("Unhandled type");
}

// Function to read a single JSON file
py::object read_json_file(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + file_path);
    }

    json j;
    file >> j;
    return json_to_py(j);
}

// Thread function to read each JSON file and store the result
void process_file(const std::string& file_path, py::dict& result, std::mutex& result_mutex) {
    try {
        auto file_result = read_json_file(file_path).cast<py::dict>();
        std::lock_guard<std::mutex> guard(result_mutex);
        // Explicitly convert std::string to py::str for the key
        result[py::str(file_path)] = file_result;    
    } catch (const std::exception& e) {
        std::cerr << "Error processing file " << file_path << ": " << e.what() << std::endl;
    }
}

// Function to read multiple JSON files
py::dict read_json_files(const std::vector<std::string>& file_paths) {
    std::vector<std::thread> threads;
    py::dict result;
    std::mutex result_mutex;

    for (const auto& path : file_paths) {
        threads.emplace_back(process_file, path, std::ref(result), std::ref(result_mutex));
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return result;
}