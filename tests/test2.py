#!/usr/bin/python

print "Simple btagging test."

from vertigo import RaveVertexFactory, RaveFlavorTagFactory, RaveConstantMagneticField,\
                    RaveVacuumPropagator, EventFactory, RavePoint3D, LoopSettings, RaveTrackContainer

method="avr-primcut:1.8-seccut:6.0-primT:256-secT:256-primr:0.25-secr:0.25-smoothing:0-weightthreshold:0.001-minweight:0.5"

ravefactory=RaveVertexFactory ( RaveConstantMagneticField(0.,0.,4.), RaveVacuumPropagator(), method )
btagfactory=RaveFlavorTagFactory ( ravefactory )
# btagfactory=RaveFlavorTagFactory ( )

set=LoopSettings.Instance()
set.setVerbosity(0)

eventfactory=EventFactory ( "bb.1.txt" )

for event in eventfactory:
  primary=ravefactory.create ( event.tracks(), "avf" )
  jets=event.jets()
  print len(jets),"jets. Primary vertex at ",primary[0].position()
  for jet in jets:
    # print "has flavor tagging: ", btagfactory.hasFlavorTagging()
    # print jet.tracks().__class__
    # print primary[0].__class__
    # print jet.momentum().__class__

    tag=btagfactory.tag ( jet.tracks(), primary[0], jet.momentum() )
    
    print "Tagging jet: %(mc)s %(tag).2f jet axis: %(axis)s (pt %(pt).4f) %(sz)d tracks" % \
      { "mc": jet.getMCTag(), "tag": tag, "axis": jet.momentum(), "pt":jet.momentum().perp(), "sz" : len(jet.tracks() ) }

