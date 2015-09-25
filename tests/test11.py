#!/usr/bin/python

print "Extensively test the ParameterSet building"

from vertigo import ParameterSet, ParameterSetBuilder_create

teststrings = [ "avr", "finder:avr-sigmacut:3.0-Tini:256", "avr-sigmacut:-3.0-Tini:256", "mvf-ini:(bseeder)" ]

for string in teststrings:
  param = ParameterSetBuilder_create ( string )
  print
  print '    string: "'+string+'"'
  print 'toString(): "'+str(param)+'"'
  for key in param.getParameterNames():
    if key!="ini":
      print key, "=", param.getStringParameter(key)
    else:
      print key, "=", param.getNestedParameter(key)
