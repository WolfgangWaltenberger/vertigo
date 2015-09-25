AM_CPPFLAGS = -I$(top_srcdir) $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) -DVERTIGO_PLUGIN_DIR=\"$(pkglibdir)\"

lib_LTLIBRARIES = libVertigoAnalysis.la

EXTRA_DIST = \
	Observer/skel/Skeleton.cc \
	Observer/skel/Skeleton.h 

libVertigoAnalysis_la_SOURCES = \
	Observer/interface/ObserverBuilder.h \
	Observer/interface/ObserverManager.h \
	Observer/interface/Observer.h \
	Observer/src/ObserverManager.cc \
	Observer/src/Observer.cc \
	Score/interface/Score.h \
	Score/interface/SimpleScore.h \
	Score/src/SimpleScore.cc
