/* Copyright (C) 2011 Bjarke Hammersholt Roune (www.broune.com)
   MemTailor is distributed under the Modified BSD License. See license.txt. */
#ifndef MEMT_OBJECT_POOL_GUARD
#define MEMT_OBJECT_POOL_GUARD

#include "MemoryBlocks.h"
#include <cstddef>

namespace memt {
  /** Allocator for allocating and freeing same-size buffers. Uses
      a free list. All allocations are automatically freed when
      the buffer pool is destructed. */
  class BufferPool {
  public:
    /** bufferSize is how many bytes are returned by each call to alloc. */
    BufferPool(size_t bufferSize);

    /** Returns a pointer to an array of getBufferSize() chars. The
        alignment is as for Arena. The lifetime of the buffer is until free
        is called with the returned value as parameter on this same object
        or clear() is called or this object is destructed.

        Do not pass the returned value to ::free, do not delete it and do
        not free it on a different BufferPool.  Throws an exception if no
        more memory can be allocated. Never returns null. */
    inline void* alloc();

    /** Makes the buffer at ptr available for reuse. ptr must be a value
        previously returned by alloc on this same object that hasn't been
        freed already since then. ptr must not be null. This method cannot
        throw an exception. */
    inline void free(void* ptr);

    /** Returns how many bytes are in each buffer. Can be a few bytes
        more than requested due to internal requirements on the size of the
        buffers. Will never be less than requested. */
    size_t getBufferSize() const {return _bufferSize;}

    /** Marks all allocated buffers as available for reuse. Does not
        deallocate all the backing memory. */
    void freeAllBuffers();

    /** Marks all allcoated buffers as available for reuse and frees
        all backing memory too. */
    void freeAllBuffersAndBackingMemory();

    /** Returns the total amount of memory allocated by this object. Includes
        excess capacity that has not been allocated by a client yet. */
    size_t getMemoryUsage() const {return _blocks.getMemoryUsage();}

  private:
    typedef MemoryBlocks::Block Block;
    Block& block() {return _blocks.getFrontBlock();}
    const Block& block() const {return _blocks.getFrontBlock();}

    /** Allocate another block of double the size. */
    void growCapacity();

    /** A node of the linked list of free buffers. */
    struct FreeNode {
      FreeNode* next;
    };

    const size_t _bufferSize; /// size of the buffers returned by alloc
    FreeNode* _free; /// null indicates that the free list is empty
    MemoryBlocks _blocks;
  };

  inline void* BufferPool::alloc() {
    void* ptr;
    if (_free != 0) {
      ptr = _free;
      _free = _free->next;
    } else {
      if (block().position() == block().end())
        growCapacity();
      ptr = block().position();
      block().setPosition(block().position() + getBufferSize());
    }
    return ptr;
  }

  inline void BufferPool::free(void* ptr) {
    FreeNode* node = reinterpret_cast<FreeNode*>(ptr);
    node->next = _free;
    _free = node;
  }
}

#endif
