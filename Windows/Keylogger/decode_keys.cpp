#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ifstream file("D:\\VCS\\System Programming\\system-programming\\Windows\\Keylogger\\log.txt");
    string xor_key = "key";
    string str((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());     // Read file to string

    if (file.is_open()) {
        for (int i = 0; i < str.size(); i++) {
            str[i] = str[i] ^ xor_key[i % xor_key.size()];
        }    
    }

    cout << str << endl;
    file.close();
}