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

        const auto& docs = engine.getRawMemoryData();
        out << "{\n";
        out << "  \"total\": " << docs.size() << ",\n";
        out << "  \"hash_capacity\": " << engine.getHashTableCapacity() << ",\n";
        out << "  \"load_factor\": " << engine.getHashTableLoadFactor() << ",\n";
        out << "  \"hash_buckets\": " << engine.getHashTableVisualJson() << ",\n";
        out << "  \"items\": [\n";
        for (size_t i = 0; i < docs.size(); ++i) {
            out << "    " << docs[i].toJson();
            if (i + 1 < docs.size()) out << ",";
            out << "\n";
        }
        out << "  ]\n";
        out << "}\n";
        out.close();
        std::cout << "[Tầng 1] Da cap nhat trang thai he thong vao: " << outputPath << "\n";
    }
};

#endif
