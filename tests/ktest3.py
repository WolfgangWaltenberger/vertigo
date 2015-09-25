#!/usr/bin/python

print "running vertigo/tests/ktest2.py ..."

from vertigo import LoopSettings, EventFactory, RaveConstantMagneticField, RaveVacuumPropagator, RaveKTransientTrackParticle, RaveVertexFactory, RaveKTreeFactory, RaveKParticleContainer, RaveKTreeContainer, RaveKConstraintBuilder, RavePerigeeCovariance6D, RaveCovariance7D, RavePerigeeParameters6D, RaveVector7D

LoopSettings.Instance().setVerbosity(100)

magneticfield = RaveConstantMagneticField( 0., 0., 4. )
eventfactory  = EventFactory ( "file:test.txt.gz" )
treefactory   = RaveKTreeFactory( magneticfield ) # This is only to set the magnetic field singleton

print "Shooting kinematics gun ..."
event = eventfactory.next()

print "Using the first particle as starting point:"
track = event.tracks()[0]

simparams = track.state()
inputparams = RaveVector7D(simparams.x(),simparams.y(),simparams.z(), \
                           simparams.px(),simparams.py(),simparams.pz(), 1.)

simerror = track.error()
inputerror = RaveCovariance7D(simerror.dxx(), simerror.dxy(), simerror.dxz(),
                              simerror.dyy(), simerror.dyz(), simerror.dzz(),
                              simerror.dxpx(), simerror.dxpy(), simerror.dxpz(),
                              simerror.dypx(), simerror.dypy(), simerror.dypz(),
                              simerror.dzpx(), simerror.dzpy(), simerror.dzpz(),
                              simerror.dpxpx(), simerror.dpxpy(), simerror.dpxpz(),
                              simerror.dpypy(), simerror.dpypz(), simerror.dpzpz(),
                              0.,0.,0.,0.,0.,0.,0.)

inputparticle = RaveKTransientTrackParticle(inputparams, inputerror, track.charge(), \
                                            track.chi2(), track.ndof())

print " parameters: ", inputparticle.fullstate()
errortext = str(inputparticle.fullerror())
errortext = errortext.replace("\n","\n   ")
print " covariance: \n  ", errortext

print " perigee parameters: ", inputparticle.fullPerigeeParameters()
errortext = str(inputparticle.fullPerigeeCovariance())
errortext = errortext.replace("\n","\n   ")
print " perigee covariance: \n  ", errortext

# forth and back ========================================================

op = inputparticle.fullPerigeeParameters()
np = RavePerigeeParameters6D(op.rho(), op.theta(), op.phip(), op.epsilon(), op.zp(), op.mass())

oe = inputparticle.fullPerigeeCovariance()
ne = RavePerigeeCovariance6D(oe.drr(), oe.drt(), oe.drp(), oe.dre(), oe.drz(), oe.drm(), \
                                       oe.dtt(), oe.dtp(), oe.dte(), oe.dtz(), oe.dtm(), \
                                                 oe.dpp(), oe.dpe(), oe.dpz(), oe.dpm(), \
                                                           oe.dee(), oe.dez(), oe.dem(), \
                                                                     oe.dzz(), oe.dzm(), \
                                                                               oe.dmm() )
newparticle = RaveKTransientTrackParticle(np, ne, inputparticle.charge(), \
                                          inputparticle.chi2(), inputparticle.ndof())

print "The new particle looks like this:"
print " parameters: ", newparticle.fullstate()
errortext = str(newparticle.fullerror())
errortext = errortext.replace("\n","\n   ")
print " covariance: \n  ", errortext

print " perigee parameters: ", newparticle.fullPerigeeParameters()
errortext = str(newparticle.fullPerigeeCovariance())
errortext = errortext.replace("\n","\n   ")
print " perigee covariance: \n  ", errortext

# once again ========================================================

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
