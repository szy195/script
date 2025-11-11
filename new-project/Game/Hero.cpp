#include "Hero.h"
#include "../Core/MemoryManager.h"
#include "../Core/Offsets.h"
#include "../Utils/Math.h"
#include <iostream>

// 构造函数
Hero::Hero(uintptr_t address) : GameObject(address, GameObjectType::Hero) {
    level = 1;
    experience = 0;
    gold = 0;
    isRecalling = false;
    isDashing = false;
    isStealthed = false;
    isTaunted = false;
    isCharmed = false;
    isFeared = false;
    isSilenced = false;
    isBlinded = false;
    isStunned = false;
    isRooted = false;
    isDisarmed = false;
    isSuppressed = false;
    isPolymorphed = false;
    isUntargetable = false;
    isGhosted = false;
    isSlowed = false;
    isHasted = false;
    
    // 初始化技能信息
    for (int i = 0; i < 5; ++i) {
        spells[i].slot = static_cast<SpellSlot>(i);
        spells[i].level = 0;
        spells[i].cooldown = 0.0f;
        spells[i].currentCooldown = 0.0f;
        spells[i].range = 0.0f;
        spells[i].damage = 0.0f;
        spells[i].manaCost = 0.0f;
        spells[i].isReady = false;
        spells[i].isLearned = false;
        spells[i].castTime = 0.0f;
        spells[i].totalCooldown = 0.0f;
        spells[i].charges = 0;
        spells[i].maxCharges = 0;
        spells[i].chargeTimer = 0.0f;
    }
    
    // 更新英雄特有数据
    UpdateHeroData();
}

// 析构函数
Hero::~Hero() {
}

// 更新数据
void Hero::UpdateData() {
    // 更新基类数据
    GameObject::UpdateData();
    
    // 更新英雄特有数据
    UpdateHeroData();
}

// 更新英雄特有数据
void Hero::UpdateHeroData() {
    auto memManager = MemoryManager::GetInstance();
    if (!memManager || address == 0) {
        return;
    }
    
    try {
        // 读取等级
        uint32_t levelValue;
        if (!memManager->Read(address + Offsets::Hero::Level, levelValue)) {
            levelValue = 1;
        }
        level = levelValue;
        
        // 读取经验值
        uint32_t expValue;
        if (!memManager->Read(address + Offsets::Hero::Experience, expValue)) {
            expValue = 0;
        }
        experience = expValue;
        
        // 读取金币
        uint32_t goldValue;
        if (!memManager->Read(address + Offsets::Hero::Gold, goldValue)) {
            goldValue = 0;
        }
        gold = goldValue;
        
        // 读取状态
        uint32_t statusFlags;
        if (!memManager->Read(address + Offsets::Hero::StatusFlags, statusFlags)) {
            statusFlags = 0;
        }
        
        isRecalling = (statusFlags & 0x1) != 0;
        isDashing = (statusFlags & 0x2) != 0;
        isStealthed = (statusFlags & 0x4) != 0;
        isTaunted = (statusFlags & 0x8) != 0;
        isCharmed = (statusFlags & 0x10) != 0;
        isFeared = (statusFlags & 0x20) != 0;
        isSilenced = (statusFlags & 0x40) != 0;
        isBlinded = (statusFlags & 0x80) != 0;
        isStunned = (statusFlags & 0x100) != 0;
        isRooted = (statusFlags & 0x200) != 0;
        isDisarmed = (statusFlags & 0x400) != 0;
        isSuppressed = (statusFlags & 0x800) != 0;
        isPolymorphed = (statusFlags & 0x1000) != 0;
        isUntargetable = (statusFlags & 0x2000) != 0;
        isGhosted = (statusFlags & 0x4000) != 0;
        isSlowed = (statusFlags & 0x8000) != 0;
        isHasted = (statusFlags & 0x10000) != 0;
        
        // 读取技能信息
        uintptr_t spellBookPtr;
        if (!memManager->Read(address + Offsets::Hero::SpellBook, spellBookPtr)) {
            spellBookPtr = 0;
        }
        if (spellBookPtr != 0) {
            for (int i = 0; i < 5; ++i) {
                uintptr_t spellPtr;
                if (!memManager->Read(spellBookPtr + i * Offsets::Hero::SpellSize, spellPtr)) {
                    spellPtr = 0;
                }
                if (spellPtr != 0) {
                    UpdateSpellInfo(i, spellPtr);
                }
            }
        }
        
        // 读取物品信息
        uintptr_t inventoryPtr;
        if (!memManager->Read(address + Offsets::Hero::Inventory, inventoryPtr)) {
            inventoryPtr = 0;
        }
        if (inventoryPtr != 0) {
            for (int i = 0; i < 7; ++i) {
                uintptr_t itemPtr;
                if (!memManager->Read(inventoryPtr + i * Offsets::Hero::ItemSize, itemPtr)) {
                    itemPtr = 0;
                }
                if (itemPtr != 0) {
                    uint32_t itemId, itemStacks, itemCharges;
                    float itemCooldown;
                    
                    if (!memManager->Read(itemPtr + Offsets::Hero::ItemID, itemId)) {
                        itemId = 0;
                    }
                    if (!memManager->Read(itemPtr + Offsets::Hero::ItemStacks, itemStacks)) {
                        itemStacks = 0;
                    }
                    if (!memManager->Read(itemPtr + Offsets::Hero::ItemCharges, itemCharges)) {
                        itemCharges = 0;
                    }
                    if (!memManager->Read(itemPtr + Offsets::Hero::ItemCooldown, itemCooldown)) {
                        itemCooldown = 0.0f;
                    }
                    
                    items[i].id = itemId;
                    items[i].stacks = itemStacks;
                    items[i].charges = itemCharges;
                    items[i].cooldown = itemCooldown;
                } else {
                    items[i].id = 0;
                    items[i].stacks = 0;
                    items[i].charges = 0;
                    items[i].cooldown = 0.0f;
                }
            }
        }
        
    } catch (...) {
        // 忽略异常
    }
}

