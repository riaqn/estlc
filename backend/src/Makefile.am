COMMONDIR = $(top_srcdir)/../common
AM_CPPFLAGS += -I$(COMMONDIR)/include -I../include
AM_CPPFLAGS += `llvm-config --cppflags`
AM_CXXFLAGS += `llvm-config --cxxflags`
noinst_LTLIBRARIES = libbackend.la
libbackend_la_SOURCES = codegen.cpp exception.cpp
libbackend_la_LDFLAGS = `llvm-config --ldflags --libs`
