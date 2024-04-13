#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>

//writer

int main(int argc, char* argv[]) {

    int nMessages = std::stoi(argv[1]);

    std::cout << "started writer process\n";
    HANDLE eventA;
    HANDLE eventB;
    HANDLE writerMutex;
    HANDLE eventWriterEnd;
    writerMutex = OpenMutex(SYNCHRONIZE, FALSE, L"writerMutex");
    eventWriterEnd = OpenEvent(EVENT_MODIFY_STATE, FALSE, L"eventWriterEnd");
    if (writerMutex == NULL)
    {
        std::cout << "Open mutex failed.\n";
        std::cout << "Press any key to exit.\n";
        std::cin.get();
        return GetLastError();
    }
    std::cout << "waiting for mutex...\n";
    WaitForSingleObject(writerMutex, INFINITE);
    eventA = OpenEventW(SYNCHRONIZE | EVENT_MODIFY_STATE, FALSE, L"eventA");
    eventB = OpenEventW(SYNCHRONIZE | EVENT_MODIFY_STATE, FALSE, L"eventB");
    for (int i = 0; i < nMessages; i++) {
        std::string message;
        std::cout << "Enter message (A or B): ";
        std::cin >> message;
        if (message == "A") {
            SetEvent(eventA);
        }
        else if (message == "B") {
            SetEvent(eventB);
        }
    }
    CloseHandle(eventA);
    CloseHandle(eventB);
    ReleaseMutex(writerMutex);
    CloseHandle(writerMutex);
    SetEvent(eventWriterEnd);

    Sleep(2000);

    return 0;
}
