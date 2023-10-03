# Author: Mahmoud Helmy
# Date: 2023-09-27
# Project: bootloader

# Import necessary libraries
import re
import numpy as np

# Function to convert a list of bytes into a list of formatted hexadecimal strings
def bytes_to_c_arr(data):
    return [format(b, '#04x') for b in data]

# Function to read binary data from a file and convert it into a bytearray
def read_file(file_name):
    data = np.fromfile(file_name, dtype='uint8')  # Read binary data from file
    data = bytearray(data)  # Convert it into a bytearray for easy manipulation
    return data

# Function to write a C array to a file
def write_c_array(file_name, data, bytes_per_line=32):
    c_file = open(file_name + ".c", "w")  # Open a C file for writing
    static_content = "unsigned char array[" + str(len(data)) + "] = {\n"  # C array declaration
    array_content = bytes_to_c_arr(data)  # Convert binary data into formatted hexadecimal strings
    formatted_lines = []  # List to store lines of the C array initialization

    # Iterate over the data, breaking it into lines with a fixed number of bytes per line
    for i in range(0, len(array_content), bytes_per_line):
        line = array_content[i:i+bytes_per_line]  # Get a slice of data for this line
        formatted_line = ", ".join(line)  # Join the hexadecimal bytes with commas

        # Add a comma to the end of lines except the last one
        if i + bytes_per_line < len(array_content):
            formatted_line += ","
        
        formatted_lines.append("    " + formatted_line)  # Add indentation and add line to the list

    final_content = "\n".join(formatted_lines) + "\n};\n"  # Join lines with newlines and close the C array
    c_file.write(static_content + final_content)  # Write the entire C array to the file

# Read binary data from a file
raw_data = read_file("C:\\Users\\Helmy\\Desktop\\Application.bin")

# Write the formatted C array to a C file with 32 bytes per line
write_c_array("imageArray", raw_data, bytes_per_line=32)
