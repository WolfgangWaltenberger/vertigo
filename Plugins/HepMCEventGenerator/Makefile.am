AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) $(HEPMC_INCLUDES)

pkglib_LTLIBRARIES = evtgen_hepmc.la
evtgen_hepmc_la_LDFLAGS = -module $(MAINLIB) $(LDFLAGS_HEPMC) -lHepMCfio -lpythia6
evtgen_hepmc_la_SOURCES = src/HepMCEventGenerator.cc interface/HepMCEventGenerator.h  src/initPythia.cc interface/initPythia.h
