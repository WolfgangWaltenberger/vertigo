AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS)

pkglib_LTLIBRARIES = obs_ObjectCoder.la
obs_ObjectCoder_la_LDFLAGS = -module  $(MAINLIB)
obs_ObjectCoder_la_SOURCES = interface/ObjectCoder.h src/ObjectCoder.cc
