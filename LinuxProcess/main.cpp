#include <iostream>
#include <fstream>
#include <dirent.h>
#include <string>
#include <cctype>
#include <iomanip>

using namespace std;

void getProcDetail(const string& PID) {
    string status = "/proc/" + PID + "/status";      // Get PPID and PID info
    string cmdline = "/proc/" + PID + "/cmdline";    // Get cmdline executed

    ifstream statusFile(status);
    if (!statusFile.is_open()) {
        cout << "Error getting PPID info." << endl;
        return;
    }
    
    // Get PPID
    string content, PPID;
    while (getline(statusFile, content)) {
        if (content.find("PPid") == 0) {
            PPID = content.substr(6);
            break;
        }
    }
    statusFile.close();

    // Get cmdline
    ifstream cmdlineFile(cmdline);
    if (!cmdlineFile.is_open()) {
        cout << "Error getting command line." << endl;
        return;
    }
    
    getline(cmdlineFile, content, '\0');    // Cuz content is null-terminated, hence the '\0'
    cmdlineFile.close();

    // Print result
    cout << "PPID: " << right << setw(6) << PPID << " PID: " << right << setw(6) << PID << " Cmd: " << content << endl;
}

int main() {
    DIR *dir;

    // Open /proc directory to read
    if ((dir = opendir("/proc")) == nullptr) {
        cout << "Failed to fetch process list." << endl;
        return EXIT_FAILURE;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            getProcDetail(entry->d_name);
        }
    }

    closedir(dir);
    return EXIT_SUCCESS;
}
