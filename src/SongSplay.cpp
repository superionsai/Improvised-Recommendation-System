// =============================================================
// File: src/SongSplay.cpp
// -------------------------------------------------------------
#include "SongSplay.hpp"

void SongSplay::insert(Song* s) {
    root = _insert(root, s);
    int sc = s->finalScore(profile->getAverage(), profile->total_interactions, ml_similarity);
    root = _splay(root, sc);
}

void SongSplay::promote(Song* s, int delta) {
    s->user_score += delta;
    if (delta >= 0) profile->update(s, delta); // only move centroid for positive signal
    int sc = s->finalScore(profile->getAverage(), profile->total_interactions, ml_similarity);
    root = _splay(root, sc);
}

std::vector<std::string> SongSplay::inorder() {
    std::vector<std::string> v; _inorder(root, v); return v;
}

Node* SongSplay::_insert(Node* node, Song* s) {
    if (!node) return new Node(s);
    int score = s->finalScore(profile->getAverage(), profile->total_interactions, ml_similarity);
    int rootScore = node->song->finalScore(profile->getAverage(), profile->total_interactions, ml_similarity);
    if (score < rootScore) node->left = _insert(node->left, s);
    else node->right = _insert(node->right, s);
    return node;
}

void SongSplay::_inorder(Node* node, std::vector<std::string>& v) {
    if (!node) return;
    _inorder(node->left, v);
    int score = node->song->finalScore(profile->getAverage(), profile->total_interactions, ml_similarity);
    v.push_back(node->song->track_name + " (score=" + std::to_string(score) + ")");
    _inorder(node->right, v);
}

Node* SongSplay::_rightRotate(Node* x) {
    Node* y = x->left; x->left = y->right; y->right = x; return y;
}
Node* SongSplay::_leftRotate(Node* x) {
    Node* y = x->right; x->right = y->left; y->left = x; return y;
}

Node* SongSplay::_splay(Node* r, int key) {
    if (!r) return r;
    int rScore = r->song->finalScore(profile->getAverage(), profile->total_interactions, ml_similarity);

    if (key < rScore) {
        if (!r->left) return r;
        int lScore = r->left->song->finalScore(profile->getAverage(), profile->total_interactions, ml_similarity);
        if (key < lScore) {
            r->left->left = _splay(r->left->left, key);
            r = _rightRotate(r);
        } else if (key > lScore) {
            r->left->right = _splay(r->left->right, key);
            if (r->left->right) r->left = _leftRotate(r->left);
        }
        return (!r->left) ? r : _rightRotate(r);
    } else if (key > rScore) {
        if (!r->right) return r;
        int rrScore = r->right->song->finalScore(profile->getAverage(), profile->total_interactions, ml_similarity);
        if (key > rrScore) {
            r->right->right = _splay(r->right->right, key);
            r = _leftRotate(r);
        } else if (key < rrScore) {
            r->right->left = _splay(r->right->left, key);
            if (r->right->left) r->right = _rightRotate(r->right);
        }
        return (!r->right) ? r : _leftRotate(r);
    }
    return r;
}