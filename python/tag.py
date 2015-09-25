#!/usr/bin/python

import vertigo
import sys

ravefactory=vertigo.RaveFactory (
    vertigo.RaveConstantMagneticField(0.,0.,4.), vertigo.RaveVacuumPropagator() )

ravefactory.calibrate()
 
source="gun:bjet"
# source="lcio:local-analyses/ilc/reconstructed.slcio:MCTracksRel" 
method="avf"

vertigo.LoopSettings.Instance().setVerbosity(1)

eventfactory=vertigo.EventFactory ( source )

max=1
ctr=0

for event in eventfactory:
  ctr=ctr+1

  vertices=ravefactory.create ( event.tracks(), method )
  for vertex in vertices: event.add ( vertex )
  event.associateVertices()

  tag=ravefactory.tag ( event.tracks() )
  print "[Event]",len(event.tracks()), "tracks.  d="+str(tag)

  if ctr >= max:
    break
