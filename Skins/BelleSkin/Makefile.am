AM_CPPFLAGS = -I$(top_srcdir) $(RAVE_CFLAGS) $(BOOST_CPPFLAGS)

pkglib_LTLIBRARIES = skin_belle.la

skin_belle_la_LDFLAGS = -module $(MAINLIB)

skin_belle_la_SOURCES = \
	BelleFactory/src/BelleVertexFactory.cc \
	BelleBeamSpot/src/BelleBeamSpot.cc \
	BelleFactory/interface/BelleVertexFactory.h \
	BelleBeamSpot/interface/BelleBeamSpot.h

