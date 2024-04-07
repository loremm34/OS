#include <windows.h>
#include <iostream>
#include <string>

int main() {
    int arraySize;
    std::wcout << L"Enter size of array: ";
    std::wcin >> arraySize;
    std::wcout << L"Enter elements: ";

    std::wstring commandLine = L"ChildProcess.exe " + std::to_wstring(arraySize) + L" ";

    for (size_t i = 0; i < arraySize; i++) {
        double e;
        std::wcin >> e;
        commandLine += std::to_wstring(e) + L" ";
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    std::wstring title = L"Child console window";
    si.lpTitle = &title[0];

    LPWSTR lpCommandLine = &commandLine[0];

    if (!CreateProcess(NULL, lpCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        std::cerr << "Error creating child process: " << GetLastError() << std::endl;
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}