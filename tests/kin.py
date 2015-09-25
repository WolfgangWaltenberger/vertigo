#!/usr/bin/python

from vertigo import LoopSettings, EventFactory, RaveConstantMagneticField, \
                    RaveVacuumPropagator, RaveKTransientTrackParticle, \
                    RaveVertexFactory, RaveKTreeFactory, RaveKParticleContainer, \
                    RaveKTreeContainer, RaveKConstraintBuilder, RaveEllipsoid3D, \
                    LoopSettings, RavePoint3D, RaveCovariance3D
from vertigo import ObserverManager
import dataharvester

writer=dataharvester.Writer_file("x.root;x.txt")

muonmass  = 0.1056583
muonsigma = 0.0000000001
Zmass     = 91.187
Zsigma    = 2.4952
magneticfield = RaveConstantMagneticField( 0., 0., 4. )
beamspot=RaveEllipsoid3D( RavePoint3D(), RaveCovariance3D( .0015**2,0.,0.,.0015**2,0.,5.3**2 ) )                                              

eventfactory  = EventFactory ( "gun:kinematics" )
# eventfactory  = EventFactory ( "file:input6.txt" )
vertexfactory = RaveVertexFactory ( magneticfield, RaveVacuumPropagator(), beamspot )
treefactory   = RaveKTreeFactory( magneticfield, RaveVacuumPropagator(), beamspot )

def discussTrack ( simtrk, rectrk, name, eventid, d ):
    t=dataharvester.Tuple( name )
    t["rpt"]=rectrk.pt()
    t["spt"]=simtrk.pt()
    t["dpt"]=abs ( simtrk.pt() - rectrk.pt() )
    t["eventid"]=eventid
    t["rectrkid"]=rectrk.id()
    t["simtrkid"]=simtrk.id()
    for (k,v) in d.items():
        t[k]=v
    t["name"]=name
    writer.save(t)

def discussVertex ( simvtx, recvtx, name, eventid ):
    t=dataharvester.Tuple( "vertex"+name )
    t["eventid"]=eventid
    t["simvtxid"]=simvtx.id()
    t["recvtxid"]=recvtx.id()
    t["dx"]=recvtx.position().x()-simvtx.position().x()
    writer.save(t)


ctr=0
max=1000
while ctr<max:
    event = eventfactory.next()
    ctr=event.id()

    tracks=event.tracks()
    particles=event.particles()
    simtracks=event.simTracks()
    simvtxs=event.simVertices()
    print "len simvtxs=",len(simvtxs)

    vtces0=vertexfactory.create ( tracks, "kalman-smoothing:0" )
    if len(vtces0)>0:
        discussVertex ( simvtxs[0], vtces0[0], "kalman", ctr )
        trks0=vtces0[0].tracks()
        for rtrk in trks0:
            simtracks=event.associatedTracks ( rtrk )
            if len(simtracks)!=1:
                print "Huh?",len(simtracks),"simtracks??"
                continue
            print "not smoothed",rtrk.state()
            discussTrack ( simtracks[0], rtrk, "kalmannosmooth", ctr, { "smooth": 0 } )

    vtces1=vertexfactory.create ( tracks, "kalman-smoothing:1" )
    if len(vtces1)>0:
        rtrks1=vtces1[0].refittedTracks()
        trks1=vtces1[0].tracks()

        for (idx,trk) in enumerate(trks1):
            simtracks=event.associatedTracks ( trk )
            if len(simtracks)!=1:
                print "Huh?",len(simtracks),"simtracks??"
                continue
            rtrk=rtrks1[idx]
            print "smoothed",rtrk.state()
            print "   `--- orig ",trk.momentum()
            discussTrack ( simtracks[0], rtrk, "kalmansmooth", ctr, { "smooth": 1 } )

    vtree = treefactory.useVertexFitter( particles )
    vtree.movePointerToTheTop()
    rvtx=vtree.currentDecayVertex()
    discussVertex ( simvtxs[0], rvtx, "vtx", ctr )
# printTree ( vtree )

dataharvester.Writer_close()
