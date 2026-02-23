#pragma once
#include <vector>
#include <cstdint>

constexpr size_t PAGE_SIZE = 4096;

struct Slot {
    uint16_t offset;
    uint16_t length;
};

class Page {
public:
    Page();

    bool insert(const std::vector<uint8_t>& data);
    std::vector<uint8_t> read(uint16_t slotId) const;

private:
    uint8_t data_[PAGE_SIZE];
    uint16_t freeStart_;
    uint16_t freeEnd_;
    std::vector<Slot> slots_;
};

