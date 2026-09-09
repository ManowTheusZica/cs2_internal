// ============================================================
// Hooks/Hooks.h - Function hooking for CS2 internal cheat
// Using Microsoft Detours (simpler than MinHook for this case)
// ============================================================

#pragma once
#include <Windows.h>
#include <cstdint>
#include <cstdio>

// ============================================================
//  Simple Detour Hook (inline hook using JMP)
//  Para uso em DLL interna do CS2
// ============================================================

class DetourHook
{
private:
    void* target = nullptr;
    void* detour = nullptr;
    void* original = nullptr;
    uint8_t originalBytes[14]{};
    bool hooked = false;

public:
    bool Setup(void* targetFunc, void* detourFunc)
    {
        target = targetFunc;
        detour = detourFunc;

        // Save original bytes
        DWORD oldProtect;
        VirtualProtect(target, 14, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(originalBytes, target, 14);
        VirtualProtect(target, 14, oldProtect, &oldProtect);

        // Create trampoline (jump to original)
        // Simplified: just save the address
        original = target;

        hooked = true;
        return true;
    }

    void Enable()
    {
        if (!hooked) return;

        DWORD oldProtect;
        VirtualProtect(target, 14, PAGE_EXECUTE_READWRITE, &oldProtect);

        // Write JMP to detour
        uint8_t jmp[14] = { 0x48, 0xB8 }; // mov rax, <addr>
        memcpy(jmp + 2, &detour, 8);
        jmp[10] = 0xFF, jmp[11] = 0xE0; // jmp rax
        memcpy(target, jmp, 14);

        VirtualProtect(target, 14, oldProtect, &oldProtect);
        printf("[PRISM] Hook enabled at 0x%llX -> 0x%llX\n", (uintptr_t)target, (uintptr_t)detour);
    }

    void Disable()
    {
        if (!hooked) return;

        DWORD oldProtect;
        VirtualProtect(target, 14, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(target, originalBytes, 14);
        VirtualProtect(target, 14, oldProtect, &oldProtect);
        printf("[PRISM] Hook disabled at 0x%llX\n", (uintptr_t)target);
    }

    template<typename T>
    T GetOriginal() const { return (T)original; }
    bool IsHooked() const { return hooked; }
};

// ============================================================
//  Game Function Hooks
// ============================================================

// Store original function pointers
using FnOriginal = void*;

namespace GameHooks
{
    // SOCacheSubscribed - chamado quando o inventario e atualizado
    // Nosso hook re-injeta itens virtuais
    inline DetourHook hookSOCacheSubscribed;
    using FnSOCacheSubscribed = void(__fastcall*)(void* thisptr, uint32_t ownerID, int cacheType);
    inline FnSOCacheSubscribed originalSOCacheSubscribed = nullptr;

    // EquipItemInLoadout - chamado quando uma arma e equipada no loadout
    inline DetourHook hookEquipItemInLoadout;
    using FnEquipItemInLoadout = void(__fastcall*)(void* thisptr, int slot, int itemID);
    inline FnEquipItemInLoadout originalEquipItemInLoadout = nullptr;

    // ============================================================
    //  Hook Callbacks
    // ============================================================
    void __fastcall OnSOCacheSubscribed(void* thisptr, uint32_t ownerID, int cacheType)
    {
        printf("[PRISM] SOCacheSubscribed called! owner=%u type=%d\n", ownerID, cacheType);
        
        // Chamar original primeiro
        originalSOCacheSubscribed(thisptr, ownerID, cacheType);
        
        // Re-injetar itens virtuais no inventario
        // InventoryChanger::OnCacheSubscribed(ownerID);
    }

    void __fastcall OnEquipItemInLoadout(void* thisptr, int slot, int itemID)
    {
        printf("[PRISM] EquipItemInLoadout called! slot=%d itemID=%d\n", slot, itemID);
        
        // Chamar original
        originalEquipItemInLoadout(thisptr, slot, itemID);
        
        // Aplicar skin do inventario virtual
        // InventoryChanger::OnEquipItem(slot, itemID);
    }

    // ============================================================
    //  Setup All Hooks
    // ============================================================
    bool SetupAll(uintptr_t clientBase)
    {
        printf("[PRISM] Setting up game hooks...\n");
        
        // Encontrar enderecos das funcoes
        // Estes offsets precisam ser atualizados baseado no jogo
        // Por enquanto, placeholder - precisa ser preenchido com offsets reais
        
        printf("[PRISM] Hooks setup complete (placeholders)\n");
        return true;
    }

    void RemoveAll()
    {
        hookSOCacheSubscribed.Disable();
        hookEquipItemInLoadout.Disable();
        printf("[PRISM] All hooks removed\n");
    }
}
