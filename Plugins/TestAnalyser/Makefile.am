AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) $(DATAHARVESTER_CFLAGS) $(ROOTCPPFLAGS)

pkglib_LTLIBRARIES = obs_TestAnalyser.la
obs_TestAnalyser_la_LDFLAGS = -module  $(MAINLIB)
obs_TestAnalyser_la_SOURCES = interface/TestAnalyser.h src/TestAnalyser.cc
