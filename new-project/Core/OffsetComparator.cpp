#include "OffsetComparator.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <regex>
#include <ctime>

OffsetComparator::OffsetComparator() {
}

OffsetComparator::~OffsetComparator() {
}

std::vector<std::string> OffsetComparator::CompareFiles(const std::string& oldFile, const std::string& newFile) {
    std::vector<std::string> differences;
    
    // 加载旧偏移量
    auto oldOffsets = LoadOffsetsFromFile(oldFile);
    if (oldOffsets.empty()) {
        std::cout << "Failed to load old offsets from: " << oldFile << std::endl;
        return differences;
    }
    
    // 加载新偏移量
    auto newOffsets = LoadOffsetsFromFile(newFile);
    if (newOffsets.empty()) {
        std::cout << "Failed to load new offsets from: " << newFile << std::endl;
        return differences;
    }
    
    // 比较偏移量
    return CompareMaps(oldOffsets, newOffsets);
}

std::vector<std::string> OffsetComparator::CompareMaps(const std::map<std::string, OffsetInfo>& oldOffsets, 
                                                      const std::map<std::string, OffsetInfo>& newOffsets) {
    std::vector<std::string> differences;
    
    // 检查旧偏移量中的每个偏移量是否在新偏移量中存在或发生变化
    for (const auto& oldPair : oldOffsets) {
        const std::string& key = oldPair.first;
        const OffsetInfo& oldInfo = oldPair.second;
        
        auto it = newOffsets.find(key);
        if (it == newOffsets.end()) {
            // 偏移量在新版本中不存在
            differences.push_back("REMOVED: " + oldInfo.category + "." + oldInfo.name + 
                                  " (0x" + std::to_string(oldInfo.offset) + ")");
        } else if (it->second.offset != oldInfo.offset) {
            // 偏移量值发生变化
            differences.push_back(FormatDifference(oldInfo.category, oldInfo.name, 
                                                  oldInfo.offset, it->second.offset));
        }
    }
    
    // 检查新偏移量中的每个偏移量是否在旧偏移量中不存在（新增的偏移量）
    for (const auto& newPair : newOffsets) {
        const std::string& key = newPair.first;
        const OffsetInfo& newInfo = newPair.second;
        
        auto it = oldOffsets.find(key);
        if (it == oldOffsets.end()) {
            // 偏移量是新添加的
            differences.push_back("ADDED: " + newInfo.category + "." + newInfo.name + 
                                 " (0x" + std::to_string(newInfo.offset) + ")");
        }
    }
    
    return differences;
}

std::map<std::string, OffsetInfo> OffsetComparator::LoadOffsetsFromFile(const std::string& filePath) {
    std::map<std::string, OffsetInfo> offsets;
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filePath << std::endl;
        return offsets;
    }
    
    std::string line;
    std::string currentCategory = "";
    
    while (std::getline(file, line)) {
        // 跳过注释和空行
        if (line.empty() || line.find("//") == 0) {
            continue;
        }
        
        // 检查是否是命名空间行
        if (line.find("namespace") != std::string::npos && line.find("{") != std::string::npos) {
            size_t nsStart = line.find("namespace") + 9;
            size_t nsEnd = line.find("{", nsStart);
            currentCategory = line.substr(nsStart, nsEnd - nsStart);
            currentCategory.erase(0, currentCategory.find_first_not_of(" \t"));
            currentCategory.erase(currentCategory.find_last_not_of(" \t") + 1);
            continue;
        }
        
        // 解析偏移量行
        std::string category, name;
        uintptr_t offset;
        
        if (ParseOffsetLine(line, category, name, offset)) {
            // 如果没有从行中获取类别，使用当前类别
            if (category.empty() && !currentCategory.empty()) {
                category = currentCategory;
            }
            
            if (!category.empty() && !name.empty()) {
                OffsetInfo info;
                info.name = name;
                info.category = category;
                info.offset = offset;
                
                std::string key = GetOffsetKey(category, name);
                offsets[key] = info;
            }
        }
    }
    
    file.close();
    return offsets;
}

