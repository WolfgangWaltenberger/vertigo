#!/usr/bin/python

import vertigo
import sys

ravefactory=vertigo.RaveVertexFactory (
    vertigo.RaveConstantMagneticField(0.,0.,4.), vertigo.RaveVacuumPropagator() )

source="gun:bjet"
# source="lcio:local-analyses/ilc/reconstructed.slcio:MCTracksRel" 
method="avf"
vis=vertigo.ObserverManager_Instance().get("Visualiser")
printer=vertigo.ObserverManager_Instance().get("EventPrinter")

eventfactory=vertigo.EventFactory ( source )

for event in eventfactory:
  vertices=ravefactory.create ( event.tracks(), method )
  for vertex in vertices: event.add ( vertex )
  event.associateVertices()
  # printer.process (event )
  vis.process ( event )

  break # only one event this time

