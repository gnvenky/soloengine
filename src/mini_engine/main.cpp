#include <iostream>
#include <string>
#include <vector>
#include "BlockPool.h"

std::vector<uint8_t> toBytes(const std::string& s) {
    return std::vector<uint8_t>(s.begin(), s.end());
}

std::string fromBytes(const std::vector<uint8_t>& v) {
    return std::string(v.begin(), v.end());
}

int main() {
    std::cout << "==== Slotted Page Demo ====\n";

    BlockPool pool(1);
    Page& page = pool.getPage(0);

    // ------------------------------------------------------------
    // Example 1: Multiple Inserts + Read Back
    // ------------------------------------------------------------
    std::cout << "\n--- Example 1: Insert and Read ---\n";

    for (int i = 0; i < 5; i++) {
        std::string value = "Record_" + std::to_string(i);
        if (page.insert(toBytes(value))) {
            std::cout << "Inserted: " << value << "\n";
        }
    }

    std::cout << "\nReading records:\n";
    for (int i = 0; i < 5; i++) {
        auto data = page.read(i);
        std::cout << "Read: " << fromBytes(data) << "\n";
    }

    // ------------------------------------------------------------
    // Example 2: Fill Page Until Full
    // ------------------------------------------------------------
    std::cout << "\n--- Example 2: Fill Page Until Full ---\n";

    int count = 0;
    while (true) {
        std::string big = "BIG_" + std::to_string(count);
        if (!page.insert(toBytes(big))) {
            break;
        }
        count++;
    }

    std::cout << "Inserted " << count
              << " additional records before page filled.\n";

    std::cout << "\n==== Demo Complete ====\n";
    return 0;
}

