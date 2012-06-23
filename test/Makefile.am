ACLOCAL_AMFLAGS = -I ../m4

AM_CPPFLAGS = -I${top_srcdir}/

TESTS=test
check_PROGRAMS=$(TESTS)

test_CXXFLAGS= -I../libs/gtest/include
test_LDFLAGS= ${top_builddir}/libmemtailor.la
# test_LIBS=
test_SOURCES= ArenaTest.cpp BufferPoolTest.cpp MemoryBlocksTest.cpp	\
  testMain.cpp gtestInclude.cpp

