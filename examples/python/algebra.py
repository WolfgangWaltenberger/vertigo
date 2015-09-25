#!/usr/bin/python

import vertigo, copy

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
print "b+b=",b+b
print "b energy=",b.energy()
print "b+b energy=",(b+b).energy()
print "b mass=",(b).m()
print "b+b mass=",(b+b).m()
