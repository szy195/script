#pragma once

#include "GameObject.h"
#include "../Core/Offsets.h"
#include "../Utils/Math.h"
#include <string>
#include <vector>

// 英雄角色枚举
enum class Champion {
    Unknown = 0,
    
    // 坦克
    Alistar = 12,
    Amumu = 32,
    DrMundo = 36,
    Galio = 3,
    Garen = 86,
    Leona = 89,
    Malphite = 54,
    Maokai = 57,
    Nautilus = 111,
    Ornn = 516,
    Rammus = 33,
    Renekton = 58,
    Sejuani = 113,
    Sion = 14,
    TahmKench = 223,
    Taric = 44,
    Thresh = 412,
    Zac = 154,
    
    // 战士
    Aatrox = 266,
    Camille = 164,
    Darius = 122,
    Fiora = 114,
    Gangplank = 41,
    Gnar = 150,
    Illaoi = 420,
    Irelia = 39,
    JarvanIV = 59,
    Jax = 24,
    Kayn = 141,
    Kled = 240,
    LeeSin = 64,
    Mordekaiser = 82,
    Nocturne = 56,
    Olaf = 2,
    Pantheon = 80,
    Poppy = 78,
    Quinn = 133,
    Rengar = 107,
    Riven = 92,
    Sett = 875,
    Shen = 98,
    Trundle = 48,
    Tryndamere = 23,
    Urgot = 6,
    Vi = 254,
    Volibear = 106,
    Warwick = 19,
    Wukong = 62,
    XinhZhao = 5,
    Yorick = 83,
    
    // 刺客
    Akali = 84,
    Diana = 131,
    Ekko = 245,
    Evelynn = 28,
    Fizz = 105,
    Kassadin = 38,
    KaynAssassin = 141,
    Khazix = 121,
    Katarina = 55,
    KogMaw = 96,
    Leblanc = 7,
    MasterYi = 11,
    NocturneAssassin = 56,
    Pyke = 555,
    Qiyana = 246,
    RengarAssassin = 107,
    Shaco = 35,
    Talon = 91,
    Teemo = 17,
    TwistedFate = 4,
    Veigar = 45,
    Zed = 238,
    
    // 法师
    Ahri = 103,
    Anivia = 34,
    Annie = 1,
    Aphelios = 523,
    Azir = 268,
    Brand = 63,
    Cassiopeia = 69,
    Corki = 42,
    Heimerdinger = 74,
    Karthus = 30,
    KassadinMage = 38,
    KatarinaMage = 55,
    LeblancMage = 7,
    Lissandra = 127,
    Lux = 99,
    Malzahar = 90,
    Morgana = 25,
    Nidalee = 76,
    Orianna = 61,
    Ryze = 13,
    Syndra = 134,
    Taliyah = 163,
    TwistedFateMage = 4,
    VeigarMage = 45,
    Viktor = 112,
    Vladimir = 8,
    Xerath = 101,
    Ziggs = 115,
    Zyra = 143,
    
    // 射手
    Ashe = 22,
    Caitlyn = 51,
    Draven = 119,
    Ezreal = 81,
    Jhin = 202,
    Jinx = 222,
    Kalista = 429,
    KogMawADC = 96,
    Lucian = 236,
    MissFortune = 21,
    Samira = 360,
    Senna = 235,
    SettADC = 875,
    Sivir = 15,
    Tristana = 18,
    Twitch = 29,
    Varus = 110,
    Vayne = 67,
    Xayah = 498,
    Zeri = 221,
    
    // 辅助
    Bard = 432,
    Blitzcrank = 53,
    BrandSupport = 63,
    Fiddlesticks = 9,
    Janna = 40,
    Karma = 43,
    Lulu = 117,
    LuxSupport = 99,
    MorganaSupport = 25,
    Nami = 267,
    NautilusSupport = 111,
    PykeSupport = 555,
    Rakan = 497,
    SennaSupport = 235,
    Soraka = 16,
    Swain = 50,
    TahmKenchSupport = 223,
    TaricSupport = 44,
    ThreshSupport = 412,
    Velkoz = 161,
    Yuumi = 350,
    Zilean = 26,
    ZyraSupport = 143
};

// 技能槽位枚举
enum class SpellSlot {
    Q = 0,
    W = 1,
    E = 2,
    R = 3,
    D = 4,  // 召唤师技能1
    F = 5   // 召唤师技能2
};

// 技能信息结构体
struct SpellInfo {
    SpellSlot slot;
    std::string name;
    std::string displayName;
    float cooldown;
    float currentCooldown;
    float range;
    float width;
    float speed;
    float castTime;
    bool isReady;
    bool isLearned;
    int level;
    
