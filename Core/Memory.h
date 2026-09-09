// ============================================================
// Core/Memory.h - Memory read/write utilities for CS2
// ============================================================

#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <vector>
#include <cstdint>

class Memory
{
private:
    HANDLE hProcess = nullptr;
    DWORD pid = 0;
    uintptr_t clientBase = 0;
    uintptr_t engineBase = 0;

public:
    // ============================================================
    //  Process Attachment
    // ============================================================
    bool Attach(const wchar_t* processName)
    {
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snap == INVALID_HANDLE_VALUE) return false;

        PROCESSENTRY32W pe;
        pe.dwSize = sizeof(pe);

        if (Process32FirstW(snap, &pe))
        {
            do
            {
                if (_wcsicmp(pe.szExeFile, processName) == 0)
                {
                    pid = pe.th32ProcessID;
                    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
                    CloseHandle(snap);
                    return hProcess != nullptr;
                }
            } while (Process32NextW(snap, &pe));
        }
        CloseHandle(snap);
        return false;
    }

    // ============================================================
    //  Module Base Address
    // ============================================================
    uintptr_t GetModuleBase(const wchar_t* moduleName)
    {
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
        if (snap == INVALID_HANDLE_VALUE) return 0;

        MODULEENTRY32W me;
        me.dwSize = sizeof(me);

        if (Module32FirstW(snap, &me))
        {
            do
            {
                if (_wcsicmp(me.szModule, moduleName) == 0)
                {
                    uintptr_t base = (uintptr_t)me.modBaseAddr;
                    CloseHandle(snap);
                    return base;
                }
            } while (Module32NextW(snap, &me));
        }
        CloseHandle(snap);
        return 0;
    }

    void CacheBases()
    {
        clientBase = GetModuleBase(L"client.dll");
        engineBase = GetModuleBase(L"engine2.dll");
        printf("[PRISM] client.dll base: 0x%llX\n", clientBase);
        printf("[PRISM] engine2.dll base: 0x%llX\n", engineBase);
    }

    // ============================================================
    //  Memory Read/Write
    // ============================================================
    template<typename T>
    T Read(uintptr_t address)
    {
        T value{};
        ReadProcessMemory(hProcess, (LPCVOID)address, &value, sizeof(T), nullptr);
        return value;
    }

    template<typename T>
    void Write(uintptr_t address, T value)
    {
        WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(T), nullptr);
    }

    std::string ReadString(uintptr_t address, size_t maxLength = 64)
    {
        std::vector<char> buffer(maxLength);
        ReadProcessMemory(hProcess, (LPCVOID)address, buffer.data(), maxLength, nullptr);
        return std::string(buffer.data());
    }

    // Read 4x4 float matrix (view matrix)
    std::vector<float> ReadMatrix(uintptr_t address)
    {
        std::vector<float> matrix(16);
        ReadProcessMemory(hProcess, (LPCVOID)address, matrix.data(), 16 * sizeof(float), nullptr);
        return matrix;
    }

    // ============================================================
    //  Handle to Entity
    // ============================================================
    uintptr_t HandleToEntity(uint32_t handle)
    {
        if (handle == 0 || handle == 0xFFFFFFFF) return 0;
        
        uintptr_t entityList = Read<uintptr_t>(clientBase + Offsets::dwEntityList);
        if (entityList == 0) return 0;

        int index = handle & 0x3FFF;
        int serial = handle >> 14;

        uintptr_t listEntry = Read<uintptr_t>(entityList + 0x8 * ((index >> 9) + 1));
        if (listEntry == 0) return 0;

        uintptr_t controller = Read<uintptr_t>(listEntry + 0x78 * (index & 0x1FF));
        if (controller == 0) return 0;

        int checkSerial = Read<int>(controller + 0x10);
        if (checkSerial != serial) return 0;

        return controller;
    }

    // ============================================================
    //  Getters
    // ============================================================
    HANDLE GetHandle() const { return hProcess; }
    DWORD GetPID() const { return pid; }
    uintptr_t GetClientBase() const { return clientBase; }
    uintptr_t GetEngineBase() const { return engineBase; }
    bool IsValid() const { return hProcess != nullptr && hProcess != INVALID_HANDLE_VALUE; }

    void Detach()
    {
        if (hProcess) CloseHandle(hProcess);
        hProcess = nullptr;
    }
};

// Global instance
inline Memory mem;
