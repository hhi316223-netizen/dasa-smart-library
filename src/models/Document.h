#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>

// Dinh nghia thong tin mot tai lieu trong thu vien
struct Document {
    int id;             // Ma tai lieu (DocumentID)
    std::string title;  // Tua de sach
    std::string author; // Tac gia
    int year;           // Nam xuat ban (dung de sap xep)
    int priority;       // Muc uu tien muon sach (1 den 5)
};

#endif
