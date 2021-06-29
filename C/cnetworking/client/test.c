#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>

DWORD WINAPI func(LPVOID lpParam) {
    printf("The parameter: %u.\n", *(DWORD*)lpParam);
    return 0;
}

int main() {
    DWORD dwThreadId, dwThrdParam = 1;
    HANDLE hThread;
    int x;

    for (x = 1; x <= 10; x++) {
        hThread = CreateThread(
            NULL,
            0,
            func,
            &dwThrdParam, // parameter
            0,
            &dwThreadId // identifier
        );

        if (hThread) {
            printf("CreateThread() worked with thread ID: %d\n", dwThreadId);
        } else {
            printf("CreateThread() failed, error: %d.\n", GetLastError());
        }

        if (CloseHandle(hThread)) {
            printf("Handle closed successfully.\n");
        }
    }

    return 0;
}