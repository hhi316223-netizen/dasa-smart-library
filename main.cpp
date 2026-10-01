#include "src/persistence/FileStorage.h"
#include "src/dsa_core/LibraryEngine.h"
#include "src/presentation/ConsoleUI.h"

int main() {
    // 1. Tầng 3: Đọc dữ liệu từ file CSV
    auto rawData = FileStorage::loadDocuments("data/documents.csv");

    // 2. Tầng 2: Nạp dữ liệu lên RAM (DSA Core)
    LibraryEngine engine;
    engine.bulkLoad(rawData);

    // 3. Tầng 1: Khởi động giao diện điều khiển
    ConsoleUI ui(engine);
    ui.run();

    return 0;
}
