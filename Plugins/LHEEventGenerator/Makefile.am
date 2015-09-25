AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS)

pkglib_LTLIBRARIES = evtgen_lhe.la
evtgen_lhe_la_LDFLAGS = -module $(MAINLIB)
evtgen_lhe_la_SOURCES = src/LHEEventGenerator.cc interface/LHEEventGenerator.h interface/LHEF.h
