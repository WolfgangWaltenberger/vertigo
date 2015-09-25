#!/usr/bin/python

import ROOT
import tables

def lims ( variable, limit ):
  return variable+">-"+str(limit)+" && "+variable+"<"+str(limit)

f=ROOT.TFile ("tracks.root")
c1=ROOT.TCanvas()
Tk=f.Get("Tk")
vars=["lip","theta","phi","tip","rho" ]
for var in vars:
  print
  print "Processing ",var
  Tk.Fit("gaus","st"+var, lims ( "st"+var, 10 ) )
  c1.Print("plots/st."+var+".eps")
  Tk.Fit("gaus", var, lims ( var, 1. ) )
  c1.Print("plots/"+var+".eps")
