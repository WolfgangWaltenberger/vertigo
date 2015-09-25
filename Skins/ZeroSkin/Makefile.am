AM_CPPFLAGS = -I$(top_srcdir) $(RAVE_CFLAGS) $(BOOST_CPPFLAGS)

pkglib_LTLIBRARIES = skin_zero.la

$(MAINLIB):
	cd ../../Vertigo && make libVertigo.la

skin_zero_la_LDFLAGS = -module
skin_zero_la_LIBADD = $(MAINLIB)

skin_zero_la_SOURCES = \
  ConstantMagneticField/interface/ConstantMagneticField.h \
  ConstantMagneticField/src/ConstantMagneticField.cc \
  ZeroFactory/src/ZeroVertexFactory.cc \
  ZeroFactory/interface/ZeroVertexFactory.h 
