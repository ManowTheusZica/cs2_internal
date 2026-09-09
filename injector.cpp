// ============================================================
// injector.cpp - DLL Injector para CS2
// Compilar: cl /EHsc injector.cpp /Fe:injector.exe
// Usar: injector.exe cs2_cheat.dll
// ============================================================

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>

DWORD FindProcess(const wchar_t* name)
{
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(pe);

    if (Process32FirstW(snap, &pe))
    {
        do
        {
            if (_wcsicmp(pe.szExeFile, name) == 0)
            {
                CloseHandle(snap);
                return pe.th32ProcessID;
            }
        } while (Process32NextW(snap, &pe));
    }
    CloseHandle(snap);
    return 0;
}

bool InjectDLL(DWORD pid, const char* dllPath)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess)
    {
        std::cerr << "[!] Falha ao abrir processo (erro: " << GetLastError() << ")" << std::endl;
        return false;
    }

    // Allocate memory in target process for DLL path
    size_t pathLen = strlen(dllPath) + 1;
    LPVOID remoteMem = VirtualAllocEx(hProcess, NULL, pathLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteMem)
    {
        std::cerr << "[!] Falha ao alocar memoria no processo" << std::endl;
        CloseHandle(hProcess);
        return false;
    }

    // Write DLL path to allocated memory
    if (!WriteProcessMemory(hProcess, remoteMem, dllPath, pathLen, NULL))
    {
        std::cerr << "[!] Falha ao escrever caminho da DLL" << std::endl;
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // Get address of LoadLibraryA
    HMODULE hKernel32 = GetModuleHandleW(L"kernel32.dll");
    FARPROC pLoadLibrary = GetProcAddress(hKernel32, "LoadLibraryA");
    if (!pLoadLibrary)
    {
        std::cerr << "[!] Falha ao encontrar LoadLibraryA" << std::endl;
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // Create remote thread to call LoadLibraryA with DLL path
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
        (LPTHREAD_START_ROUTINE)pLoadLibrary, remoteMem, 0, NULL);
    if (!hThread)
    {
        std::cerr << "[!] Falha ao criar thread remota (erro: " << GetLastError() << ")" << std::endl;
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // Wait for thread to finish
    WaitForSingleObject(hThread, 5000);

    // Check if DLL was loaded
    DWORD exitCode = 0;
    GetExitCodeThread(hThread, &exitCode);

    // Cleanup
    VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    if (exitCode != 0)
    {
        std::cout << "[+] DLL injetada com sucesso! (handle: 0x" << std::hex << exitCode << std::dec << ")" << std::endl;
        return true;
    }
    else
    {
        std::cerr << "[!] DLL falhou ao carregar" << std::endl;
        return false;
    }
}

int main(int argc, char* argv[])
{
    std::cout << "========================================" << std::endl;
    std::cout << "  PRISM CS2 Internal - DLL Injector" << std::endl;
    std::cout << "  Apenas para uso offline (-insecure)" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Get DLL path
    std::string dllPath;
    if (argc > 1)
    {
        dllPath = argv[1];
    }
    else
    {
        // Default path
        char exePath[MAX_PATH];
        GetModuleFileNameA(NULL, exePath, MAX_PATH);
        std::string dir = exePath;
        size_t pos = dir.find_last_of("\\/");
        if (pos != std::string::npos)
            dir = dir.substr(0, pos + 1);
        dllPath = dir + "cs2_cheat.dll";
    }

    std::cout << "[*] DLL: " << dllPath << std::endl;

    // Wait for CS2
    std::cout << "[*] Procurando cs2.exe..." << std::endl;
    DWORD pid = 0;
    int attempts = 0;
    while (pid == 0 && attempts < 60)
    {
        pid = FindProcess(L"cs2.exe");
        if (pid == 0)
        {
            Sleep(1000);
            attempts++;
            if (attempts % 10 == 0)
                std::cout << "[*] Aguardando CS2... (" << attempts << "s)" << std::endl;
        }
    }

    if (pid == 0)
    {
        std::cerr << "[!] CS2 nao encontrado apos 60 segundos" << std::endl;
        std::cerr << "[!] Abra o CS2 com -insecure primeiro" << std::endl;
        std::cout << "\nPressione qualquer tecla para sair...";
        std::cin.get();
        return 1;
    }

    std::cout << "[+] CS2 encontrado! PID: " << pid << std::endl;

    // Inject
    std::cout << "[*] Injetando DLL..." << std::endl;
    if (InjectDLL(pid, dllPath.c_str()))
    {
        std::cout << std::endl;
        std::cout << "[+] PRISM injetado com sucesso!" << std::endl;
        std::cout << "[+] Pressione ] no jogo para abrir o menu" << std::endl;
        std::cout << "[+] Ctrl+C no console para descarregar" << std::endl;
    }
    else
    {
        std::cerr << "[!] Injecao falhou!" << std::endl;
        std::cerr << "[!] Verifique se voce esta rodando como Administrador" << std::endl;
    }

    std::cout << "\nPressione qualquer tecla para sair...";
    std::cin.get();
    return 0;
}
