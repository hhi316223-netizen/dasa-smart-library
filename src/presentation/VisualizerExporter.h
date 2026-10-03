#ifndef WEB_EXPORTER_H
#define WEB_EXPORTER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "../models/Document.h"

class WebExporter {
public:
    // Xuat mang Document ra dinh dang JSON hop le
    static void exportJSON(const std::string& filename, const std::vector<Document>& list) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "[Loi] Khong the mo file JSON de ghi!\n";
            return;
        }

        file << "[\n";
        for (size_t i = 0; i < list.size(); i++) {
            file << "  {\n";
            file << "    \"id\": " << list[i].id << ",\n";
            file << "    \"title\": \"" << list[i].title << "\",\n";
            file << "    \"author\": \"" << list[i].author << "\",\n";
            file << "    \"year\": " << list[i].year << ",\n";
            file << "    \"priority\": " << list[i].priority << "\n";
            file << "  }";
            // Tranh them dau phay o phan tu cuoi cung de khong loi cu phap JSON
            if (i + 1 < list.size()) {
                file << ",";
            }
            file << "\n";
        }
        file << "]\n";

        file.close();
        std::cout << "-> Da xuat thanh cong tap mau JSON tai: " << filename << "\n";
    }
};

#endif
