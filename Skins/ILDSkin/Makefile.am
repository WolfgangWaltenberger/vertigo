AM_CPPFLAGS = -I$(top_srcdir) $(RAVE_CFLAGS) $(BOOST_CPPFLAGS)

pkglib_LTLIBRARIES = skin_ild.la

skin_ild_la_LDFLAGS = -module $(MAINLIB)

skin_ild_la_SOURCES = \
	ILDPropagator/src/ILDPropagator.cc \
	ILDMagneticField/src/ILDMagneticField.cc \
	ILDFactory/src/ILDVertexFactory.cc \
	ILDBeamSpot/src/ILDBeamSpot.cc \
	ILDPropagator/interface/ILDPropagator.h \
	ILDMagneticField/interface/ILDMagneticField.h \
	ILDFactory/interface/ILDVertexFactory.h \
	ILDBeamSpot/interface/ILDBeamSpot.h

