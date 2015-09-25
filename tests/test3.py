#!/usr/bin/python

print "Tests kalman, avf, and mvf against one event. Checks if output changes."

from vertigo import RaveVertexFactory, EventFactory, LoopSettings, VertexMetaFactory_Instance

def printVertex ( vertex ):
  print "pos=( %(x).4f, %(y).4f, %(z).4f ) chi2=%(chi2).1f ndf=%(n).1f" % \
    { "x": vertex.position().x(), "y": vertex.position().y(), "z": vertex.position().z(), \
      "chi2": vertex.chiSquared(), "n": vertex.ndf()  }

LoopSettings.Instance().setVerbosity(0)

ravefactory=VertexMetaFactory_Instance().get ("dummy" )

eventfactory=EventFactory ( "input3.txt" )

# methods = [ "linpt" ]
methods = [ "kalman", "avf-smoothing:0", "avf-smoothing:1", "avr-primcut:2.0-seccut:6.0-smoothing:0-weightthreshold:0.001", "avr-primcut:2.0-seccut:6.0-smoothing:0-weightthreshold:0.01", "tkf" ]
spotmethods= [ "avr", "tkf" ]
# methods = [ "linpt", "kalman", "avf-smoother:none", "avf-smoother:kalman", "mvf", "avr" ]

for event in eventfactory:
  for method in methods:
    print method+" no beamspot:"
    vertices=ravefactory.create ( event.tracks(), method, False )
    for vertex in vertices:
      printVertex ( vertex )

  for method in spotmethods:
    print method+" beamspot:"
    vertices=ravefactory.create ( event.tracks(), method, True )
    for vertex in vertices:
      printVertex ( vertex )
