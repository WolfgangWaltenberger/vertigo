#!/usr/bin/python

print "running vertigo/tests/ktest2.py ..."

from vertigo import LoopSettings, EventFactory, RaveConstantMagneticField, RaveVacuumPropagator, RaveKTransientTrackParticle, RaveKTreeFactory, RavePerigeeCovariance6D, RaveCovariance7D, RavePerigeeParameters6D, RaveVector7D, RavePoint3D

LoopSettings.Instance().setVerbosity(100)

magneticfield = RaveConstantMagneticField( 0., 0., 4. )
eventfactory  = EventFactory ( "gun:kinematics" )
treefactory   = RaveKTreeFactory( magneticfield ) # This is only to set the magnetic field singleton

print "Shooting kinematics gun ..."
event = eventfactory.next()

print "Using the first particle as starting point:"
particle = event.particles()[0]

simparams = particle.fullstate()
inputparams = RaveVector7D( simparams.x(),  simparams.y(),  simparams.z(),
                            simparams.px(), simparams.py(), simparams.pz(), simparams.m() )

simerror = particle.fullerror()
inputerror = RaveCovariance7D(simerror.dxx(), simerror.dxy(), simerror.dxz(),
                              simerror.dyy(), simerror.dyz(), simerror.dzz(),
                              simerror.dxpx(), simerror.dxpy(), simerror.dxpz(),
                              simerror.dypx(), simerror.dypy(), simerror.dypz(),
                              simerror.dzpx(), simerror.dzpy(), simerror.dzpz(),
                              simerror.dpxpx(), simerror.dpxpy(), simerror.dpxpz(),
                              simerror.dpypy(), simerror.dpypz(), simerror.dpzpz(),
                              simerror.dxm(), simerror.dym(), simerror.dzm(),
                              simerror.dpxm(), simerror.dpym(), simerror.dpzm(),
                              simerror.dmm())
#inputerror = RaveCovariance7D(1., 0., 0.,
#                              1., 0., 0.,
#                              0.,0.,0.,0.,0.,0.,0.,0.,0.,
#                              0.,0.,0.,0.,0.,0.,
#                              0.,0.,0.,0.,0.,0.,0.)
inputparticle = RaveKTransientTrackParticle(inputparams, inputerror, particle.charge(),
                                            particle.chi2(), particle.ndof())

print " parameters: ", inputparticle.fullstate()
errortext = str(inputparticle.fullerror())
errortext = errortext.replace("\n","\n   ")
print " covariance: \n  ", errortext

print " perigee parameters: ", inputparticle.fullPerigeeParameters()
errortext = str(inputparticle.fullPerigeeCovariance())
errortext = errortext.replace("\n","\n   ")
print " perigee covariance: \n  ", errortext

print "Now we propagate the particle to the point closest to the z-axis:"
newparticle = inputparticle.closestToTransversePoint()

print " parameters: ", newparticle.fullstate()
errortext = str(newparticle.fullerror())
errortext = errortext.replace("\n","\n   ")
print " covariance: \n  ", errortext

print " perigee parameters: ", newparticle.fullPerigeeParameters()
errortext = str(newparticle.fullPerigeeCovariance())
errortext = errortext.replace("\n","\n   ")
print " perigee covariance: \n  ", errortext

print "Now we propagate the particle once again to the point closest to the z-axis:"
newparticle = newparticle.closestToTransversePoint()

print " parameters: ", newparticle.fullstate()
errortext = str(newparticle.fullerror())
errortext = errortext.replace("\n","\n   ")
print " covariance: \n  ", errortext

print " perigee parameters: ", newparticle.fullPerigeeParameters()
errortext = str(newparticle.fullPerigeeCovariance())
errortext = errortext.replace("\n","\n   ")
print " perigee covariance: \n  ", errortext

print "Now we reinitialize this particle with its own perigee numbers:"

op = newparticle.fullPerigeeParameters()
np = RavePerigeeParameters6D(op.rho(), op.theta(), op.phip(), op.epsilon(), op.zp(), op.mass())

oe = newparticle.fullPerigeeCovariance()
ne = RavePerigeeCovariance6D(oe.drr(), oe.drt(), oe.drp(), oe.dre(), oe.drz(), oe.drm(), \
                                       oe.dtt(), oe.dtp(), oe.dte(), oe.dtz(), oe.dtm(), \
                                                 oe.dpp(), oe.dpe(), oe.dpz(), oe.dpm(), \
                                                           oe.dee(), oe.dez(), oe.dem(), \
                                                                     oe.dzz(), oe.dzm(), \
                                                                               oe.dmm() )
newparticle = RaveKTransientTrackParticle(np, ne, newparticle.charge(), \
                                          newparticle.chi2(), newparticle.ndof())

print "The new particle looks like this:"
print " parameters: ", newparticle.fullstate()
errortext = str(newparticle.fullerror())
errortext = errortext.replace("\n","\n   ")
print " covariance: \n  ", errortext

print " perigee parameters: ", newparticle.fullPerigeeParameters()
errortext = str(newparticle.fullPerigeeCovariance())
errortext = errortext.replace("\n","\n   ")
print " perigee covariance: \n  ", errortext

print "Now we reinitialize this particle once again with its own perigee numbers:"

op = newparticle.fullPerigeeParameters()
np = RavePerigeeParameters6D(op.rho(), op.theta(), op.phip(), op.epsilon(), op.zp(), op.mass())

oe = newparticle.fullPerigeeCovariance()
ne = RavePerigeeCovariance6D(oe.drr(), oe.drt(), oe.drp(), oe.dre(), oe.drz(), oe.drm(), \
                                       oe.dtt(), oe.dtp(), oe.dte(), oe.dtz(), oe.dtm(), \
                                                 oe.dpp(), oe.dpe(), oe.dpz(), oe.dpm(), \
                                                           oe.dee(), oe.dez(), oe.dem(), \
                                                                     oe.dzz(), oe.dzm(), \
                                                                               oe.dmm() )
newparticle = RaveKTransientTrackParticle(np, ne, newparticle.charge(), \
                                          newparticle.chi2(), newparticle.ndof())

print "The new particle looks like this:"
print " parameters: ", newparticle.fullstate()
errortext = str(newparticle.fullerror())
errortext = errortext.replace("\n","\n   ")
print " covariance: \n  ", errortext

print " perigee parameters: ", newparticle.fullPerigeeParameters()
errortext = str(newparticle.fullPerigeeCovariance())
errortext = errortext.replace("\n","\n   ")
print " perigee covariance: \n  ", errortext
