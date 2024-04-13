#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>


int main(int argc, char* argv[]) {

    int nMessages = std::stoi(argv[1]);
    std::cout << nMessages << std::endl;
    std::cout << "started child process\n";
    HANDLE events[2];
    HANDLE eventA;
    HANDLE eventB;
    HANDLE readerSemaphore;
    HANDLE eventReaderEnd;
    readerSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"readerSemaphore");
    eventReaderEnd = OpenEvent(EVENT_MODIFY_STATE, FALSE, L"eventReaderEnd");

    if (readerSemaphore == NULL)
    {
        std::cout << "Open semaphore failed.\n";
        std::cout << "Press any key to exit.\n";
        std::cin.get();
        return GetLastError();
    }

    std::cout << "waiting for semaphore\n";
    WaitForSingleObject(readerSemaphore, INFINITE);

    eventA = OpenEventW(SYNCHRONIZE | EVENT_MODIFY_STATE, FALSE, L"eventA");
    eventB = OpenEventW(SYNCHRONIZE | EVENT_MODIFY_STATE, FALSE, L"eventB");

    events[0] = eventA;
    events[1] = eventB;

    for (int i = 0; i < nMessages; i++) {
        std::cout << "waiting for events\n";
        DWORD waitResult = WaitForMultipleObjects(2, events, FALSE, INFINITE);
        if (waitResult - WAIT_OBJECT_0 == 0) {
            std::cout << "recived a message from Writer process: 'A'\n";
            ResetEvent(eventA);
        }
        else if (waitResult - WAIT_OBJECT_0 == 1) {
            std::cout << "recived a message from Writer process: 'B'\n";
            ResetEvent(eventB);
        }
        else {
            std::cout << "error while waiting for multiple objects\n" << GetLastError() << std::endl;
            return -1;
        }

    }

    bool releaseRes = ReleaseSemaphore(readerSemaphore, 1, NULL);

    CloseHandle(readerSemaphore);

    SetEvent(eventReaderEnd);

    Sleep(2000);

    return 0;
}
