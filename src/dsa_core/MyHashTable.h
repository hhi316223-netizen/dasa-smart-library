#pragma once
#include "../models/Document.h"
#include <string>
#include <iostream>

// ============================================================================
// ĐẶC TẢ THÀNH PHẦN: MY_HASH_TABLE (THÀNH VIÊN 1 CHỊU TRÁCH NHIỆM)
// - Mục đích: Giải quyết yêu cầu MC1 (Tra cứu tức thời theo DocumentID)
// - Thuật toán băm: Polynomial Rolling Hash (Hệ số nguyên tố p = 31)
// - Cơ chế giải quyết đụng độ: Chaining (Danh sách liên kết đơn tại mỗi bucket)
// - Cơ chế tự cân bằng hiệu năng: Tự động Rehash (nhân đôi) khi Load Factor > 0.75
// ============================================================================

// Node danh sách liên kết dùng cho cơ chế Chaining
struct HashNode {
    std::string key;        // DocumentID (Khóa định danh)
    Document value;         // Dữ liệu bản ghi tài liệu
    HashNode* next;         // Con trỏ trỏ tới node tiếp theo khi xảy ra đụng độ

    HashNode(const std::string& k, const Document& v)
        : key(k), value(v), next(nullptr) {}
};

class MyHashTable {
private:
    HashNode** table;       // Mảng các con trỏ quản lý từng bucket
    size_t capacity;        // Kích thước mảng bucket hiện tại
    size_t count;           // Số lượng phần tử thực tế đang lưu trữ
    const double MAX_LOAD_FACTOR = 0.75; // Ngưỡng tới hạn để kích hoạt Rehash

    // 1. HÀM BĂM ĐA THỨC (Polynomial Rolling Hash)
    // Biến đổi chuỗi DocumentID thành chỉ số index phân bố đều trên mảng
    size_t hashFunction(const std::string& key) const {
        size_t hashVal = 0;
        const size_t p = 31; // Hệ số nhân số nguyên tố giúp giảm thiểu đụng độ
        for (char c : key) {
            hashVal = (hashVal * p + static_cast<unsigned char>(c)) % capacity;
        }
        return hashVal;
    }

    // 2. CƠ CHẾ REHASH (MỞ RỘNG BẢNG BĂM)
    // Đảm bảo chiều dài danh sách liên kết không bị kéo dài khi nạp 10.000+ bản ghi
    void rehash() {
        size_t oldCapacity = capacity;
        HashNode** oldTable = table;

        // Tăng dung lượng mảng lên gấp đôi (cộng 1 để tạo số lẻ tránh trùng ước)
        capacity = oldCapacity * 2 + 1;
        table = new HashNode*[capacity](); // Khởi tạo mảng mới toàn bộ là nullptr
        count = 0;

        for (size_t i = 0; i < oldCapacity; ++i) {
            HashNode* current = oldTable[i];
            while (current != nullptr) {
                HashNode* nextNode = current->next;
                // Chèn lại từng phần tử vào bảng băm mới
                insert(current->value);
                delete current; // Giải phóng node cũ
                current = nextNode;
            }
        }
        delete[] oldTable; // Giải phóng mảng con trỏ cũ
    }

public:

    // [BỔ SUNG CHO VISUALIZER]: Xuất trạng thái các bucket và chuỗi chaining ra JSON
    std::string getBucketsJson() const {
        std::string json = "[\n";
        bool firstBucket = true;
        for (size_t i = 0; i < capacity; ++i) {
            if (table[i] != nullptr) {
                if (!firstBucket) json += ",\n";
                firstBucket = false;
                json += "    {\"bucket\": " + std::to_string(i) + ", \"chain\": [";
                HashNode* cur = table[i];
                bool firstNode = true;
                while (cur != nullptr) {
                    if (!firstNode) json += ", ";
                    firstNode = false;
                    json += cur->value.toJson();
                    cur = cur->next;
                }
                json += "]}";
            }
        }
        json += "\n  ]";
        return json;
    }
    
    // Khởi tạo bảng băm với dung lượng mặc định là số nguyên tố
    MyHashTable(size_t initialCapacity = 1009)
        : capacity(initialCapacity), count(0) {
        table = new HashNode*[capacity]();
    }

    // Destructor: Quản lý bộ nhớ nghiêm ngặt, dọn sạch con trỏ tránh memory leak
    ~MyHashTable() {
        clear();
        delete[] table;
    }

    // Ngăn chặn sao chép nông (shallow copy) gây lỗi giải phóng bộ nhớ 2 lần
    MyHashTable(const MyHashTable&) = delete;
    MyHashTable& operator=(const MyHashTable&) = delete;

    // Xóa toàn bộ dữ liệu trong bảng băm
    void clear() {
        for (size_t i = 0; i < capacity; ++i) {
            HashNode* current = table[i];
            while (current != nullptr) {
                HashNode* temp = current;
                current = current->next;
                delete temp;
            }
            table[i] = nullptr;
        }
        count = 0;
    }

    // 3. THAO TÁC CHÈN (INSERT) - Độ phức tạp trung bình O(1)
    bool insert(const Document& doc) {
        if (doc.documentId.empty()) return false;

        // Nếu tỷ lệ đầy vượt ngưỡng 0.75, mở rộng bảng trước khi chèn
        if (static_cast<double>(count + 1) / capacity > MAX_LOAD_FACTOR) {
            rehash();
        }

        size_t index = hashFunction(doc.documentId);
        HashNode* current = table[index];

        // Trường hợp ID đã tồn tại: Cập nhật lại dữ liệu
        while (current != nullptr) {
            if (current->key == doc.documentId) {
                current->value = doc;
                return true;
            }
            current = current->next;
        }

        // Chèn node mới vào ĐẦU danh sách liên kết tại bucket đó (O(1))
        HashNode* newNode = new HashNode(doc.documentId, doc);
        newNode->next = table[index];
        table[index] = newNode;
        count++;
        return true;
    }

    // 4. THAO TÁC TRA CỨU THEO ID (SEARCH - MC1) - Độ phức tạp trung bình O(1)
    bool search(const std::string& key, Document& outDoc) const {
        if (key.empty()) return false;

        size_t index = hashFunction(key);
        HashNode* current = table[index];

        // Duyệt chuỗi liên kết tại bucket tìm kiếm
        while (current != nullptr) {
            if (current->key == key) {
                outDoc = current->value;
                return true; // Tìm thấy bản ghi
            }
            current = current->next;
        }
        return false; // Không tìm thấy
    }

    // 5. THAO TÁC XÓA (REMOVE) - Độ phức tạp trung bình O(1)
    bool remove(const std::string& key) {
        if (key.empty()) return false;

        size_t index = hashFunction(key);
        HashNode* current = table[index];
        HashNode* prev = nullptr;

        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    table[index] = current->next; // Xóa node đầu
                } else {
                    prev->next = current->next; // Bỏ qua node hiện tại
                }
                delete current;
                count--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    // CÁC HÀM THỐNG KÊ (Dùng để chứng minh hiệu năng và trả lời Viva D8)
    size_t size() const { return count; }
    size_t getCapacity() const { return capacity; }
    double getLoadFactor() const { return static_cast<double>(count) / capacity; }
};