// 更新技能信息
void Hero::UpdateSpellInfo(int index, uintptr_t spellPtr) {
    auto memManager = MemoryManager::GetInstance();
    if (!memManager || spellPtr == 0 || index < 0 || index >= 5) {
        return;
    }
    
    try {
        uint32_t spellLevel, spellCharges, spellMaxCharges;
        float spellCurrentCooldown, spellTotalCooldown, spellManaCost, spellCastTime, spellRange, spellDamage, spellChargeTimer;
        
        if (!memManager->Read(spellPtr + Offsets::Hero::SpellLevel, spellLevel)) {
            spellLevel = 0;
        }
        if (!memManager->Read(spellPtr + Offsets::Hero::SpellCurrentCooldown, spellCurrentCooldown)) {
            spellCurrentCooldown = 0.0f;
        }
        if (!memManager->Read(spellPtr + Offsets::Hero::SpellTotalCooldown, spellTotalCooldown)) {
            spellTotalCooldown = 0.0f;
        }
        if (!memManager->Read(spellPtr + Offsets::Hero::SpellManaCost, spellManaCost)) {
            spellManaCost = 0.0f;
        }
        if (!memManager->Read(spellPtr + Offsets::Hero::SpellCastTime, spellCastTime)) {
            spellCastTime = 0.0f;
        }
        if (!memManager->Read(spellPtr + Offsets::Hero::SpellRange, spellRange)) {
            spellRange = 0.0f;
        }
        if (!memManager->Read(spellPtr + Offsets::Hero::SpellDamage, spellDamage)) {
            spellDamage = 0.0f;
        }
        if (!memManager->Read(spellPtr + Offsets::Hero::SpellCharges, spellCharges)) {
            spellCharges = 0;
        }
        if (!memManager->Read(spellPtr + Offsets::Hero::SpellMaxCharges, spellMaxCharges)) {
            spellMaxCharges = 0;
        }
        if (!memManager->Read(spellPtr + Offsets::Hero::SpellChargeTimer, spellChargeTimer)) {
            spellChargeTimer = 0.0f;
        }
        
        spells[index].level = spellLevel;
        spells[index].currentCooldown = spellCurrentCooldown;
        spells[index].totalCooldown = spellTotalCooldown;
        spells[index].manaCost = spellManaCost;
        spells[index].castTime = spellCastTime;
        spells[index].range = spellRange;
        spells[index].damage = spellDamage;
        spells[index].charges = spellCharges;
        spells[index].maxCharges = spellMaxCharges;
        spells[index].chargeTimer = spellChargeTimer;
        
        spells[index].isLearned = spells[index].level > 0;
        spells[index].isReady = spells[index].isLearned && spells[index].currentCooldown <= 0.0f;
        
    } catch (...) {
        // 忽略异常
    }
}

