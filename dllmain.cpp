// ============================================================
// PRISM CS2 Internal - DLL Entry Point
// Compilar: x64 Release (.NET ou MSVC)
// Injetar: injector.exe ou qualquer loader de DLL
// ============================================================

#include <Windows.h>
#include <thread>
#include <iostream>
#include <fstream>
#include <string>

// Forward declarations
void InitializeCheats();
void ShutdownCheats();

// Console para debug (opcional)
void AllocConsole()
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
}

// ============================================================
//  DLL MAIN
// ============================================================
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        
        // Alocar console pra debug
        AllocConsole();
        printf("[PRISM] DLL injetada com sucesso!\n");
        printf("[PRISM] Iniciando cheats...\n");
        
        // Thread principal do cheat
        std::thread([]()
        {
            // Esperar CS2 carregar
            Sleep(3000);
            InitializeCheats();
        }).detach();
        
        break;
    }
    case DLL_PROCESS_DETACH:
        ShutdownCheats();
        break;
    }
    return TRUE;
}

// ============================================================
//  PLACEHOLDERS (serao implementados nos modulos)
// ============================================================
// Estes serao substituidos pelos modulos reais:
// - Core/Memory.h
// - Core/Offsets.h  
// - Hooks/Hooks.h
// - Features/InventoryChanger.h
// - Features/SkinApplier.h

extern "C" __declspec(dllexport) void InitializeCheats()
{
    printf("[PRISM] InitializeCheats() chamado\n");
    // TODO: Inicializar hooks, offsets, features
}

extern "C" __declspec(dllexport) void ShutdownCheats()
{
    printf("[PRISM] ShutdownCheats() chamado\n");
    // TODO: Remover hooks, limpar recursos
}
