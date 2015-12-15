COMMONDIR = $(top_srcdir)/../common
WRAPPERDIR = $(top_srcdir)/wrapper
AM_CPPFLAGS += -I$(COMMONDIR)/include -I../include
AM_CPPFLAGS += `llvm-config --cppflags`
AM_CXXFLAGS += `llvm-config --cxxflags`

noinst_PROGRAMS = test0.out
test0_out_SOURCES = test0.cpp
test0_out_LDADD = $(COMMONDIR)/libcommon.la ../libbackend.la

ll/%.ll : %.out
	./$< >$@ 2>&1
ll/%.o : ll/%.ll
	llc -O0 -filetype=obj $<

noinst_PROGRAMS += ll/test0.out
ll_test0_out_SOURCES = dumb.cpp
ll_test0_out_LDADD = $(WRAPPERDIR)/libmain.la ll/test0.o
