/* Copyright (C) 2011 Bjarke Hammersholt Roune (www.broune.com)
   Distributed under the Modified BSD License. See license.txt. */
#include "stdinc.h"
#include "Arena.h"

#include <limits>

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
#ifdef DEBUG
  _debugAllocs.swap(std::vector<void*>());
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

  ASSERT(size >= needed);
  ASSERT(size % MemoryAlignment == 0);
  _blocks.allocBlock(size);
}

void Arena::freeTopFromOldBlock(void* ptr) {
  ASSERT(ptr != 0);
  ASSERT(block().empty());
  ASSERT(block().hasPreviousBlock());

  Block* previous = block().getPreviousBlock();
  ASSERT(previous->isInBlock(ptr));
  previous->setPosition(ptr);
  if (previous->empty())
	_blocks.freePreviousBlock();
}

void Arena::freeAndAllAfterFromOldBlock(void* ptr) {
  ASSERT(!block().isInBlock(ptr));
  ASSERT(block().getPreviousBlock() != 0);

  block().setPosition(block().begin());
  while (!(block().getPreviousBlock()->isInBlock(ptr))) {
	_blocks.freePreviousBlock();
	ASSERT(block().hasPreviousBlock()); // ptr must be in some block
  }

  ASSERT(block().getPreviousBlock()->isInBlock(ptr));
  block().getPreviousBlock()->setPosition(ptr);
  if (block().getPreviousBlock()->empty())
	_blocks.freePreviousBlock();
}
