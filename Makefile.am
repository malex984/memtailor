ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I${top_srcdir}/include
# -I${top_builddir} 

lib_LTLIBRARIES = libmemtailor.la
# libmemtailordir = $(libdir)/

#libmemtailor_la_CFLAGS   = 
#libmemtailor_la_CXXFLAGS = 

libmemtailor_la_SOURCES  = src/BufferPool.cpp src/Arena.cpp src/MemoryBlocks.cpp

EXTRA_DIST = autogen.sh

SUBDIRS=include

# TESTDIRS=$(PACKAGES)
# TESTS=??
# check_PROGRAMS=$(TESTS)

