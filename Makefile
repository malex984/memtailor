# ***** Variables

rawSources := BufferPool.cpp Arena.cpp MemoryBlocks.cpp
rawTests := testMain.cpp BufferPoolTest.cpp MemoryBlocksTest.cpp	\
  ArenaTest.cpp

testSources = $(patsubst %.cpp, src/test/%.cpp, $(rawTests))
testObjs    = $(patsubst %.cpp, $(outdir)test/%.o, $(rawTests))
testProgram = runTests

ifndef CXX
  CXX = "g++"
endif

cflags = $(CFLAGS) $(CPPFLAGS) -Wall -pedantic -I $(GMP_INC_DIR) \
         -Wno-uninitialized -Wno-unused-parameter \
         -isystem $(GTEST_DIR)/include -Isrc/
library = libspecalloc.a
GTEST_VERSION = 1.6.0
GTEST_DIR = bin/gtest/

ifndef MODE
 MODE=release
endif

ifndef ldflags
  ldflags = $(cflags) $(LDFLAGS) -lgmpxx -lgmp
endif

MATCH=false
ifeq ($(MODE), release)
  outdir = bin/release/
  cflags += -O2
  MATCH=true
endif
ifeq ($(MODE), debug)
  outdir = bin/debug/
  cflags += -g -D DEBUG -fno-inline -Werror -Wextra -Wno-uninitialized \
            -Wno-unused-parameter
  MATCH=true
endif
ifeq ($(MODE), shared)
  outdir = bin/shared/
  cflags += -O2 -fPIC
  library = libspecalloc.so
  MATCH=true
endif
ifeq ($(MODE), profile)
  outdir = bin/profile/
  cflags += -g -pg -O2 -D PROFILE
  ldflags += -pg
  MATCH=true
  benchArgs = _profile $(FROBBYARGS)
endif
ifeq ($(MODE), analysis)
  rawSources := $(rawSources) $(rawTests)
  outdir = bin/analysis/
  cflags += -Wextra -fsyntax-only -O1 -Wfloat-equal -Wundef				\
  -Wno-endif-labels -Wshadow -Wlarger-than-1000 -Wpointer-arith			\
  -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wsign-compare	\
  -Waggregate-return -Wmissing-noreturn -Wmissing-format-attribute		\
  -Wno-multichar -Wno-deprecated-declarations -Wpacked					\
  -Wno-redundant-decls -Wunreachable-code -Winline						\
  -Wno-invalid-offsetof -Winvalid-pch -Wlong-long						\
  -Wdisabled-optimization -D DEBUG -Werror
  MATCH=true
endif

ifeq ($(MATCH), false)
  $(error Unknown value of MODE: "$(MODE)")
endif

sources = $(patsubst %.cpp, src/%.cpp, $(rawSources))
objs    = $(patsubst %.cpp, $(outdir)%.o, $(rawSources))

# ***** Compilation

.PHONY: all clean test library clear fixspace gtest

all: bin/$(library)

# Link object files into library
library: bin/$(library)
bin/$(library): $(objs)
	@mkdir -p bin/
	rm -f bin/$(library)
ifeq ($(MODE), shared)
	$(CXX) -shared -o bin/$(library) $(ldflags) \
	  $(patsubst $(outdir)main.o,,$(objs))
else
	ar crs bin/$(library) $(patsubst $(outdir)main.o,,$(objs))
endif

# Create precompiled header
$(outdir)stdinc.h.gch: src/stdinc.h
	@echo Creating precompiled header $<
	@mkdir -p $(dir $@)
	@$(CXX) ${cflags} src/stdinc.h -o $@

# Compile object files.
$(outdir)%.o: src/%.cpp $(outdir)stdinc.h.gch
	@echo Compiling $<
	@mkdir -p $(dir $@)
	$(CXX) ${cflags} -MMD -c $< -o $@ -include $(outdir)stdinc.h
	@sed -e 's/.*://' -e 's/\\$$//' < $(@:.o=.d) | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(@:.o=.d)
ifeq ($(MODE), analysis) # to allow dependency analysis to work
	  echo > $@
endif

-include $(objs:.o=.d)
-include $(testObjs:.o=.d)


# ***** Miscellaneous

clean: tidy
	rm -rf bin

tidy:
	find .|grep -x ".*~\|.*/\#.*\#|.*\.stackdump\|gmon\.out\|.*\.orig\|.*/core\|core"|xargs rm -f

# Fixes various white space related issues.
fixspace:
	find src/ -type f|xargs ./fixspace;

##### testing via google gtest #####

$(outdir)gtest.a: $(GTEST_DIR)/src/gtest-all.cc
	$(CXX) $(cflags) -I$(GTEST_DIR) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $(outdir)gtest-all.o
	$(AR) $(ARFLAGS) $@ $(outdir)gtest-all.o

gtest: bin/gtest
bin/gtest:
	@mkdir -p bin/
	(cd bin/; wget http://googletest.googlecode.com/files/gtest-$(GTEST_VERSION).zip)
	(cd bin; unzip gtest-$(GTEST_VERSION).zip)
	(cd bin; rm -rf gtest)
	(cd bin; ln -s gtest-$(GTEST_VERSION) gtest)

# Make symbolic link to test program from bin/
bin/$(testprogram): $(outdir)$(program)
	@mkdir -p bin
ifneq ($(MODE), analysis)
	cd bin; rm -f $(program); ln -s ../$(outdir)$(program) $(program); cd ..
endif

# Link object files into executable
$(outdir)$(testProgram): $(testObjs) $(objs) $(outdir)gtest.a
	@mkdir -p $(dir $@)
ifeq ($(MODE), analysis)
	echo > $@
endif
ifneq ($(MODE), analysis)
	$(CXX) $(testObjs) $(objs) $(ldflags) $(outdir)gtest.a -o $@
	if [ -f $@.exe ]; then \
      mv -f $@.exe $@; \
	fi
endif

bin/$(testProgram): $(outdir)$(testProgram)
	@mkdir -p bin
ifneq ($(MODE), analysis)
	cd bin; rm -f $(testProgram); ln -s ../$(outdir)$(testProgram) $(testProgram); cd ..
endif
test: bin/$(testProgram)
ifneq ($(MODE), analysis)
	bin/runTests
endif
