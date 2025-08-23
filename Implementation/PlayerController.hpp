// =============================================================
// File: include/PlayerController.hpp
// -------------------------------------------------------------
#pragma once
#include <functional>
#include "SongRegistry.hpp"
#include "SongSplay.hpp"
#include "Logger.hpp"
#include "ActionPolicy.hpp"
#include "ml_similarity.hpp"

class PlayerController {
    SongRegistry& registry_;
    UserProfile profile_;
    SongSplay tree_;
    Logger logger_;
    int log_counter_ = 0;
    int retrain_every_ = 20; // retrain after N logs

public:
    explicit PlayerController(SongRegistry& reg)
        : registry_(reg), profile_(), tree_(&profile_), logger_() {
        ml_engine().init(); // load weights/clusters
    }

    void set_retrain_batch(int n) { retrain_every_ = n; }

    // Call when a user acts on a track
    void onAction(const std::string& user_id, const std::string& track_id, Action a,
                  int ms_listened=0) {
        Song* s = registry_.get(track_id);
        if (!s) return;

        // Apply delta policy
        int d = ActionPolicy::delta(a);
        if (a == Action::NOT_INTERESTED) {
            s->user_score = 0;      // reset personal affinity to this song
            profile_.soft_reset(0.1); // gently re-center profile
        } else {
            tree_.promote(s, d);
        }

        // Log
        Feedback fb;
        fb.user_id = user_id; fb.track_id = track_id; fb.action = a;
        fb.ms_listened = ms_listened; fb.ms_track = s->duration_ms;
        fb.ts_ms = Logger::now_ms();
        logger_.log(fb);

        // Trigger bulk retrain
        if (++log_counter_ >= retrain_every_) {
            retrain_weights();
            log_counter_ = 0;
        }

        // Hot-reload weights if changed
        ml_engine().hot_reload();
    }

    // Insert a library track into the splay once (e.g., during boot)
    void ingest_song(const std::string& track_id) {
        if (auto* s = registry_.get(track_id)) tree_.insert(s);
    }

    // naive demo: ingest first K songs
    void ingest_first_k(size_t K) {
        size_t c=0; for (const auto& fid : preload_ids_) { ingest_song(fid); if(++c>=K)break; }
    }

    // Provide a way to seed preload IDs (optional)
    std::vector<std::string> preload_ids_;

private:
    void retrain_weights() {
        // Call python script. Adjust paths to match your repo layout.
        // It will regenerate data/feature_weights.json
        std::system("python3 scripts/train_weights.py");
    }
};