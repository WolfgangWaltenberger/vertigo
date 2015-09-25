#!/usr/bin/python

""" this is a test for the 3- and 4- vector algebra in rave """

import vertigo, sys, math

ravefactory=vertigo.RaveVertexFactory (
    vertigo.RaveConstantMagneticField(0.,0.,4.), vertigo.RaveVacuumPropagator() )

eventfactory=vertigo.EventFactory ( "gun:kinematics-mass:142")

event=eventfactory.next()

simparticles=event.simParticles()
for simparticle in simparticles:
  print simparticle

s0=simparticles[0].state().p4().copy()
s1=simparticles[1].state().p4().copy()
E=s0.energy() + s1.energy()
px=s0.x() + s1.x() 
py=s0.y() + s1.y() 
pz=s0.z() + s1.z() 
print "Manually s0+s1 %.4f " % math.sqrt ( E*E - px*px - py*py - pz*pz )
print "Direct method s0+s1=%.4f " % ( s0+s1 ).m()
