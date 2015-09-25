#!/usr/bin/python

""" This will be a test for rave's two-jet feature """

width=0.0015
length=5.3

from vertigo import RaveVertexFactory, EventFactory, RaveConstantMagneticField, \
       RaveVacuumPropagator, LoopSettings, RaveTrackContainer, WeightedRaveTrack, \
       ObserverManager_Instance, RaveCovariance3D, RaveEllipsoid3D, RavePoint3D

c=RaveCovariance3D( width**2, 0,0,width**2,0,length**2 )
e=RaveEllipsoid3D( RavePoint3D(),c )

LoopSettings.Instance().setVerbosity(0)
ravefactory=RaveVertexFactory ( RaveConstantMagneticField(0.,0.,4.), RaveVacuumPropagator(), e, "avr-primcut:3.0" )
eventfactory=EventFactory ( "bjets.170.1.txt.gz" )
event=eventfactory.next()

for simvtx in event.simVertices():
   print simvtx

print len(event.jets()),"jets in event."
print len(event.tracks()),"tracks in event."

primaries=RaveTrackContainer()
first=True

secondaries=None

for jet in event.jets():
  tracks=jet.tracks()
  print len(tracks), "tracks in jet."
  if not first: # put all tracks but of "first jet" in "primaries"
    for track in tracks:
      primaries.append ( track )
  else:
    secondaries=tracks
  first=False

vertices=ravefactory.create ( event.tracks(), True )
print len(vertices),"vertices with all tracks"
for vtx in vertices:
  print "Vtx Pos: (%.4f, %.4f, %.4f)" % (vtx.position().x(),vtx.position().y(),vtx.position().z() )

print len(primaries),"primary tracks."
vertices=ravefactory.create ( primaries, secondaries, True )
# vertices=ravefactory.create ( primaries, True )
print len(vertices),"vertices with all tracks"
for vtx in vertices:
  print "Vtx Pos: (%.4f, %.4f, %.4f)" % (vtx.position().x(),vtx.position().y(),vtx.position().z() )

# obs=ObserverManager_Instance().get("EventPrinter")
# obs.process(event)
