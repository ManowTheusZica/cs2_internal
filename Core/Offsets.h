// ============================================================
// Core/Offsets.h - Dynamic offset loading from cs2-dumper
// ============================================================

#pragma once
#include <Windows.h>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>

// ============================================================
//  Global Offsets (atualizados automaticamente)
// ============================================================
namespace Offsets
{
    // client.dll
    inline uintptr_t dwEntityList = 0;
    inline uintptr_t dwLocalPlayerController = 0;
    inline uintptr_t dwLocalPlayerPawn = 0;
    inline uintptr_t dwViewMatrix = 0;
    inline uintptr_t dwCSGOInput = 0;
    inline uintptr_t dwViewAngles = 0;
    inline uintptr_t dwSensitivity = 0;
    inline uintptr_t dwSensitivity_sensitivity = 0;
    inline uintptr_t dwGameRules = 0;
    inline uintptr_t dwGlobalVars = 0;

    // Entity offsets (C_BaseEntity)
    inline uintptr_t m_iHealth = 0;
    inline uintptr_t m_iTeamNum = 0;
    inline uintptr_t m_vOldOrigin = 0;
    inline uintptr_t m_pGameSceneNode = 0;
    inline uintptr_t m_bDormant = 0;
    inline uintptr_t m_iszPlayerName = 0;
    inline uintptr_t m_hPlayerPawn = 0;
    inline uintptr_t m_lifeState = 0;
    inline uintptr_t m_iShotsFired = 0;
    inline uintptr_t m_vecViewOffset = 0;
    inline uintptr_t m_vecVelocity = 0;
    inline uintptr_t m_fFlags = 0;
    inline uintptr_t m_iIDEntIndex = 0;
    inline uintptr_t m_modelState = 0;
    inline uintptr_t m_pAimPunchServices = 0;
    inline uintptr_t m_entitySpottedState = 0;
    inline uintptr_t m_bSpotted = 0;
    inline uintptr_t m_unpredictableBaseTick = 0;
    inline uintptr_t m_nNextThinkTick = 0;
    inline uintptr_t m_pWeaponServices = 0;
    inline uintptr_t m_hMyWeapons = 0;
    inline uintptr_t m_hActiveWeapon = 0;
    inline uintptr_t m_AttributeManager = 0;
    inline uintptr_t m_Item = 0;
    inline uintptr_t m_iItemDefinitionIndex = 0;
    inline uintptr_t m_iItemIDHigh = 0;
    inline uintptr_t m_iItemIDLow = 0;
    inline uintptr_t m_iAccountID = 0;
    inline uintptr_t m_nFallbackPaintKit = 0;
    inline uintptr_t m_nFallbackSeed = 0;
    inline uintptr_t m_flFallbackWear = 0;
    inline uintptr_t m_nFallbackStatTrak = 0;
    inline uintptr_t m_OriginalOwnerXuidLow = 0;
    inline uintptr_t m_OriginalOwnerXuidHigh = 0;
    inline uintptr_t m_bAttributesInitialized = 0;
    inline uintptr_t m_flFlashDuration = 0;

    // ============================================================
    //  Fallback offsets (aug 2026 - cs2-dumper)
    // ============================================================
    namespace Fallback
    {
        constexpr uintptr_t dwEntityList = 0x2571220;
        constexpr uintptr_t dwLocalPlayerController = 0x23A0F30;
        constexpr uintptr_t dwLocalPlayerPawn = 0x23C6268;
        constexpr uintptr_t dwViewMatrix = 0x23CB830;
        constexpr uintptr_t dwCSGOInput = 0x23DBC70;
        constexpr uintptr_t dwViewAngles = 0x23DC2F8;
        constexpr uintptr_t dwSensitivity = 0x23C3578;
        constexpr uintptr_t dwSensitivity_sensitivity = 0x58;
        constexpr uintptr_t dwGameRules = 0x23C5D28;
        constexpr uintptr_t dwGlobalVars = 0x20AF5F0;

