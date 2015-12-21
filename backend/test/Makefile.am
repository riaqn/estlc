COMMONDIR = $(top_srcdir)/../common
FRONTDIR = $(top_srcdir)/../frontend

WRAPPERDIR = $(top_srcdir)/wrapper

AM_CPPFLAGS += -I$(COMMONDIR)/include -I$(FRONTDIR) -I../include
AM_CPPFLAGS += `llvm-config --cppflags`
AM_CXXFLAGS += `llvm-config --cxxflags`

noinst_LTLIBRARIES = libdumb.la
libdumb_la_SOURCES = dumb.cxx

LDADD = $(COMMONDIR)/libcommon.la $(FRONTDIR)/libfrontend.la ../libbackend.la 

TEST_CPP = $(patsubst %.cpp,%,$(wildcard *.cpp))
TEST = $(TEST_CPP)
all : $(patsubst %,ll/%.out,$(TEST))

.SECONDARY :
run-% : ll/%.out
	$^
run-%-gdb : ll/%.out ll/%.s
	gdb $<

ll/%.s : ll/%.out
	objdump -D $< >$@

ll/%.out : ll/%.o $(WRAPPERDIR)/libwrapper.la
	libtool --tag=CXX --mode=link $(CXX) $(CXXFLAGS) -o $@ $^

ll/%.o : ll/%.ll
	llc -O0 -filetype=obj $<

ll/%.ll : %.out
	./$<  2>$@

%.out : %.o $(LDADD) main.o
	libtool --tag=CXX --mode=link $(CXX) $(CXXFLAGS) -o $@ $^

%.o : %.cpp
	$(CXXCOMPILE) -c $^


.PHONY : $(COMMONDIR)/libcommon.la $(FRONTDIR)/libfrontend.la ../libbackend.la $(WRAPPERDIR)/libwrapper.la
$(COMMONDIR)/libcommon.la :
	$(MAKE) -C ${@D} ${@F}
$(FRONTDIR)/libfrontend.la :
	$(MAKE) -C ${@D} ${@F}
$(WRAPPERDIR)/libwrapper.la:
	$(MAKE) -C ${@D} ${@F}
../libbackend.la :
	$(MAKE) -C ${@D} ${@F}