// 获取等级
uint32_t Hero::GetLevel() const {
    return level;
}

// 获取经验值
uint32_t Hero::GetExperience() const {
    return experience;
}

// 获取金币
uint32_t Hero::GetGold() const {
    return gold;
}

// 是否在回城
bool Hero::IsRecalling() const {
    return isRecalling;
}

// 是否在冲刺
bool Hero::IsDashing() const {
    return isDashing;
}

// 是否潜行
bool Hero::IsStealthed() const {
    return isStealthed;
}

// 是否被嘲讽
bool Hero::IsTaunted() const {
    return isTaunted;
}

// 是否被魅惑
bool Hero::IsCharmed() const {
    return isCharmed;
}

// 是否被恐惧
bool Hero::IsFeared() const {
    return isFeared;
}

// 是否被沉默
bool Hero::IsSilenced() const {
    return isSilenced;
}

// 是否被致盲
bool Hero::IsBlinded() const {
    return isBlinded;
}

// 是否被眩晕
bool Hero::IsStunned() const {
    return isStunned;
}

// 是否被禁锢
bool Hero::IsRooted() const {
    return isRooted;
}

// 是否被缴械
bool Hero::IsDisarmed() const {
    return isDisarmed;
}

// 是否被压制
bool Hero::IsSuppressed() const {
    return isSuppressed;
}

// 是否被变形
bool Hero::IsPolymorphed() const {
    return isPolymorphed;
}

// 是否无法被选中
bool Hero::IsUntargetable() const {
    return isUntargetable;
}

// 是否鬼魂状态
bool Hero::IsGhosted() const {
    return isGhosted;
}

// 是否被减速
bool Hero::IsSlowed() const {
    return isSlowed;
}

// 是否被加速
bool Hero::IsHasted() const {
    return isHasted;
}

// 是否无法行动
bool Hero::IsCrowdControlled() const {
    return isStunned || isRooted || isSilenced || isTaunted || isCharmed || 
           isFeared || isBlinded || isDisarmed || isSuppressed || isPolymorphed;
}

// 是否可以行动
bool Hero::CanMove() const {
    return !IsDead() && !isStunned && !isRooted && !isSuppressed && !isRecalling;
}

// 是否可以攻击
bool Hero::CanAttack() const {
    return !IsDead() && !isStunned && !isRooted && !isDisarmed && !isSuppressed && !isRecalling;
}

// 是否可以使用技能
bool Hero::CanCastSpells() const {
    return !IsDead() && !isSilenced && !isSuppressed && !isRecalling;
}

// 获取技能信息
const SpellInfo& Hero::GetSpell(SpellSlot slot) const {
    int index = static_cast<int>(slot);
    if (index >= 0 && index < 5) {
        return spells[index];
    }
    static SpellInfo emptySpell;
    return emptySpell;
}

// 获取Q技能信息
const SpellInfo& Hero::GetQ() const {
    return GetSpell(SpellSlot::Q);
}

// 获取W技能信息
const SpellInfo& Hero::GetW() const {
    return GetSpell(SpellSlot::W);
}

// 获取E技能信息
const SpellInfo& Hero::GetE() const {
    return GetSpell(SpellSlot::E);
}

// 获取R技能信息
const SpellInfo& Hero::GetR() const {
    return GetSpell(SpellSlot::R);
}

// 获取D技能信息（召唤师技能1）
const SpellInfo& Hero::GetD() const {
    return GetSpell(SpellSlot::D);
}

// 获取F技能信息（召唤师技能2）
const SpellInfo& Hero::GetF() const {
    return GetSpell(SpellSlot::F);
}

