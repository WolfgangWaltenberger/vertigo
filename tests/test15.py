#!/usr/bin/python

print "running vertigo/tests/test15.py ..."

from vertigo import LoopSettings, EventFactory, RaveConstantMagneticField, RaveVacuumPropagator, RaveKTransientTrackParticle, RaveVertexFactory, RaveKTreeFactory, RaveKParticleContainer, RaveKTreeContainer, RaveKConstraintBuilder

LoopSettings.Instance().setVerbosity(0)

eventfactory  = EventFactory ( "file:input15.txt" )
vertexfactory = RaveVertexFactory ( RaveConstantMagneticField( 0.,0., 4.0 ), RaveVacuumPropagator() )

event=eventfactory.next()
tracks=event.tracks()
for track in tracks:
  print "*=========================="
  print track,"id=",track.id()
  comp=track.components()
  for c in comp:
    print "Component %.2f %s" % (c[0],c[1])
