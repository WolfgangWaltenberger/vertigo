// C++ Implementation: TestAnalyser
// Description:
// Copyright: See COPYING file that comes with this distribution

#include "TestAnalyser.h"
#include <numeric>

#include <rave/TransientTrackKinematicParticle.h>
#include <rave/KinematicTreeFactory.h>

#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>

#include "boost/bind.hpp"

#include <iostream>
#include <sstream>
#include <cassert>
#include "Environment/MCObjects/interface/PDGCodes.h"

#define WITH_HARVESTER
#ifdef WITH_HARVESTER
#include <dataharvester/Tuple.h>
#include <dataharvester/Writer.h>
#endif

using namespace std;

#include "Math/LorentzVector.h"
#include "Math/GenVector/PxPyPzM4D.h"

namespace {
  float eta ( const rave::Vector4D & v )
  {
    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzM4D<double> > l ( v.x(), v.y(), v.z(), v.m() );
    return l.Rapidity();
    // return l.eta();
  }

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
  }

  map < int, int > mypids()
  {
    int pids[] = { 1,2,3,4,5,6,11,12,13,14,15,16,22,24,1000001,\
      1000002,1000003,1000004,1000005,1000006,1000021,\
        1000022,1000023,1000024,1000025,1000012,1000014,1000016 };
    // int pids[] = { 24, 1000021, 1000024, 1000022 };
    map < int, int > ret;
    for ( unsigned i=0; i< sizeof ( pids ) / 4 ; i++ )
    {
      ret[ pids[i] ] = 0;
      ret[ -pids[i] ] = 0;
    }
    return ret;
  }

  map < int, string > pNames()
  {
    static map < int, string > ret;
    static map < string, int > ret2;
    static bool filled=false;
    if (!filled)
    {
      ret[21]="gluon";
      ret[23]="Z";
      ret[24]="W";
      ret[11]="e";
      ret[12]="nue";
      ret[13]="mu";
      ret[14]="numu";
      ret[15]="tau";
      ret[16]="nutau";
      ret[1]="d";
      ret[2]="u";
      ret[3]="s";
      ret[4]="c";
      ret[5]="b";
      ret[6]="t";
      ret[1000021]="gluino";
      ret2["gluino"]=1000021;
      ret[1000022]="chi10";
      ret2["chi10"]=1000022;
      ret[1000023]="chi20";
      ret2["chi20"]=1000023;
      ret[1000024]="chi1p";
      ret2["chi1p"]=1000024;
      ret[1000025]="chi30";
      ret2["chi30"]=1000025;
      ret[1000037]="chi2p";
      ret2["chi2p"]=1000037;
      ret[1000001]="sdownL";
      ret[2000001]="sdownR";
      ret[1000002]="supL";
      ret[2000002]="supR";
      ret[1000003]="sstrangeL";
      ret[2000003]="sstrangeR";
      ret[1000004]="scharmL";
      ret[2000004]="scharmR";
      ret[1000005]="sbottomL";
      ret[2000005]="sbottomR";
      ret[1000006]="stopL";
      ret[2000006]="stopR";
      ret[1000011]="selL";
      ret[2000011]="selR";
      ret[1000013]="smuL";
      ret[2000013]="smuR";
      ret[1000015]="stauL";
      ret[2000015]="stauR";
      ret[1000012]="snu_elL";
      ret[2000012]="snu_elR";
      ret[1000014]="snu_muL";
      ret[2000014]="snu_muR";
      ret[1000016]="snu_tauL";
      ret[2000016]="snu_tauR";
      filled=true;
    }
    return ret;
  }

}

