AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ -I$(srcdir)/marlinrave/include/ $(LCIO_CFLAGS) $(RAVE_CFLAGS) $(BOOST_CPPFLAGS)

pkglib_LTLIBRARIES = obs_LcioWriter.la
obs_LcioWriter_la_LDFLAGS = -module $(MAINLIB) $(LCIO_LIBS)
obs_LcioWriter_la_SOURCES = \
  interface/LcioWriter.h src/LcioWriter.cc interface/VertigoToL3LcioObjects.h src/VertigoToL3LcioObjects.cc marlinrave/include/RaveToL3LcioObjects.h marlinrave/src/RaveToL3LcioObjects.cc
