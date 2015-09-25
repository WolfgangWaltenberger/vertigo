// C++ Implementation: ParticleDecayPrinter
// Description:
// Copyright: See COPYING file that comes with this distribution

#include "ParticleDecayPrinter.h"
#include <sstream>
#include <iostream>
#include "Environment/MCObjects/interface/PDGCodes.h"

using namespace std;

namespace {
  /*
  vector < string > getToken ( string s )
  {                                                                                       vector < string > ret;
    // splits Adaptive-cut:3.0 into "Adaptive", "cut:3.0"                                 // code is the same as in MultiVertexHistogrammer
    while ( s.find ("-") != string::npos )
    {
      string::size_type pos = s.find ( "-" );
      string sub = s.substr ( 0, pos );
      s=s.substr ( pos+1, s.size() );
      ret.push_back ( sub );
    };
    ret.push_back ( s );                                                                  return ret;
  } */
}

ParticleDecayPrinter::ParticleDecayPrinter()
  : Observer()
{}


ParticleDecayPrinter::~ParticleDecayPrinter()
{
}

void ParticleDecayPrinter::printOld ( const vertigo::Event & e ) const
{
  // a simple version
  vector < vertigo::SimTrack > simtracks = e.simTracks();
  cout << "[ParticleDecayPrinter] we have " << simtracks.size() << " simtracks: ";
  for ( vector< vertigo::SimTrack >::const_iterator i=simtracks.begin();
        i!=simtracks.end() ; ++i )
  {
    if ( i->pid() == 0 ) continue;
    cout << PDGCodes::name ( i->pid() );
    if ( i->vertex() )
    {
      cout << "<-" << PDGCodes::name ( i->vertex()->pid() ) << "";
    }
    if ( i!=simtracks.end()-1 ) cout << ", ";
  }
  cout << endl;
  vector < vertigo::SimVertex > simvertices = e.simVertices();
  cout << "[ParticleDecayPrinter] we have " << simvertices.size() << " simvertices: ";
  for ( vector< vertigo::SimVertex >::const_iterator i=simvertices.begin();
        i!=simvertices.end() ; ++i )
  {
    if ( i->pid() == 0 ) continue;
    cout << PDGCodes::name ( i->pid() );
    if ( i->isPrimary() ) cout << "[P]";
    vector < vertigo::SimTrack > trks = i->tracks();
    if ( trks.size() )
    {
      cout << " ->";
      for ( vector< vertigo::SimTrack >::const_iterator i=trks.begin();
            i!=trks.end() ; ++i )
      {
        if ( i->pid() == 0 ) continue;
        cout << " " << PDGCodes::name ( i->pid() );
      }
      cout << "";
    }
    if ( i!=simvertices.end()-1 ) cout << ", ";
  }
  cout << endl;
}

string ParticleDecayPrinter::printVertex ( const vertigo::SimVertex & v ) const
{
  ostringstream ostr;
  ostr << PDGCodes::name( v.pid() ) << " ->";
  vector < vertigo::SimTrack > trks = v.tracks();
  for ( vector< vertigo::SimTrack >::const_iterator i=trks.begin();
        i!=trks.end() ; ++i )
  {
    if ( i->pid() == 0 ) continue;
    ostr << " " << PDGCodes::name ( i->pid() );
    /*
    string trk = printTrack ( *i );
    if ( trk!="" )
    {
      ostr << "(" << trk << ")  ";
    }*/
  }
  return ostr.str();
}

string ParticleDecayPrinter::printTrack ( const vertigo::SimTrack & v ) const
{
  ostringstream ostr;
  // if ( v.pid() == 0 ) return;
  // ostr << PDGCodes::name ( v.pid() );
  if ( v.vertex() )
  {
    ostr << "<-" << PDGCodes::name ( v.vertex()->pid() );
  }
  return ostr.str();
}

void ParticleDecayPrinter::print ( const vertigo::Event & e ) const
{
  cout << "[Event]" << endl;
  vector < vertigo::SimVertex > simvertices = e.simVertices();
  for ( vector< vertigo::SimVertex >::const_iterator i=simvertices.begin();
        i!=simvertices.end() ; ++i )
  {
    if ( i->isPrimary() ) cout << printVertex ( *i) << endl;
  }
  /*
  vector < vertigo::SimTrack > simtracks = e.simTracks();
  for ( vector< vertigo::SimTrack >::const_iterator i=simtracks.begin();
        i!=simtracks.end() ; ++i )
  {
    // if ( i->vertex() && i->vertex()->pid()==0 ) printTrack ( *i );
    if ( i->vertex() ) printTrack ( *i );
  }
  */
  printOld ( e );
}

void ParticleDecayPrinter::process ( const vertigo::Event & e )
{
  print ( e );
}

void ParticleDecayPrinter::process ( const std::exception & e, 
                                     const std::string & type )
{
  except ( e.what(), -1 );
}

void ParticleDecayPrinter::configure( const std::string & s )
{
  /*
  vector < string > tokens = getToken ( s );
  for ( vector< string >::const_iterator i=tokens.begin(); 
        i!=tokens.end() ; ++i )
  {
  }*/
}

void ParticleDecayPrinter::except( const std::string & e, int id )
{
}

ParticleDecayPrinter * ParticleDecayPrinter::clone() const
{
  return new ParticleDecayPrinter ( *this );
}

#include "Analysis/Observer/interface/ObserverBuilder.h"

namespace
{
ObserverBuilder<ParticleDecayPrinter> ParticleDecayPrinterBuilder (
  "ParticleDecayPrinter", "Print the particle decay chains",
  false );
}

