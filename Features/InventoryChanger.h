// ============================================================
// Features/InventoryChanger.h - CS2 Inventory Changer
// Adiciona skins reais ao inventario do jogador
// ============================================================

#pragma once
#include "../Core/Memory.h"
#include "../Core/Offsets.h"
#include <vector>
#include <string>
#include <map>

// ============================================================
//  Item Structure
// ============================================================
struct InventoryItem
{
    int weaponDefIndex;      // Ex: 7 = AK-47, 9 = AWP, 507 = Karambit
    int paintKit;            // Paint kit ID
    int seed;                // Pattern seed
    float wear;              // Float value (0.0 = Factory New)
    int stattrak;            // StatTrak kills (-1 = sem StatTrak)
    int rarity;              // Raridade (0=Consumer, 6=Contraband)
    std::string skinName;    // Nome da skin
    bool isKnife;            // Se e faca
    bool isGlove;            // Se e luva
};

// ============================================================
//  Knife Definitions
// ============================================================
namespace Knives
{
    constexpr int DEFAULT_T = 500;    // Default T knife
    constexpr int DEFAULT_CT = 503;   // Default CT knife
    constexpr int BAYONET = 500;
    constexpr int M9_BAYONET = 508;
    constexpr int KARAMBIT = 507;
    constexpr int FLIP = 505;
    constexpr int GUT = 506;
    constexpr int BOWIE = 514;
    constexpr int BUTTERFLY = 515;
    constexpr int SHADOW_DAGGERS = 516;
    constexpr int HUNTSMAN = 517;
    constexpr int FALCHION = 512;
    constexpr int NAVAJA = 518;
    constexpr int STILETTO = 519;
    constexpr int TALON = 520;
    constexpr int SKELETON = 525;
    constexpr int NOMAD = 521;
    constexpr int PARACORD = 522;
    constexpr int SURVIVAL = 523;
    constexpr int URSUS = 524;
    constexpr int CLASSIC = 526;
}

// ============================================================
//  Inventory Changer Class
// ============================================================
class InventoryChanger
{
private:
    std::vector<InventoryItem> virtualItems;
    bool initialized = false;
    uintptr_t localPawn = 0;
    int localTeam = 0;

    // ============================================================
    //  Entity Access
    // ============================================================
    uintptr_t GetLocalPawn()
    {
        uintptr_t controller = mem.Read<uintptr_t>(mem.GetClientBase() + Offsets::dwLocalPlayerController);
        if (controller == 0) return 0;
        uint32_t pawnHandle = mem.Read<uint32_t>(controller + Offsets::m_hPlayerPawn);
        return mem.HandleToEntity(pawnHandle);
    }

    uintptr_t GetWeaponEntity(uintptr_t weaponHandle)
    {
        return mem.HandleToEntity(weaponHandle);
    }

    // ============================================================
    //  Apply Skin to Weapon Entity
    // ============================================================
    bool ApplySkinToWeapon(uintptr_t weapon, const InventoryItem& item)
    {
        if (weapon == 0) return false;

        // Get attribute manager -> item view
        uintptr_t attrManager = mem.Read<uintptr_t>(weapon + Offsets::m_AttributeManager);
        if (attrManager == 0) return false;

        uintptr_t itemView = mem.Read<uintptr_t>(attrManager + Offsets::m_Item);
        if (itemView == 0) return false;

        // Change item definition index (pra trocar modelo de faca/luva)
        if (item.isKnife || item.isGlove)
        {
            int currentDef = mem.Read<int>(itemView + Offsets::m_iItemDefinitionIndex);
            if (currentDef != item.weaponDefIndex)
            {
                mem.Write<int>(itemView + Offsets::m_iItemDefinitionIndex, item.weaponDefIndex);
            }
        }

        // Apply paint kit, seed, wear
        mem.Write<int>(weapon + Offsets::m_nFallbackPaintKit, item.paintKit);
        mem.Write<int>(weapon + Offsets::m_nFallbackSeed, item.seed % 1000);
        mem.Write<float>(weapon + Offsets::m_flFallbackWear, item.wear);

        // StatTrak
        if (item.stattrak >= 0)
            mem.Write<int>(weapon + Offsets::m_nFallbackStatTrak, item.stattrak);

        // Force skin update
        mem.Write<int>(itemView + Offsets::m_iItemIDHigh, -1);
        mem.Write<int>(itemView + Offsets::m_iItemIDLow, 0);

        // Set account ID
        int xuidLow = mem.Read<int>(weapon + Offsets::m_OriginalOwnerXuidLow);
        if (xuidLow != 0)
            mem.Write<int>(itemView + Offsets::m_iAccountID, xuidLow);

        // Reset attributes to force refresh
        if (Offsets::m_bAttributesInitialized != 0)
            mem.Write<int>(weapon + Offsets::m_bAttributesInitialized, 0);

        return true;
    }

