#include "ObjectCoder.h"
#include "Vertigo/Event/interface/Event.h"
#include "RaveBase/Converters/interface/RaveStreamers.h"
#include <iostream>
#include <iomanip>

using namespace std;

void ObjectCoder::printCovariance ( const rave::Covariance6D & c ) const
{
  cout << "( " << scientific << setprecision(3)
       << "   " << c.dxx() << ", " << c.dxy() << ", " << c.dxz() << ", " << endl
       << "   " << c.dyy() << ", " << c.dyz() << ", " << c.dzz() << ", " << endl
       << "   " << c.dxpx() << ", " << c.dxpy() << ", " << c.dxpz() << ", " << endl
       << "   " << c.dypx() << ", " << c.dypy() << ", " << c.dypz() << ", " << endl
       << "   " << c.dzpx() << ", " << c.dzpy() << ", " << c.dzpz() << ", " << endl
       << "   " << c.dpxpx() << ", " << c.dpxpy() << ", " << c.dpxpz() << ", " << endl
       << "   " << c.dpypy() << ", " << c.dpypz() << ", " << c.dpzpz() << ")";
}

void ObjectCoder::printCovariance ( const rave::Covariance7D & c ) const
{
  cout << "( " << scientific << setprecision(3)
       << "   " << c.dxx() << ", " << c.dxy() << ", " << c.dxz() << ", " << endl
       << "   " << c.dyy() << ", " << c.dyz() << ", " << c.dzz() << ", " << endl
       << "   " << c.dxpx() << ", " << c.dxpy() << ", " << c.dxpz() << ", " << endl
       << "   " << c.dypx() << ", " << c.dypy() << ", " << c.dypz() << ", " << endl
       << "   " << c.dzpx() << ", " << c.dzpy() << ", " << c.dzpz() << ", " << endl
       << "   " << c.dpxpx() << ", " << c.dpxpy() << ", " << c.dpxpz() << ", " << endl
       << "   " << c.dpypy() << ", " << c.dpypz() << ", " << c.dpzpz() << ", " << endl
       << "   " << c.dxm() << ", " << c.dym() << ", " << c.dzm() << ", " << endl
       << "   " << c.dpxm() << ", " << c.dpym() << ", " << c.dpzm() << ", " << endl
       << "   " << c.dmm() << ")"; // << endl;
}

void ObjectCoder::printKinematicParticle ( const rave::KinematicParticle & trk )const
{
  if (python_)
  {
    cout << "state" << trk.id() << "=vertigo.RaveVector7D" << trk.fullstate() << endl;
    cout << "cov" << trk.id() << "=vertigo.RaveCovariance7D";
    printCovariance ( trk.fullerror() );
    cout << endl;
    cout << "t" << trk.id() << "=vertigo.RaveKParticle(state" << trk.id() 
         << ", cov" << trk.id() << ", " << trk.charge()
         << ", " << fixed << setprecision(2) << trk.chi2() << ", " << trk.ndof()
         << ", \"" << trk.tag() << "\" )" << endl;
    cout << endl;
  } else {
    cout << "rave::Vector7D state" << trk.id() << trk.fullstate() << "; " << endl;
    cout << "rave::Covariance7D cov" << trk.id();
    printCovariance ( trk.fullerror() );
    cout << "; " << endl;
    cout << "rave::KinematicParticle t" << trk.id() << "(" << trk.id()
         << ", state" << trk.id() << ", cov" << trk.id() << ", " << trk.charge()
         << ", " << fixed << setprecision(2) << trk.chi2() << ", " << trk.ndof() << ", "
         << ", \"" << trk.tag() << "\" ); " << endl;
    cout << endl;
  }
}


void ObjectCoder::printRecTrack ( const rave::Track & trk ) const
{
  if (python_)
  {
    cout << "state" << trk.id() << "=vertigo.RaveVector6D" << trk.state() << endl;
    cout << "cov" << trk.id() << "=vertigo.RaveCovariance6D";
    printCovariance ( trk.error() );
    cout << endl;
    cout << "t" << trk.id() << "=vertigo.RaveTrack(state" << trk.id() 
         << ", cov" << trk.id() << ", " << trk.charge()
         << ", " << fixed << setprecision(2) << trk.chi2() << ", " << trk.ndof() << ", "
         << trk.originalObject() << ", \"" << trk.tag() << "\" )" << endl;
    cout << endl;
  } else {
    cout << "rave::Vector6D state" << trk.id() << trk.state() << "; " << endl;
    cout << "rave::Covariance6D cov" << trk.id();
    printCovariance ( trk.error() );
    cout << "; " << endl;
    cout << "rave::Track t" << trk.id() << "(" << trk.id()
         << ", state" << trk.id() << ", cov" << trk.id() << ", " << trk.charge()
         << ", " << fixed << setprecision(2) << trk.chi2() << ", " << trk.ndof() << ", "
         << trk.originalObject() << ", \"" << trk.tag() << "\" ); " << endl;
    cout << endl;
  }
}

ObjectCoder::ObjectCoder() : python_  ( false )
{}

ObjectCoder * ObjectCoder::clone() const
{
  return new ObjectCoder ( *this );
}

void ObjectCoder::configure( const string & s )
{
  if ( s=="python" )
  {
    python_=true;
  }
}

void ObjectCoder::process ( const exception & e, const string & type )
{}

void ObjectCoder::process ( const vertigo::Event & e )
{
  vector < rave::Track > trks = e.tracks();
  for ( vector< rave::Track >::const_iterator 
        i=trks.begin(); i!=trks.end() ; ++i )
  {
    printRecTrack ( *i );
  }
  vector < rave::KinematicParticle > prtcls = e.particles();
  for ( vector< rave::KinematicParticle >::const_iterator 
        i=prtcls.begin(); i!=prtcls.end() ; ++i )
  {
    printKinematicParticle ( *i );
  }
}

#include "Analysis/Observer/interface/ObserverBuilder.h"
namespace {
  ObserverBuilder<ObjectCoder> t( "ObjectCoder", "Write lines of code to stdout that can be copied and pasted into C++/python files.", false );
}
