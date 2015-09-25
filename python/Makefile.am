if WITH_PYTHON
BUILT_SOURCES = $(srcdir)/vertigo.cc
SWIG_SOURCES = $(top_srcdir)/vertigo.i
DESTDIR="/"
## DESTDIR=/ #$(pkgpythondir)

SWIG_OUTPUT=vertigo.h

${SWIG_OUTPUT}: ${SWIG_SOURCES}

EXTRA_DIST=${SWIG_OUTPUT}

pkgpython_PYTHON = vertigo.py __init__.py
pkgpyexec_LTLIBRARIES = _vertigo.la

_vertigo_la_SOURCES = $(srcdir)/vertigo.cc $(SWIG_SOURCES)
_vertigo_la_CPPFLAGS = $(SWIG_PYTHON_CPPFLAGS) -I$(top_srcdir) $(RAVE_CFLAGS) $(BOOST_CPPFLAGS)
_vertigo_la_LDFLAGS = -module -lpython${PYTHON_VERSION}
_vertigo_la_LIBADD = ../Vertigo/libVertigo.la ../tools/libVertigoTools.la

helpme:
	mkdir -p .deps
	touch .deps/_vertigo_la-vertigo.Plo

.deps/_vertigo_la-vertigo.Plo: helpme

deps:
	$(SWIG) -I$(top_srcdir) -I$(oldincludedir) $(RAVE_CFLAGS) -M $(top_srcdir)/vertigo.i > vertigo.d

$(srcdir)/vertigo.cc $(srcdir)/vertigo.py: $(top_srcdir)/vertigo.i vertigo.h $(top_srcdir)/Vertigo/EventGeneration/interface/EventFactory.h $(top_srcdir)/Environment/MCObjects/interface/SimVertex.h
	$(SWIG) -w401 -w319 -w473 $(SWIG_PYTHON_OPT) -I$(top_srcdir) -I$(oldincludedir) $(RAVE_CFLAGS) -o $(srcdir)/vertigo.cc $<

CLEANFILES = $(srcdir)/vertigo.cc $(srcdir)/vertigo.py ${SWIG_OUTPUT} $(srcdir)/_vertigo.la $(srcdir)/_vertigo_la-vertigo.lo $(srcdir)/_vertigo_la-vertigo.o 
endif
