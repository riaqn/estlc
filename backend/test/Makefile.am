COMMONDIR = $(top_srcdir)/../common
WRAPPERDIR = $(top_srcdir)/wrapper

AM_CPPFLAGS += -I$(COMMONDIR)/include -I../include
AM_CPPFLAGS += `llvm-config --cppflags`
AM_CXXFLAGS += `llvm-config --cxxflags`

noinst_LTLIBRARIES = libdumb.la
libdumb_la_SOURCES = dumb.cxx

LDADD = $(COMMONDIR)/libcommon.la ../libbackend.la

TEST_CPP = $(patsubst %.cpp,%,$(wildcard *.cpp))
TEST = $(TEST_CPP)
all : $(patsubst %,ll/%.ll,$(TEST))
all : $(patsubst %,ll/%.out,$(TEST))

ll/%.out : ll/%.o $(WRAPPERDIR)/libmain.la
	libtool --tag=CXX --mode=link $(CXX) $(CXXFLAGS) -o $@ $^

ll/%.o : ll/%.ll
	llc -O0 -filetype=obj $<

ll/%.ll : %.out
	./$< >$@ 2>&1

%.out : %.o $(LDADD)
	libtool --tag=CXX --mode=link $(CXX) $(CXXFLAGS) -o $@ $^

%.o : %.cpp
	$(CXXCOMPILE) -c $^


.PHONY : $(COMMONDIR)/libcommon.la ../libbackend.la $(WRAPPERDIR)/libmain.la
$(COMMONDIR)/libcommon.la :
	$(MAKE) -C ${@D} ${@F}
$(WRAPPERDIR)/libmain.la :
	$(MAKE) -C ${@D} ${@F}
../libbackend.la :
	$(MAKE) -C ${@D} ${@F}
