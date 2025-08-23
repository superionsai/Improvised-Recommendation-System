
#pragma once
#include <array>
#include <algorithm>
#include "Features.hpp"
#include "Song.hpp"

class UserProfile {
    // Exponentially weighted moving average per feature (normalized)
    std::array<double, 10> avg_{}; // init 0
    bool seeded_ = false;
public:
    int total_interactions = 0; // count of actions considered

    // Update profile using a song and a signed delta (policy decides values)
    void update(const Song* s, int delta) {
        // We only move the profile towards positive feedback; negative deltas don't
        // pull the centroid away (they are handled via user_score directly).
        int pos = std::max(0, delta);
        double alpha = 0.10 * std::min(1, pos); // 0.10 if pos>0 else 0
        if (!seeded_) {
            for (int i=0;i<10;++i) avg_[i] = s->feature_norm_by_index(i);
            seeded_ = true;
        } else if (alpha > 0) {
            for (int i=0;i<10;++i) {
                double x = s->feature_norm_by_index(i);
                avg_[i] = (1.0 - alpha) * avg_[i] + alpha * x;
            }
        }
        total_interactions += 1; // count all actions for blend schedule
    }

    FeatureVector getAverage() const {
        FeatureVector fv; fv.v = avg_; return fv;
    }

    // Optional reset if user marks not interested on many songs etc.
    void soft_reset(double factor=0.5) {
        for (auto& x : avg_) x = (1.0 - factor)*x + factor*0.5; // pull towards neutral 0.5
    }

};
