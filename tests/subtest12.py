#!/usr/bin/python

from vertigo import RaveVertexFactory, EventFactory, RaveConstantMagneticField, RaveVacuumPropagator, LoopSettings, RaveTrackContainer, RaveLogger

LoopSettings.Instance().setVerbosity(0)

ravefactory=RaveVertexFactory ( RaveConstantMagneticField(0.,0.,4.), RaveVacuumPropagator() )

RaveLogger().writeToFile ( "rave_log.txt" )

event=EventFactory ( "input1.txt" ).next()

ravefactory.create ( event.tracks(), "avr" )

