AM_CPPFLAGS = -I$(top_srcdir) $(RAVE_CFLAGS) $(BOOST_CPPFLAGS)

pkglib_LTLIBRARIES = skin_tiny.la

$(MAINLIB):
	cd ../../Vertigo && make libVertigo.la

skin_tiny_la_LDFLAGS = -module
skin_tiny_la_LIBADD = $(MAINLIB)

skin_tiny_la_SOURCES = \
  ConstantMagneticField/interface/ConstantMagneticField.h \
  ConstantMagneticField/src/ConstantMagneticField.cc \
  TinyFactory/src/TinyVertexFactory.cc \
  TinyFactory/interface/TinyVertexFactory.h 
