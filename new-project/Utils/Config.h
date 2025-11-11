#pragma once

#include <string>
#include <map>
#include <memory>
#include <mutex>

namespace KBot {

class Config {
public:
    // 单例模式
    static Config& GetInstance();
    
    // 初始化和清理
    bool Initialize();
    void Shutdown();
    
    // 配置管理
    bool Load(const std::string& configPath = "config.json");
    bool Save(const std::string& configPath = "config.json");
    
    // 值操作
    void SetValue(const std::string& key, const std::string& value);
    const char* GetValue(const std::string& key) const;
    const char* GetValue(const std::string& key, const std::string& defaultValue) const;
    
    // 类型安全的获取方法
    int GetInt(const std::string& key, int defaultValue = 0) const;
    float GetFloat(const std::string& key, float defaultValue = 0.0f) const;
    bool GetBool(const std::string& key, bool defaultValue = false) const;
    
    // 类型安全的设置方法
    void SetInt(const std::string& key, int value);
    void SetFloat(const std::string& key, float value);
    void SetBool(const std::string& key, bool value);
    
    // 检查配置是否存在
    bool HasValue(const std::string& key) const;
    
    // 删除配置项
    void RemoveValue(const std::string& key);
    
    // 获取所有配置项
    std::map<std::string, std::string> GetAllValues() const;
    
    // 重置为默认配置
    void ResetToDefaults();
    
    // 获取配置路径
    const std::string& GetConfigPath() const;
    
    // 检查配置是否已加载
    bool IsLoaded() const;

private:
    Config() = default;
    ~Config() = default;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    
    // 默认配置
    void InitializeDefaults();
    
    // 成员变量
    std::map<std::string, std::string> m_values;
    std::string m_configPath = "config.json";
    mutable std::mutex m_mutex;
    bool m_loaded = false;
    bool m_initialized = false;
};

} // namespace KBot