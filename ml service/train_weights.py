// =============================================================
// File: scripts/train_weights.py (Python)
// -------------------------------------------------------------
# NOTE: save this as scripts/train_weights.py
# Trains RF on joined logs + songs and exports feature_weights.json
# Run: python3 scripts/train_weights.py

import pandas as pd
import numpy as np
import json
from sklearn.ensemble import RandomForestClassifier
from pathlib import Path

LOGS_PATH = Path("logs/interactions.csv")
SONGS_PATH = Path("spotify_songs.csv")  # adjust if located elsewhere
OUT_PATH  = Path("data/feature_weights.json")
OUT_PATH.parent.mkdir(parents=True, exist_ok=True)

FEATURE_COLS = [
    "danceability", "energy", "valence",
    "instrumentalness", "liveness", "acousticness",
    "speechiness", "tempo", "loudness", "duration_ms",
]

POSITIVE = {"PLAY_COMPLETE", "REPLAY", "LIKE"}
NEGATIVE = {"SKIP_EARLY", "SKIP_LATE", "DISLIKE", "NOT_INTERESTED"}


def build_dataset():
    if not LOGS_PATH.exists():
        print("No logs yet; skipping train.")
        return None, None
    logs = pd.read_csv(LOGS_PATH)
    if logs.empty:
        print("Empty logs; skipping train.")
        return None, None
    songs = pd.read_csv(SONGS_PATH)
    data = logs.merge(songs, on="track_id", how="inner")
    if data.empty:
        print("Join produced 0 rows; check track_id consistency.")
        return None, None
    data["label"] = data["action"].apply(lambda a: 1 if a in POSITIVE else 0)
    X = data[FEATURE_COLS].fillna(0)
    y = data["label"].astype(int)
    if y.nunique() < 2:
        print("Not enough class diversity; need both positive/negative.")
        return None, None
    return X, y


def train_and_export():
    X, y = build_dataset()
    if X is None:
        return
    model = RandomForestClassifier(n_estimators=200, random_state=42)
    model.fit(X, y)
    w = model.feature_importances_
    w = (w / w.sum()).tolist()
    feature_weights = dict(zip(FEATURE_COLS, w))
    with open(OUT_PATH, "w") as f:
        json.dump(feature_weights, f, indent=2)
    print("Exported:", feature_weights)


if __name__ == "__main__":
    train_and_export()
