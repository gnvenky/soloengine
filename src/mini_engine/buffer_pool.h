#pragma once
#include "block_file.h"
#include <unordered_map>

struct Frame {
    uint8_t data[PAGE_SIZE];
    bool dirty = false;
};

class BufferPool {
public:
    BufferPool(BlockFile& file) : file_(file) {}

    uint8_t* Fetch(uint64_t block_id) {
        if (!cache_.count(block_id)) {
            Frame frame{};
            file_.Read(block_id, frame.data);
            cache_[block_id] = frame;
        }
        return cache_[block_id].data;
    }

    void MarkDirty(uint64_t block_id) {
        cache_[block_id].dirty = true;
    }

    void Flush(uint64_t block_id) {
        if (cache_.count(block_id) && cache_[block_id].dirty) {
            file_.Write(block_id, cache_[block_id].data);
            cache_[block_id].dirty = false;
        }
    }

private:
    BlockFile& file_;
    std::unordered_map<uint64_t, Frame> cache_;
};

