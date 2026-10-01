#pragma once
#include "../dsa_core/LibraryEngine.h"
#include <fstream>
#include <iostream>

class VisualizerExporter {
public:
    static void exportState(const LibraryEngine& engine, const std::string& outputPath) {
        std::ofstream out(outputPath);
        if (!out.is_open()) {
            std::cerr << "[Tầng 1] Khong the ghi file state.json\n";
            return;
        }

        const auto& docs = engine.getRawMemoryData();
        out << "{\n  \"total\": " << docs.size() << ",\n  \"items\": [\n";
        for (size_t i = 0; i < docs.size(); ++i) {
            out << "    " << docs[i].toJson();
            if (i + 1 < docs.size()) out << ",";
            out << "\n";
        }
        out << "  ]\n}\n";
        out.close();
        std::cout << "[Tầng 1] Da cap nhat trang thai he thong vao: " << outputPath << "\n";
    }
};