    SpellInfo() : slot(SpellSlot::Q), cooldown(0.0f), currentCooldown(0.0f),
        range(0.0f), width(0.0f), speed(0.0f), castTime(0.0f),
        isReady(false), isLearned(false), level(0) {}
};

// 英雄类
class Hero : public GameObject {
private:
    Champion champion;
    int level;
    float experience;
    float gold;
    float attackDamage;
    float abilityPower;
    float armor;
    float magicResist;
    float criticalChance;
    float lifeSteal;
    float spellVamp;
    float cooldownReduction;
    std::vector<SpellInfo> spells;
    
public:
    // 构造函数
    Hero(uintptr_t addr = 0) : GameObject(addr), champion(Champion::Unknown),
        level(0), experience(0.0f), gold(0.0f), attackDamage(0.0f), abilityPower(0.0f),
        armor(0.0f), magicResist(0.0f), criticalChance(0.0f), lifeSteal(0.0f),
        spellVamp(0.0f), cooldownReduction(0.0f) {
        
        // 初始化技能信息
        spells.resize(6); // Q, W, E, R, D, F
        for (int i = 0; i < 6; i++) {
            spells[i].slot = static_cast<SpellSlot>(i);
        }
        
        // 设置类型为英雄
        SetType(GameObjectType::AIHeroClient);
    }
    
    // 更新英雄数据
    void UpdateData() override {
        GameObject::UpdateData();
        
        if (address == 0) {
            return;
        }
        
        MemoryManager* memManager = MemoryManager::GetInstance();
        if (!memManager) {
            return;
        }
        
        // 读取英雄特有属性
        if (!memManager->Read(address + Offsets::Hero::Level, level)) {
            level = 0;
        }
        if (!memManager->Read(address + Offsets::Hero::Experience, experience)) {
            experience = 0.0f;
        }
        if (!memManager->Read(address + Offsets::Hero::Gold, gold)) {
            gold = 0.0f;
        }
        
        // 读取战斗属性
        if (!memManager->Read(address + Offsets::Hero::AttackDamage, attackDamage)) {
            attackDamage = 0.0f;
        }
        if (!memManager->Read(address + Offsets::Hero::AbilityPower, abilityPower)) {
            abilityPower = 0.0f;
        }
        if (!memManager->Read(address + Offsets::Hero::Armor, armor)) {
            armor = 0.0f;
        }
        if (!memManager->Read(address + Offsets::Hero::MagicResist, magicResist)) {
            magicResist = 0.0f;
        }
        
        // 读取其他属性
        if (!memManager->Read(address + Offsets::Hero::CriticalChance, criticalChance)) {
            criticalChance = 0.0f;
        }
        if (!memManager->Read(address + Offsets::Hero::LifeSteal, lifeSteal)) {
            lifeSteal = 0.0f;
        }
        if (!memManager->Read(address + Offsets::Hero::SpellVamp, spellVamp)) {
            spellVamp = 0.0f;
        }
        if (!memManager->Read(address + Offsets::Hero::CooldownReduction, cooldownReduction)) {
            cooldownReduction = 0.0f;
        }
        
        // 根据名称确定英雄
        champion = GetChampionFromName(name);
        
        // 更新技能信息
        UpdateSpells();
    }
    
    // 获取英雄
    Champion GetChampion() const {
        return champion;
    }
    
    // 获取英雄名称
    std::string GetChampionName() const {
        return GetChampionName(champion);
    }
    
    // 获取等级
    int GetLevel() const {
        return level;
    }
    
    // 获取经验值
    float GetExperience() const {
        return experience;
    }
    
    // 获取金币
    float GetGold() const {
        return gold;
    }
    
    // 获取攻击力
    float GetAttackDamage() const {
        return attackDamage;
    }
    
    // 获取法强
    float GetAbilityPower() const {
        return abilityPower;
    }
    
    // 获取护甲
    float GetArmor() const {
        return armor;
    }
    
    // 获取魔抗
    float GetMagicResist() const {
        return magicResist;
    }
    
    // 获取暴击率
    float GetCriticalChance() const {
        return criticalChance;
    }
    
    // 获取生命偷取
    float GetLifeSteal() const {
        return lifeSteal;
    }
    
    // 获取法术吸血
    float GetSpellVamp() const {
        return spellVamp;
    }
    
    // 获取冷却缩减
    float GetCooldownReduction() const {
        return cooldownReduction;
    }
    