// 技能是否就绪
bool Hero::IsSpellReady(SpellSlot slot) const {
    const SpellInfo& spell = GetSpell(slot);
    return spell.isReady;
}

// 技能是否已学习
bool Hero::IsSpellLearned(SpellSlot slot) const {
    const SpellInfo& spell = GetSpell(slot);
    return spell.isLearned;
}

// 获取物品信息
const ItemInfo& Hero::GetItem(int slot) const {
    if (slot >= 0 && slot < 7) {
        return items[slot];
    }
    static ItemInfo emptyItem;
    return emptyItem;
}

// 获取物品数量
int Hero::GetItemCount(uint32_t itemId) const {
    int count = 0;
    for (int i = 0; i < 7; ++i) {
        if (items[i].id == itemId) {
            count += items[i].stacks;
        }
    }
    return count;
}

// 是否有物品
bool Hero::HasItem(uint32_t itemId) const {
    return GetItemCount(itemId) > 0;
}

// 物品是否就绪
bool Hero::IsItemReady(int slot) const {
    if (slot < 0 || slot >= 7) {
        return false;
    }
    return items[slot].id != 0 && items[slot].cooldown <= 0.0f;
}

// 获取技能伤害
float Hero::GetSpellDamage(SpellSlot slot, const std::shared_ptr<GameObject>& target) const {
    const SpellInfo& spell = GetSpell(slot);
    if (!spell.isLearned || !spell.isReady || !target) {
        return 0.0f;
    }
    
    // 简单的伤害计算
    // 在实际项目中，需要根据英雄和技能进行更复杂的计算
    float damage = spell.damage;
    
    // 根据技能等级增加伤害
    damage += spell.level * 10.0f;
    
    // 根据英雄等级增加伤害
    damage += level * 5.0f;
    
    // 根据攻击力增加伤害
    damage += attackDamage * 0.5f;
    
    // 根据法术强度增加伤害（假设英雄有法术强度属性）
    float abilityPower = GetAbilityPower();
    damage += abilityPower * 0.6f;
    
    return damage;
}

// 获取技能范围
float Hero::GetSpellRange(SpellSlot slot) const {
    const SpellInfo& spell = GetSpell(slot);
    if (!spell.isLearned) {
        return 0.0f;
    }
    
    return spell.range;
}

// 获取技能冷却
float Hero::GetSpellCooldown(SpellSlot slot) const {
    const SpellInfo& spell = GetSpell(slot);
    if (!spell.isLearned) {
        return 0.0f;
    }
    
    return spell.currentCooldown;
}

// 获取技能充能数
uint32_t Hero::GetSpellCharges(SpellSlot slot) const {
    const SpellInfo& spell = GetSpell(slot);
    if (!spell.isLearned) {
        return 0;
    }
    
    return spell.charges;
}

// 获取法术强度（假设英雄有此属性）
float Hero::GetAbilityPower() const {
    auto memManager = MemoryManager::GetInstance();
    if (!memManager || address == 0) {
        return 0.0f;
    }
    
    float abilityPower;
    if (!memManager->Read(address + Offsets::Hero::AbilityPower, abilityPower)) {
        abilityPower = 0.0f;
    }
    return abilityPower;
}

// 获取冷却缩减（假设英雄有此属性）
float Hero::GetCooldownReduction() const {
    auto memManager = MemoryManager::GetInstance();
    if (!memManager || address == 0) {
        return 0.0f;
    }
    
    float cooldownReduction;
    if (!memManager->Read(address + Offsets::Hero::CooldownReduction, cooldownReduction)) {
        cooldownReduction = 0.0f;
    }
    return cooldownReduction;
}

// 获取暴击几率（假设英雄有此属性）
float Hero::GetCriticalChance() const {
    auto memManager = MemoryManager::GetInstance();
    if (!memManager || address == 0) {
        return 0.0f;
    }
    
    float criticalChance;
    if (!memManager->Read(address + Offsets::Hero::CriticalChance, criticalChance)) {
        criticalChance = 0.0f;
    }
    return criticalChance;
}

