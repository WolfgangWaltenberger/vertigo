#!/usr/bin/python

print "Simple btagging test, but now with beamspot constraint!"

from vertigo import RaveVertexFactory, RaveFlavorTagFactory, RaveConstantMagneticField,\
                    RaveVacuumPropagator, EventFactory, VertigoToCmsObjects,\
                    RavePoint3D, LoopSettings, RaveTrackContainer, VertexMetaFactory_Instance, \
                    RaveEllipsoid3D, RaveCovariance3D, LoopSettings_Instance


LoopSettings_Instance().setVerbosity(255)
ravefactory=VertexMetaFactory_Instance().get("dummy")
method="avr-primcut:2.5-primT:4096-primr:0.125-secT:1024-secr:0.25-smoothing:0-weightthreshold:0.0-seccut:2.3"
ravefactory.setDefaultMethod ( method )
# beamspot=RaveEllipsoid3D()
beamspot=RaveEllipsoid3D( RavePoint3D(), RaveCovariance3D( .0015**2,0.,0.,.0015**2,0.,5.3**2 ) )
ravefactory.setBeamSpot ( beamspot )

if False:
  print "bs=",beamspot.isValid()
  print "Has Beamspot?",ravefactory.hasBeamspot()
  print "dzz",ravefactory.getBeamspot().covariance().dzz()
  print "mz=",ravefactory.getMagneticField().inTesla ( RavePoint3D() ).z()

btagfactory=RaveFlavorTagFactory ( ravefactory )

set=LoopSettings.Instance()

set.setVerbosity(0)

eventfactory=EventFactory ( "bb.1.txt" )

converter=VertigoToCmsObjects()

for event in eventfactory:
  primary=ravefactory.create ( event.tracks(), "avf", True )
  jets=event.jets()
  print len(jets),"jets. Primary vertex at ",primary[0].position()
  for jet in jets:
    # print "has flavor tagging: ", btagfactory.hasFlavorTagging()
    # print jet.tracks().__class__
    # print primary[0].__class__
    # print jet.momentum().__class__

    tag=btagfactory.tag ( jet.tracks(), primary[0], jet.momentum() )
    print "FIXME: check track filter!"
    print "Tagging jet: %(mc)s %(tag).4f jet axis: %(axis)s (pt %(pt).4f) %(sz)d tracks" % \
      { "mc": jet.getMCTag(), "tag": tag, "axis": jet.momentum(), "pt":jet.momentum().perp(), "sz" : len(jet.tracks() ) }

