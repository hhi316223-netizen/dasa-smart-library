#pragma once
#include "../models/Document.h"
#include <vector>
#include <iostream>

// Khi các bạn viết xong, hãy mở include các file tương ứng:
// #include "MyHashTable.h"
// #include "MyMinHeap.h"
// #include "MyAVLTree.h"

class LibraryEngine {
private:
    // [TV1 sẽ thay thế mảng này bằng MyHashTable docTable;]
    // [TV2 sẽ thay thế mảng này bằng MyMinHeap waitlistQueue;]
    // [TV3 sẽ thay thế mảng này bằng MyAVLTree auditTree;]
    std::vector<Document> memoryStorage; 

public:
    void bulkLoad(const std::vector<Document>& rawData) {
        memoryStorage = rawData;
        std::cout << "[Tầng 2 - Core] Da nap " << memoryStorage.size() 
                  << " ban ghi vao bo nho RAM.\n";
    }

    // MC1: Tra cứu theo ID (TV1 phụ trách)
    bool findDocumentById(const std::string& id, Document& result) {
        // [TV1 nhét code: return docTable.search(id, result);]
        for (const auto& item : memoryStorage) {
            if (item.documentId == id) {
                result = item;
                return true;
            }
        }
        return false;
    }

    // FR1: Rút người có độ ưu tiên cao nhất (TV2 phụ trách)
    bool pollNextPriority(Document& result) {
        // [TV2 nhét code: return waitlistQueue.extractMin(result);]
        if (memoryStorage.empty()) return false;
        result = memoryStorage[0];
        return true;
    }

    // FR2: Kiểm toán giao dịch theo khoảng ngày (TV3 phụ trách)
    std::vector<Document> auditRange(long long startDate, long long endDate) {
        std::vector<Document> res;
        // [TV3 nhét code: auditTree.rangeQuery(startDate, endDate, res);]
        for (const auto& item : memoryStorage) {
            if (item.dueDate >= startDate && item.dueDate <= endDate) {
                res.push_back(item);
            }
        }
        return res;
    }

    // Trả về dữ liệu để Tầng 1 xuất file JSON trực quan hóa
    const std::vector<Document>& getRawMemoryData() const {
        return memoryStorage;
    }
};