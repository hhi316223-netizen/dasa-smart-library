#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "../models/Document.h"
#include "../dsa_core/LibraryEngine.h"

class VisualizerExporter {
public:
    // Ham xuat trang thai dung chuan cho Web Visualizer
    static void exportState(const LibraryEngine& engine, const std::string& outputPath) {
        std::ofstream out(outputPath);
        if (!out.is_open()) {
            std::cerr << "[Tầng 1] Khong the mo file JSON de ghi: " << outputPath << "\n";
            return;
        }

        const auto& docs = engine.getRawMemoryData();
        out << "{\n";
        out << "  \"total\": " << docs.size() << ",\n";
        out << "  \"hash_capacity\": " << engine.getHashTableCapacity() << ",\n";
        out << "  \"load_factor\": " << engine.getHashTableLoadFactor() << ",\n";
        out << "  \"hash_buckets\": [],\n";
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

    // Ham phu tuong thich neu ban cung nhom goi exportJSON
    static void exportJSON(const std::string& filename, const std::vector<Document>& list) {
        std::ofstream out(filename);
        if (!out.is_open()) return;
        out << "{\n  \"total\": " << list.size() << ",\n  \"items\": [\n";
        for (size_t i = 0; i < list.size(); ++i) {
            out << "    " << list[i].toJson();
            if (i + 1 < list.size()) out << ",";
            out << "\n";
        }
        out << "  ]\n}\n";
        out.close();
    }
};

// Dinh danh tuong thich neu ai do trong nhom goi WebExporter
using WebExporter = VisualizerExporter;