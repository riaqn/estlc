ACLOCAL_AMFLAGS = -I m4

SUBDIRS = include src .
noinst_LTLIBRARIES = libcommon.la
libcommon_la_SOURCES = 
libcommon_la_LIBADD = src/libcommon.la

.PHONY :
src/libcommon.la :
	$(MAKE) -C $(@D) $(@F)
