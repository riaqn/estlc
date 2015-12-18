ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS += -I$(top_srcdir)/../include


SUBDIRS = src . test

noinst_LTLIBRARIES = libbackend.la
libbackend_la_SOURCES = 
libbackend_la_LIBADD = src/libbackend.la

.PHONY: src/libbackend.la
src/libbackend.la :
	$(MAKE) -C src libbackend.la
