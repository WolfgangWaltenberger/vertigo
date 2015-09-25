AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) $(DATAHARVESTER_CFLAGS)

pkglib_LTLIBRARIES = obs_ParameterScanner.la
obs_ParameterScanner_la_LDFLAGS = -module $(MAINLIB)
obs_ParameterScanner_la_SOURCES = interface/ParameterScanner.h src/ParameterScanner.cc
