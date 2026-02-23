#pragma once
#include <fstream>
#include <string>

class BlockFile {
public:
    BlockFile(const std::string& filename) {
        file_.open(filename,
                   std::ios::in | std::ios::out |
                   std::ios::binary | std::ios::trunc);

        if (!file_.is_open())
            throw std::runtime_error("Cannot open file");
    }

    void Write(uint64_t block_id, uint8_t* data) {
        file_.seekp(block_id * PAGE_SIZE);
        file_.write(reinterpret_cast<char*>(data), PAGE_SIZE);
        file_.flush();
    }

    void Read(uint64_t block_id, uint8_t* data) {
        file_.seekg(block_id * PAGE_SIZE);
        file_.read(reinterpret_cast<char*>(data), PAGE_SIZE);
    }

private:
    std::fstream file_;
};

