AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) $(DATAHARVESTER_CFLAGS) 

pkglib_LTLIBRARIES = obs_LHEObserver.la
obs_LHEObserver_la_LDFLAGS = -module $(MAINLIB)
obs_LHEObserver_la_SOURCES = interface/LHEObserver.h src/LHEObserver.cc
