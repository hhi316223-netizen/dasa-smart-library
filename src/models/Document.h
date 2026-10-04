#pragma once
#include <string>

enum class DocumentStatus { Available, Borrowed, Overdue, Reserved };

struct Document {
    std::string documentId;
    std::string title;
    std::string borrowerId;
    DocumentStatus status;
    long long dueDate;
    int priorityLevel;
    long long registrationTime;
    std::string auditCategory;
    std::string conditionState;

    std::string toJson() const {
        return "{\"id\":\"" + documentId + "\",\"title\":\"" + title + 
               "\",\"priority\":" + std::to_string(priorityLevel) + 
               ",\"dueDate\":" + std::to_string(dueDate) + "}";
    }
};