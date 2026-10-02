#pragma once
#include <vector>
#include <functional>
#include <utility>

template <typename T, typename Compare = std::less<T>>
class MyMergeSort {
private:
    static void merge(std::vector<T>& arr, std::vector<T>& temp, int left, int mid, int right, Compare comp) {
        int i = left;
        int j = mid + 1;
        int k = left;

        // Trộn 2 nửa vào mảng đệm temp
        while (i <= mid && j <= right) {
            // Đảm bảo Stable Sort: Nếu R[j] không strictly đứng trước L[i], ưu tiên L[i]
            if (!comp(arr[j], arr[i])) {
                temp[k++] = std::move(arr[i++]);
            } else {
                temp[k++] = std::move(arr[j++]);
            }
        }

        while (i <= mid) {
            temp[k++] = std::move(arr[i++]);
        }
        while (j <= right) {
            temp[k++] = std::move(arr[j++]);
        }

        // Chép ngược lại từ temp về arr gốc
        for (int idx = left; idx <= right; ++idx) {
            arr[idx] = std::move(temp[idx]);
        }
    }

    static void mergeSortInternal(std::vector<T>& arr, std::vector<T>& temp, int left, int right, Compare comp) {
        if (left >= right) return;
        int mid = left + (right - left) / 2;

        mergeSortInternal(arr, temp, left, mid, comp);
        mergeSortInternal(arr, temp, mid + 1, right, comp);
        merge(arr, temp, left, mid, right, comp);
    }

public:
    static void sort(std::vector<T>& arr, Compare comp = Compare()) {
        if (arr.size() <= 1) return;
        // Chỉ cấp phát bộ nhớ phụ 1 lần duy nhất cho toàn bộ quá trình sắp xếp
        std::vector<T> temp(arr.size());
        mergeSortInternal(arr, temp, 0, static_cast<int>(arr.size()) - 1, comp);
    }
};
