#pragma once
#include <unordered_map>
#include "Song.hpp"
#include "FeatureWeightLoader.hpp"
#include "ClusterModel.hpp"


namespace MLSim {
// default fallback weights (sum to 1)
static std::unordered_map<std::string,double> kDefault = {
{"danceability",0.20},{"energy",0.20},{"valence",0.15},{"instrumentalness",0.10},{"liveness",0.10},
{"acousticness",0.05},{"speechiness",0.05},{"tempo",0.05},{"loudness",0.05},{"duration_ms",0.05}
};


class Engine {
FeatureWeightLoader loader_;
ClusterModel cluster_;
std::string weights_path_="data/feature_weights.json";
std::string clusters_path_="data/kmeans_centroids.json";
public:
void set_paths(const std::string& w, const std::string& c) { weights_path_=w; clusters_path_=c; }
void init() {
loader_.load(weights_path_); // ignore failure -> use default
cluster_.load(clusters_path_); // optional
}
void hot_reload() { loader_.reload_if_changed(weights_path_); }


// main similarity in [0,1]
double similarity(const Song* s, const FeatureVector& user_avg) const {
const auto& W = loader_.empty()? kDefault : loader_.weights();
double acc=0.0, wsum=0.0;
for (auto& kv : W) {
int idx = feature_index(kv.first);
if (idx<0) continue; double w = kv.second; wsum += w;
double sv = s->feature_norm_by_index(idx);
double uv = user_avg.v[idx];
double sim = 1.0 - std::abs(sv - uv); // per-feature similarity
if (sim < 0) sim = 0; // guard
acc += w * sim;
}
double base = (wsum>0? acc/wsum : acc);
// optional tiny cluster bonus
base = std::clamp(base + cluster_.affinity(user_avg), 0.0, 1.0);
return base;
}
};
}


// Global instance getter (for simple integration)
inline MLSim::Engine& ml_engine() { static MLSim::Engine e; return e; }


inline double ml_similarity(const Song* s, const FeatureVector& user_avg) {
return ml_engine().similarity(s, user_avg);
}