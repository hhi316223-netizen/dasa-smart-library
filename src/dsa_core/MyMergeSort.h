#pragma once
#include <vector>
#include "../models/Document.h"

class MyMergeSort {
private:
    // Ham tron hai nua mang con da sap xep: [left..mid] va [mid+1..right]
    static void merge(std::vector<Document>& arr, std::vector<Document>& temp, 
                      int left, int mid, int right) {
        int i = left;      // Con tro duyet nua ben trai
        int j = mid + 1;   // Con tro duyet nua ben phai
        int k = left;      // Vi tri ghi vao mang phu temp

        // So sanh tung cap phan tu giua hai nua
        while (i <= mid && j <= right) {
            // Sap xep tang dan theo nam (year), neu trung nam thi xep tang dan theo id
            // Dau <= o id giup giu tinh Stable Sort (on dinh): bang nhau thi chon ben trai truoc
            if (arr[i].year < arr[j].year || (arr[i].year == arr[j].year && arr[i].id <= arr[j].id)) {
                temp[k] = arr[i];
                i++;
            } else {
                temp[k] = arr[j];
                j++;
            }
            k++;
        }

        // Chep vet cac phan tu con lai cua nua ben trai (neu con)
        while (i <= mid) {
            temp[k] = arr[i];
            i++;
            k++;
        }

        // Chep vet cac phan tu con lai cua nua ben phai (neu con)
        while (j <= right) {
            temp[k] = arr[j];
            j++;
            k++;
        }

        // Chep nguoc du lieu da sap xep tu mang temp ve lai mang goc arr
        for (int idx = left; idx <= right; idx++) {
            arr[idx] = temp[idx];
        }
    }

    // Ham de quy chia doi mang theo nguyen ly Chia de tri (Divide and Conquer)
    static void mergeSort(std::vector<Document>& arr, std::vector<Document>& temp, 
                          int left, int right) {
        // Dieu kien dung de quy: khi mang chi con 0 hoac 1 phan tu
        if (left < right) {
            // Tinh vi tri o giua (tranh tran so nguyen)
            int mid = left + (right - left) / 2;

            // De quy chia doi nua trai va nua phai
            mergeSort(arr, temp, left, mid);
            mergeSort(arr, temp, mid + 1, right);

            // Tron hai nua da sap xep lai voi nhau
            merge(arr, temp, left, mid, right);
        }
    }

public:
    // Ham goi cong khai tu ben ngoai
    static void sort(std::vector<Document>& arr) {
        if (arr.size() <= 1) return;

        // Cap phat duy nhat 1 mang phu temp de toi uu bo nho, tranh cap phat lai nhieu lan
        std::vector<Document> temp(arr.size());
        mergeSort(arr, temp, 0, (int)arr.size() - 1);
    }
};

