AM_CPPFLAGS = -I$(top_srcdir) $(RAVE_CFLAGS) $(BOOST_CPPFLAGS)

pkglib_LTLIBRARIES = skin_ship.la

skin_ship_la_LDFLAGS = -module $(MAINLIB)

skin_ship_la_SOURCES = \
	SHiPFactory/src/SHiPVertexFactory.cc \
	SHiPBeamSpot/src/SHiPBeamSpot.cc \
	SHiPMagneticField/src/SHiPMagneticField.cc \
	SHiPFactory/interface/SHiPVertexFactory.h \
	SHiPBeamSpot/interface/SHiPBeamSpot.h \
	SHiPMagneticField/interface/SHiPMagneticField

