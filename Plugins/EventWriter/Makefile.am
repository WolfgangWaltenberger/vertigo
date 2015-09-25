AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) $(DATAHARVESTER_CFLAGS)

pkglib_LTLIBRARIES = obs_EventWriter.la
obs_EventWriter_la_LDFLAGS = -module $(MAINLIB)
obs_EventWriter_la_SOURCES = interface/EventWriter.h src/EventWriter.cc interface/RaveObjectWriter.h src/RaveObjectWriter.cc
