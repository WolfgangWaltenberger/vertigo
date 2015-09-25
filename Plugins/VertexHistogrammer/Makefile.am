AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) $(DATAHARVESTER_CFLAGS)

pkglib_LTLIBRARIES = obs_VertexHistogrammer.la
obs_VertexHistogrammer_la_LDFLAGS = -module $(MAINLIB)
obs_VertexHistogrammer_la_SOURCES = interface/VertexHistogrammer.h src/VertexHistogrammer.cc
