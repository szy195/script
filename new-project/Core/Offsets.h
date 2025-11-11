#pragma once

#include <cstdint>

namespace Offsets {
    // 游戏基址偏移量
    namespace Game {
        constexpr uintptr_t GameTime = 0x2F8E5F8;
        constexpr uintptr_t GameTickCount = 0x2F8E5FC;
        constexpr uintptr_t ViewMatrixPtr = 0x1F8E5A0;
        constexpr uintptr_t ChatFunction = 0x2F8E600;
        constexpr uintptr_t MoveFunction = 0x2F8E604;
        constexpr uintptr_t AttackFunction = 0x2F8E608;
        constexpr uintptr_t SpellFunction = 0x2F8E60C;
        constexpr uintptr_t GameState = 0x2F8E610;
        constexpr uintptr_t ObjectManager = 0x18F8E50;
        constexpr uintptr_t LocalPlayer = 0x18F8E54;
    }
    
    // GameObject偏移量
    namespace GameObject {
        constexpr uintptr_t Type = 0x14;
        constexpr uintptr_t Team = 0x34;
        constexpr uintptr_t NamePtr = 0x38;
        constexpr uintptr_t PositionX = 0x1D0;
        constexpr uintptr_t PositionY = 0x1D4;
        constexpr uintptr_t PositionZ = 0x1D8;
        constexpr uintptr_t Health = 0x1E0;
        constexpr uintptr_t MaxHealth = 0x1E4;
        constexpr uintptr_t Mana = 0x1E8;
        constexpr uintptr_t MaxMana = 0x1EC;
        constexpr uintptr_t Armor = 0x1F0;
        constexpr uintptr_t MagicResist = 0x1F4;
        constexpr uintptr_t AttackDamage = 0x1F8;
        constexpr uintptr_t AttackSpeed = 0x1FC;
        constexpr uintptr_t MovementSpeed = 0x200;
        constexpr uintptr_t IsVisible = 0x204;
        constexpr uintptr_t IsTargetable = 0x205;
        constexpr uintptr_t IsInvulnerable = 0x206;
        constexpr uintptr_t IsDead = 0x207;
        constexpr uintptr_t Direction = 0x208;
        
        // 旧版偏移量（保留以备兼容性）
        constexpr uintptr_t OldPosition = 0x1D4;
        constexpr uintptr_t OldIsVisible = 0x1C0;
        constexpr uintptr_t OldIsAlive = 0x18;
        constexpr uintptr_t OldHealth = 0x10C0;
        constexpr uintptr_t OldMaxHealth = 0x10C4;
        constexpr uintptr_t OldMana = 0x290;
        constexpr uintptr_t OldMaxMana = 0x294;
        constexpr uintptr_t OldMovementSpeed = 0x12D8;
        constexpr uintptr_t OldNamePtr = 0x60;
    }
    
    // Hero偏移量
    namespace Hero {
        constexpr uintptr_t Level = 0x20C;
        constexpr uintptr_t Experience = 0x210;
        constexpr uintptr_t Gold = 0x214;
        constexpr uintptr_t StatusFlags = 0x218;
        constexpr uintptr_t SpellBook = 0x220;
        constexpr uintptr_t Inventory = 0x228;
        constexpr uintptr_t SpellSlot = 0x0;
        constexpr uintptr_t SpellLevel = 0x0;
        constexpr uintptr_t SpellCooldown = 0x4;
        constexpr uintptr_t SpellTotalCooldown = 0x8;
        constexpr uintptr_t SpellManaCost = 0xC;
        constexpr uintptr_t SpellCastTime = 0x10;
        constexpr uintptr_t SpellRange = 0x14;
        constexpr uintptr_t SpellDamage = 0x18;
        constexpr uintptr_t SpellCharges = 0x1C;
        constexpr uintptr_t SpellMaxCharges = 0x20;
        constexpr uintptr_t SpellChargeTimer = 0x24;
        constexpr uintptr_t SpellInfo = 0x200;
        constexpr uintptr_t SpellInfoLevel = 0x1C;
        constexpr uintptr_t SpellInfoCooldown = 0x20;
        constexpr uintptr_t SummonerSpellInfo = 0x220;
        constexpr uintptr_t ItemId = 0x0;
        constexpr uintptr_t ItemStacks = 0x4;
        constexpr uintptr_t ItemCharges = 0x8;
        constexpr uintptr_t ItemCooldown = 0xC;
        constexpr uintptr_t AbilityPower = 0x22C;
        constexpr uintptr_t CooldownReduction = 0x230;
        constexpr uintptr_t CriticalChance = 0x234;
        constexpr uintptr_t LifeSteal = 0x238;
        constexpr uintptr_t SpellVamp = 0x23C;
        constexpr uintptr_t ArmorPenetration = 0x240;
        constexpr uintptr_t MagicPenetration = 0x244;
        
