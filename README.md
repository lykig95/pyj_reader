`sudo apt update`
`sudo apt install nlohmann-json3-dev`
`sudo apt install pybind11-dev`
`sudo apt-get install python3-dev`

`pip install pybind11`

g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` json_reader.cpp pybind11_binding.cpp -o json_reader_module`python3-config --extension-suffix --cflags --ldflags` -pthread
