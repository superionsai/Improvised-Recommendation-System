#pragma once
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <nlohmann/json.hpp>

class FeatureWeightLoader {
    std::unordered_map<std::string,double> w_; // by feature name
    std::filesystem::file_time_type mtime_{};
public:
    bool load(const std::string& path) {
        std::ifstream f(path);
        if (!f.is_open()) return false;
        nlohmann::json j; f >> j;
        std::unordered_map<std::string,double> tmp;
        for (auto it = j.begin(); it != j.end(); ++it) tmp[it.key()] = it.value();
        if (tmp.empty()) return false;
        w_.swap(tmp);
        mtime_ = std::filesystem::last_write_time(path);
        return true;
    }
    bool reload_if_changed(const std::string& path) {
        if (!std::filesystem::exists(path)) return false;
        auto mt = std::filesystem::last_write_time(path);
        if (mt != mtime_) return load(path);
        return false;
    }
    const std::unordered_map<std::string,double>& weights() const { return w_; }
    bool empty() const { return w_.empty(); }

};
