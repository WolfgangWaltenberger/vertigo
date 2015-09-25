AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ -I$(srcdir)/marlinrave/include/ $(LCIO_CFLAGS) $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) 

pkglib_LTLIBRARIES = evtgen_lcio.la
evtgen_lcio_la_LDFLAGS = -module $(LCIO_LIBS) $(MAINLIB)
evtgen_lcio_la_SOURCES = \
  src/BrahmsLcioToRaveObjects.cc marlinrave/src/L3LcioToRaveObjects.cc \
  src/L3LcioToVertigoObjects.cc src/LcioEventGenerator.cc \
  interface/BrahmsLcioToRaveObjects.h marlinrave/include/L3LcioToRaveObjects.h \
  interface/L3LcioToVertigoObjects.h interface/LcioEventGenerator.h
# src/L3TrackChecker.cc interface/L3TrackChecker.h
