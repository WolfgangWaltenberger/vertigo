#!/usr/bin/python

print "running vertigo/tests/test6.py ..."

from vertigo import LoopSettings, EventFactory, RaveConstantMagneticField, \
                    RaveVacuumPropagator, RaveKTransientTrackParticle, \
                    RaveVertexFactory, RaveKTreeFactory, RaveKParticleContainer, \
                    RaveKTreeContainer, RaveKConstraintBuilder, RaveEllipsoid3D, \
                    LoopSettings, RavePoint3D, RaveCovariance3D

LoopSettings.Instance().setVerbosity(100)

muonmass  = 0.1056583
muonsigma = 0.0000000001
kchi2 = 0.
kndf  = 0.
magneticfield = RaveConstantMagneticField( 0., 0., 4. )
beamspot=RaveEllipsoid3D( RavePoint3D(), RaveCovariance3D( .0015**2,0.,0.,.0015**2,0.,5.3**2 ) )                                              

# eventfactory  = EventFactory ( "gun:kinematics" )
eventfactory  = EventFactory ( "file:input6.txt" )
vertexfactory = RaveVertexFactory ( magneticfield, RaveVacuumPropagator(), beamspot )
treefactory   = RaveKTreeFactory( magneticfield, RaveVacuumPropagator(), beamspot )

muonmass  = 0.1056583
muonsigma = 0.0000000001
Zmass     = 91.187
Zsigma    = 2.4952

event = eventfactory.next()

tracks=event.tracks()
particles=event.particles()

print "\nreconstructing the vertex using method 'kalman'", len(tracks)
vertices = vertexfactory.create( tracks, "kalman" )
for vertex in vertices:
  print vertex, "chi2=", ( int ( 1000 * vertex.chiSquared() )) / 1000.," ntrks=", len(vertex.tracks())
print "\nreconstructing the vertex using method 'kalman' with beamspot", len(tracks)
vertices = vertexfactory.create( tracks, "kalman", True )
for vertex in vertices:
  print vertex, "chi2=", ( int ( 1000 * vertex.chiSquared() )) / 1000.," ntrks=", len(vertex.tracks())

def printParticle( prefix, name, particle ):
  fs=particle.fullstate()
  print prefix, name, " : ", fs

def printTree( tree ):
  tree.movePointerToTheTop()
  cp=tree.currentParticle()
  printParticle( " ", "top particle", tree.currentParticle() )
  
  print "  vertex:      ", tree.currentDecayVertex().position()

  print "  children:"
  children = tree.finalStateParticles()
  for child in children:
    printParticle( "   ","- ", child )

# --------------------------------------------------- Unconstrained Vertex Fitting

print "\nreconstructing the decay chain for",len(particles),"particles"
vtree = treefactory.useVertexFitter( particles )

printTree( vtree )

print "\nreconstructing the decay chain for",len(particles),"particles, with beamspot"
vtree = treefactory.useVertexFitter( particles, "", True )

printTree( vtree )

print "\napplying the mass constraint Zmass = ", Zmass
ttmconstraint = RaveKConstraintBuilder().createTwoTrackMassKinematicConstraint( Zmass )
ctree = treefactory.useVertexFitter( particles, ttmconstraint )
printTree( ctree )
