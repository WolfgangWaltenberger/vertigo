#include "ParameterScanner.h"
#include "Vertigo/Event/interface/Event.h"
#include "Environment/Configuration/interface/SimpleConfigurable.h"
#include <dataharvester/Writer.h>
#include <dataharvester/Tuple.h>

using namespace std;
using namespace dataharvester;

namespace {
  vector < string > getToken ( string s )
  {
    vector < string > ret;
    // splits Adaptive-cut:3.0 into "Adaptive", "cut:3.0"
    // code is the same as in MultiVertexHistogrammer
    while ( s.find ("-") != string::npos )
    {
      string::size_type pos = s.find ( "-" );
      string sub = s.substr ( 0, pos );
      s=s.substr ( pos+1, s.size() );
      ret.push_back ( sub );
    };
    ret.push_back ( s );
    return ret;
  }

  void getDescription( string s, Tuple & t )
  {
    // transforms Adaptive-cut:3.0-linpt:Fsmw into
    // "fitter":"Adaptive", "cut":3.0, "linpt":"Fsmw"
    // code is the same as in MultiVertexHistogrammer
    vector < string > d = getToken ( s );
    for ( vector< string >::const_iterator i=d.begin(); i!=d.end() ; ++i )
    {
      // cout << "  *i=" << *i << endl;
      string::size_type pos = (*i).find (":");
      if ( pos == string::npos )
      {
        t["finder"]=(*i);
      } else {
        string name = (*i).substr ( 0, pos );
        MultiType value = (*i).substr ( pos+1, (*i).size() );
        value.trimType();
        t[name]=value;
      };
    };
  }
}

ParameterScanner::ParameterScanner() : theNVertices(0)
{}

ParameterScanner::~ParameterScanner()
{
  /*
  cout << endl
       << "SimpleScore conf=" << theConf << ": " << theScore.current() << endl;
       */
  if ( theConf != "" )
  {
    Tuple tuple ("data");
    Measurement1D eff = theScore.efficiency();
    Measurement1D fake = theScore.fake();
    tuple["eff"]=eff.value();
    tuple["sigma_eff"]=eff.error();
    tuple["fake"]=fake.value();
    tuple["sigma_fake"]=fake.error();
    getDescription ( theScore.algorithm(), tuple );
    tuple["method"]=theScore.algorithm();
    tuple["nvertices"]=(int) theNVertices;
    tuple.fill();
    Mode::FileMode mode= Mode::Update;
    string fm = SimpleConfigurable<string>("Recreate",
        "Mode:FileMode").value();
    if ( fm == "Recreate" )
    {
      mode = Mode::Recreate;
    }

    dataharvester::Writer::file ( theConf, mode ) << tuple;
    dataharvester::Writer::close();
  }
}

ParameterScanner * ParameterScanner::clone() const
{
  return new ParameterScanner ( *this );
}

void ParameterScanner::configure( const string & s )
{
  theConf=s;
}

void ParameterScanner::process ( const exception & e, const string & )
{
}

void ParameterScanner::process ( const vertigo::Event & e )
{
  theScore ( e );
  theNVertices+=e.vertices().size();
}

#include "Analysis/Observer/interface/ObserverBuilder.h"

namespace {
  ObserverBuilder<ParameterScanner> t( "ParameterScanner", "Class for scanning configurable parameter of an algo.", false );
}
