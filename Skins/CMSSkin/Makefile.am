AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/Skins/CMSSkin $(RAVE_CFLAGS) $(BOOST_CPPFLAGS)

pkglib_LTLIBRARIES = skin_cms.la

skin_cms_la_LDFLAGS = -module $(MAINLIB)

skin_cms_la_SOURCES = \
	cmssw/src/BFit.cc \
	cmssw/src/TkBfield.cc \
	cmssw/src/OAE85lParametrizedMagneticField.cc \
	cmssw/src/PolyFit2DParametrizedMagneticField.cc \
	cmssw/src/OAEParametrizedMagneticField.cc \
	cmssw/src/rz_poly.cc \
	cmssw/src/TkBfield.h \
	cmssw/src/OAE85lParametrizedMagneticField.h \
	cmssw/src/PolyFit2DParametrizedMagneticField.h \
	cmssw/src/rz_poly.h \
	cmssw/src/BFit.h \
	cmssw/src/OAEParametrizedMagneticField.h \
	CMSPropagator/src/CMSPropagator.cc \
	CMSBeamSpot/src/CMSBeamSpot.cc \
	CMSMagneticField/src/CMSMagneticField.cc \
	CMSFactory/src/CMSVertexFactory.cc\
	CMSPropagator/interface/CMSPropagator.h \
	CMSBeamSpot/interface/CMSBeamSpot.h \
	CMSFactory/interface/CMSVertexFactory.h \
	CMSMagneticField/interface/CMSMagneticField.h
