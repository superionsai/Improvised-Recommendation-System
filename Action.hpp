// =============================================================
// File: include/Action.hpp
// -------------------------------------------------------------
#pragma once
#include <string>

enum class Action {
    PLAY_START,
    PLAY_COMPLETE,
    REPLAY,
    SKIP_EARLY,
    SKIP_LATE,
    LIKE,
    DISLIKE,
    NOT_INTERESTED
};

inline const char* to_cstr(Action a) {
    switch (a) {
        case Action::PLAY_START:     return "PLAY_START";
        case Action::PLAY_COMPLETE:  return "PLAY_COMPLETE";
        case Action::REPLAY:         return "REPLAY";
        case Action::SKIP_EARLY:     return "SKIP_EARLY";
        case Action::SKIP_LATE:      return "SKIP_LATE";
        case Action::LIKE:           return "LIKE";
        case Action::DISLIKE:        return "DISLIKE";
        case Action::NOT_INTERESTED: return "NOT_INTERESTED";
    }
    return "UNKNOWN";
}