    // 获取技能信息
    const SpellInfo& GetSpellInfo(SpellSlot slot) const {
        int index = static_cast<int>(slot);
        if (index >= 0 && index < spells.size()) {
            return spells[index];
        }
        static SpellInfo empty;
        return empty;
    }
    
    // 获取技能信息
    SpellInfo& GetSpellInfo(SpellSlot slot) {
        int index = static_cast<int>(slot);
        if (index >= 0 && index < spells.size()) {
            return spells[index];
        }
        static SpellInfo empty;
        return empty;
    }
    
    // 技能是否就绪
    bool IsSpellReady(SpellSlot slot) const {
        const SpellInfo& spell = GetSpellInfo(slot);
        return spell.isReady && spell.isLearned && spell.currentCooldown <= 0.0f;
    }
    
    // 技能是否已学习
    bool IsSpellLearned(SpellSlot slot) const {
        const SpellInfo& spell = GetSpellInfo(slot);
        return spell.isLearned;
    }
    
    // 获取技能冷却
    float GetSpellCooldown(SpellSlot slot) const {
        const SpellInfo& spell = GetSpellInfo(slot);
        return spell.currentCooldown;
    }
    
    // 获取技能等级
    int GetSpellLevel(SpellSlot slot) const {
        const SpellInfo& spell = GetSpellInfo(slot);
        return spell.level;
    }
    
    // 获取技能范围
    float GetSpellRange(SpellSlot slot) const {
        const SpellInfo& spell = GetSpellInfo(slot);
        return spell.range;
    }
    
    // 获取技能宽度
    float GetSpellWidth(SpellSlot slot) const {
        const SpellInfo& spell = GetSpellInfo(slot);
        return spell.width;
    }
    
    // 获取技能速度
    float GetSpellSpeed(SpellSlot slot) const {
        const SpellInfo& spell = GetSpellInfo(slot);
        return spell.speed;
    }
    
    // 获取技能施法时间
    float GetSpellCastTime(SpellSlot slot) const {
        const SpellInfo& spell = GetSpellInfo(slot);
        return spell.castTime;
    }
    
    // 是否可以杀死目标
    bool CanKill(const Hero& target) const {
        return target.GetHealth() <= GetEstimatedDamage(target);
    }
    
    // 获取预估伤害
    float GetEstimatedDamage(const Hero& target, bool includeSummoners = true) const {
        float damage = 0.0f;
        
        // 基础攻击伤害
        damage += attackDamage;
        
        // 技能伤害
        for (int i = 0; i < 4; i++) { // Q, W, E, R
            SpellSlot slot = static_cast<SpellSlot>(i);
            if (IsSpellReady(slot)) {
                damage += GetSpellDamage(slot, target);
            }
        }
        
        // 召唤师技能伤害
        if (includeSummoners) {
            for (int i = 4; i < 6; i++) { // D, F
                SpellSlot slot = static_cast<SpellSlot>(i);
                if (IsSpellReady(slot)) {
                    damage += GetSpellDamage(slot, target);
                }
            }
        }
        
        return damage;
    }
    
    // 获取字符串表示
    std::string ToString() const {
        std::string result = "Hero[";
        result += GameObject::ToString();
        result += ", Champion: " + GetChampionName();
        result += ", Level: " + std::to_string(level);
        result += ", Gold: " + std::to_string(gold);
        result += ", AD: " + std::to_string(attackDamage);
        result += ", AP: " + std::to_string(abilityPower);
        result += "]";
        return result;
    }
    
private:
    // 更新技能信息
    void UpdateSpells() {
        if (address == 0) {
            return;
        }
        
        MemoryManager* memManager = MemoryManager::GetInstance();
        if (!memManager) {
            return;
        }
        
        // 读取技能信息
        for (int i = 0; i < 4; i++) { // Q, W, E, R
            uintptr_t spellInfoPtr;
            if (memManager->Read(address + Offsets::Hero::SpellInfoBase + i * 0x8, spellInfoPtr) && spellInfoPtr != 0) {
                if (!memManager->Read(spellInfoPtr + Offsets::Hero::SpellLevel, spells[i].level)) {
                    spells[i].level = 0;
                }
                if (!memManager->Read(spellInfoPtr + Offsets::Hero::SpellCooldown, spells[i].currentCooldown)) {
                    spells[i].currentCooldown = 0.0f;
                }
                spells[i].isReady = spells[i].currentCooldown <= 0.0f;
                spells[i].isLearned = spells[i].level > 0;
                
                // 根据英雄和技能设置技能参数
                SetSpellParameters(static_cast<SpellSlot>(i));
            }
        }
        
        // 读取召唤师技能信息
        for (int i = 0; i < 2; i++) { // D, F
            uintptr_t spellInfoPtr;
            if (memManager->Read(address + Offsets::Hero::SummonerSpellBase + i * 0x8, spellInfoPtr) && spellInfoPtr != 0) {
                if (!memManager->Read(spellInfoPtr + Offsets::Hero::SpellCooldown, spells[i + 4].currentCooldown)) {
                    spells[i + 4].currentCooldown = 0.0f;
                }
                spells[i + 4].isReady = spells[i + 4].currentCooldown <= 0.0f;
                spells[i + 4].isLearned = true;
                
                // 根据召唤师技能类型设置参数
                SetSummonerSpellParameters(static_cast<SpellSlot>(i + 4));
            }
        }
    }
    
