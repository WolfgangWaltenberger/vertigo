AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) $(DATAHARVESTER_CFLAGS)

pkglib_LTLIBRARIES = obs_EventHistogrammer.la
obs_EventHistogrammer_la_LDFLAGS = -module  $(MAINLIB)
obs_EventHistogrammer_la_SOURCES = interface/EventHistogrammer.h src/EventHistogrammer.cc
