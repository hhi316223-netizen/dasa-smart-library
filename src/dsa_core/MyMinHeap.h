#pragma once
#include <vector>
#include <stdexcept>
#include "../models/Document.h"

class MyMinHeap {
private:
    std::vector<Document> heap;

    // So sanh do uu tien: 
    // 1. priorityLevel nho hon thi uu tien cao hon (1 khan cap hon 5)
    // 2. Neu trung priorityLevel -> ai dang ky truoc (registrationTime nho hon) duoc uu tien truoc (FIFO)
    bool hasHigherPriority(const Document& a, const Document& b) const {
        if (a.priorityLevel != b.priorityLevel) {
            return a.priorityLevel < b.priorityLevel;
        }
        return a.registrationTime < b.registrationTime;
    }

    // Vun dong len (Heapify Up) khi them phan tu moi vao cuoi dong
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (hasHigherPriority(heap[index], heap[parent])) {
                std::swap(heap[index], heap[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    // Vun dong xuong (Heapify Down) khi rut goc ra khoi dong
    void heapifyDown(int index) {
        int size = static_cast<int>(heap.size());
        while (index < size) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size && hasHigherPriority(heap[left], heap[smallest])) {
                smallest = left;
            }
            if (right < size && hasHigherPriority(heap[right], heap[smallest])) {
                smallest = right;
            }

            if (smallest != index) {
                std::swap(heap[index], heap[smallest]);
                index = smallest;
            } else {
                break;
            }
        }
    }

public:
    MyMinHeap() = default;

    // Them tai lieu vao hang doi uu tien: O(log N)
    void push(const Document& doc) {
        heap.push_back(doc);
        heapifyUp(static_cast<int>(heap.size()) - 1);
    }

    // Lay va xoa tai lieu co do uu tien cao nhat o nut goc: O(log N)
    Document extractMin() {
        if (isEmpty()) {
            throw std::runtime_error("Hang doi uu tien rong!");
        }
        Document root = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
        return root;
    }

    // Xem tai lieu o nut goc ma khong xoa: O(1)
    Document peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Hang doi uu tien rong!");
        }
        return heap[0];
    }

    bool isEmpty() const {
        return heap.empty();
    }

    size_t size() const {
        return heap.size();
    }
};