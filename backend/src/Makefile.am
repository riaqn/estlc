COMMONDIR = $(top_srcdir)/../common
AM_CPPFLAGS = -I$(COMMONDIR)/include
AM_CPPFLAGS += `llvm-config --cppflags`
AM_CXXFLAGS = `llvm-config --cxxflags`
lib_LTLIBRARIES = libbackend.la
libbackend_la_SOURCES = codegen.cpp codegen.hpp exception.hpp exception.cpp env.hpp