void TestAnalyser::lheSMS ( const std::string & t, bool debug ) const
{
  // map < string, float > smsParams;
  size_t foundLength = t.size();
  size_t found = t.find("T");
  size_t found_ = t.find("_");

  if (debug)
  {
    cout << "[ModelParameters] interpreting ``"<< t.substr(0,t.size()-1) << "'' as an SMS model string"
         << endl;
  }

  std::string smaller = t.substr(found+1,foundLength);
  std::string mmodel = t.substr(found,found_-found);
  found = smaller.find("_");
  smaller = smaller.substr(found+1,smaller.size());

  std::istringstream iss(smaller);
  double xCHI;
  iss >> xCHI;
  iss.clear();
  //
  found = smaller.find("_");
  smaller = smaller.substr(found+1,smaller.size());
  iss.str(smaller);
  double mGL;
  iss >> mGL;
  iss.clear();
  double mSQ=mGL;
  //
  found = smaller.find("_");
  smaller = smaller.substr(found+1,smaller.size());
  iss.str(smaller);
  double mLSP;
  iss >> mLSP;
  iss.clear();
  
  float mchi=xCHI * mLSP + ( 1. - xCHI ) * mGL;

  if ( mmodel == "T2" || mmodel=="T2tt" )
  {
    mLSP = mGL;
    mGL = xCHI;
    xCHI=0.;
    mchi=0.;
  }

  if ( mmodel == "TChizz" || mmodel == "TChiwz" )
  {
    mGL = xCHI;
  }

  if ( mmodel == "TGQ" )
  {
    mSQ=mLSP;
    mLSP=xCHI*mSQ;
    mchi=0.;
  }
  smsParams_["mgl"]=mGL;
  smsParams_["msq"]=mSQ;
  smsParams_["mn"]=mLSP;
  smsParams_["xfrac"]=xCHI;
  smsParams_["mc"]= mchi;
  if (debug)
  {
    cout << "[ModelParameters] ==> gluino " << mGL 
         << " squark " << mSQ << " mc " << mchi 
         << " LSP " << mLSP  << " xFrac "<< xCHI << endl;
  }
  if ( !isfinite ( mGL ) || !isfinite (mLSP ) || !isfinite(xCHI) )
  {
    cout << "[ModelParameters] sth went wrong with the parsing of the comment line"
         << endl
         << "                  mgl=" << mGL << " mlsp=" << mLSP
         << " mchi=" << mchi << endl;
  }
}

TestAnalyser::TestAnalyser()
  : Observer(), theFileName( "pids.txt;pids.root" ),
    theTree ( false ), writePids_ ( true ),
    massesPerEvent_ ( false ), smsMasses_( false ), theCtr ( 0 )
{}

void TestAnalyser::storePts() const
{
  #ifdef WITH_HARVESTER
  map < int, string > pn = pNames();
  assert ( pts_.size() == rapidities_.size() );
  assert ( pts_.size() == masses_.size() );

  for ( map < int, std::vector < float > >::const_iterator i=pts_.begin();
        i!=pts_.end() ; ++i )
  {
    int pid = i->first;
    if ( pid < 1000000 ) continue;
    vector < float > pts = i->second;
    vector < float > raps = rapidities_[i->first];
    vector < float > masses = masses_[i->first];
    assert ( pts.size() == raps.size() );
    vector< float >::const_iterator rj=raps.begin();
    vector< float >::const_iterator mj=masses.begin();
    for ( vector< float >::const_iterator j=pts.begin(); j!=pts.end() ; ++j )
    {
      dataharvester::Tuple t("particle");
      t["eventnr"]=theCtr;
      t["pid"]=pid;
      t["pt"]=(*j);
      t["m"]=(*mj);
      t["smsmC"]=smsParams_["mgl"];
      t["smsmN"]=smsParams_["mn"];
      t["eta"]=(*rj);
      dataharvester::Writer::file ( theFileName ) << t;
      ++rj; ++mj;
    }
  }
  #endif
}

