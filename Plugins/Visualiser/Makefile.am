AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) $(DATAHARVESTER_CFLAGS) $(RAVEVIS_CFLAGS)

pkglib_LTLIBRARIES = obs_Visualiser.la
obs_Visualiser_la_LDFLAGS = -module $(MAINLIB) $(RAVEVIS_LIBS) ## $(DATAHARVESTER_LIBS) $(RAVEVIS_LIBS)
obs_Visualiser_la_SOURCES = interface/Visualiser.h src/Visualiser.cc
