#include <iostream>
#include <windows.h>
#include <string>

int main(int argc, char* argv[])
{
    HANDLE hReadPipe;

    hReadPipe = (HANDLE)atoi(argv[1]);

    int n = std::stoi(argv[2]);

    float product = 1.0f;
    for (int i = 0; i < n; i++) {
        float readFloat;
        DWORD dwBytesRead;
        if (!ReadFile(hReadPipe, &readFloat, sizeof(readFloat), &dwBytesRead, NULL)) {
            std::cout << "Read from pipe failed\n";
            std::cout << "Press any key to finish\n";
            std::getc(stdin);
            return GetLastError();
        }
        std::cout << "Read float from pipe: " << readFloat << std::endl;
        product *= readFloat;
    }

    std::cout << "Product of array elements received from pipe: " << product << std::endl;

    std::cout << "Press any key to exit...";
    std::getc(stdin);

    return 0;
}
