import json_reader_module

try:
    json_dict = json_reader_module.read_json_files(["test.json", "test2.json"])
    print(json_dict)
except Exception as e:
    print(f"Error: {e}")
