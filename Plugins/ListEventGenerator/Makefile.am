AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) $(DATAHARVESTER_CFLAGS)

pkglib_LTLIBRARIES = evtgen_list.la
evtgen_list_la_LDFLAGS = -module $(MAINLIB)
evtgen_list_la_SOURCES = src/ListEventGenerator.cc interface/ListEventGenerator.h
