AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) $(DATAHARVESTER_CFLAGS)

pkglib_LTLIBRARIES = obs_FindingObserver.la
obs_FindingObserver_la_LDFLAGS = -module $(MAINLIB)
obs_FindingObserver_la_SOURCES = interface/FindingObserver.h src/FindingObserver.cc
