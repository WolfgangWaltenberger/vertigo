#!/usr/bin/python

""" This test compares avr with sequential and "manual" application of avf. """

from vertigo import RaveVertexFactory, EventFactory, RaveConstantMagneticField, \
       RaveVacuumPropagator, LoopSettings, RaveTrackContainer, WeightedRaveTrack, \
       LoopSettings

# LoopSettings.Instance().setVerbosity(100)

doAvr=True
doAvf=True

def pprint ( vtx ):
  print "Vtx Pos: (%.4f, %.4f, %.4f) chi2=%.3f ndf=%.3f" % ( vtx.position().x(),vtx.position().y(),vtx.position().z(), 
                                                             vtx.chiSquared(), vtx.ndf() )
  # print vtx
  rftrks=vtx.refittedTracks()

  ctr=0
  for wtrk in sorted ( vtx.weightedTracks() ):
    # wtrk=WeightedRaveTrack ( tmp )
    if wtrk[0] > 1.e-8:
      print "w=%0.3f"%wtrk[0], "  Orig.pz(): %0.3f"%wtrk[1].state().pz(),"  Refitted.pz(): %0.3f"% vtx.refittedTrack ( wtrk[1] ).state().pz()
      ##  print "w=%0.3f"%wtrk[0]," %i" %wtrk[1].id(), "  Orig.pz(): %0.3f"%wtrk[1].state().pz(),"  Refitted.pz(): %0.3f"% vtx.refittedTrack ( wtrk[1] ).state().pz()
    ctr=ctr+1

LoopSettings.Instance().setVerbosity(0)
ravefactory=RaveVertexFactory ( RaveConstantMagneticField(0.,0.,4.), RaveVacuumPropagator() )
eventfactory=EventFactory ( "input8.txt" )
event=eventfactory.next()

if doAvr:
  vertices=ravefactory.create ( event.tracks(), "avr-primcut:2.0-seccut:3.0-smoothing:1-minweight:0.5" )
  print "[avr]"
  for vertex in vertices:
    pprint ( vertex )

print


if doAvf:
  print "[avf]"
  vertex=ravefactory.create ( event.tracks(), "avf-sigmacut:2.0-smoothing:1-ratio:0.0078125" )[0]

  newtracks=RaveTrackContainer()

  pprint ( vertex )
  wtracks = vertex.weightedTracks()

  for t in wtracks:
    if t[0] < 0.5: newtracks.push_back ( t[1] )

  vertex=ravefactory.create ( newtracks, "avf-sigmacut:3.0-smoothing:1-ratio:0.25" )[0]

  pprint ( vertex )
