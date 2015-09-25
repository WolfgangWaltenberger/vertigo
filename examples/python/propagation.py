#!/usr/bin/python

""" this thing tests rave's vacuum propagator """

import vertigo, dataharvester

factory=vertigo.RaveVertexFactory ( vertigo.RaveConstantMagneticField ( 3.8 ) )


state1=vertigo.RaveVector6D(-0.069847, -1.169135, -2.450486 , 0.924591, 3.043066, 3.419684)
cov1=vertigo.RaveCovariance6D(    5.241e-04, -1.065e-05, -4.964e-05,
   5.060e-04, -1.074e-04, 2.796e-05,
   0.000e+00, 0.000e+00, 0.000e+00,
   0.000e+00, 0.000e+00, 0.000e+00,
   0.000e+00, 0.000e+00, 0.000e+00,
   1.068e-03, 4.949e-04, 2.282e-03,
   1.937e-03, 5.042e-03, 2.409e-02)
t1=vertigo.RaveTrack(state1, cov1, -1, 0.00, 0.00, 0, "" )

prop=vertigo.RaveVacuumPropagator()
prop2=vertigo.RaveVacuumPropagator()
  
print t1.state()

writer=dataharvester.Writer_file ( "path.txt" )

event=dataharvester.Tuple("event")
event["id"]=0
event["run"]=0
event["tag"]="propagation"

for ix in range(-1350,1360,10):
  ## for iy in range(-50,60,10):
  for iy in [ 0 ]:
    x=float(ix/10.)
    y=float(iy/10.)
    plane = vertigo.RavePlane ( vertigo.RavePoint3D( 0.,0.,float(x)),
                                vertigo.RavePoint3D( 0.,0.,1. ) )
    # t=prop.closestTo ( t1, vertigo.RavePoint3D(x,y,0.), False )
    t=prop.to ( t1, plane )
    print "t=",t
    s=t.state()
    pre="arrow"
    # pre="track"
    # mag=0.1
    mag=1.0
    p=0
    event[pre+":x"]=s.x()
    event[pre+":y"]=s.y()
    event[pre+":z"]=s.z()
    event[pre+":px"]=s.px()
    event[pre+":py"]=s.py()
    event[pre+":pz"]=s.pz()
    event[pre+":dx"]=p*s.x()+s.px()*mag
    event[pre+":dy"]=p*s.y()+s.py()*mag
    event[pre+":dz"]=p*s.z()+s.pz()*mag
    event.fill(pre)
  
writer.save(event)
dataharvester.Writer_close()
