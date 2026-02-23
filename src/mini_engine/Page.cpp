#include "Page.h"
#include <cstring>

Page::Page() {
    freeStart_ = 0;
    freeEnd_ = PAGE_SIZE;
}

bool Page::insert(const std::vector<uint8_t>& data) {
    uint16_t len = data.size();

    if (freeStart_ + sizeof(Slot) > freeEnd_ - len)
        return false;

    freeEnd_ -= len;
    std::memcpy(data_ + freeEnd_, data.data(), len);

    slots_.push_back({freeEnd_, len});
    freeStart_ += sizeof(Slot);

    return true;
}

std::vector<uint8_t> Page::read(uint16_t slotId) const {
    if (slotId >= slots_.size())
        return {};

    const Slot& s = slots_[slotId];
    return std::vector<uint8_t>(data_ + s.offset,
                                data_ + s.offset + s.length);
}

