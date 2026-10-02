#ifndef MY_DOUBLY_LINKED_LIST_H
#define MY_DOUBLY_LINKED_LIST_H

#include <iostream>
#include <string>

// Cấu trúc Node tự định nghĩa
struct Node {
    std::string documentID;
    std::string data; // Chứa thông tin tài liệu hoặc người dùng
    Node* prev;
    Node* next;

    Node(std::string id, std::string d) : documentID(id), data(d), prev(nullptr), next(nullptr) {}
};

class MyDoublyLinkedList {
private:
    Node* head;
    Node* tail;

public:
    MyDoublyLinkedList() : head(nullptr), tail(nullptr) {}

    ~MyDoublyLinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    // Thêm vào cuối danh sách (duy trì thứ tự Waitlist)
    Node* append(std::string id, std::string data) {
        Node* newNode = new Node(id, data);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        return newNode;
    }

    // Xóa một Node bất kỳ trong O(1) (Node này được lấy ra từ Hash Table)
    void removeNode(Node* node) {
        if (!node) return;
        
        if (node->prev) {
            node->prev->next = node->next;
        } else {
            head = node->next; // Nếu là node đầu
        }

        if (node->next) {
            node->next->prev = node->prev;
        } else {
            tail = node->prev; // Nếu là node cuối
        }
        delete node;
    }
};

#endif
