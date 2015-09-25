#!/usr/bin/python

print "Tests the track to vertex covariance."

## 1. In rave::Vertex: do the tk-to-vtx covariance matrices 
##                     belong to original track or refitted track?
## 2. In VertexTrackFactory: create cov matrix!

from vertigo import RaveVertexFactory, EventFactory, RaveConstantMagneticField, \
         RaveVacuumPropagator, LoopSettings, RaveTrackContainer, RaveException

LoopSettings.Instance().setVerbosity(0)

ravefactory=RaveVertexFactory ( RaveConstantMagneticField(0.,0.,4.), RaveVacuumPropagator() )

def printVertex ( vertex ):
    import sys
    a=sys.argv[0].replace(".py","").replace("./","")
    print "[%(a)s] pos=( %(x).4f, %(y).4f, %(z).4f ) chi2=%(chi2).3f ntrks=%(n)d" % \
        { "a": a, "x": vertex.position().x(), "y": vertex.position().y(), 
          "z": vertex.position().z(), "chi2": vertex.chiSquared(), 
          "n": len (vertex.tracks())  }
    t=vertex.tracks()[0]
    has=vertex.hasTrackToVertexCovariance()
    # print "[%s] has covs %d" % ( a, has )
    if has: print "[test21] cov =",vertex.trackToVertexCovariance( t )

eventfactory=EventFactory ( "input1.txt" )

for event in eventfactory:
    vertices=ravefactory.create ( event.tracks(), "kalman-smoothing:1" )
    for vertex in vertices:
        printVertex ( vertex  )
