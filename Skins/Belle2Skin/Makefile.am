AM_CPPFLAGS = -I$(top_srcdir) $(RAVE_CFLAGS) $(BOOST_CPPFLAGS)

pkglib_LTLIBRARIES = skin_belle2.la

skin_belle2_la_LDFLAGS = -module $(MAINLIB)

skin_belle2_la_SOURCES = \
	Belle2Factory/src/Belle2VertexFactory.cc \
	Belle2BeamSpot/src/Belle2BeamSpot.cc \
	Belle2Factory/interface/Belle2VertexFactory.h \
	Belle2BeamSpot/interface/Belle2BeamSpot.h

