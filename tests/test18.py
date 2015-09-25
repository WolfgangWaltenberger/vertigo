#!/usr/bin/python

""" this is a test for the 3- and 4- vector algebra in rave """

import vertigo

a=vertigo.RavePoint3D ( 1, 2, 3 )
 
print
print "3-vector algebra"
print "================"
print "a=",a
print "a+a=",a+a
print "2*a=",a*2.0

a2=a.copy()
a.multiply(2.0)

print "a.multiply(2)=",a

print "a2=",a2


print
print "4-vector algebra"
print "================"
b=vertigo.RaveVector4D ( 1,1,1,2)

print "b=",b
c=b.copy()
c.add ( b )
print "c=(b.add(b))=",c

d=b.copy()
d+=b

print "c=(b+=b)=",d
print "b+b=", (b+b)
print "b energy=%.4f" % b.energy()
print "b+b energy=%.4f " % (b+b).energy()
print "b mass=%.4f" % (b).m()
print "b+b mass=%.4f" % (b+b).m()
