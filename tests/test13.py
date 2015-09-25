#!/usr/bin/python

print "Test the avf with beamspot and a single track!"

from vertigo import RaveVertexFactory, RaveFlavorTagFactory, RaveConstantMagneticField,\
                    RaveVacuumPropagator, EventFactory, RavePoint3D, LoopSettings, RaveTrackContainer,\
                    RaveEllipsoid3D, RaveCovariance3D

ravefactory=RaveVertexFactory ( RaveConstantMagneticField(0.,0.,4.), RaveVacuumPropagator(), "avr" )
beamspot=RaveEllipsoid3D( RavePoint3D(), RaveCovariance3D( .0015**2,0.,0.,.0015**2,0.,5.3**2 ) )
ravefactory.setBeamSpot ( beamspot )

eventfactory=EventFactory ( "onetrack.txt" )

event=eventfactory.next()
primary=ravefactory.create ( event.tracks(), "avf", True )
print "Vtx Pos: (%.4f, %.4f, %.4f)  %d tracks" % ( primary[0].position().x(),primary[0].position().y(),primary[0].position().z(),len(primary[0].tracks()))
print "%.4f" % primary[0].weightedTracks()[0][0]
