#pragma once
#include <cstdint>
#include <cstring>
#include <vector>
#include <stdexcept>

constexpr uint32_t PAGE_SIZE = 16 * 1024;

struct PageHeader {
    uint16_t slot_count;
    uint16_t free_start;
    uint16_t free_end;
};

struct Slot {
    uint16_t offset;
    uint16_t length;
    uint8_t deleted;
};

class SlottedPage {
public:
    SlottedPage(uint8_t* buffer) : data_(buffer) {
        header_ = reinterpret_cast<PageHeader*>(data_);
    }

    static void Init(uint8_t* buffer) {
        auto* h = reinterpret_cast<PageHeader*>(buffer);
        h->slot_count = 0;
        h->free_start = sizeof(PageHeader);
        h->free_end = PAGE_SIZE;
    }

    bool HasSpace(uint16_t len) {
        return (header_->free_end - header_->free_start) >= (len + sizeof(Slot));
    }

    uint16_t Insert(const void* record, uint16_t len) {
        if (!HasSpace(len))
            throw std::runtime_error("Page full");

        header_->free_end -= len;
        std::memcpy(data_ + header_->free_end, record, len);

        Slot* slot = GetSlot(header_->slot_count);
        slot->offset = header_->free_end;
        slot->length = len;
        slot->deleted = 0;

        header_->slot_count++;
        header_->free_start += sizeof(Slot);

        return header_->slot_count - 1;
    }

    std::vector<std::string> Scan() {
        std::vector<std::string> rows;
        for (uint16_t i = 0; i < header_->slot_count; i++) {
            Slot* slot = GetSlot(i);
            if (slot->deleted) continue;

            rows.emplace_back(
                reinterpret_cast<char*>(data_ + slot->offset),
                slot->length
            );
        }
        return rows;
    }

private:
    Slot* GetSlot(uint16_t id) {
        return reinterpret_cast<Slot*>(
            data_ + sizeof(PageHeader) + id * sizeof(Slot)
        );
    }

    uint8_t* data_;
    PageHeader* header_;
};

