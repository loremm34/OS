#include <iostream>
#include <windows.h>
#include <vector>
#include <string>

int main()
{
    int n;
    std::cout << "Enter the number of elements in array: ";
    std::cin >> n;

    std::vector<float> arr(n);
    std::cout << "Enter the elements of array (separated by a space): ";
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }

    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES sa;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        std::cout << "Create pipe failed\n";
        std::cout << "Press any key to finish\n";
        std::getc(stdin);
        return GetLastError();
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    std::wstring commandLine = L"Mult.exe " + std::to_wstring((int)hReadPipe) + L" " + std::to_wstring(n);
    LPWSTR lpCommandLine = &commandLine[0];

    if (!CreateProcess(NULL, lpCommandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        std::cout << "Create client process failed\n";
        std::cout << "Press any key to finish\n";
        std::getc(stdin);
        return GetLastError();
    }

    std::cout << "Press any key to start communication...";
    std::getc(stdin);
    std::cout << std::endl;

    for (float elem : arr) {
        DWORD dwBytesWritten;
        if (!WriteFile(hWritePipe, &elem, sizeof(elem), &dwBytesWritten, NULL)) {
            std::cout << "Write to pipe failed\n";  
            std::cout << "Press any key to finish\n";
            std::getc(stdin);
            return GetLastError();
        }
        std::cout << "Wrote to pipe: " << elem << std::endl;
    }

    CloseHandle(hReadPipe);
    CloseHandle(hWritePipe);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
