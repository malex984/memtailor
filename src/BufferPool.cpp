/* Copyright (C) 2011 Bjarke Hammersholt Roune (www.broune.com)
   MemTailor is distributed under the Modified BSD License. See license.txt. */
#include "BufferPool.h"

#include <utility>
#include <limits>
#include <algorithm>

namespace memt {
  BufferPool::BufferPool(size_t bufferSize):
    _bufferSize(MemoryBlocks::alignThrowOnOverflow(
      std::max(bufferSize, sizeof(FreeNode)))),
    _free(0),
    _blocks() {
  }

  void BufferPool::growCapacity() {
    // ** Calcuate size of block (doubles capacity)
    size_t size = block().getBytesInBlock();
    if (size == 0) {
      // start out at 10 buffers
      MEMT_ASSERT(block().isNull());
      if (_bufferSize > std::numeric_limits<size_t>::max() / 10)
        throw std::bad_alloc(); // _bufferSize * 10 overflows
      size = _bufferSize * 10;
    } else {
      // double the size
      if (size > std::numeric_limits<size_t>::max() / 2)
        throw std::bad_alloc(); // size * 2 overflows
      size *= 2;
    }

    // ** Allocate next block
    MEMT_ASSERT(MemoryBlocks::alignNoOverflow(size) == size);
    MEMT_ASSERT(size > block().getBytesInBlock());
    _blocks.allocBlock(size);
  }

  void BufferPool::freeAllBuffers() {
    _free = 0;
    _blocks.freeAllPreviousBlocks();
  }

  void BufferPool::freeAllBuffersAndBackingMemory() {
    _free = 0;
    _blocks.freeAllBlocks();
  }
}
