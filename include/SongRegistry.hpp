#pragma once
#include <unordered_map>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Song.hpp"
#include "UtilCSV.hpp"

class SongRegistry {
    std::unordered_map<std::string, std::unique_ptr<Song>> by_id_;
public:
    Song* get(const std::string& id) const {
        auto it = by_id_.find(id); return it==by_id_.end()? nullptr : it->second.get();
    }

    void addSong(std::unique_ptr<Song> s) {
        if (!s) return; by_id_[s->track_id] = std::move(s);
    }

    size_t size() const { return by_id_.size(); }

    // Load from CSV with the columns provided in your dataset
    size_t loadFromCSV(const std::string& path) {
        std::ifstream f(path); if (!f.is_open()) { std::cerr << "Cannot open "<<path<<"\n"; return 0; }
        std::string header; if (!std::getline(f, header)) return 0;
        auto H = csv::split_row(header);
        auto idx = [&](const std::string& name){
            for (size_t i=0;i<H.size();++i) if (H[i]==name) return (int)i; return -1; };
        int i_track_id = idx("track_id");
        int i_track_name = idx("track_name");
        int i_track_artist = idx("track_artist");
        int i_pop = idx("track_popularity");
        int i_album_id = idx("track_album_id");
        int i_album_name = idx("track_album_name");
        int i_album_date = idx("track_album_release_date");
        int i_playlist_name = idx("playlist_name");
        int i_playlist_id = idx("playlist_id");
        int i_playlist_genre = idx("playlist_genre");
        int i_playlist_subgenre = idx("playlist_subgenre");
        int i_dance = idx("danceability");
        int i_energy = idx("energy");
        int i_key = idx("key");
        int i_loud = idx("loudness");
        int i_mode = idx("mode");
        int i_speech = idx("speechiness");
        int i_acoust = idx("acousticness");
        int i_instr = idx("instrumentalness");
        int i_live = idx("liveness");
        int i_val = idx("valence");
        int i_tempo = idx("tempo");
        int i_dur = idx("duration_ms");
        std::string line; size_t n=0;
        while (std::getline(f, line)) {
            if (line.empty()) continue;
            auto cols = csv::split_row(line);
            auto s = std::make_unique<Song>();
            auto at = [&](int i)->std::string{ return (i>=0 && i<(int)cols.size()? cols[i] : std::string()); };
            s->track_id = at(i_track_id);
            if (s->track_id.empty()) continue;
            s->track_name = at(i_track_name);
            s->track_artist = at(i_track_artist);
            s->track_popularity = i_pop>=0 ? std::stoi(at(i_pop).empty()?"0":at(i_pop)) : 0;
            s->track_album_id = at(i_album_id);
            s->track_album_name = at(i_album_name);
            s->track_album_release_date = at(i_album_date);
            s->playlist_name = at(i_playlist_name);
            s->playlist_id = at(i_playlist_id);
            s->playlist_genre = at(i_playlist_genre);
            s->playlist_subgenre = at(i_playlist_subgenre);
            s->danceability = i_dance>=0 ? std::stod(at(i_dance).empty()?"0":at(i_dance)) : 0.0;
            s->energy = i_energy>=0 ? std::stod(at(i_energy).empty()?"0":at(i_energy)) : 0.0;
            s->key = i_key>=0 ? std::stoi(at(i_key).empty()?"0":at(i_key)) : 0;
            s->loudness = i_loud>=0 ? std::stod(at(i_loud).empty()?"0":at(i_loud)) : 0.0;
            s->mode = i_mode>=0 ? std::stoi(at(i_mode).empty()?"0":at(i_mode)) : 0;
            s->speechiness = i_speech>=0 ? std::stod(at(i_speech).empty()?"0":at(i_speech)) : 0.0;
            s->acousticness = i_acoust>=0 ? std::stod(at(i_acoust).empty()?"0":at(i_acoust)) : 0.0;
            s->instrumentalness = i_instr>=0 ? std::stod(at(i_instr).empty()?"0":at(i_instr)) : 0.0;
            s->liveness = i_live>=0 ? std::stod(at(i_live).empty()?"0":at(i_live)) : 0.0;
            s->valence = i_val>=0 ? std::stod(at(i_val).empty()?"0":at(i_val)) : 0.0;
            s->tempo = i_tempo>=0 ? std::stod(at(i_tempo).empty()?"0":at(i_tempo)) : 0.0;
            s->duration_ms = i_dur>=0 ? std::stoi(at(i_dur).empty()?"0":at(i_dur)) : 0;
            addSong(std::move(s)); ++n;
        }
        return n;
    }
};