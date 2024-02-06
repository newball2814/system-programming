import os

def read_binary_data(exe_path):
    with open(exe_path, 'rb') as exe_file:
        # Read the entire file into a bytes object
        binary_data = exe_file.read()
    return binary_data

# Usage example
exe_path = "path/to/your.exe"
binary_data = read_binary_data(exe_path)
# Now, binary_data contains the binary data of the EXE file
