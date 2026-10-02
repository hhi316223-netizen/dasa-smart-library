#pragma once
#include <iostream>
#include <string>

// Node hỗ trợ Template cho mọi kiểu dữ liệu (kể cả Document)
template <typename T>
struct DNode {
    std::string key; // DocumentID (dùng kết nối O(1) với Hash Table)
    T data;
    DNode* prev;
    DNode* next;

    DNode(const std::string& k, const T& val) 
        : key(k), data(val), prev(nullptr), next(nullptr) {}
};

template <typename T>
class MyDoublyLinkedList {
private:
    DNode<T>* head;
    DNode<T>* tail;
    int size;

    // Ngăn chặn copy ngầm định để tránh lỗi giải phóng bộ nhớ 2 lần (Double Free)
    MyDoublyLinkedList(const MyDoublyLinkedList&) = delete;
    MyDoublyLinkedList& operator=(const MyDoublyLinkedList&) = delete;

public:
    MyDoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~MyDoublyLinkedList() {
        clear();
    }

    void clear() {
        DNode<T>* curr = head;
        while (curr != nullptr) {
            DNode<T>* nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
        head = tail = nullptr;
        size = 0;
    }

    // Thêm vào cuối danh sách - O(1)
    DNode<T>* pushBack(const std::string& key, const T& val) {
        DNode<T>* newNode = new DNode<T>(key, val);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
        return newNode;
    }

    // Xóa Node trực tiếp trong O(1)
    void removeNode(DNode<T>* node) {
        if (!node) return;

        if (node->prev != nullptr) {
            node->prev->next = node->next;
        } else {
            head = node->next;
        }

        if (node->next != nullptr) {
            node->next->prev = node->prev;
        } else {
            tail = node->prev;
        }

        delete node;
        size--;
    }

    DNode<T>* getHead() const { return head; }
    DNode<T>* getTail() const { return tail; }
    int getSize() const { return size; }
    bool isEmpty() const { return size == 0; }
};
