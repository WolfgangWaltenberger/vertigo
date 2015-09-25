#include <rave/VertexFactory.h>
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
#include <rave/ConstantMagneticField.h>
#include <rave/Point3D.h>
#include "Vertigo/EventGeneration/interface/EventFactory.h"
#include <iostream>

using namespace std;

class MyPropagator : public rave::Propagator
{
  public:
  virtual MyPropagator * copy() const {
    return new MyPropagator ( *this );
  }
  rave::Track closestTo ( const rave::Track & orig,
                          const rave::Point3D & pt, bool transverse ) const
  {
    // cout << "[MyPropagator] closestToPoint called" << endl;
    rave::Vector3D mom = orig.state().momentum();
    rave::Point3D pos = orig.state().position();
    mom+=rave::Vector3D ( 0.1, 0.1, 0.1 );
    rave::Vector6D state ( pos, mom );
    rave::Covariance6D cov = orig.error();
    int id = orig.id();
    void * p = orig.originalObject();
    string tag = orig.tag();
    rave::Track ret ( orig );
    return ret;
  }

  std::pair < rave::Track, double > to ( const rave::Track & orig,
                   const ravesurf::Plane & ) const
  {
    // cout << "[MyPropagator] toPlane called on " << orig.id() << endl;
    rave::Track ret ( orig );
    return pair < rave::Track, double > ( ret, 0. );
  }

  std::pair < rave::Track, double > to ( const rave::Track & orig,
                   const ravesurf::Cylinder & ) const
  {
    cout << "[MyPropagator] toCylinder called" << endl;
    rave::Track ret ( orig );
    return pair < rave::Track, double > ( ret, 0. );
  }
};

int main()
{
  MyPropagator propagator;
  rave::VertexFactory f ( rave::ConstantMagneticField ( 3.8 ), propagator );
  EventFactory factory ( "file:input1.txt" );
  vertigo::Event e = factory.next();
  vector < rave::Track > oldtracks = e.tracks();
  vector < rave::Track > tracks;
  rave::Point3D ppt ( 10., 10., 10. );
  cout << "[propagatorTest] propagate tracks to " << ppt << endl;
  cout << "[propagatorTest] ids ";
  for ( vector< rave::Track >::const_iterator i=oldtracks.begin();
        i!=oldtracks.end() ; ++i )
  { 
    rave::Track n = propagator.closestTo (
        *i, ppt, true );
    cout << i->id() << "(" << n.id() << ") "; 
    tracks.push_back ( n );
  }
  cout << endl;
  /*
  cout << "[propagatorTest] we have " << tracks.size() << " tracks."
       << endl; */
  cout << "[propagatorTest] now fit!" << endl;
  vector < rave::Vertex > vtces = f.create ( oldtracks );
  cout << "[propagatorTest] we have " << vtces.size() << " vertices."
       << endl;
  vector < rave::Track > newtrks = vtces[0].tracks();

  cout << "[propagatorTest] new tracks ";
  for ( vector< rave::Track >::const_iterator i=newtrks.begin();
        i!=newtrks.end() ; ++i )
  { 
    cout << i->id() << " ";
  }
  cout << endl;
}
