ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I${top_srcdir}/
# -I${top_builddir} 

lib_LTLIBRARIES = libmemtailor.la
# libmemtailordir = $(libdir)/

#libmemtailor_la_CFLAGS   = 
#libmemtailor_la_CXXFLAGS = 

libmemtailor_la_SOURCES  = memtailor/BufferPool.cpp memtailor/Arena.cpp memtailor/MemoryBlocks.cpp
nobase_include_HEADERS = memtailor.h memtailor/stdinc.h \
 memtailor/BufferPool.h memtailor/Arena.h memtailor/ArenaVector.h memtailor/MemoryBlocks.h


EXTRA_DIST = autogen.sh

SUBDIRS=test
TESTDIRS=test

