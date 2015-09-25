#!/usr/bin/python

print "Tests kalman, avf, and mvf against one event. Checks if output changes."

from vertigo import RaveVertexFactory, EventFactory, RaveConstantMagneticField, RaveVacuumPropagator, LoopSettings, RaveTrackContainer, RavePoint3D
from vertigo import ObserverManager_Instance, VertigoHarvest_close, VertexMetaFactory_Instance

LoopSettings.Instance().setVerbosity(0)

# ravefactory=RaveVertexFactory ( RaveConstantMagneticField(4.0,0.,0.), RaveVacuumPropagator() )
ravefactory=VertexMetaFactory_Instance().get ("ship" )
p=RavePoint3D(0.,0.,0.)
print "p=",p
print "field",ravefactory.getMagneticField().inTesla( p )

def printVertex ( vertex ):
  print "pos=( %(x).4f, %(y).4f, %(z).4f ) chi2=%(chi2).3f ntrks=%(n)d" % \
    { "x": vertex.position().x(), "y": vertex.position().y(), "z": vertex.position().z(), \
      "chi2": vertex.chiSquared(), "n": len (vertex.tracks())  }

eventfactory=EventFactory ( "input1.txt" )

# methods = [ "kalman", "avf", "mvf" ]
methods = [ "kalman", "avf-smoothing:0", "avf-smoothing:1", "avr", "tkf", "mvf" ]
methods = [ "avf" ]
    
vis=ObserverManager_Instance().get("VertexHistogrammer")

for event in eventfactory:
  for method in methods:
    print method+":"
    vertices=ravefactory.create ( event.tracks(), method )
    for vertex in vertices:
      printVertex ( vertex  )
      event.add ( vertex )
    simvertices=event.simVertices()
    print simvertices.size()
    event.associate (  simvertices[0], vertex )
    #vis=ObserverManager_Instance().get("Visualiser")
    vis.process ( event )

print "done."
ObserverManager_Instance().close()
##VertigoHarvest_close()
