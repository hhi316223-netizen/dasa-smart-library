#pragma once
#include <string>

enum class DocumentStatus { Available, Borrowed, Overdue, Reserved };

struct Document {
    std::string documentId;       // Mã định danh duy nhất (Alphanumeric: BK00001)
    std::string title;            // Tên tài liệu
    std::string borrowerId;       // Mã người mượn
    DocumentStatus status;        // Trạng thái hiện tại
    long long dueDate;            // Dấu thời gian hạn trả
    int priorityLevel;            // Mức ưu tiên (1 đến 5)
    long long registrationTime;   // Dấu thời gian đăng ký (FIFO)
    std::string auditCategory;    // Mã phân nhóm kiểm toán
    std::string conditionState;   // Tình trạng vật lý

    // Hàm xuất JSON phục vụ Web Visualizer
    std::string toJson() const {
        return "{\"id\":\"" + documentId + "\",\"title\":\"" + title + 
               "\",\"priority\":" + std::to_string(priorityLevel) + 
               ",\"dueDate\":" + std::to_string(dueDate) + "}";
    }
};
