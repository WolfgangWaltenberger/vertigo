AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS) $(ROOTCPPFLAGS)

pkglib_LTLIBRARIES = evtgen_cms.la
evtgen_cms_la_LDFLAGS = -module $(ROOTLDFLAGS) $(ROOTLIBS) $(MAINLIB)
evtgen_cms_la_SOURCES = src/CmsEventGenerator.cc interface/CmsEventGenerator.h \
						src/CmsBeamSpotBuilder.cc interface/CmsBeamSpotBuilder.h \
						src/SecondaryVertexTagInfoPrinter.cc interface/SecondaryVertexTagInfoPrinter.h
