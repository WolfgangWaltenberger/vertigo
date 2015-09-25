AM_CPPFLAGS = -I$(top_srcdir) $(RAVE_CFLAGS) -DVERTIGO_PLUGIN_DIR=\"$(pkglibdir)\" $(BOOST_CPPFLAGS) $(DATAHARVESTER_CFLAGS)

bin_PROGRAMS = vertigo

vertigo_SOURCES = main/main.cpp main/sigsegv.c
vertigo_LDADD = libVertigo.la \
	../VertigoHarvest/libVertigoHarvest.la \
	../Environment/libVertigoEnvironment.la \
	../tools/libVertigoTools.la \
	../Analysis/libVertigoAnalysis.la
vertigo_LDFLAGS = $(RAVE_LIBS) -ldl  $(BOOST_FILESYSTEM_LIB) $(BOOST_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_SYSTEM_LIB) 

../Environment/libVertigoEnvironment.la:
	cd ../Environment && make libVertigoEnvironment.la

../Analysis/libVertigoAnalysis.la:
	cd ../Analysis && make libVertigoAnalysis.la

../VertigoHarvest/libVertigoHarvest.la:
	cd ../VertigoHarvest && make libVertigoHarvest.la

../tools/libVertigoTools.la:
	cd ../tools && make libVertigoTools.la


##############################################

lib_LTLIBRARIES = libVertigo.la

libVertigo_la_LDFLAGS = $(RAVE_LIBS) -ldl $(CLHEPLIBPATH) $(BOOST_LDFLAGS) $(BOOST_FILESYSTEM_LIB) $(BOOST_PROGRAM_OPTIONS_LIB)

libVertigo_la_LIBADD = -l$(CLHEPLIBNAME) \
	../Environment/libVertigoEnvironment.la \
	../Analysis/libVertigoAnalysis.la \
	../VertigoHarvest/libVertigoHarvest.la
libVertigo_la_SOURCES = \
  main/Version.cc \
	Event/src/EventCombiner.cc \
	Event/src/Event.cc \
	Event/src/BasicEvent.cc \
	TrackAssociation/src/TrackDistanceByChiSquared.cc \
	TrackAssociation/src/TrackAssociatorByPulls.cc \
	TrackAssociation/src/TrackDistanceByPulls.cc \
	TrackAssociation/src/TrivialTrackDistance.cc \
	TrackSmearing/src/ElectronSmearer.cc \
	TrackSmearing/src/GaussianRecTrackSmearer.cc \
	TrackSmearing/src/RecTrackSmearer.cc \
	TrackSmearing/src/NComponentRecTrackSmearer.cc \
	TrackSmearing/src/TwoGaussianRecTrackSmearer.cc \
	Converters/src/VertigoToCmsObjects.cc \
	VertexAssociation/src/VertexAssociatorByDistance.cc \
	VertexAssociation/src/SimVertexShareTracks.cc \
	VertexAssociation/src/VertexAssociatorByTracks.cc \
	VertexAssociation/src/BVertexAssociator.cc \
	VertexAssociation/src/VertexDistance.cc \
	EventLoop/src/EventLoop.cc \
	EventLoop/src/LoopThread.cc \
	EventLoop/src/VertexMetaFactory.cc \
	EventLoop/src/LoopSettings.cc \
	EventGeneration/src/GeneratorManager.cc \
	EventGeneration/src/EventFactory.cc

noinst_HEADERS = \
  main/Version.h \
	Event/interface/EventCombiner.h \
	Event/interface/Event.h \
	Event/interface/BasicEvent.h \
	TrackAssociation/interface/TrackDistanceByPulls.h \
	TrackAssociation/interface/TrivialTrackDistance.h \
	TrackAssociation/interface/TrackAssociator.h \
	TrackAssociation/interface/TrackDistance.h \
	TrackAssociation/interface/TrackDistanceByChiSquared.h \
	TrackAssociation/interface/TrackAssociatorByPulls.h \
	TrackSmearing/interface/ElectronSmearer.h \
	TrackSmearing/interface/GaussianRecTrackSmearer.h \
	TrackSmearing/interface/NComponentRecTrackSmearer.h \
	TrackSmearing/interface/TwoGaussianRecTrackSmearer.h \
	TrackSmearing/interface/RecTrackSmearer.h \
	Converters/interface/VertigoToCmsObjects.h \
	VertexAssociation/interface/VertexAssociatorByDistance.h \
	VertexAssociation/interface/SimVertexShareTracks.h \
	VertexAssociation/interface/VertexAssociator.h \
	VertexAssociation/interface/VertexAssociatorByTracks.h \
	VertexAssociation/interface/BVertexAssociator.h \
	VertexAssociation/interface/VertexDistance.h \
	EventLoop/interface/LoopSettings.h \
	EventLoop/interface/EventLoop.h \
	EventLoop/interface/LoopThread.h \
	EventLoop/interface/VertexMetaFactory.h \
	EventLoop/interface/VertexFactoryBuilder.h \
	EventGeneration/interface/EventFactory.h \
	EventGeneration/interface/GeneratorBuilder.h \
	EventGeneration/interface/GeneratorManager.h \
	EventGeneration/interface/EventGenerator.h 