void TestAnalyser::storeMasses() const
{
  #ifdef WITH_HARVESTER
  dataharvester::Tuple t("masses");
  if ( masses_.size() > 0 ) t["eventnr"]=theCtr;
  map < int, string > pn = pNames();
  map < int, float > avgmasses;
  for ( map < int, std::vector < float > >::const_iterator i=masses_.begin();
        i!=masses_.end() ; ++i )
  {
    float m = accumulate ( i->second.begin(), i->second.end(), 0. ) / i->second.size();
    avgmasses[i->first]=m;
    if ( i->first < 1000000) continue;
    ostringstream o;
    o << pn [i->first];
    t[o.str()]=m;
  }
  int mglid = 1000021;
  int mcid = 1000024;
  int mnid = 1000022;
  if ( (masses_.count (mnid)) && (masses_.count(mcid)) && (masses_.count(mglid)) ) 
  {
    float mgl=avgmasses[mglid];
    float mc=avgmasses[mcid];
    float mn=avgmasses[mnid];
    float xfrac=(mgl-mc)/(mgl-mn);
    /*
    static bool first=false;
    if ( first ) // && ( (mgl-mn)>350 ) && (xfrac > 0.9 || xfrac<0.1) )
    {
      cout << "[TestAnalyser] event #" << theCtr <<  " mgl=" << mgl
           << " mc=" << mc << " mn=" << mn << " x=" << xfrac << endl;
      first=false;
    }*/
    t["xfrac"]=xfrac;
  }
  dataharvester::Writer::file ( theFileName ) << t;
  #endif
}

TestAnalyser::~TestAnalyser()
{
  #ifdef WITH_HARVESTER
  dataharvester::Writer::close();
  #endif
}

