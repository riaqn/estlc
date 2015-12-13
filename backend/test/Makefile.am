COMMONDIR = $(top_srcdir)/../common
AM_CPPFLAGS = -I$(COMMONDIR)/include -I../include
AM_CPPFLAGS += `llvm-config --cppflags`
AM_CXXFLAGS = `llvm-config --cxxflags`

noinst_PROGRAMS = test0.out
test0_out_SOURCES = test0.cpp
test0_out_LDADD = $(COMMONDIR)/libcommon.la ../libbackend.la

