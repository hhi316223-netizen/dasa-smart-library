#pragma once
#include "../dsa_core/LibraryEngine.h"
#include "VisualizerExporter.h"
#include <iostream>

class ConsoleUI {
private:
    LibraryEngine& engine;

public:
    ConsoleUI(LibraryEngine& eng) : engine(eng) {}

    void run() {
        int choice = -1;
        do {
            std::cout << "\n========================================\n";
            std::cout << "   SMART LIBRARY SYSTEM (DSA CORE)\n";
            std::cout << "========================================\n";
            std::cout << "1. Tra cuu tai lieu theo ID (MC1)\n";
            std::cout << "2. Dieu phoi tai lieu uu tien cao nhat (FR1)\n";
            std::cout << "3. Kiem toan theo khoang thoi gian (FR2)\n";
            std::cout << "4. Xuat trang thai he thong cho Web Visualizer\n";
            std::cout << "0. Thoat chuong trinh\n";
            std::cout << "Nhap lua chon: ";
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::string dummy;
                std::cin >> dummy;
                continue;
            }

            if (choice == 1) {
                std::string id;
                std::cout << "Nhap ma DocumentID: ";
                std::cin >> id;
                Document res;
                if (engine.findDocumentById(id, res)) {
                    std::cout << "-> TIM THAY: " << res.title 
                              << " | Uu tien: " << res.priorityLevel << "\n";
                } else {
                    std::cout << "-> KHONG TIM THAY ma tai lieu nay!\n";
                }
            } else if (choice == 2) {
                Document res;
                if (engine.pollNextPriority(res)) {
                    std::cout << "-> DIEU PHOI: " << res.title 
                              << " (Muc uu tien: " << res.priorityLevel << ")\n";
                } else {
                    std::cout << "-> Hang doi hien dang rong!\n";
                }
            } else if (choice == 3) {
                long long start, end;
                std::cout << "Nhap StartDate (timestamp): ";
                std::cin >> start;
                std::cout << "Nhap EndDate (timestamp): ";
                std::cin >> end;
                auto list = engine.auditRange(start, end);
                std::cout << "-> Tim thay " << list.size() << " ban ghi trong khoang nay.\n";
            } else if (choice == 4) {
                VisualizerExporter::exportState(engine, "visualizer/state.json");
            }
        } while (choice != 0);
    }
};