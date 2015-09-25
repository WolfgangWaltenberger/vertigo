AM_CPPFLAGS = -I$(top_srcdir) $(RAVE_CFLAGS) $(BOOST_CPPFLAGS)

pkglib_LTLIBRARIES = skin_sid.la

skin_sid_la_LDFLAGS = -module $(MAINLIB)

skin_sid_la_SOURCES = \
	SIDMagneticField/src/SIDMagneticField.cc \
	SIDBeamSpot/src/SIDBeamSpot.cc \
	SIDFactory/src/SIDVertexFactory.cc \
	SIDPropagator/src/SIDPropagator.cc \
	SIDMagneticField/interface/SIDMagneticField.h \
	SIDPropagator/interface/SIDPropagator.h \
	SIDFactory/interface/SIDVertexFactory.h \
	SIDBeamSpot/interface/SIDBeamSpot.h