    // ============================================================
    //  Find Weapon in Inventory
    // ============================================================
    int FindWeaponIndex(int defIndex)
    {
        for (int i = 0; i < (int)virtualItems.size(); i++)
        {
            if (virtualItems[i].weaponDefIndex == defIndex)
                return i;
        }
        return -1;
    }

public:
    // ============================================================
    //  Add Item to Virtual Inventory
    // ============================================================
    void AddItem(const InventoryItem& item)
    {
        // Check if item already exists
        int existing = FindWeaponIndex(item.weaponDefIndex);
        if (existing >= 0)
        {
            virtualItems[existing] = item;
            printf("[PRISM] Item updated: %s (def=%d, paint=%d)\n", 
                   item.skinName.c_str(), item.weaponDefIndex, item.paintKit);
        }
        else
        {
            virtualItems.push_back(item);
            printf("[PRISM] Item added: %s (def=%d, paint=%d)\n", 
                   item.skinName.c_str(), item.weaponDefIndex, item.paintKit);
        }
    }

    // ============================================================
    //  Remove Item
    // ============================================================
    void RemoveItem(int defIndex)
    {
        for (auto it = virtualItems.begin(); it != virtualItems.end(); ++it)
        {
            if (it->weaponDefIndex == defIndex)
            {
                virtualItems.erase(it);
                printf("[PRISM] Item removed (def=%d)\n", defIndex);
                return;
            }
        }
    }

    // ============================================================
    //  Get all virtual items
    // ============================================================
    const std::vector<InventoryItem>& GetItems() const { return virtualItems; }

    // ============================================================
    //  Main Tick - Aplica skins a cada arma no inventario
    // ============================================================
    void Tick()
    {
        localPawn = GetLocalPawn();
        if (localPawn == 0) return;

        // Get team
        uintptr_t controller = mem.Read<uintptr_t>(mem.GetClientBase() + Offsets::dwLocalPlayerController);
        if (controller == 0) return;
        localTeam = mem.Read<int>(controller + Offsets::m_iTeamNum);

        // Get weapon services
        uintptr_t weaponServices = mem.Read<uintptr_t>(localPawn + Offsets::m_pWeaponServices);
        if (weaponServices == 0) return;

        uintptr_t myWeaponsAddr = weaponServices + Offsets::m_hMyWeapons;

        // Iterate through all weapons
        for (int i = 0; i < 64; i++)
        {
            try
            {
                uint32_t handle = mem.Read<uint32_t>(myWeaponsAddr + (uintptr_t)i * 4);
                if (handle == 0 || handle == 0xFFFFFFFF) continue;

                uintptr_t weapon = GetWeaponEntity(handle);
                if (weapon == 0) continue;

                // Get current weapon definition index
                uintptr_t attrManager = mem.Read<uintptr_t>(weapon + Offsets::m_AttributeManager);
                if (attrManager == 0) continue;

                uintptr_t itemView = mem.Read<uintptr_t>(attrManager + Offsets::m_Item);
                if (itemView == 0) continue;

                int itemDef = mem.Read<int>(itemView + Offsets::m_iItemDefinitionIndex);

                // Find matching virtual item
                int itemIdx = FindWeaponIndex(itemDef);
                if (itemIdx >= 0)
                {
                    ApplySkinToWeapon(weapon, virtualItems[itemIdx]);
                }
            }
            catch (...)
            {
                continue;
            }
        }
    }

    // ============================================================
    //  Quick Setup - Add common skins
    // ============================================================
    void SetupDefaults()
    {
        // AK-47 | Redline
        AddItem({ 7, 279, 661, 0.01f, -1, 6, "AK-47 | Redline", false, false });

        // AWP | Dragon Lore
        AddItem({ 9, 344, 36, 0.001f, -1, 6, "AWP | Dragon Lore", false, false });

        // Karambit | Fade
        AddItem({ 507, 38, 100, 0.001f, -1, 6, "Karambit | Fade", true, false });

        // M9 Bayonet | Doppler
        AddItem({ 508, 416, 568, 0.005f, -1, 6, "M9 Bayonet | Doppler", true, false });

        // Butterfly Knife | Crimson Web
        AddItem({ 515, 12, 413, 0.02f, -1, 6, "Butterfly | Crimson Web", true, false });

        // Sport Gloves | Superconductor
        AddItem({ 5028, 10066, 661, 0.01f, -1, 6, "Sport Gloves | Superconductor", false, true });

        printf("[PRISM] Default skins loaded: %d items\n", (int)virtualItems.size());
    }

    // ============================================================
    //  Print Inventory
    // ============================================================
    void PrintInventory()
    {
        printf("\n=== PRISM Inventory Changer ===\n");
        printf("Virtual items: %d\n", (int)virtualItems.size());
        for (int i = 0; i < (int)virtualItems.size(); i++)
        {
            const auto& item = virtualItems[i];
            printf("  [%d] %s | def=%d paint=%d seed=%d wear=%.3f%s%s\n",
                   i, item.skinName.c_str(), item.weaponDefIndex,
                   item.paintKit, item.seed, item.wear,
                   item.isKnife ? " [KNIFE]" : "",
                   item.isGlove ? " [GLOVE]" : "");
        }
        printf("==============================\n\n");
    }

    bool IsInitialized() const { return initialized; }
    int GetItemCount() const { return (int)virtualItems.size(); }
};
