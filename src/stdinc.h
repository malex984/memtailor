/* Copyright (C) 2011 Bjarke Hammersholt Roune (www.broune.com)
   Distributed under the Modified BSD License. See license.txt. */

#ifndef SPECALLOC_STDINC_GUARD
#define SPECALLOC_STDINC_GUARD

#if defined DEBUG || defined _DEBUG
#define SPECALLOC_DEBUG
#include <cassert>
#define SPECALLOC_ASSERT(X) ::assert(X);
#endif
#endif

#ifndef SPECALLOC_ASSERT
#define SPECALLOC_ASSERT(X)
#endif

/// The alignment that memory allocators must ensure. In other words
/// allocators must return pointer addresses that are divisible by
/// MemoryAlignment. MemoryAlignment must be a power of 2.
namespace SpecAlloc {
  static const unsigned int MemoryAlignment = sizeof(void*);
  static const unsigned int BitsPerByte = 8;
}

#endif
