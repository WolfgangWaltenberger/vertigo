AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS)

pkglib_LTLIBRARIES = obs_EventPrinter.la
obs_EventPrinter_la_LDFLAGS = -module  $(MAINLIB)
obs_EventPrinter_la_SOURCES = interface/EventPrinter.h src/EventPrinter.cc interface/ObjectPrinter.h src/ObjectPrinter.cc
