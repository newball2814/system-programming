#include <cstdlib>
#include <filesystem>
#include <string>

#define okay(msg, ...) printf("[+] " msg "\n", ##__VA_ARGS__)
#define info(msg, ...) printf("[*] " msg "\n", ##__VA_ARGS__)
#define warn(msg, ...) printf("[-] " msg "\n", ##__VA_ARGS__)

namespace fs = std::filesystem;

std::string findFilePathRecursive(const std::string &fileName, const fs::path &currentDir) {
    for (const auto &entry: fs::directory_iterator(currentDir)) {
        if (entry.is_directory()) {
            std::string subdirectoryResult = findFilePathRecursive(fileName, entry.path());
            if (!subdirectoryResult.empty()) {
                return subdirectoryResult;
            }
        } else if (entry.is_regular_file() && entry.path().filename() == fileName) {
            return entry.path().string();
        }
    }

    return "";
}

std::string findFilePath(const std::string &fileName, const std::string &parentDir) {
    // Init parentPath object from path string
    fs::path parentPath(parentDir);

    if (!fs::exists(parentPath)) {
        return "DIREXS";
    }

    if (!fs::is_directory(parentPath)) {
        return "DIRERR";
    }

    return findFilePathRecursive(fileName, parentPath);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        warn("Usage: %s [FILENAME] [DIRECTORY]", argv[0]);
        return EXIT_FAILURE;
    }

    std::string fileName = argv[1];
    std::string parentDir = argv[2];

    std::string absolutePath = findFilePath(fileName, parentDir);

    if (!absolutePath.empty() && absolutePath != "DIRERR" && absolutePath != "DIREXS") {
        okay("Path: %s", absolutePath.c_str());
    } else if (absolutePath == "DIRERR") {
        warn("%s is not a directory", parentDir.c_str());
    } else if (absolutePath == "DIREXS") {
        warn("Directory %s does not exist", parentDir.c_str());
    } else {
        warn("Could not find file named %s in directory %s or its subdirectories", fileName.c_str(), parentDir.c_str());
    }

    return EXIT_SUCCESS;
}
