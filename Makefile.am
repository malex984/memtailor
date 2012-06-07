ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I${top_srcdir}/include
# -I${top_builddir} 

lib_LTLIBRARIES = libmemtailor.la
# libmemtailordir = $(libdir)/

#libmemtailor_la_CFLAGS   = 
#libmemtailor_la_CXXFLAGS = 

libmemtailor_la_SOURCES  = src/BufferPool.cpp src/Arena.cpp src/MemoryBlocks.cpp

libmemtailor_includedir = $(includedir)/
nobase_libmemtailor_include_HEADERS = include/memtailor.h include/memtailor/stdinc.h \
include/memtailor/BufferPool.h include/memtailor/Arena.h include/memtailor/ArenaVector.h include/memtailor/MemoryBlocks.h

#TESTS=??
#check_PROGRAMS=$(TESTS)

EXTRA_DIST = autogen.sh