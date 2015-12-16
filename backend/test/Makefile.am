COMMONDIR = $(top_srcdir)/../common
WRAPPERDIR = $(top_srcdir)/wrapper

AM_CPPFLAGS += -I$(COMMONDIR)/include -I../include
AM_CPPFLAGS += `llvm-config --cppflags`
AM_CXXFLAGS += `llvm-config --cxxflags`

LDADD = $(COMMONDIR)/libcommon.la ../libbackend.la

noinst_PROGRAMS = id.out cdr.out
id_out_SOURCES = id.cpp
cdr_out_SOURCES = cdr.cpp

all : $(patsubst %.cpp,ll/%.out,$(wildcard *.cpp))

ll/%.ll : %.out
	./$< >$@ 2>&1
ll/%.o : ll/%.ll
	llc -O0 -filetype=obj $<
ll/%.out : ll/%.o $(WRAPPERDIR)/libmain.la
	libtool --tag=CXX --mode=link $(CXX) $(CXXFLAGS) -o $@ $^

.PHONY : $(COMMONDIR)/libcommon.la ../libbackend.la $(WRAPPERDIR)/libmain.la
$(COMMONDIR)/libcommon.la :
	$(MAKE) -C $(COMMONDIR) libcommon.la
$(WRAPPERDIR)/libmain.la :
	$(MAKE) -C $(WRAPPERDIR) libmain.la
../libbackend.la :
	$(MAKE) -C .. libbackend.la
