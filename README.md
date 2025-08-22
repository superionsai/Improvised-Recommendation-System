# 🎵 Dynamic Playlist Optimizer

A personalized **music recommendation system** built using Spotify's dataset. This project combines advanced data structures with machine learning to create an adaptive music recommendation engine.

## 🌟 Overview

The Dynamic Playlist Optimizer features:
- **C++ Splay Tree** backend for dynamic reordering of songs
- **Song Registry** (hashmap) for fast metadata lookup  
- **Adaptive scoring** balancing feature similarity, user interactions, and popularity
- Optional **Python ML microservice** for training similarity models and feature weights

## 🚀 Key Features

- **Dynamic Splay Tree** → Keeps most relevant songs near the root for fast recommendations
- **Rich Song Metadata** → Stores features like danceability, energy, liveness, valence, etc.
- **Adaptive Scoring System** → Balances three components:
  - Base score (feature similarity to user profile)
  - User score (clicks, skips, replays)
  - Popularity score (Spotify global popularity)
- **Log Tracking + Retraining** → User logs are tracked and retraining occurs in batches
- **Modular ML Integration** → C++ pipeline connects with Python service for similarity calculation

## 📂 Project Structure

```
project_root/
├─ include/            # Header files
│   ├─ Song.hpp
│   ├─ UserProfile.hpp
│   ├─ SongRegistry.hpp
│   ├─ SongSplay.hpp
│   └─ ml_similarity.hpp
├─ src/                # C++ implementations
│   ├─ Song.cpp
│   ├─ UserProfile.cpp
│   ├─ SongRegistry.cpp
│   ├─ SongSplay.cpp
│   └─ main.cpp
├─ ml_service/         # Python ML pipeline
│   ├─ train.py
│   ├─ similarity.py
│   └─ requirements.txt
├─ data/
│   └─ spotify_songs.csv
├─ logs/
│   └─ interactions.csv
└─ README.md
```

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

### 3. Install Python Dependencies (Optional)

```bash
cd ml_service
pip install -r requirements.txt
```

### 4. Run the Demo

```bash
./spotify_recommender data/spotify_songs.csv
```

## ⚖️ Scoring System

Final recommendation scores are computed with **adaptive weighting**:

```cpp
final_score = α * base_score + β * user_score + γ * popularity
```

### 🔹 Score Components

- **Base Score** → Similarity between song features and user profile (via ML or cosine similarity)
- **User Score** → Dynamic score based on skips, likes, and replays (+/- reinforcement)
- **Popularity** → Spotify's popularity metric (normalized 0–1)

### 📈 Adaptive Weighting Algorithm

```cpp
double interaction_factor = std::min(1.0, interactions / 100.0);

double alpha = 0.3 + 0.4 * interaction_factor;   // base similarity weight
double beta  = 0.2 + 0.6 * interaction_factor;   // user interaction weight  
double gamma = 1.0 - (alpha + beta);             // popularity weight
gamma = std::max(0.1, gamma);                    // clamp minimum
```

### 📊 Weight Evolution

```
Interactions → 0%          50%           100%

Alpha (base)   0.3 --------- 0.5 --------- 0.7
Beta  (user)   0.2 --------- 0.5 --------- 0.8  
Gamma (pop)    0.5 --------- 0.2 --------- 0.1
```

**Key Insight:** More interactions → more personalization, less popularity bias

- **Cold Start** (few logs): α=0.3, β=0.2, γ=0.5 → popularity-driven recommendations
- **Mature Profile** (many logs): α=0.7, β=0.8, γ=0.1 → highly personalized recommendations

## 💻 Usage Example

```cpp
#include "SongRegistry.hpp"
#include "SongSplay.hpp"
#include "UserProfile.hpp"

int main() {
    // Initialize components
    SongRegistry registry;
    registry.loadFromCSV("data/spotify_songs.csv");
    
    UserProfile profile;
    SongSplay tree(&profile);
    
    // Insert songs into the recommendation tree
    auto song = registry.getSong("track_id_example");
    tree.insert(song);
    
    // Simulate user interactions
    tree.promote(song, +1); // User liked the song
    tree.promote(song, -3); // User skipped the song
    
    // Get personalized recommendations
    std::cout << "🎵 Your Recommendations:" << std::endl;
    for (auto& s : tree.inorder()) {
        std::cout << s << std::endl;
    }
    
    return 0;
}
```

## 🐍 ML Integration

### Training the ML Model

```bash
cd ml_service
python train.py
```

### Using ML in C++

- `ml_similarity.hpp` automatically loads trained weights from `feature_weights.npy`
- Similarity computation between user profile and candidate songs happens in C++
- Real-time feature-based recommendations without Python runtime dependency

## 📝 Logging & Retraining

- **Automatic Logging:** Every user action is logged to `logs/interactions.csv`
- **Batch Retraining:** After ~20 interactions → triggers automatic retraining (Python)
- **Seamless Updates:** Updated weights automatically influence future recommendations
- **No Downtime:** Model updates happen in background without service interruption

## 🛣️ Roadmap

- [ ] **Real-time Streaming** → Kafka integration for live user events
- [ ] **Advanced ML Models** → Neural collaborative filtering, transformer-based embeddings  
- [ ] **Multi-user Support** → User session management and concurrent recommendations
- [ ] **A/B Testing Framework** → Compare recommendation algorithms
- [ ] **REST API** → Web service endpoints for integration
- [ ] **Playlist Generation** → Create themed playlists based on mood/activity
- [ ] **Cross-platform Support** → iOS/Android mobile apps

## 🤝 Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Spotify** for providing the music dataset
- **Open source community** for C++ and Python libraries
- **Contributors** who help improve this project

---

⭐ **Star this repo** if you find it helpful!

📧 **Questions?** Open an issue or reach out via email.
