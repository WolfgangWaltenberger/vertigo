#!/usr/bin/python

from vertigo import RaveVertexFactory, EventFactory, RaveConstantMagneticField, RaveVacuumPropagator, LoopSettings, RaveTrackContainer

LoopSettings.Instance().setVerbosity(0)

ravefactory=RaveVertexFactory ( RaveConstantMagneticField(0.,0.,4.), RaveVacuumPropagator() )

def printVertex ( vertex ):
  print "pos=( %(x).4f, %(y).4f, %(z).4f ) chi2=%(chi2).3f ntrks=%(n)d" % \
    { "x": vertex.position().x(), "y": vertex.position().y(), "z": vertex.position().z(), \
      "chi2": vertex.chiSquared(), "n": len (vertex.tracks())  }

eventfactory=EventFactory ( "input1.txt" )

for event in eventfactory:
  method="avf-smoothing:1"
  print method+":"
  vertices=ravefactory.create ( event.tracks(), method )
  for vertex in vertices:
    printVertex ( vertex  )