    // 设置技能参数
    void SetSpellParameters(SpellSlot slot) {
        // 这里根据英雄和技能设置具体的技能参数
        // 在实际项目中，这些数据应该从配置文件或数据库中读取
        
        switch (champion) {
            case Champion::Ahri:
                switch (slot) {
                    case SpellSlot::Q: // Q - 欧米伽小队
                        spells[0].range = 880.0f;
                        spells[0].width = 90.0f;
                        spells[0].speed = 2500.0f;
                        spells[0].castTime = 0.25f;
                        break;
                    case SpellSlot::W: // W - 妖异狐火
                        spells[1].range = 800.0f;
                        spells[1].width = 0.0f;
                        spells[1].speed = 0.0f;
                        spells[1].castTime = 0.25f;
                        break;
                    case SpellSlot::E: // E - 魅惑妖术
                        spells[2].range = 975.0f;
                        spells[2].width = 70.0f;
                        spells[2].speed = 1500.0f;
                        spells[2].castTime = 0.25f;
                        break;
                    case SpellSlot::R: // R - 灵魄突袭
                        spells[3].range = 450.0f;
                        spells[3].width = 0.0f;
                        spells[3].speed = 2200.0f;
                        spells[3].castTime = 0.25f;
                        break;
                    default:
                        break;
                }
                break;
            // 其他英雄的技能参数...
            default:
                break;
        }
    }
    
    // 设置召唤师技能参数
    void SetSummonerSpellParameters(SpellSlot slot) {
        // 这里根据召唤师技能类型设置具体的技能参数
        // 在实际项目中，这些数据应该从配置文件或数据库中读取
        
        // 默认参数
        spells[static_cast<int>(slot)].range = 600.0f;
        spells[static_cast<int>(slot)].width = 0.0f;
        spells[static_cast<int>(slot)].speed = 0.0f;
        spells[static_cast<int>(slot)].castTime = 0.25f;
    }
    
    // 获取技能伤害
    float GetSpellDamage(SpellSlot slot, const Hero& target) const {
        // 这里根据英雄和技能计算伤害
        // 在实际项目中，这些数据应该从配置文件或数据库中读取
        
        const SpellInfo& spell = GetSpellInfo(slot);
        
        switch (champion) {
            case Champion::Ahri:
                switch (slot) {
                    case SpellSlot::Q: // Q - 欧米伽小队
                        return 40.0f + spell.level * 25.0f + abilityPower * 0.35f;
                    case SpellSlot::W: // W - 妖异狐火
                        return 40.0f + spell.level * 20.0f + abilityPower * 0.4f;
                    case SpellSlot::E: // E - 魅惑妖术
                        return 60.0f + spell.level * 45.0f + abilityPower * 0.6f;
                    case SpellSlot::R: // R - 灵魄突袭
                        return 70.0f + spell.level * 40.0f + abilityPower * 0.3f;
                    default:
                        return 0.0f;
                }
                break;
            // 其他英雄的技能伤害...
            default:
                return 0.0f;
        }
    }
    
    // 根据名称获取英雄
    static Champion GetChampionFromName(const std::string& name) {
        if (name == "Ahri") return Champion::Ahri;
        if (name == "Annie") return Champion::Annie;
        if (name == "Ashe") return Champion::Ashe;
        // 其他英雄...
        return Champion::Unknown;
    }
    
    // 获取英雄名称
    static std::string GetChampionName(Champion champ) {
        switch (champ) {
            case Champion::Ahri: return "Ahri";
            case Champion::Annie: return "Annie";
            case Champion::Ashe: return "Ashe";
            // 其他英雄...
            default: return "Unknown";
        }
    }
};