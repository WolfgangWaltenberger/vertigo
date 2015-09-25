AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) $(DATAHARVESTER_CFLAGS)

pkglib_LTLIBRARIES = obs_BTagPrinter.la
obs_BTagPrinter_la_LDFLAGS = -module  $(MAINLIB)
obs_BTagPrinter_la_SOURCES = interface/BTagPrinter.h src/BTagPrinter.cc