// 获取生命偷取（假设英雄有此属性）
float Hero::GetLifeSteal() const {
    auto memManager = MemoryManager::GetInstance();
    if (!memManager || address == 0) {
        return 0.0f;
    }
    
    float lifeSteal;
    if (!memManager->Read(address + Offsets::Hero::LifeSteal, lifeSteal)) {
        lifeSteal = 0.0f;
    }
    return lifeSteal;
}

// 获取法术吸血（假设英雄有此属性）
float Hero::GetSpellVamp() const {
    auto memManager = MemoryManager::GetInstance();
    if (!memManager || address == 0) {
        return 0.0f;
    }
    
    float spellVamp;
    if (!memManager->Read(address + Offsets::Hero::SpellVamp, spellVamp)) {
        spellVamp = 0.0f;
    }
    return spellVamp;
}

// 获取护甲穿透（假设英雄有此属性）
float Hero::GetArmorPenetration() const {
    auto memManager = MemoryManager::GetInstance();
    if (!memManager || address == 0) {
        return 0.0f;
    }
    
    float armorPenetration;
    if (!memManager->Read(address + Offsets::Hero::ArmorPenetration, armorPenetration)) {
        armorPenetration = 0.0f;
    }
    return armorPenetration;
}

// 获取魔法穿透（假设英雄有此属性）
float Hero::GetMagicPenetration() const {
    auto memManager = MemoryManager::GetInstance();
    if (!memManager || address == 0) {
        return 0.0f;
    }
    
    float magicPenetration;
    if (!memManager->Read(address + Offsets::Hero::MagicPenetration, magicPenetration)) {
        magicPenetration = 0.0f;
    }
    return magicPenetration;
}

// 计算对目标的伤害
float Hero::CalculateDamage(const std::shared_ptr<GameObject>& target, DamageType type) const {
    if (!target) {
        return 0.0f;
    }
    
    float damage = 0.0f;
    
    switch (type) {
        case DamageType::Physical:
            damage = attackDamage;
            // 计算护甲减免
            float armorPen = GetArmorPenetration();
            float effectiveArmor = Math::Max(0.0f, target->GetArmor() - armorPen);
            damage *= 100.0f / (100.0f + effectiveArmor);
            break;
            
        case DamageType::Magical:
            damage = GetAbilityPower();
            // 计算魔法抗性减免
            float magicPen = GetMagicPenetration();
            float effectiveMagicResist = Math::Max(0.0f, target->GetMagicResist() - magicPen);
            damage *= 100.0f / (100.0f + effectiveMagicResist);
            break;
            
        case DamageType::True:
            damage = attackDamage * 0.5f + GetAbilityPower() * 0.3f; // 真实伤害示例
            break;
    }
    
    return damage;
}

// 预测技能命中位置
Vector3 Hero::PredictSpellPosition(const std::shared_ptr<GameObject>& target, SpellSlot slot, float delay) const {
    if (!target) {
        return Vector3::Zero;
    }
    
    const SpellInfo& spell = GetSpell(slot);
    if (!spell.isLearned || spell.range <= 0.0f) {
        return target->GetPosition();
    }
    
    // 计算技能飞行时间
    float travelTime = delay;
    float spellSpeed = 1000.0f; // 假设技能速度
    if (spellSpeed > 0.0f) {
        travelTime += DistanceTo(target) / spellSpeed;
    }
    
    // 预测目标位置
    return target->PredictPosition(travelTime);
}

// 是否可以击杀目标
bool Hero::CanKill(const std::shared_ptr<GameObject>& target, SpellSlot slot) const {
    if (!target) {
        return false;
    }
    
    float damage = GetSpellDamage(slot, target);
    return damage >= target->GetHealth();
}

// 是否可以击杀目标（考虑所有技能）
bool Hero::CanKill(const std::shared_ptr<GameObject>& target) const {
    if (!target) {
        return false;
    }
    
    float totalDamage = 0.0f;
    
    // 计算普通攻击伤害
    totalDamage += CalculateDamage(target, DamageType::Physical);
    
    // 计算技能伤害
    for (int i = 0; i < 4; ++i) {
        SpellSlot slot = static_cast<SpellSlot>(i);
        if (IsSpellReady(slot)) {
            totalDamage += GetSpellDamage(slot, target);
        }
    }
    
    return totalDamage >= target->GetHealth();
}