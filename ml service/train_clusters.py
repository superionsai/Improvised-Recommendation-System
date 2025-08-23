# Optional clustering export for centroids (used for a small bonus in ml_similarity)
# Run: python3 scripts/train_clusters.py

import pandas as pd
import json
from sklearn.preprocessing import StandardScaler
from sklearn.cluster import KMeans
from pathlib import Path

SONGS_PATH = Path("spotify_songs.csv")
OUT_PATH = Path("data/kmeans_centroids.json")
OUT_PATH.parent.mkdir(parents=True, exist_ok=True)

FEATURE_COLS = [
    "danceability", "energy", "valence",
    "instrumentalness", "liveness", "acousticness",
    "speechiness", "tempo", "loudness", "duration_ms",
]

K = 12

songs = pd.read_csv(SONGS_PATH)
X = songs[FEATURE_COLS].fillna(0).copy()
# normalize roughly as in C++ (tempo/loudness/duration ms)
X["tempo"]=X["tempo"].clip(0,250)/250.0
X["loudness"]=X["loudness"].clip(-60,0).add(60).div(60)
X["duration_ms"]=X["duration_ms"].clip(0,600000)/600000.0

km = KMeans(n_clusters=K, random_state=42, n_init=10)
km.fit(X)
centroids = km.cluster_centers_.tolist()
with open(OUT_PATH, "w") as f:
    json.dump({"centroids": centroids}, f, indent=2)
print("Saved centroids to", OUT_PATH)