        constexpr uintptr_t m_iHealth = 0x34C;
        constexpr uintptr_t m_iTeamNum = 0x3E7;
        constexpr uintptr_t m_vOldOrigin = 0x13B8;
        constexpr uintptr_t m_pGameSceneNode = 0x330;
        constexpr uintptr_t m_bDormant = 0x103;
        constexpr uintptr_t m_iszPlayerName = 0x768;
        constexpr uintptr_t m_hPlayerPawn = 0x914;
        constexpr uintptr_t m_lifeState = 0x354;
        constexpr uintptr_t m_iShotsFired = 0x1C8C;
        constexpr uintptr_t m_vecViewOffset = 0xE78;
        constexpr uintptr_t m_vecVelocity = 0x430;
        constexpr uintptr_t m_fFlags = 0x3F4;
        constexpr uintptr_t m_iIDEntIndex = 0x342C;
        constexpr uintptr_t m_modelState = 0x140;
        constexpr uintptr_t m_pAimPunchServices = 0x14B8;
        constexpr uintptr_t m_entitySpottedState = 0x1C60;
        constexpr uintptr_t m_bSpotted = 0x8;
        constexpr uintptr_t m_unpredictableBaseTick = 0x14;
        constexpr uintptr_t m_nNextThinkTick = 0x3EC;
        constexpr uintptr_t m_pWeaponServices = 0x1208;
        constexpr uintptr_t m_hMyWeapons = 0x60;
        constexpr uintptr_t m_hActiveWeapon = 0x54;
        constexpr uintptr_t m_AttributeManager = 0x11A8;
        constexpr uintptr_t m_Item = 0x50;
        constexpr uintptr_t m_iItemDefinitionIndex = 0x1BA;
        constexpr uintptr_t m_iItemIDHigh = 0x1D0;
        constexpr uintptr_t m_iItemIDLow = 0x1D4;
        constexpr uintptr_t m_iAccountID = 0x1D8;
        constexpr uintptr_t m_nFallbackPaintKit = 0x1680;
        constexpr uintptr_t m_nFallbackSeed = 0x1684;
        constexpr uintptr_t m_flFallbackWear = 0x1688;
        constexpr uintptr_t m_nFallbackStatTrak = 0x168C;
        constexpr uintptr_t m_OriginalOwnerXuidLow = 0x1678;
        constexpr uintptr_t m_bAttributesInitialized = 0x11A0;
        constexpr uintptr_t m_flFlashDuration = 0x1428;
    }

    // ============================================================
    //  Load offsets from JSON (cs2-dumper format)
    // ============================================================
    inline bool LoadFromJson(const std::string& json)
    {
        // Parse simple JSON - offsets.json from cs2-dumper
        // Format: { "client.dll": { "dwEntityList": 39260704, ... } }
        
        // For simplicity, try to download and parse
        // In production, use a JSON library like nlohmann/json
        
        // Find client.dll section
        auto clientStart = json.find("\"client.dll\"");
        if (clientStart == std::string::npos) return false;
        
        auto blockStart = json.find('{', clientStart);
        if (blockStart == std::string::npos) return false;
        
        // Extract key-value pairs
        size_t pos = blockStart + 1;
        while (pos < json.size())
        {
            auto keyStart = json.find('\"', pos);
            if (keyStart == std::string::npos || keyStart > blockStart + 2000) break;
            auto keyEnd = json.find('\"', keyStart + 1);
            if (keyEnd == std::string::npos) break;
            
            std::string key = json.substr(keyStart + 1, keyEnd - keyStart - 1);
            
            auto valStart = json.find(':', keyEnd);
            if (valStart == std::string::npos) break;
            auto valEnd = json.find_first_of(",}", valStart);
            if (valEnd == std::string::npos) break;
            
            std::string valStr = json.substr(valStart + 1, valEnd - valStart - 1);
            
            // Trim whitespace
            size_t vStart = valStr.find_first_not_of(" \t\n\r");
            size_t vEnd = valStr.find_last_not_of(" \t\n\r");
            if (vStart != std::string::npos)
                valStr = valStr.substr(vStart, vEnd - vStart + 1);
            
            try {
                uintptr_t val = std::stoull(valStr);
                SetOffset(key, val);
            } catch (...) {}
            
            pos = valEnd + 1;
        }
        
        // Set derived offsets
        if (dwCSGOInput != 0 && dwViewAngles == 0)
            dwViewAngles = dwCSGOInput + 0x10C;
        
        return true;
    }

