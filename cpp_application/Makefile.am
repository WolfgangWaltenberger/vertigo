AM_CPPFLAGS = -I$(top_srcdir) $(RAVE_CFLAGS) -DVERTIGO_PLUGIN_DIR=\"$(pkglibdir)\" $(BOOST_CPPFLAGS) $(DATAHARVESTER_CFLAGS)

bin_PROGRAMS = propagatorTest

propagatorTest_SOURCES = propagatorTest.cc
propagatorTest_LDADD = ../Vertigo/libVertigo.la ../Analysis/libVertigoAnalysis.la ../VertigoHarvest/libVertigoHarvest.la
propagatorTest_LDFLAGS = $(RAVE_LIBS) -ldl  $(BOOST_FILESYSTEM_LIB) $(BOOST_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_SYSTEM_LIB)

../Vertigo/libVertigo.la:
	cd ../Vertigo && ${MAKE} libVertigo.la
