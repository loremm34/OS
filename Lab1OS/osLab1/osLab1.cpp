#include <iostream>
#include <vector>
#include <windows.h>

DWORD WINAPI workerThread(LPVOID lrParam) {
    auto params = reinterpret_cast<std::pair<std::vector<int>*, std::pair<int, int>>*>(lrParam);
    auto array = params->first;
    auto range = params->second;

    int a = range.first;
    int b = range.second;

    for (int num : *array) {
        if (num >= a && num <= b) {
            std::cout << num << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}

int main() {
    int arraySize;
    std::cout << "Enter the size of the array: ";
    std::cin >> arraySize;
    DWORD IDThread;
    std::vector<int> array(arraySize);

    std::cout << "Enter the elements of the array: ";
    for (int i = 0; i < arraySize; ++i) {
        std::cin >> array[i];
    }

    int a, b;
    std::cout << "Enter the range [a, b]: ";
    std::cin >> a >> b;

    auto params = new std::pair<std::vector<int>*, std::pair<int, int>>(&array, std::make_pair(a, b));
    HANDLE hThread = CreateThread(NULL, 0, workerThread, params, 0, &IDThread);

    if (hThread == NULL) {
        std::cerr << "Failed to create thread" << std::endl;
        return 1;
    }

    SuspendThread(hThread);

    /*Sleep(100);*/

    ResumeThread(hThread);

    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);
    delete params;

    return 0;
}
