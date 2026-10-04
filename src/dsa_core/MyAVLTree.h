#ifndef MY_AVL_TREE_H
#define MY_AVL_TREE_H
#include <vector>
#include <algorithm>
using namespace std;
template <typename Key, typename Value>
class MyAVLTree {
private:
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;
        int height;
        Node(const Key& k, const Value& v)
            : key(k), value(v), left(nullptr), right(nullptr), height(0) {}
    };
    Node* root_;
    int count_;
    static int height_of(Node* t) {
        if (t != nullptr) {
            return (*t).height;
        }
        else {
            return -1;
        }
    }
    static void update_height(Node* t) {
        (*t).height = 1 + max(height_of((*t).left), height_of((*t).right));
    }
    static int balance_factor(Node* t) {
        if (t != nullptr) {
            return height_of((*t).left) - height_of((*t).right);
        }
        else {
            return 0;
        }
    }
    static Node* rotate_right(Node* y) {
        Node* x = (*y).left;
        Node* b = (*x).right;
        (*x).right = y;
        (*y).left = b;
        update_height(y);
        update_height(x);
        return x;
    }
    static Node* rotate_left(Node* x) {
        Node* y = (*x).right;
        Node* b = (*y).left;
        (*y).left = x;
        (*x).right = b;
        update_height(x);
        update_height(y);
        return y;
    }
    static Node* rebalance(Node* t) {
        update_height(t);
        int bf = balance_factor(t);

        if (bf > 1) {
            if (balance_factor((*t).left) >= 0) {
                return rotate_right(t);
            }
            (*t).left = rotate_left((*t).left);
            return rotate_right(t);
        }
        if (bf < -1) {
            if (balance_factor((*t).right) <= 0) {
                return rotate_left(t);
            }
            (*t).right = rotate_right((*t).right);
            return rotate_left(t);
        }
        return t;
    }
    static Node* insert(Node* t, const Key& k, const Value& v, bool& inserted) {
        if (t == nullptr) {
            inserted = true;
            return new Node(k, v);
        }
        if (k < (*t).key) (*t).left = insert((*t).left, k, v, inserted);
        else if ((*t).key < k) (*t).right = insert((*t).right, k, v, inserted);
        else return t;
        return rebalance(t);
    }
    static Node* find_min(Node* t) {
        while ((*t).left != nullptr) t = (*t).left;
        return t;
    }
    static Node* erase(Node* t, const Key& target, bool& removed) {
        if (t == nullptr) return nullptr;

        if (target < (*t).key) {
            (*t).left = erase((*t).left, target, removed);
        }
        else if ((*t).key < target) {
            (*t).right = erase((*t).right, target, removed);
        }
        else {
            removed = true;
            if ((*t).left == nullptr) { Node* r = (*t).right; delete t; return r; }
            if ((*t).right == nullptr) { Node* l = (*t).left;  delete t; return l; }
            Node* successor = find_min((*t).right);
            (*t).key = (*successor).key;
            t->value = (*successor).value;
            bool dummy = false;
            (*t).right = erase((*t).right, (*successor).key, dummy);
        }
        return rebalance(t);
    }
    static Node* search(Node* t, const Key& target) {
        while (t != nullptr) {
            if (target < (*t).key) {
                t = (*t).left;
            }
            else if ((*t).key < target) {
                t = (*t).right;
            }
            else return t;
        }
        return nullptr;
    }
    static void range_query(Node* t, const Key& low, const Key& high,
        vector<Value>& out, long long* visited) {
        if (t == nullptr) return;
        if (visited) ++(*visited);
        if (low < (*t).key) range_query((*t).left, low, high, out, visited);
        if (!((*t).key < low) && !(high < (*t).key)) out.push_back((*t).value);
        if ((*t).key < high) range_query((*t).right, low, high, out, visited);
    }
    static void inorder(Node* t, vector<Value>& out) {
        if (t == nullptr) return;
        inorder((*t).left, out);
        out.push_back((*t).value);
        inorder((*t).right, out);
    }
    static void destroy(Node* t) {
        if (t == nullptr) return;
        destroy((*t).left);
        destroy((*t).right);
        delete t;
    }
    static bool valid_bst(Node* t, const Key* lo, const Key* hi) {
        if (t == nullptr) return true;
        if (lo && !(*lo < (*t).key)) return false;
        if (hi && !((*t).key < *hi)) return false;
        return valid_bst((*t).left, lo, &(*t).key) && valid_bst((*t).right, &(*t).key, hi);
    }

    static bool valid_avl(Node* t) {
        if (t == nullptr) return true;
        int bf = balance_factor(t);
        if (bf < -1 || bf > 1) return false;
        if ((*t).height != 1 + max(height_of((*t).left), height_of((*t).right))) return false;
        return valid_avl((*t).left) && valid_avl((*t).right);
    }
public:
    MyAVLTree() : root_(nullptr), count_(0) {}
    ~MyAVLTree() { destroy(root_); }
    MyAVLTree(const MyAVLTree&) = delete;
    MyAVLTree& operator=(const MyAVLTree&) = delete;
    bool insert(const Key& k, const Value& v) {
        bool inserted = false;
        root_ = insert(root_, k, v, inserted);
        if (inserted) ++count_;
        return inserted;
    }
    bool remove(const Key& k) {
        bool removed = false;
        root_ = erase(root_, k, removed);
        if (removed) --count_;
        return removed;
    }
    const Value* find(const Key& k) const {
        Node* n = search(root_, k);
        if (n != nullptr) {
            return &(*n).value;
        } else {
            return nullptr;
        }
    }
    bool contains(const Key& k) const { return find(k) != nullptr; }
    vector<Value> range_query(const Key& low, const Key& high,
        long long* visited = nullptr) const {
        vector<Value> out;
        if (visited) *visited = 0;
        if (high < low) return out;
        range_query(root_, low, high, out, visited);
        return out;
    }
    vector<Value> inorder() const {
        vector<Value> out;
        inorder(root_, out);
        return out;
    }
    int size() const { return count_; }
    bool empty() const { return root_ == nullptr; }
    int height() const { return height_of(root_); }
    bool is_valid_bst() const { return valid_bst(root_, nullptr, nullptr); }
    bool is_balanced() const { return valid_avl(root_); }
};
#endif
