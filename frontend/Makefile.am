COMMONDIR=$(top_srcdir)/../common
AM_CPPFLAGS += -I$(COMMONDIR)/include
noinst_LTLIBRARIES = libfrontend.la
libfrontend_la_SOURCES = lexicalAnalyzer.h  syntaxAnalyzer.h  tokenStream.h \
lexicalAnalyzer.cpp  myException.h      syntaxAnalyzer.cpp  tokenStream.cpp
libfrontend_la_LIBADD = $(COMMONDIR)/libcommon.la
.PHONY: $(COMMONDIR)/libcommon.la
$(COMMONDIR)/libcommon.la:
	$(MAKE) -C $(@D) $(@F)
