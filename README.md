# 🎵 Dynamic Playlist Optimizer

A personalized **music recommendation system** built using Spotify's dataset.
It combines:

* A **C++ Splay Tree** backend for dynamic reordering of songs.
* A **Song Registry** (hashmap) for fast metadata lookup.
* **Adaptive scoring** balancing feature similarity, user interactions, and popularity.
* Optional **Python ML microservice** for training similarity models and feature weights.

---

## 🚀 Features

* **Dynamic Splay Tree** → keeps most relevant songs near the root for fast recommendations.
* **Song Objects with Rich Metadata** → stores features like danceability, energy, liveness, valence, etc.
* **Adaptive Scoring** → balances three components:

  * Base score (feature similarity to user profile).
  * User score (clicks, skips, replays).
  * Popularity score (Spotify global popularity).
* **Log Tracking + Retraining** → user logs are tracked and retraining occurs in batches.
* **Modular ML Integration** → C++ pipeline connects with Python service for similarity calculation.

---

## 📂 Project Structure

```
project_root/
 ├─ include/            # All header files
 │   ├─ Song.hpp
 │   ├─ SongRegistry.hpp
 │   ├─ SongSplay.hpp
 │   ├─ UserProfile.hpp
 │   ├─ ml_similarity.hpp
 │   ├─ Features.hpp
 │   ├─ FeatureWeightLoader.hpp
 │   ├─ ClusterModel.hpp
 │   ├─ Logger.hpp
 │   ├─ Action.hpp
 │   ├─ ActionPolicy.hpp
 │   ├─ PlayerController.hpp
 │   └─ UtilCSV.hpp
 ├─ src/                
 │   └─ SongSplay.cpp   # main C++ implementation
 ├─ ml_service/         # Python ML pipeline
 │   ├─ train_clusters.py
 │   ├─ train_weights.py
 │   └─ requirements.txt
 ├─ data/
 │   └─ spotify_songs.csv
 ├─ logs/
 │   └─ interactions.csv
 └─ README.md
```

---

## ⚙️ Installation

### 1. Clone Repository

```bash
git clone https://github.com/your-username/dynamic-playlist-optimizer.git
cd dynamic-playlist-optimizer
```

### 2. Build C++ Backend

```bash
g++ -std=c++17 src/*.cpp -Iinclude -o spotify_recommender
```

### 3. Run the Demo

```bash
./spotify_recommender data/spotify_songs.csv
```

---

## ⚖️ Scoring System

Final recommendation scores are computed with **adaptive weighting**:

```
final_score = α * base_score + β * user_score + γ * popularity
```

### 🔹 Components

* **Base Score** → similarity between song features and user profile (via ML or cosine similarity).
* **User Score** → dynamic, based on skips (+/- reinforcement).
* **Popularity** → Spotify's popularity (normalized 0–1).

### 📈 Adaptive Weighting

```cpp
double interaction_factor = std::min(1.0, interactions / 100.0);

double alpha = 0.3 + 0.4 * interaction_factor;   // base similarity
double beta  = 0.2 + 0.6 * interaction_factor;   // user logs
double gamma = 1.0 - (alpha + beta);
gamma = std::max(0.1, gamma);                    // clamp minimum
```

* **Cold Start** (few logs): α=0.3, β=0.2, γ=0.5 → more popularity-driven.
* **Mature Profile** (many logs): α=0.7, β=0.8, γ=0.1 → more personalized.

---

### 📊 ASCII Weight Evolution

```
Interactions → 0%          50%           100%

Alpha (base)   0.3 ----------- 0.5 ------------- 0.7
Beta  (user)   0.2 ----------- 0.5 ------------- 0.8
Gamma (pop)    0.5 ----------- 0.2 ------------- 0.1
```

📌 More interactions → more personalization, less popularity bias.

---

## 💻 Example Usage

```cpp
#include "SongRegistry.hpp"
#include "SongSplay.hpp"
#include "UserProfile.hpp"

int main() {
    SongRegistry registry;
    registry.loadFromCSV("data/spotify_songs.csv");

    UserProfile profile;
    SongSplay tree(&profile);

    // Insert some songs
    auto song = registry.getSong("track_id_example");
    tree.insert(song);

    // Simulate user actions
    tree.promote(song, +1); // liked
    tree.promote(song, -3); // skipped

    // Get recommendations
    for (auto& s : tree.inorder()) {
        std::cout << s << std::endl;
    }
    return 0;
}
```

---

## 🐍 ML Integration

### Training ML Model

```bash
cd ml_service
pip install -r requirements.txt
python train_weights.py
python train_clusters.py
```

### Using in C++

* `ml_similarity.hpp` loads weights from generated files.
* Similarity between user profile and candidate songs is computed in C++.

---

## 📝 Logs & Retraining

* Every user action is logged to `logs/interactions.csv`.
* After a batch of ~20 interactions → trigger retraining (Python).
* Updated weights automatically influence future recommendations.

---

## ✅ Roadmap

* Add real-time Spotify API integration
* Implement collaborative filtering
* Support for multiple user profiles
* Web interface for playlist management
* A/B testing framework for recommendation algorithms

---

## 📜 License

MIT License — free to use and modify.
