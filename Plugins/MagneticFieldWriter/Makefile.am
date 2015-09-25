AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) $(DATAHARVESTER_CFLAGS)

pkglib_LTLIBRARIES = obs_MagneticFieldWriter.la
obs_MagneticFieldWriter_la_LDFLAGS = -module $(MAINLIB)
obs_MagneticFieldWriter_la_SOURCES = interface/MagneticFieldWriter.h src/MagneticFieldWriter.cc
