/* Copyright (C) 2011 Bjarke Hammersholt Roune (www.broune.com)
   Distributed under the Modified BSD License. See license.txt. */
#include "stdinc.h"
#include "BufferPool.h"

#include <utility>
#include <limits>
#include <algorithm>

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
    ASSERT(block().isNull());
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
  ASSERT(MemoryBlocks::alignNoOverflow(size) == size);
  ASSERT(size > block().getBytesInBlock());
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
