#!/usr/bin/python

""" this thing tests rave's vacuum propagator """

import vertigo

factory=vertigo.RaveVertexFactory ( vertigo.RaveConstantMagneticField ( 3.8 ) )


state33=vertigo.RaveVector6D(-0.069847, -1.169135, -2.450486 , 0.924591, 2.043066, 9.419684)
cov33=vertigo.RaveCovariance6D(    5.241e-04, -1.065e-05, -4.964e-05,
   5.060e-04, -1.074e-04, 2.796e-05,
   0.000e+00, 0.000e+00, 0.000e+00,
   0.000e+00, 0.000e+00, 0.000e+00,
   0.000e+00, 0.000e+00, 0.000e+00,
   1.068e-03, 4.949e-04, 2.282e-03,
   1.937e-03, 5.042e-03, 2.409e-02)
t33=vertigo.RaveTrack(state33, cov33, -1, 0.00, 0.00, 0, "" )

prop=vertigo.RaveVacuumPropagator()

print t33
t=prop.closestTo ( t33, vertigo.RavePoint3D(0.,0.,0.), True )
print t
