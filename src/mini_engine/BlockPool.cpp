#include "BlockPool.h"

BlockPool::BlockPool(size_t numPages) {
    pages_.resize(numPages);
}

Page& BlockPool::getPage(size_t id) {
    return pages_[id];
}

size_t BlockPool::size() const {
    return pages_.size();
}

