AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) $(DATAHARVESTER_CFLAGS) $(ROOTCPPFLAGS)

pkglib_LTLIBRARIES = obs_TrackParameterHistogrammer.la
obs_TrackParameterHistogrammer_la_LDFLAGS = -module $(MAINLIB)
obs_TrackParameterHistogrammer_la_SOURCES = interface/TrackParameterHistogrammer.h src/TrackParameterHistogrammer.cc
