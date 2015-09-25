AM_CPPFLAGS = -I$(top_srcdir)/ -I$(srcdir)/interface/ $(RAVE_CFLAGS) $(BOOST_CPPFLAGS)

pkglib_LTLIBRARIES = evtgen_gun.la
evtgen_gun_la_LDFLAGS = -module  $(MAINLIB)
evtgen_gun_la_SOURCES = interface/ConfigurableVertexGun.h interface/VertexGunPropagator.h interface/DirectionalParticleGun.h src/ConfigurableVertexGun.cc interface/DirectionalVertexGun.h src/DirectionalParticleGun.cc src/DirectionalVertexGun.cc interface/GunEventGenerator.h interface/JetVertexGun.h src/GunEventGenerator.cc interface/MultipleVertexGun.h src/JetVertexGun.cc interface/ParticleGun.h src/MultipleVertexGun.cc src/VertexGunFromParticleGun.cc interface/VertexGunFromParticleGun.h src/VertexGunPropagator.cc interface/VertexGun.h

#if HAVE_KINEMATICS
  evtgen_gun_la_SOURCES += interface/KinematicsParticleGun.h src/KinematicsParticleGun.cc interface/KinematicsVertexGun.h src/KinematicsVertexGun.cc
#endif

