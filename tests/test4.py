#!/usr/bin/python

print "Tests the geometric annealing schedule"

def ndigits ( obj, n=3, m=-1 ):
  if m == -1: m=n+1
  tmp=str ( int ( 10**n * obj ) / float( 10**n) )
  if m > 1:
    tmp=tmp+"     "
    tmp=tmp[:m]
  return tmp

from vertigo import GeometricAnnealing

schedule=GeometricAnnealing()

for event in range(0,10):
  print "T="+ndigits(schedule.currentTemp(),4,5)+" w(0)="+ndigits(schedule.weight(0.0),3,4)+" w(8.9)="+ndigits(schedule.weight(8.9),3)+" w(9)="+ndigits(schedule.weight(9.0),3)+" w(9.1)="+ndigits(schedule.weight(9.1),3)+" w(16)="+ndigits(schedule.weight(16.),3)+" w(10^9)="+ndigits(schedule.weight(10.e9),2)+" ann:"+str(schedule.isAnnealed())
  schedule.anneal()
