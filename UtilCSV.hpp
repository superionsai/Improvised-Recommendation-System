
// =============================================================
// File: include/UtilCSV.hpp
// -------------------------------------------------------------
#pragma once
#include <string>
#include <vector>

// Minimal CSV parser that handles quoted fields and commas inside quotes
namespace csv {
    inline std::vector<std::string> split_row(const std::string& line) {
        std::vector<std::string> out; out.reserve(32);
        std::string cur; cur.reserve(line.size());
        bool inq=false; for (size_t i=0;i<line.size();++i){
            char c=line[i];
            if (c=='"') {
                if (inq && i+1<line.size() && line[i+1]=='"') { cur.push_back('"'); ++i; }
                else inq = !inq;
            } else if (c==',' && !inq) {
                out.push_back(cur); cur.clear();
            } else {
                cur.push_back(c);
            }
        }
        out.push_back(cur);
        return out;
    }
}