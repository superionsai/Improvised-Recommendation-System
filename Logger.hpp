
// =============================================================
// File: include/Logger.hpp
// -------------------------------------------------------------
#pragma once
#include <fstream>
#include <string>
#include <chrono>
#include "Action.hpp"

struct Feedback {
    std::string user_id;
    std::string track_id;
    Action action;
    int ms_listened = 0;
    int ms_track = 0;
    long long ts_ms = 0; // epoch ms
};

class Logger {
    std::ofstream file_;
public:
    explicit Logger(const std::string& path = "logs/interactions.csv") {
        file_.open(path, std::ios::app);
        if (file_.tellp() == 0) {
            file_ << "user_id,track_id,action,ms_listened,ms_track,timestamp\n";
        }
    }
    void log(const Feedback& fb) {
        file_ << fb.user_id << ','
              << fb.track_id << ','
              << to_cstr(fb.action) << ','
              << fb.ms_listened << ','
              << fb.ms_track << ','
              << fb.ts_ms << "\n";
        file_.flush();
    }
    static long long now_ms() {
        using namespace std::chrono;
        return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }
};