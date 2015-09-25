AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(DATAHARVESTER_CFLAGS) $(BOOST_CPPFLAGS)

pkglib_LTLIBRARIES = evtgen_file.la
evtgen_file_la_LDFLAGS = -module $(MAINLIB)
evtgen_file_la_SOURCES = src/HarvesterEventGenerator.cc interface/HarvesterEventGenerator.h src/VertigoObjectReader.cc interface/VertigoObjectReader.h src/DelphiToRaveObjects.cc interface/DelphiToRaveObjects.h
