/* Copyright (C) 2011 Bjarke Hammersholt Roune (www.broune.com)
   Distributed under the Modified BSD License. See license.txt. */

#ifndef STDINC_GUARD
#define STDINC_GUARD

#ifdef _MSC_VER // For Microsoft Compiler in Visual Studio C++.
#define _SCL_SECURE_NO_WARNINGS // no std::copy on pointers warnings
#pragma warning (disable: 4996) // std::copy is flagged as dangerous.
#pragma warning (disable: 4290) // VC++ ignores throw () specification.
#pragma warning (disable: 4127) // Warns about using "while (true)".
#endif

#ifdef _DEBUG
#ifndef DEBUG
#define DEBUG
#endif
#endif

#include <new> // for std::bad_alloc and placement new
#include <cstddef> // for size_t

#ifdef DEBUG
#include <cassert>
#define ASSERT(X) assert(X);
#else
#define ASSERT(X)
#endif

/// The alignment that memory allocators must ensure. In other words
/// allocators must return pointer addresses that are divisible by
/// MemoryAlignment. MemoryAlignment must be a power of 2.
static const size_t MemoryAlignment = sizeof(void*);

#endif
