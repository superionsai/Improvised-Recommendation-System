#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <nlhomann/json.hpp>

class ClusterModel {
    std::vector<std::array<double,10>> centroids_;
public:
    bool load(const std::string& path) {
        std::ifstream f(path); if (!f.is_open()) return false;
        nlohmann::json j; f>>j;
        centroids_.clear();
        if (!j.contains("centroids")) return false;
        for (auto& c : j["centroids"]) {
            std::array<double,10> a{};
            for (int i=0;i<10;++i) a[i] = (double)c[i];
            centroids_.push_back(a);
        }
        return true;
    }
    double affinity(const FeatureVector& fv) const {
        if (centroids_.empty()) return 0.0;
        auto dist = [&](const std::array<double,10>& a){
            double s=0; for(int i=0;i<10;++i){ double d=a[i]-fv.v[i]; s+=d*d; } return std::sqrt(s); };
        double best=1e9; for (auto& c: centroids_) best = std::min(best, dist(c));
        double bonus = std::max(0.0, 0.1 - best*0.1); 
        return bonus;
    }

};
