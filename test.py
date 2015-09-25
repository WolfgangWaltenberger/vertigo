#!/usr/bin/python

import vertigo

ravefactory=vertigo.RaveFactory (
    vertigo.RaveConstantMagneticField(0.,0.,4.), vertigo.RaveVacuumPropagator() )
 
# source="gun:simple"
source="lcio:local-analyses/ilc/reconstructed.slcio" 
eventfactory=vertigo.EventFactory ( source )

event=eventfactory.next()

vertices=ravefactory.create ( event.tracks(), "avf" )

for vertex in vertices: event.add ( vertex )

event.associateVertices()

# vertigo.EventPrinter()(event)
vertigo.Visualiser()(event)
#histo=vertigo.VertexHistogrammer()
#histo.configure("t.hdf")
#histo(event)
