#include <iostream>
#include <vector>
#include <Windows.h>
#include <random>

CRITICAL_SECTION cs1;
CRITICAL_SECTION cs2;
HANDLE event;
std::vector<char> array;
int k = 0;
double result = 0;

void workThread() {
    EnterCriticalSection(&cs1);
    std::cout << "Enter interval\n";
    int restInterval;
    std::cin >> restInterval;

    for (size_t i = 0; i < array.size(); ++i) {
        if (std::isdigit(array[i])) {
            std::cout << "Processing digit: " << array[i] << std::endl;
            Sleep(restInterval);
        }
        else {
            std::cout << "Skipping non-digit: " << array[i] << std::endl;
        }
    }


    std::cout << "Work thread has finished." << std::endl;
    LeaveCriticalSection(&cs1);
}

void sumElementThread() {
    EnterCriticalSection(&cs2);
    std::cout << "SumElement thread is waiting to start summing." << std::endl;

    int sum = 0;
    int count = 0;
    for (size_t i = 0; i < k && i < array.size(); ++i) {
        if (std::isdigit(array[i])) {
            sum += (array[i] - '0');
            ++count;
        }
    }
    result = static_cast<double>(sum) / count;

    std::cout << "Result of summing: " << result << std::endl;
    LeaveCriticalSection(&cs2);
    SetEvent(event);
}

int main() {
    InitializeCriticalSection(&cs1);
    InitializeCriticalSection(&cs2);
    event = CreateEvent(NULL, FALSE, FALSE, NULL);

    std::cout << "Input array size\n";
    int arraySize;
    std::cin >> arraySize;


    std::cout << "Input or random numbers\n";
    std::string arrayType;
    std::cin >> arrayType;

   

    if (arrayType == "input") {
        for (int i = 0; i < arraySize; ++i) {
            std::cout << "Input numbers";
            char element;
            std::cin >> element;
            array.push_back(element);
        }
    }
    else if (arrayType == "random") {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 9);

        for (int i = 0; i < arraySize; ++i) {
            array.push_back(dis(gen) + '0');
        }
    }

    for (char c : array) {
        std::cout << c << " ";
    }
    std::cout << std::endl;

    HANDLE workThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)workThread, NULL, CREATE_SUSPENDED, NULL);
    HANDLE sumElementThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sumElementThread, NULL, CREATE_SUSPENDED, NULL);
    std::cout << "Enter k\n";
    std::cin >> k;
    EnterCriticalSection(&cs2);
    ResumeThread(workThreadHandle);
    ResumeThread(sumElementThreadHandle);


    Sleep(2);
    EnterCriticalSection(&cs1);
    std::cout << "Start summing:" << std::endl;
    for (size_t i = 0; i < k && i < array.size(); ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
    LeaveCriticalSection(&cs1);
    std::cout << "Leave criticl section\n";
    LeaveCriticalSection(&cs2);

    WaitForSingleObject(event, INFINITE);

   // Sleep(100);
    std::cout << "Final result: " << result << std::endl;

    DeleteCriticalSection(&cs1);
    DeleteCriticalSection(&cs2);
    CloseHandle(event);

    CloseHandle(workThreadHandle);
    CloseHandle(sumElementThreadHandle);

    return 0;
}
