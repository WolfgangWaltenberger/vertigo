AM_CPPFLAGS = -I$(top_srcdir) $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) $(DATAHARVESTER_CFLAGS)

if HAVE_DATAHARVESTER
  AM_CPPFLAGS += -DWITH_HARVESTER
  libVertigoHarvest_la_LDFLAGS = $(DATAHARVESTER_LIBS)
endif

lib_LTLIBRARIES = libVertigoHarvest.la

libVertigoHarvest_la_SOURCES = \
	src/HarvesterConfigurationSetter.cc \
	interface/HarvesterConfigurationSetter.h \
	src/VertexHisto.cc \
	interface/VertexHisto.h \
	src/ParameterSetWriter.cc \
	interface/ParameterSetWriter.h \
	src/VertigoHarvest.cc \
	interface/VertigoHarvest.h \
	src/EventToTuple.cc \
	interface/ObserverTuples.h \
	src/ObserverTuples.cc \
	interface/EventToTuple.h
