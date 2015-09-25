AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) $(DATAHARVESTER_CFLAGS) 

pkglib_LTLIBRARIES = obs_KinematicsObserver.la
obs_KinematicsObserver_la_LDFLAGS = -module $(MAINLIB)
obs_KinematicsObserver_la_SOURCES = \
  interface/KinematicsObserver.h src/KinematicsObserver.cc
