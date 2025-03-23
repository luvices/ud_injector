// Injector/main.cpp
#include <windows.h>
#include <iostream>
#include "ManualMapper.h"
#include "PEUtils.h"

int main() {
    const wchar_t* targetProcess = L"Medal.exe";
    const char* dllPath = "../Payload/payload.dll";

    DWORD pid = FindProcessId(targetProcess);
    if (!pid) {
        std::cerr << "[-] Could not find process.\n";
        return -1;
    }

    HANDLE hProc = OpenTargetProcess(pid);
    if (!hProc) {
        std::cerr << "[-] Could not open process.\n";
        return -1;
    }

    BYTE* dllData = nullptr;
    SIZE_T dllSize = 0;
    if (!ReadFileToMemory(dllPath, &dllData, &dllSize)) {
        std::cerr << "[-] Failed to read DLL from disk.\n";
        return -1;
    }

    LPVOID remoteBase = ManualMap(hProc, dllData, dllSize);
    if (!remoteBase) {
        std::cerr << "[-] Manual mapping failed.\n";
        return -1;
    }

    std::cout << "[+] DLL manually mapped successfully at: " << remoteBase << "\n";

    delete[] dllData;
    CloseHandle(hProc);
    return 0;
}
