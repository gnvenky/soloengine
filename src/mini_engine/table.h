#pragma once
#include "buffer_pool.h"
#include "slotted_page.h"
#include <vector>

class Table {
public:
    Table(BufferPool& pool)
        : pool_(pool), current_block_(0), total_blocks_(1)
    {
        uint8_t* page = pool_.Fetch(current_block_);
        SlottedPage::Init(page);
        pool_.MarkDirty(current_block_);
    }

    void Insert(const std::string& value) {
        uint8_t* page = pool_.Fetch(current_block_);
        SlottedPage sp(page);

        if (!sp.HasSpace(value.size())) {
            pool_.MarkDirty(current_block_);
            current_block_++;
            total_blocks_++;

            page = pool_.Fetch(current_block_);
            SlottedPage::Init(page);
            sp = SlottedPage(page);
        }

        sp.Insert(value.data(), value.size());
        pool_.MarkDirty(current_block_);
    }

    std::vector<std::string> ScanAll() {
        std::vector<std::string> all;

        for (uint64_t i = 0; i < total_blocks_; i++) {
            uint8_t* page = pool_.Fetch(i);
            SlottedPage sp(page);

            auto rows = sp.Scan();
            all.insert(all.end(), rows.begin(), rows.end());
        }

        return all;
    }

private:
    BufferPool& pool_;
    uint64_t current_block_;
    uint64_t total_blocks_;
};