bool OffsetComparator::SaveOffsetsToFile(const std::map<std::string, OffsetInfo>& offsets, const std::string& filePath) {
    std::ofstream file(filePath);
    
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filePath << std::endl;
        return false;
    }
    
    // 获取当前时间
    time_t now = time(0);
    char* dt = ctime(&now);
    
    file << "// Offset Configuration File" << std::endl;
    file << "// Generated: " << dt;
    file << "// Game Version: " << std::endl;
    file << std::endl;
    
    // 按类别分组
    std::map<std::string, std::vector<OffsetInfo>> categorizedOffsets;
    for (const auto& pair : offsets) {
        categorizedOffsets[pair.second.category].push_back(pair.second);
    }
    
    // 写入各类别的偏移量
    for (const auto& category : categorizedOffsets) {
        file << "namespace " << category.first << " {" << std::endl;
        
        for (const auto& info : category.second) {
            file << "    constexpr uintptr_t " << info.name << " = 0x" 
                 << std::hex << info.offset << ";" << std::dec << std::endl;
        }
        
        file << "}" << std::endl;
        file << std::endl;
    }
    
    file.close();
    return true;
}

bool OffsetComparator::GenerateComparisonReport(const std::vector<std::string>& differences, const std::string& filePath) {
    std::ofstream file(filePath);
    
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filePath << std::endl;
        return false;
    }
    
    // 获取当前时间
    time_t now = time(0);
    char* dt = ctime(&now);
    
    file << "===== Offset Comparison Report =====" << std::endl;
    file << "Generated: " << dt;
    file << "Total Differences: " << differences.size() << std::endl;
    file << "====================================" << std::endl << std::endl;
    
    if (differences.empty()) {
        file << "No differences found. All offsets are the same." << std::endl;
    } else {
        // 分类差异
        std::vector<std::string> added;
        std::vector<std::string> removed;
        std::vector<std::string> changed;
        
        for (const auto& diff : differences) {
            if (diff.find("ADDED:") == 0) {
                added.push_back(diff);
            } else if (diff.find("REMOVED:") == 0) {
                removed.push_back(diff);
            } else if (diff.find("CHANGED:") == 0) {
                changed.push_back(diff);
            }
        }
        
        // 输出新增的偏移量
        if (!added.empty()) {
            file << "===== Added Offsets =====" << std::endl;
            for (const auto& offset : added) {
                file << offset << std::endl;
            }
            file << std::endl;
        }
        
        // 输出删除的偏移量
        if (!removed.empty()) {
            file << "===== Removed Offsets =====" << std::endl;
            for (const auto& offset : removed) {
                file << offset << std::endl;
            }
            file << std::endl;
        }
        
        // 输出变化的偏移量
        if (!changed.empty()) {
            file << "===== Changed Offsets =====" << std::endl;
            for (const auto& offset : changed) {
                file << offset << std::endl;
            }
            file << std::endl;
        }
    }
    
    file.close();
    std::cout << "Comparison report generated: " << filePath << std::endl;
    return true;
}

