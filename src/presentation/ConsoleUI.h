#pragma once
#include "../dsa_core/LibraryEngine.h"
#include "VisualizerExporter.h"
#include <ctime>
#include <sstream>
#include <iostream>
#include <string>

class ConsoleUI {
private:
    LibraryEngine& engine;

    // Hàm chuyển đổi: Hỗ trợ linh hoạt cả DD/MM/YYYY lẫn Unix Timestamp
    long long parseDateInput(const std::string& input) const {
        if (input.empty()) return 0;

        // Nếu người dùng nhập định dạng ngày tháng (chứa '/' hoặc '-')
        if (input.find('/') != std::string::npos || input.find('-') != std::string::npos) {
            int day = 0, month = 0, year = 0;
            char sep1 = 0, sep2 = 0;
            std::stringstream ss(input);
            if (ss >> day >> sep1 >> month >> sep2 >> year) {
                std::tm tm_struct = {};
                tm_struct.tm_mday = day;
                tm_struct.tm_mon = month - 1;      // Tháng trong C/C++ tính từ 0 đến 11
                tm_struct.tm_year = year - 1900;   // Năm tính từ mốc 1900
                tm_struct.tm_isdst = -1;
                std::time_t t = std::mktime(&tm_struct);
                if (t != -1) return static_cast<long long>(t);
            }
        }

        // Nếu người dùng nhập số timestamp thô
        try {
            return std::stoll(input);
        } catch (...) {
            return 0;
        }
    }

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
                std::string startStr, endStr;
                std::cout << "Nhap StartDate (DD/MM/YYYY hoac timestamp): ";
                std::cin >> startStr;
                std::cout << "Nhap EndDate (DD/MM/YYYY hoac timestamp): ";
                std::cin >> endStr;

                long long start = parseDateInput(startStr);
                long long end = parseDateInput(endStr);

                auto list = engine.auditRange(start, end);
                std::cout << "-> Tim thay " << list.size() << " ban ghi trong khoang nay.\n";
            } else if (choice == 4) {
                VisualizerExporter::exportState(engine, "visualizer/state.json");
            }
        } while (choice != 0);
    }
};