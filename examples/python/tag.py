#!/usr/bin/python

import vertigo
import sys

ravefactory=vertigo.RaveVertexFactory (
    vertigo.RaveConstantMagneticField(0.,0.,4.), vertigo.RaveVacuumPropagator() )

btagfactory=vertigo.RaveFlavorTagFactory ( ravefactory )

source="gun:bjet"
method="avf"

eventfactory=vertigo.EventFactory ( source )

max=1
ctr=0

for event in eventfactory:
  ctr+=1
  vertices=ravefactory.create ( event.tracks(), method )
  for vertex in vertices: event.add ( vertex )
  event.associateVertices()
  for jet in event.jets():
    tag=btagfactory.tag ( event.tracks(),vertices[0], jet.momentum() )
    print "[Event]",len(event.tracks()), "tracks.  d="+str(tag)

  if ctr >= max:
    break

