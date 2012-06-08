ACLOCAL_AMFLAGS = -I ../m4

AM_CPPFLAGS = -I${top_srcdir}/

TESTS=test
check_PROGRAMS=$(TESTS)

test_LDFLAGS=-lgtest ${top_builddir}/libmemtailor.la
# test_LIBS=
test_SOURCES= ArenaTest.cpp BufferPoolTest.cpp MemoryBlocksTest.cpp testMain.cpp
