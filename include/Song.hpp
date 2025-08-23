#pragma once
#include <string>
#include <array>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include "Features.hpp"

class UserProfile; // fwd
struct FeatureVector { // normalized averages for user
    std::array<double, 10> v{}; // order = kFeatureNames
};

class Song {
public:
    // IDs/metadata
    std::string track_id;
    std::string track_name;
    std::string track_artist;
    int track_popularity = 0; // 0..100
    std::string track_album_id;
    std::string track_album_name;
    std::string track_album_release_date;
    std::string playlist_name;
    std::string playlist_id;
    std::string playlist_genre;
    std::string playlist_subgenre;

    // raw features
    double danceability = 0;
    double energy = 0;
    int key = 0;
    double loudness = 0; // dB
    int mode = 0;
    double speechiness = 0;
    double acousticness = 0;
    double instrumentalness = 0;
    double liveness = 0;
    double valence = 0;
    double tempo = 0;
    int duration_ms = 0;

    // dynamic
    int user_score = 0; // accumulates +/- deltas from actions

    // helpers
    double feature_by_index(int i) const {
        switch (i) {
            case 0: return danceability;
            case 1: return energy;
            case 2: return valence;
            case 3: return instrumentalness;
            case 4: return liveness;
            case 5: return acousticness;
            case 6: return speechiness;
            case 7: return tempo;
            case 8: return loudness;
            case 9: return (double)duration_ms;
        }
        return 0.0;
    }

    double feature_norm_by_index(int i) const {
        const std::string name = kFeatureNames[i];
        return FeatureNorm::norm(name, feature_by_index(i));
    }

    // Blend weights: alpha=base sim, beta=user feedback, gamma=popularity
    static void blend_weights(int total_interactions, double& alpha, double& beta, double& gamma) {
        // Start more on base+pop, grow user feedback with interactions
        // total_interactions capped at 200 for schedule
        int t = total_interactions;
        if (t < 0) t = 0; if (t > 200) t = 200;
        beta = 0.15 + 0.50 * (t / 200.0);       // 0.15 -> 0.65
        alpha = 0.70 - 0.40 * (t / 200.0);      // 0.70 -> 0.30
        gamma = 1.0 - (alpha + beta);           // rest -> 0.05 at high t
        if (gamma < 0.05) gamma = 0.05;         // keep minimum pop influence
        double s = alpha + beta + gamma;        // renormalize
        alpha /= s; beta /= s; gamma /= s;
    }

    // final score: scaled to int for splay key
    int finalScore(const FeatureVector& user_avg,
                   int total_interactions,
                   double (*ml_similarity_fn)(const Song*, const FeatureVector&)) const {
        double base = ml_similarity_fn ? ml_similarity_fn(this, user_avg) : 0.0; // 0..1
        // user feedback -> squash to 0..1 via logistic-ish mapping
        double u = 1.0 / (1.0 + std::exp(-0.35 * (double)user_score));
        double pop = std::clamp(track_popularity / 100.0, 0.0, 1.0);

        double a,b,g; blend_weights(total_interactions, a,b,g);
        double score = a*base + b*u + g*pop; // 0..1
        int key = (int)std::llround(score * 1000000.0); // stable int key for splay
        return key;
    }
};