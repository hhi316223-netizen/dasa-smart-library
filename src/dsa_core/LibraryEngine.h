#pragma once
#include "../models/Document.h"
#include "MyHashTable.h" // Nhúng Bảng băm của Thành viên 1
#include <vector>
#include <iostream>

class LibraryEngine {
private:
    MyHashTable docTable;                // CẤU TRÚC 1 (TV1): Quản lý tra cứu O(1)
    std::vector<Document> memoryStorage; // Tạm thời giữ để nạp các thao tác khác

public:

    // [BỔ SUNG CHO VISUALIZER]: Cung cấp thông số cấu trúc dữ liệu cho Tầng 1
    std::string getHashTableVisualJson() const { return docTable.getBucketsJson(); }
    size_t getHashTableCapacity() const { return docTable.getCapacity(); }
    double getHashTableLoadFactor() const { return docTable.getLoadFactor(); }
    
    void bulkLoad(const std::vector<Document>& rawData) {
        memoryStorage = rawData;
        
        // Nạp toàn bộ dữ liệu vào Bảng băm của Thành viên 1
        for (const auto& doc : rawData) {
            docTable.insert(doc);
        }

        std::cout << "[Tầng 2 - DSA Core] Da nap " << docTable.size() 
                  << " ban ghi vao Bang Bam. (Load factor: " 
                  << docTable.getLoadFactor() << ")\n";
    }

    // NGHIỆP VỤ MC1: Tra cứu chính xác theo ID thông qua Bảng băm
    bool findDocumentById(const std::string& id, Document& result) {
        return docTable.search(id, result); // O(1) trung bình!
    }

    // FR1: (Đang chờ Thành viên 2 nhúng Min-Heap)
    bool pollNextPriority(Document& result) {
        if (memoryStorage.empty()) return false;
        result = memoryStorage[0];
        return true;
    }

    // FR2: (Đang chờ Thành viên 3 nhúng AVL Tree)
    std::vector<Document> auditRange(long long startDate, long long endDate) {
        std::vector<Document> res;
        for (const auto& item : memoryStorage) {
            if (item.dueDate >= startDate && item.dueDate <= endDate) {
                res.push_back(item);
            }
        }
        return res;
    }

    const std::vector<Document>& getRawMemoryData() const {
        return memoryStorage;
    }
};