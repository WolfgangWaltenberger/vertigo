#!/usr/bin/python

print "Simple test for rave points"

from vertigo import RavePoint3D

a=RavePoint3D(0.,0.,1.)
b=RavePoint3D(0.,0.,2.)

print b-a
print b+a
