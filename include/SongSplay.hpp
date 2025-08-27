#pragma once
#include <vector>
#include <string>
#include "Song.hpp"
#include "UserProfile.hpp"
#include "ml_similarity.hpp"

struct Node {
    Song* song;
    Node* left = nullptr;
    Node* right = nullptr;
    explicit Node(Song* s) : song(s) {}
};

class SongSplay {
private:
    Node* root = nullptr;
    UserProfile* profile; // not owned

    Node* _insert(Node* node, Song* s);
    void _inorder(Node* node, std::vector<std::string>& v);
    Node* _rightRotate(Node* x);
    Node* _leftRotate(Node* x);
    Node* _splay(Node* r, int key);

public:
    explicit SongSplay(UserProfile* prof) : profile(prof) {}

    void insert(Song* s);
    void promote(Song* s, int delta);
    std::vector<std::string> inorder();

};
