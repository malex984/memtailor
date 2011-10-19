/* Copyright (C) 2011 Bjarke Hammersholt Roune (www.broune.com)
   MemTailor is distributed under the Modified BSD License. See license.txt. */

#ifndef MEMT_STDINC_GUARD
#define MEMT_STDINC_GUARD

#if (defined DEBUG || defined _DEBUG) && (!defined MEMT_NDEBUG)
#define MEMT_DEBUG
#include <cassert>
#define MEMT_ASSERT(X) assert(X)
#endif

#ifndef MEMT_ASSERT
#define MEMT_ASSERT(X)
#endif

/// The alignment that memory allocators must ensure. In other words
/// allocators must return pointer addresses that are divisible by
/// MemoryAlignment. MemoryAlignment must be a power of 2.
namespace memt {
  static const unsigned int MemoryAlignment = sizeof(void*);
  static const unsigned int BitsPerByte = 8;
}

#endif
