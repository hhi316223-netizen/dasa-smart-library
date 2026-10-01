#pragma once
#include <string>

enum class DocumentStatus { Available, Borrowed, Overdue, Reserved };

struct Document {
    std::string documentId;       // Khóa định danh duy nhất (MC1)
    std::string title;            // Tên tài liệu
    std::string borrowerId;       // Mã người mượn
    DocumentStatus status;        // Trạng thái hiện tại
    long long dueDate;            // Dấu thời gian hạn trả (dùng cho FR2)
    int priorityLevel;            // 1 (Cao nhất) -> 5 (Thấp nhất) (dùng cho FR1)
    long long registrationTime;   // Dấu thời gian đăng ký (tie-breaker FIFO)
    std::string auditCategory;    // Mã phân nhóm kiểm toán
    std::string conditionState;   // Tình trạng vật lý

    // Chuyển đối tượng thành chuỗi JSON đơn giản để ghi ra state.json
    std::string toJson() const {
        return "{\"id\":\"" + documentId + "\",\"title\":\"" + title + 
               "\",\"priority\":" + std::to_string(priorityLevel) + 
               ",\"dueDate\":" + std::to_string(dueDate) + "}";
    }
};