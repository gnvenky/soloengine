#pragma once
#include "Page.h"
#include <vector>

class BlockPool {
public:
    BlockPool(size_t numPages);

    Page& getPage(size_t id);
    size_t size() const;

private:
    std::vector<Page> pages_;
};