std::map<std::string, OffsetInfo> OffsetComparator::GetCurrentOffsets() {
    std::map<std::string, OffsetInfo> offsets;
    
    // 这里应该从Offsets.h加载当前偏移量
    // 为了简化，我们手动添加一些偏移量
    
    // Game偏移量
    offsets["Game.GameTime"] = {"GameTime", "Game", 0x350A0E0, "游戏时间"};
    offsets["Game.GameState"] = {"GameState", "Game", 0x350B4F8, "游戏状态"};
    offsets["Game.ObjectManager"] = {"ObjectManager", "Game", 0x28BCCC0, "对象管理器"};
    offsets["Game.LocalPlayer"] = {"LocalPlayer", "Game", 0x14, "本地玩家"};
    
    // GameObject偏移量
    offsets["GameObject.PositionX"] = {"PositionX", "GameObject", 0x1D4, "X坐标"};
    offsets["GameObject.PositionY"] = {"PositionY", "GameObject", 0x1D8, "Y坐标"};
    offsets["GameObject.PositionZ"] = {"PositionZ", "GameObject", 0x1DC, "Z坐标"};
    offsets["GameObject.Health"] = {"Health", "GameObject", 0x10C0, "生命值"};
    offsets["GameObject.MaxHealth"] = {"MaxHealth", "GameObject", 0x10C4, "最大生命值"};
    offsets["GameObject.Mana"] = {"Mana", "GameObject", 0x290, "法力值"};
    offsets["GameObject.MaxMana"] = {"MaxMana", "GameObject", 0x294, "最大法力值"};
    offsets["GameObject.IsVisible"] = {"IsVisible", "GameObject", 0x1C0, "可见性"};
    offsets["GameObject.IsDead"] = {"IsDead", "GameObject", 0x18, "存活状态"};
    offsets["GameObject.MovementSpeed"] = {"MovementSpeed", "GameObject", 0x12D8, "移动速度"};
    offsets["GameObject.NamePtr"] = {"NamePtr", "GameObject", 0x60, "名称指针"};
    
    // Hero偏移量
    offsets["Hero.Level"] = {"Level", "Hero", 0x34E0, "等级"};
    offsets["Hero.Experience"] = {"Experience", "Hero", 0x34E4, "经验值"};
    offsets["Hero.Gold"] = {"Gold", "Hero", 0x1B30, "金币"};
    offsets["Hero.AttackDamage"] = {"AttackDamage", "Hero", 0x12A0, "攻击力"};
    offsets["Hero.AbilityPower"] = {"AbilityPower", "Hero", 0x12A4, "法术强度"};
    offsets["Hero.Armor"] = {"Armor", "Hero", 0x12A8, "护甲"};
    offsets["Hero.MagicResist"] = {"MagicResist", "Hero", 0x12AC, "魔法抗性"};
    offsets["Hero.CriticalChance"] = {"CriticalChance", "Hero", 0x12B0, "暴击率"};
    offsets["Hero.LifeSteal"] = {"LifeSteal", "Hero", 0x12B4, "生命偷取"};
    offsets["Hero.SpellVamp"] = {"SpellVamp", "Hero", 0x12B8, "法术吸血"};
    offsets["Hero.CooldownReduction"] = {"CooldownReduction", "Hero", 0x12BC, "冷却缩减"};
    offsets["Hero.SpellInfoBase"] = {"SpellInfoBase", "Hero", 0x200, "技能信息基址"};
    offsets["Hero.SpellLevel"] = {"SpellLevel", "Hero", 0x1C, "技能等级"};
    offsets["Hero.SpellCooldown"] = {"SpellCooldown", "Hero", 0x20, "技能冷却时间"};
    offsets["Hero.SummonerSpellBase"] = {"SummonerSpellBase", "Hero", 0x220, "召唤师技能基址"};
    
    // Minion偏移量
    offsets["Minion.Type"] = {"Type", "Minion", 0x1014, "小兵类型"};
    offsets["Minion.AttackDamage"] = {"AttackDamage", "Minion", 0x40, "小兵攻击力"};
    
    // Turret偏移量
    offsets["Turret.Type"] = {"Type", "Turret", 0x1014, "防御塔类型"};
    offsets["Turret.AttackDamage"] = {"AttackDamage", "Turret", 0x40, "防御塔攻击力"};
    
    // GameObjectManager偏移量
    offsets["GameObjectManager.ObjectListPtr"] = {"ObjectListPtr", "GameObjectManager", 0x8, "对象列表指针"};
    offsets["GameObjectManager.ObjectCount"] = {"ObjectCount", "GameObjectManager", 0xC, "对象数量"};
    
    return offsets;
}

std::vector<std::string> OffsetComparator::DetectChanges(const std::string& oldOffsetsFile) {
    // 获取当前偏移量
    auto currentOffsets = GetCurrentOffsets();
    
    // 加载旧偏移量
    auto oldOffsets = LoadOffsetsFromFile(oldOffsetsFile);
    if (oldOffsets.empty()) {
        std::cout << "Failed to load old offsets from: " << oldOffsetsFile << std::endl;
        return {};
    }
    
    // 比较偏移量
    return CompareMaps(oldOffsets, currentOffsets);
}

bool OffsetComparator::ParseOffsetLine(const std::string& line, std::string& category, std::string& name, uintptr_t& offset) {
    // 使用正则表达式匹配偏移量行
    std::regex offsetRegex(R"(constexpr\s+uintptr_t\s+(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;)");
    std::smatch match;
    
    if (std::regex_search(line, match, offsetRegex)) {
        name = match[1].str();
        std::stringstream ss;
        ss << std::hex << match[2].str();
        ss >> offset;
        return true;
    }
    
    return false;
}

std::string OffsetComparator::FormatDifference(const std::string& category, const std::string& name, 
                                             uintptr_t oldOffset, uintptr_t newOffset) {
    std::stringstream ss;
    ss << "CHANGED: " << category << "." << name 
       << " (0x" << std::hex << oldOffset << " -> 0x" << newOffset << ")";
    return ss.str();
}

std::string OffsetComparator::GetOffsetKey(const std::string& category, const std::string& name) {
    return category + "." + name;
}