    inline void SetOffset(const std::string& name, uintptr_t value)
    {
        if (name == "dwEntityList") dwEntityList = value;
        else if (name == "dwLocalPlayerController") dwLocalPlayerController = value;
        else if (name == "dwLocalPlayerPawn") dwLocalPlayerPawn = value;
        else if (name == "dwViewMatrix") dwViewMatrix = value;
        else if (name == "dwCSGOInput") dwCSGOInput = value;
        else if (name == "dwViewAngles") dwViewAngles = value;
        else if (name == "dwSensitivity") dwSensitivity = value;
        else if (name == "dwSensitivity_sensitivity") dwSensitivity_sensitivity = value;
        else if (name == "dwGameRules") dwGameRules = value;
        else if (name == "dwGlobalVars") dwGlobalVars = value;
    }

    // ============================================================
    //  Apply fallback offsets
    // ============================================================
    inline void UseFallback()
    {
        dwEntityList = Fallback::dwEntityList;
        dwLocalPlayerController = Fallback::dwLocalPlayerController;
        dwLocalPlayerPawn = Fallback::dwLocalPlayerPawn;
        dwViewMatrix = Fallback::dwViewMatrix;
        dwCSGOInput = Fallback::dwCSGOInput;
        dwViewAngles = Fallback::dwViewAngles;
        dwSensitivity = Fallback::dwSensitivity;
        dwSensitivity_sensitivity = Fallback::dwSensitivity_sensitivity;
        dwGameRules = Fallback::dwGameRules;
        dwGlobalVars = Fallback::dwGlobalVars;

        m_iHealth = Fallback::m_iHealth;
        m_iTeamNum = Fallback::m_iTeamNum;
        m_vOldOrigin = Fallback::m_vOldOrigin;
        m_pGameSceneNode = Fallback::m_pGameSceneNode;
        m_bDormant = Fallback::m_bDormant;
        m_iszPlayerName = Fallback::m_iszPlayerName;
        m_hPlayerPawn = Fallback::m_hPlayerPawn;
        m_lifeState = Fallback::m_lifeState;
        m_iShotsFired = Fallback::m_iShotsFired;
        m_vecViewOffset = Fallback::m_vecViewOffset;
        m_vecVelocity = Fallback::m_vecVelocity;
        m_fFlags = Fallback::m_fFlags;
        m_iIDEntIndex = Fallback::m_iIDEntIndex;
        m_modelState = Fallback::m_modelState;
        m_pAimPunchServices = Fallback::m_pAimPunchServices;
        m_entitySpottedState = Fallback::m_entitySpottedState;
        m_bSpotted = Fallback::m_bSpotted;
        m_unpredictableBaseTick = Fallback::m_unpredictableBaseTick;
        m_nNextThinkTick = Fallback::m_nNextThinkTick;
        m_pWeaponServices = Fallback::m_pWeaponServices;
        m_hMyWeapons = Fallback::m_hMyWeapons;
        m_hActiveWeapon = Fallback::m_hActiveWeapon;
        m_AttributeManager = Fallback::m_AttributeManager;
        m_Item = Fallback::m_Item;
        m_iItemDefinitionIndex = Fallback::m_iItemDefinitionIndex;
        m_iItemIDHigh = Fallback::m_iItemIDHigh;
        m_iItemIDLow = Fallback::m_iItemIDLow;
        m_iAccountID = Fallback::m_iAccountID;
        m_nFallbackPaintKit = Fallback::m_nFallbackPaintKit;
        m_nFallbackSeed = Fallback::m_nFallbackSeed;
        m_flFallbackWear = Fallback::m_flFallbackWear;
        m_nFallbackStatTrak = Fallback::m_nFallbackStatTrak;
        m_OriginalOwnerXuidLow = Fallback::m_OriginalOwnerXuidLow;
        m_bAttributesInitialized = Fallback::m_bAttributesInitialized;
        m_flFlashDuration = Fallback::m_flFlashDuration;
    }
}
