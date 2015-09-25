AM_CPPFLAGS = -I$(top_srcdir) $(RAVE_CFLAGS) $(BOOST_CPPFLAGS)

pkglib_LTLIBRARIES = skin_dummy.la

$(MAINLIB):
	cd ../../Vertigo && make libVertigo.la

skin_dummy_la_LDFLAGS = -module
skin_dummy_la_LIBADD = $(MAINLIB)

skin_dummy_la_SOURCES = \
  ConstantMagneticField/interface/ConstantMagneticField.h \
  ConstantMagneticField/src/ConstantMagneticField.cc \
  DummyFactory/src/DummyVertexFactory.cc \
  DummyFactory/interface/DummyVertexFactory.h
