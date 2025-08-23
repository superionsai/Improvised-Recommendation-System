
// =============================================================
// File: include/ActionPolicy.hpp
// -------------------------------------------------------------
#pragma once
#include "Action.hpp"

struct ActionPolicy {
    // Map actions to user_score deltas
    static int delta(Action a) {
        switch (a) {
            case Action::LIKE:          return +3;
            case Action::REPLAY:        return +1;
            case Action::PLAY_COMPLETE: return +1;         // mild positive
            case Action::SKIP_EARLY:    return -3;         // strong negative
            case Action::SKIP_LATE:     return -1;         // mild negative
            case Action::DISLIKE:       return -4;         // very strong negative
            case Action::NOT_INTERESTED:return -1000;      // special: handled as reset
            case Action::PLAY_START:    return 0;
        }
        return 0;
    }
};
