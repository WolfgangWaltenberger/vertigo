#!/usr/bin/python

print "running vertigo/tests/test6.py ..."

from vertigo import LoopSettings, EventFactory, RaveConstantMagneticField, \
                    RaveVacuumPropagator, RaveKTransientTrackParticle, \
                    RaveVertexFactory, RaveKTreeFactory, RaveKParticleContainer, \
                    RaveKTreeContainer, RaveKConstraintBuilder

LoopSettings.Instance().setVerbosity(100)

muonmass  = 0.1056583
muonsigma = 0.0000000001
kchi2 = 0.
kndf  = 0.
magneticfield = RaveConstantMagneticField( 0., 0., 4. )

# eventfactory  = EventFactory ( "file:input6.txt" )
eventfactory  = EventFactory ( "gun:kinematics" )
vertexfactory = RaveVertexFactory ( magneticfield, RaveVacuumPropagator() )
treefactory   = RaveKTreeFactory( magneticfield )

muonmass  = 0.1056583
muonsigma = 0.0000000001
Zmass     = 91.187
Zsigma    = 2.4952

# print "\nskipping the first shot"
# eventfactory.skip(1)
# print "using the following shot"
event = eventfactory.next()

tracks = event.tracks()

print "\nfound ", len( tracks ), " generated tracks"
for e in tracks:
  print "track",e

eparticles = event.particles()
print "\nfound ", len( eparticles ), " generated particles"
for e in eparticles:
  print "particle",e

print "\nThe simvertices are as follows:"
for v in event.simVertices():
  print " ", v.position(), " tag: ", v.tag()

print "\nThe simparticles are as follows:"
for p in event.simParticles():
  print " ", p.state()

print "\nreconstructing the vertex using method 'kalman'", len(tracks)
vertices = vertexfactory.create( tracks, "kalman" )
for vertex in vertices:
  print vertex, "chi2=", ( int ( 1000 * vertex.chiSquared() )) / 1000.," ntrks=", len(vertex.tracks())

print "\nassigning muon mass information to the tracks to produce particles"
#particles = RaveKParticleContainer()
#for track in tracks:
#  particles.push_back( RaveKTransientTrackParticle( track, muonmass, muonsigma, kchi2, kndf ) )
particles = RaveKParticleContainer()
for p in eparticles:
  particles.push_back(p)

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
vtree = treefactory.useVertexFitter( eparticles )

print "yau!"

printTree( vtree )

#ZParticle = vtree.topParticle()
#print "\nguessed value for Zmass = %(mZ).4f" % { "mZ": ZParticle.fullstate().m() }

# --------------------------------------------------- TwoTrackMass Constraint

print "\napplying the mass constraint Zmass = ", Zmass
ttmconstraint = RaveKConstraintBuilder().createTwoTrackMassKinematicConstraint( Zmass )
ctree = treefactory.useVertexFitter( particles, ttmconstraint )
printTree( ctree )

# --------------------------------------------------- MassKinematicConstraint

do_soft=True # try with soft mass constraint?
if do_soft:
  print "\napplying the mass constraint Zmass = ", Zmass, " and Zsigma = ", Zsigma
  trees = RaveKTreeContainer()
  trees.push_back( vtree )
  mkconstraint = RaveKConstraintBuilder().createMassKinematicConstraint( Zmass, Zsigma )
  ctrees = treefactory.useParticleFitterWithTree( trees, mkconstraint )
  printTree( ctrees[0] )
