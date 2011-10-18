/* Copyright (C) 2011 Bjarke Hammersholt Roune (www.broune.com)
   Distributed under the Modified BSD License. See license.txt. */
#include "Arena.h"

#include <limits>

namespace SpecAlloc {
  Arena Arena::_scratchArena;

  Arena::Arena() {
  }

  Arena::~Arena() {
    freeAllAllocsAndBackingMemory();
  }

  void Arena::freeAllAllocs() {
    while (block().hasPreviousBlock())
      _blocks.freePreviousBlock();
    block().clear();
  }

  void Arena::freeAllAllocsAndBackingMemory() {
    _blocks.freeAllBlocks();
#ifdef SPECALLOC_DEBUG
    std::vector<void*>().swap(_debugAllocs);
#endif
  }

  void Arena::growCapacity(const size_t needed) {
    // ** Calcuate size of block (doubles capacity)
    size_t size = std::max(needed, block().getBytesInBlock());
    if (size > std::numeric_limits<size_t>::max() / 2)
      throw std::bad_alloc(); // size * 2 overflows
    size *= 2;
    const size_t minimumAlloc = 16 * 1024 - sizeof(Block) - 16;
    size = std::max(size, minimumAlloc); // avoid many small blocks
    size = MemoryBlocks::alignThrowOnOverflow(size);

    SPECALLOC_ASSERT(size >= needed);
    SPECALLOC_ASSERT(size % MemoryAlignment == 0);
    _blocks.allocBlock(size);
  }

  void Arena::freeTopFromOldBlock(void* ptr) {
    SPECALLOC_ASSERT(ptr != 0);
    SPECALLOC_ASSERT(block().empty());
    SPECALLOC_ASSERT(block().hasPreviousBlock());

    Block* previous = block().getPreviousBlock();
    SPECALLOC_ASSERT(previous->isInBlock(ptr));
    previous->setPosition(ptr);
    if (previous->empty())
      _blocks.freePreviousBlock();
  }

  void Arena::freeAndAllAfterFromOldBlock(void* ptr) {
    SPECALLOC_ASSERT(!block().isInBlock(ptr));
    SPECALLOC_ASSERT(block().getPreviousBlock() != 0);

    block().setPosition(block().begin());
    while (!(block().getPreviousBlock()->isInBlock(ptr))) {
      _blocks.freePreviousBlock();
      SPECALLOC_ASSERT(block().hasPreviousBlock()); // ptr must be in some block
    }

    SPECALLOC_ASSERT(block().getPreviousBlock()->isInBlock(ptr));
    block().getPreviousBlock()->setPosition(ptr);
    if (block().getPreviousBlock()->empty())
      _blocks.freePreviousBlock();
  }
}
