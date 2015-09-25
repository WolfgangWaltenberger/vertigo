#!/usr/bin/python

import vertigo

f=vertigo.RaveVertexFactory ( vertigo.RaveConstantMagneticField(4.))
factory=vertigo.EventFactory("hepmc:file-example.txt")

ctr=0
for event in factory:
  a=vertigo.RaveVector4D(0.,0.,0.,0.)
  print "-----"
  for trk in event.simParticles():
    p=trk.p4().copy()
    print p
    a+=p
  print "sum=",a
  ctr+=1
  if  ctr>2:
    break
