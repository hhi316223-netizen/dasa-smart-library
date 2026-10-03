#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include "../models/Document.h"
#include "../dsa_core/MyMergeSort.h"

class Benchmark {
public:
    static std::vector<Document> generateData(int n) {
        std::vector<Document> list(n);
        for (int i = 0; i < n; i++) {
            list[i].id = 1000 + i;
            list[i].title = "Sach_" + std::to_string(i + 1);
            list[i].author = "TacGia_" + std::to_string((i % 50) + 1);
            list[i].year = 1990 + (rand() % 36);
            list[i].priority = 1 + (rand() % 5);
        }
        return list;
    }

    static void runBenchmark(const std::string& filename) {
        std::vector<int> testSizes = {100, 500, 1000, 2500, 5000, 7500, 10000};
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "Loi mo file CSV!\n";
            return;
        }

        file << "N,Time_ms\n";
        std::cout << "\nBat dau do thoi gian chay Merge Sort:\n";

        for (size_t idx = 0; idx < testSizes.size(); idx++) {
            int n = testSizes[idx];
            std::vector<Document> data = generateData(n);

            auto start = std::chrono::high_resolution_clock::now();
            MyMergeSort::sort(data);
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> elapsed = end - start;
            double timeTaken = elapsed.count();

            std::cout << "So luong N = " << n << " tai lieu: " << timeTaken << " ms\n";
            file << n << "," << timeTaken << "\n";
        }

        file.close();
        std::cout << "Da luu ket qua vao file " << filename << "\n";
    }
};
```[cite: 1, 2]
