#include "Plugins/EventPrinter/interface/ObjectPrinter.h"
#include <iostream>
#include "TrackingTools/TrajectoryState/interface/PerigeeConversions.h"
#include "Vertigo/Converters/interface/VertigoToCmsObjects.h"
#include "RaveBase/Converters/interface/RaveToCmsObjects.h"
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
#include "Environment/Configuration/interface/SimpleConfigurable.h"

using namespace std;

void ObjectPrinter::print ( const vertigo::SimTrack & trk, 
    bool euclidean, bool perigee, const string & prefix,
    const string & postfix, float w, bool printweight )
{
  cout << prefix << trk.id();
  if ( euclidean )
  {
    cout << ": ";
    if ( trk.tag().size() && trk.tag()!="0" ) cout << "\"" << trk.tag() << "\" ";
    cout << trk.state();
  }

  if ( perigee )
  {
    VertigoToCmsObjects converter;
    PerigeeConversions perigee;
    double pt;
    PerigeeTrajectoryParameters pars = perigee.ftsToPerigeeParameters
      ( converter.freeState ( trk ), GlobalPoint(0.,0.,0.) , pt ) ;
    cout << " <per " << pars.vector() << ">";
  }

  cout << postfix;
}

void ObjectPrinter::print ( const rave::Track & t, bool euclidean,
    bool perigee, const string & prefix, const string & postfix,
    float w, bool printweight, bool collapsed )
{
  if ( collapsed && t.components().size() )
  {
    printMultiComponent ( t, euclidean, perigee, prefix, postfix, w, printweight );
    return;
  }
  cout << prefix << t.id();
  if ( t.tag().size() )
  {
    cout << " \"" << t.tag() << "\"";
  }

  if ( euclidean ) 
  {
    cout << ": " << t.state() << " q=" << t.charge();
    bool error=SimpleConfigurable<bool>(false,"EventPrinter:TrackErrors").value();
    if ( error )
    {
      cout << "Err:" << t.error() << endl;
    }
  }

  if ( perigee )
  {
    RaveToCmsObjects raveconv;
    PerigeeConversions converter;
    double pt;
    PerigeeTrajectoryParameters pars = converter.ftsToPerigeeParameters ( 
        raveconv.tTrack ( t ).initialFreeState(), GlobalPoint(0.,0.,0.), pt  );
    cout << " <per=" << pars.vector() << ">";
  }

  if ( printweight )
  {
    cout << " w=" << setprecision(2) << w;
  }

  /*
  if ( t.components().size() )
  {
    cout << " [mc]";
  }*/

  cout << postfix;
}

void ObjectPrinter::printMultiComponent ( const rave::Track & t, bool euclidean,
    bool perigee, const string & prefix, const string & postfix,
    float w, bool printweight )
{
  // cout << "# --- MultiComponentTrack #";//
  print ( t, euclidean, perigee, "# --- MultiComponentTrack #", postfix, w, printweight, false );
  cout << endl;
  vector < pair < float, rave::Track > > comps = t.components();
  for ( vector< pair < float, rave::Track > >::const_iterator i=comps.begin(); i!=comps.end() ; ++i )
  {
    print ( i->second, euclidean, perigee, "   * component #", postfix, i->first, true );
    cout << endl;
  }
}

void ObjectPrinter::print ( const vertigo::SimParticle & sp, bool euclidean,
                            bool perigee, const string & prefix, const string & postfix,
                            float w, bool printweight )
{
  cout << prefix << sp.id();
  if ( euclidean ) 
  {
    cout << ": " << sp.state() << " q=" << sp.charge();
  }

  if ( printweight )
  {
    cout << " w=" << w;
  }

  cout << postfix;
}

void ObjectPrinter::print ( const rave::KinematicParticle & t, bool euclidean,
                            bool perigee, const string & prefix, const string & postfix,
                            float w, bool printweight )
{
  cout << prefix << t.id();
  if ( euclidean ) 
  {
    cout << ": " << t.fullstate() << " q=" << t.charge();
  }

  if ( printweight )
  {
    cout << " w=" << w;
  }

  cout << postfix;
}
