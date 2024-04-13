#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>

int main() {

    //parent - writer, child - reader

    HANDLE events[2];

    HANDLE eventA, eventB, eventReaderEnd, eventWriterEnd;
    HANDLE writerMutex;

    writerMutex = CreateMutex(NULL, FALSE, L"writerMutex");
    if (writerMutex == NULL)
    {
        std::cout << "Create mutex failed.\n";
        std::cout << "Press any key to exit.\n";
        std::cin.get();
        return GetLastError();
    }

    HANDLE readerSemaphore;
    readerSemaphore = CreateSemaphore(NULL, 2, 2, L"readerSemaphore");
    if (readerSemaphore == NULL)
        return GetLastError();


    eventA = CreateEventW(NULL, TRUE, FALSE, L"eventA");
    eventB = CreateEventW(NULL, TRUE, FALSE, L"eventB");

    eventReaderEnd = CreateEvent(NULL, FALSE, FALSE, L"eventReaderEnd");
    eventWriterEnd = CreateEvent(NULL, FALSE, FALSE, L"eventWriterEnd");

    events[0] = eventReaderEnd;
    events[1] = eventWriterEnd;


    int nWriter;
    std::cout << "Enter amount of Writer processes: ";
    std::cin >> nWriter;

    int nReader = 2 * nWriter;

    int nMessages;
    std::cout << "Enter amount of messages: ";
    std::cin >> nMessages;

    PROCESS_INFORMATION* pis;
    pis = new PROCESS_INFORMATION[nWriter + nReader];

    std::wstring commandLine = L"Writer.exe " + std::to_wstring(nMessages);
    LPWSTR lpCommandLine = &commandLine[0];
    std::wstring title = L"Writer console window";
    int k = 0;
    for (int i = 0; i < nWriter; i++) {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.lpTitle = &title[0];
        if (!CreateProcess(NULL, lpCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            std::cerr << "Error creating writer process: " << GetLastError() << std::endl;
            return 1;
        }
        pis[k++] = pi;
    }

    commandLine = L"Reader.exe " + std::to_wstring(nMessages);
    lpCommandLine = &commandLine[0];
    title = L"Reader console window";
    for (int i = 0; i < nReader; i++) {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.lpTitle = &title[0];
        if (!CreateProcess(NULL, lpCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            std::cerr << "Error creating reader process: " << GetLastError() << std::endl;
            return 1;
        }
        pis[k++] = pi;
    }

    for (int i = 0; i < nWriter + nReader; i++) {
        DWORD waitResult = WaitForMultipleObjects(2, events, FALSE, INFINITE);

        if (waitResult - WAIT_OBJECT_0 == 0) {
            std::cout << "reader process ended\n";
        }
        else if (waitResult - WAIT_OBJECT_0 == 1) {
            std::cout << "writer process ended\n";
        }
        else {
            std::cout << "error while waiting for multiple objects\n";
            return -1;
        }

    }

    for (int i = 0; i < k; i++) {
        CloseHandle(pis[i].hProcess);
        CloseHandle(pis[i].hThread);
    }

    return 0;
}
