AM_CPPFLAGS = -I$(top_srcdir) $(ROOTCPPFLAGS) $(RAVE_CFLAGS)  $(BOOST_CPPFLAGS)

lib_LTLIBRARIES = libVertigoTools.la

libVertigoTools_la_SOURCES = \
	src/BayesianUpperLimit.cc \
	interface/BayesianUpperLimit.h \
	interface/LHEHelpers.h \
	src/LHEHelpers.cc \
	src/XSection.cc \
	interface/XSection.h 

libVertigoTools_la_LDFLAGS = $(ROOTLDFLAGS) $(ROOTLIBS)
