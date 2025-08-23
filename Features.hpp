#pragma once
#include <array>
#include <string>
#include <unordered_map>

// 10 numeric features we use for similarity/weights
// Keep the order stable everywhere
static const std::array<const char*, 10> kFeatureNames = {
    "danceability", "energy", "valence",
    "instrumentalness", "liveness", "acousticness",
    "speechiness", "tempo", "loudness", "duration_ms"
};

inline int feature_index(const std::string& name) {
    for (size_t i = 0; i < kFeatureNames.size(); ++i) {
        if (name == kFeatureNames[i]) return (int)i;
    }
    return -1;
}


// Simple static ranges for normalization
// (tempo ~ [0,250], loudness ~ [-60,0], duration up to ~10min)
struct FeatureNorm {
    static double norm(const std::string& name, double value) {
        if (name == "tempo") {
            if (value < 0) value = 0; if (value > 250) value = 250;
            return value / 250.0;
        } else if (name == "loudness") {
            if (value < -60) value = -60; if (value > 0) value = 0;
            return (value + 60.0) / 60.0; // -60..0 -> 0..1
        } else if (name == "duration_ms") {
            if (value < 0) value = 0; if (value > 600000) value = 600000;
            return value / 600000.0;
        }
        // others already 0..1
        if (value < 0) value = 0; if (value > 1) value = 1;
        return value;
    }
};