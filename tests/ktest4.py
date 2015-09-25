#!/usr/bin/python

print "running vertigo/tests/ktest4.py ..."

from vertigo import *

LoopSettings.Instance().setVerbosity(100)

magneticfield = RaveConstantMagneticField( 0., 0., 4. )

eventfactory  = EventFactory ( "gun:kinematics" )
vertexfactory = RaveVertexFactory ( magneticfield, RaveVacuumPropagator() )
treefactory   = RaveKTreeFactory( magneticfield )

event = eventfactory.next()

eparticles = event.particles()
print "\nfound ", len( eparticles ), " generated particles"

print "\nThe simvertices are as follow:"
for v in event.simVertices():
  print " ", v.position(), " tag: ", v.tag()

print "\nThe simparticles are as follow:"
for p in event.simParticles():
  print " ", p.state()

print "\nThe particles are as follow:"
for p in event.particles():
  print " ", p.state(), p.error()

tracks = RaveTrackContainer()
for p in eparticles:
  tracks.push_back(RaveTrack(p.state(),p.error(),p.charge(),p.chi2(),p.ndof()))

print "\nreconstructing the vertex using method 'kalman'"
vertices = vertexfactory.create( tracks, "kalman" )
for vertex in vertices:
  print vertex, "chi2=", ( int ( 1000. * vertex.chiSquared() )) / 1000.," ntrks=", len(vertex.tracks())
  print vertex.error()

print "\nassigning muon mass information to the tracks to produce particles"
particles = RaveKParticleContainer()
for p in eparticles:
  particles.push_back(p)

def printParticle( prefix, name, particle ):
  print prefix, name, " : ", particle.fullstate()

def printTree( tree ):
  tree.movePointerToTheTop()

  printParticle( " ", "top particle", tree.currentParticle() )
  print "  vertex:      ", tree.currentDecayVertex().position()
  print tree.currentDecayVertex().error()

  print "  children:"
  children = tree.finalStateParticles()
  for child in children:
    printParticle( "   ","- ", child )

# --------------------------------------------------- Unconstrained Vertex Fitting

print "\nreconstructing the decay chain"
vtree = treefactory.useVertexFitter( particles )

printTree( vtree )

ZParticle = vtree.topParticle()
print "\nguessed value for Zmass = ", ZParticle.fullstate().m()
