# options passed to aclocal, which is a tool for making macroes visible to
# autoconf. We distribute local macro definitions in m4/, so we need to
# let aclocal know about that via -I m4.
ACLOCAL_AMFLAGS = -I m4

# Options passed to the C PreProcessor (CPP), NOT the C Plus Plus compiler.
AM_CPPFLAGS = -I${top_srcdir}/

# tell Libtool what the name of the library is.
lib_LTLIBRARIES = libmemtailor.la

# the sources that are built to make libmemtailor
libmemtailor_la_SOURCES = memtailor/BufferPool.cpp memtailor/Arena.cpp	\
  memtailor/MemoryBlocks.cpp

# The headers that libmemtailor installs.
# Normally, automake strips the path from the files when installing them,
# so memtailor/x.h gets installed as just x.h. The nobase_ prefix
# tells Automake to install memtailor/x.h into memtailor/x.h instead.
nobase_include_HEADERS = memtailor.h memtailor/stdinc.h				\
  memtailor/BufferPool.h memtailor/Arena.h memtailor/ArenaVector.h	\
  memtailor/MemoryBlocks.h

# When making a distribution file, Automake knows to include all files
# that are necessary to build the project. EXTRA_DIST specifies files
# to include beyond those used in the build process.
EXTRA_DIST = autogen.sh

# Tell Automake to recursively run Automake files in the listed directories
#SUBDIRS=test

# I don't know what this does
#TESTDIRS=test

# allow to download gtest in case of needing to run unit tests
GTEST_DIR = $(top_srcdir)/libs/
GTEST_TMP_DIR = $(top_srcdir)/libs/
GTEST_VERSION = 1.6.0
GTEST_DOWNLOAD_FILE = gtest-$(GTEST_VERSION).zip
gtest: $(GTEST_DIR)/gtest
$(GTEST_DIR)/gtest:
	mkdir -p $(GTEST_TMP_DIR);
	rm -rf $(GTEST_TMP_DIR)/$(GTEST_DOWNLOAD_FILE);
	rm -rf $(GTEST_TMP_DIR)/gtest-$(GTEST_VERSION);
	rm -rf $(GTEST_TMP_DIR)/gtest;
	(cd $(GTEST_TMP_DIR); \
	  wget http://googletest.googlecode.com/files/$(GTEST_DOWNLOAD_FILE); \
	  unzip $(GTEST_DOWNLOAD_FILE);)
	rm -rf $(GTEST_DIR)/gtest;
	mv $(GTEST_TMP_DIR)/gtest-$(GTEST_VERSION) $(GTEST_DIR)/gtest;
.PHONY: gtest

TESTS=test
check_PROGRAMS=$(TESTS)

test_CXXFLAGS= -I$(top_srcdir)/libs/gtest/include -I$(top_srcdir)/libs/gtest/
test_LDFLAGS= $(top_builddir)/libmemtailor.la
# test_LIBS=
test_SOURCES= test/ArenaTest.cpp test/BufferPoolTest.cpp			\
  test/MemoryBlocksTest.cpp test/testMain.cpp test/gtestInclude.cpp
