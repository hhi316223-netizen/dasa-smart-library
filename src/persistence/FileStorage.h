#pragma once
#include "../models/Document.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class FileStorage {
public:
    static std::vector<Document> loadDocuments(const std::string& filePath) {
        std::vector<Document> list;
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "[Tầng 3] Canh bao: Khong the mo file: " << filePath << "\n";
            return list;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string id, title, borrower, statusStr, dueStr, prioStr, regStr, cat, cond;

            // Đọc các trường ngăn cách bởi dấu phẩy
            std::getline(ss, id, ',');
            std::getline(ss, title, ',');
            std::getline(ss, borrower, ',');
            std::getline(ss, statusStr, ',');
            std::getline(ss, dueStr, ',');
            std::getline(ss, prioStr, ',');
            std::getline(ss, regStr, ',');
            std::getline(ss, cat, ',');
            std::getline(ss, cond, ',');

            try {
                Document doc;
                doc.documentId = id;
                doc.title = title;
                doc.borrowerId = borrower;
                doc.status = DocumentStatus::Available; // Mặc định
                doc.dueDate = dueStr.empty() ? 0 : std::stoll(dueStr);
                doc.priorityLevel = prioStr.empty() ? 5 : std::stoi(prioStr);
                doc.registrationTime = regStr.empty() ? 0 : std::stoll(regStr);
                doc.auditCategory = cat;
                doc.conditionState = cond;
                list.push_back(doc);
            } catch (...) {
                // Bỏ qua dòng lỗi định dạng để tránh crash chương trình
                continue;
            }
        }
        file.close();
        return list;
    }
};