void TestAnalyser::printOld ( const vertigo::Event & e ) const
{
  // a simple version
  vector < vertigo::SimTrack > simtracks = e.simTracks();
  cout << "[TestAnalyser] we have " << simtracks.size() << " simtracks: ";
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
  cout << "[TestAnalyser] we have " << simvertices.size() << " simvertices: ";
  for ( vector< vertigo::SimVertex >::const_iterator i=simvertices.begin();
        i!=simvertices.end() ; ++i )
  {
    if ( i->pid() == 0 ) continue;
    cout << PDGCodes::name ( i->pid() );
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

void TestAnalyser::printVertex ( const vertigo::SimVertex & v ) const
{
  cout << "[Vtx] " << PDGCodes::name( v.pid() ) << " -> ";
  vector < vertigo::SimTrack > trks = v.tracks();
  for ( vector< vertigo::SimTrack >::const_iterator i=trks.begin();
        i!=trks.end() ; ++i )
  {
    if ( i->pid() == 0 ) continue;
    cout << PDGCodes::name ( i->pid() ) << " ";
  }
  cout << endl;
}

void TestAnalyser::print ( const vertigo::Event & e ) const
{
  cout << "[Event]" << endl;
  vector < vertigo::SimTrack > simtracks = e.simTracks();
  bool has_pid0=false;
  for ( vector< vertigo::SimTrack >::const_iterator i=simtracks.begin();
        i!=simtracks.end() ; ++i )
  {
    if ( i->pid() != 0 ) continue;
    if ( !(i->vertex()) ) continue;
    printVertex ( *(i->vertex()) );
    has_pid0=true;
  }
  printOld ( e );

  if (!has_pid0) printOld ( e );
}
void TestAnalyser::storeSmsMasses ( const vertigo::Event & e ) const
{
  string tg = e.tag();
  if ( tg.size() == 0 ) return;
  bool debug=false;
  lheSMS ( tg, debug );

  #ifdef WITH_HARVESTER
  dataharvester::Tuple t("sms");
  t["eventnr"]=theCtr;
  for ( map< string, float >::const_iterator i=smsParams_.begin(); 
        i!=smsParams_.end() ; ++i )
  {
    t["sms"+i->first]=i->second;
  }
  dataharvester::Writer::file ( theFileName ) << t;
  #endif
}

void TestAnalyser::store ( const vertigo::Event & e ) const
{
  map < int, int > pids = mypids();
  vector < vertigo::SimTrack > simtracks = e.simTracks();
  for ( vector< vertigo::SimTrack >::const_iterator i=simtracks.begin();
        i!=simtracks.end() ; ++i )
  {
    int p = i->pid();
    if ( !(pids.count ( p ) ) )
    {
      if ( p > 1000000 )
      {
        cout << "[TestAnalyser] warning: pid " << p << " goes unobserved?" << endl;
      }
      continue;
    }
    pids[p]=pids[p]+1;
  }
  vector < vertigo::SimVertex > simvertices = e.simVertices();
  for ( vector< vertigo::SimVertex >::const_iterator i=simvertices.begin();
        i!=simvertices.end() ; ++i )
  {
    int p = i->pid();
    if ( !(pids.count ( p ) ) )
    {
      if ( p > 1000000 )
      {
        cout << "[TestAnalyser] warning: pid " << p << " goes unobserved?" << endl;
      }
      continue;
    }
    pids[p]=pids[p]+1;
  }

  #ifdef WITH_HARVESTER
  dataharvester::Tuple t("pids");
  t["eventnr"]=theCtr;
  map < int, string > names = pNames();
  for ( map < int, int >::const_iterator i=pids.begin(); i!=pids.end() ; ++i )
  {
    stringstream s;
    int p = abs ( i->first );
    if ( names.count ( p ) )
    {
      string name=names[p];
      if ( i->first < 0 )
      {
        name=name+"_bar";
      }
      t[name]=i->second;
    }
    if ( i->first < 0 )
    {
      s << "N" << p; // negative
    } else {
      s << "P" << p; // positive
    }
    t[s.str()]=i->second;
    stringstream s2;
    s2 << "U" << p; // unsigned = negative + positive
    t[s2.str()]=i->second + pids[-(i->first)];
  }
  dataharvester::Writer::file ( theFileName ) << t;
  #endif

  vector < vertigo::SimParticle > simparticles = e.simParticles();
  for ( vector< vertigo::SimParticle>::const_iterator i=
        simparticles.begin(); i!=simparticles.end() ; ++i )
  {
    masses_[abs(i->pid())].push_back ( i->p4().m() );
    pts_[abs(i->pid())].push_back ( i->p4().perp() );
    rapidities_[abs(i->pid())].push_back ( eta ( i->p4() ) );
  }
}

void TestAnalyser::process ( const vertigo::Event & e )
{
  theCtr++;
  if ( theTree )
  {
    print ( e );
  };

  if ( writePids_ ) store ( e );
  if ( smsMasses_ ) storeSmsMasses ( e );

  if ( massesPerEvent_ )
  {
    vector < vertigo::SimParticle > simparticles = e.simParticles();
    for ( vector< vertigo::SimParticle>::const_iterator i=
          simparticles.begin(); i!=simparticles.end() ; ++i )
    {
      masses_[abs(i->pid())].push_back ( i->p4().m() );
    }
    storeMasses();
    storePts();
    masses_.clear();
    smsParams_.clear();
    pts_.clear();
    rapidities_.clear();
  }
}

void TestAnalyser::process ( const std::exception & e, const std::string & type )
{
  except ( e.what(), -1 );
}

void TestAnalyser::configure( const std::string & s )
{
  vector < string > tokens = getToken ( s );
  for ( vector< string >::const_iterator i=tokens.begin(); 
        i!=tokens.end() ; ++i )
  {
    if ( (*i) == "tree" )
    {
      theTree=true;
      continue;
    }
    if ( (*i) == "masses" )
    {
      massesPerEvent_=true;
      continue;
    }
    if ( (*i) == "sms" )
    {
      smsMasses_=true;
      continue;
    }
    if ( (*i) == "pids" )
    {
      writePids_=true;
      continue;
    }
    if ( (*i) == "nopids" )
    {
      writePids_=false;
      continue;
    }
    if ( i->find("file:")!=string::npos )
    {
      theFileName = i->substr(5,-1);
      // cout << "[TestAnalyser] filename=" << theFileName << endl;
    }
  }
}

void TestAnalyser::except( const std::string & e, int id )
{
}

TestAnalyser * TestAnalyser::clone() const
{
  return new TestAnalyser ( *this );
}

#include "Analysis/Observer/interface/ObserverBuilder.h"

namespace
{
ObserverBuilder<TestAnalyser> TestAnalyserBuilder (
  "TestAnalyser", "Temporary code, to try things out", false );
}