        // 旧版偏移量（保留以备兼容性）
        constexpr uintptr_t OldLevel = 0x34E0;
        constexpr uintptr_t OldExperience = 0x34E4;
        constexpr uintptr_t OldGold = 0x1B30;
        constexpr uintptr_t OldAttackDamage = 0x12A0;
        constexpr uintptr_t OldAbilityPower = 0x12A4;
        constexpr uintptr_t OldArmor = 0x12A8;
        constexpr uintptr_t OldMagicResist = 0x12AC;
        constexpr uintptr_t OldCriticalChance = 0x12B0;
        constexpr uintptr_t OldLifeSteal = 0x12B4;
        constexpr uintptr_t OldSpellVamp = 0x12B8;
        constexpr uintptr_t OldCooldownReduction = 0x12BC;
    }
    
    // Minion偏移量
    namespace Minion {
        constexpr uintptr_t Type = 0x220;
        constexpr uintptr_t AttackDamage = 0x224;
        constexpr uintptr_t AttackSpeed = 0x228;
        constexpr uintptr_t AttackRange = 0x22C;
        constexpr uintptr_t GoldBounty = 0x230;
        constexpr uintptr_t ExperienceBounty = 0x234;
        constexpr uintptr_t RespawnTime = 0x238;
        constexpr uintptr_t PathIndex = 0x23C;
        constexpr uintptr_t PathLength = 0x240;
        constexpr uintptr_t Target = 0x244;
        
        // 旧版偏移量（保留以备兼容性）
        constexpr uintptr_t OldAttackDamage = 0x12A0;
        constexpr uintptr_t OldAttackSpeed = 0x12A4;
        constexpr uintptr_t OldArmor = 0x12A8;
        constexpr uintptr_t OldMagicResist = 0x12AC;
        constexpr uintptr_t OldHealthRegen = 0x12B0;
        constexpr uintptr_t OldManaRegen = 0x12B4;
        constexpr uintptr_t OldVisionRadius = 0x12B8;
    }
    
    // Turret偏移量
    namespace Turret {
        constexpr uintptr_t Type = 0x220;
        constexpr uintptr_t AttackDamage = 0x224;
        constexpr uintptr_t AttackSpeed = 0x228;
        constexpr uintptr_t AttackRange = 0x22C;
        constexpr uintptr_t GoldBounty = 0x230;
        constexpr uintptr_t Target = 0x234;
        constexpr uintptr_t IsDead = 0x238;
        
        // 旧版偏移量（保留以备兼容性）
        constexpr uintptr_t OldAttackDamage = 0x12A0;
        constexpr uintptr_t OldAttackSpeed = 0x12A4;
        constexpr uintptr_t OldArmor = 0x12A8;
        constexpr uintptr_t OldMagicResist = 0x12AC;
        constexpr uintptr_t OldHealthRegen = 0x12B0;
        constexpr uintptr_t OldVisionRadius = 0x12B8;
        constexpr uintptr_t OldIsInvulnerable = 0x1C4;
        constexpr uintptr_t OldIsTargetable = 0x1C8;
    }
    
    // GameObjectManager偏移量
    namespace GameObjectManager {
        constexpr uintptr_t ObjectListPtr = 0x10;
        constexpr uintptr_t ObjectCount = 0x14;
        constexpr uintptr_t NodeNext = 0x0;
        constexpr uintptr_t NodeObject = 0x20;
        
        // 旧版偏移量（保留以备兼容性）
        constexpr uintptr_t OldObjectListPtr = 0x18F0A88;
        constexpr uintptr_t OldLocalPlayerPtr = 0x18F0A80;
        constexpr uintptr_t OldObjectCount = 0x10;
        constexpr uintptr_t OldNodeHead = 0x8;
        constexpr uintptr_t OldNodeObject = 0x20;
    }
}