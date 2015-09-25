#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <rave/Logger.h>
#include <sstream>

using namespace std;

string LoopSettings::method() const
{
  return theMethod;
}

string LoopSettings::flavorTagMethod() const
{
  return theFlavorTagMethod;
}

string LoopSettings::source() const
{
  return theSource;
}

int LoopSettings::verbosity() const
{
  return theVerbosity;
}

void LoopSettings::setVerbosity ( int v, bool silent )
{
  //cout << "[LoopSettings] setVerbosity " << v << endl;
  // vexit(0);
  theVerbosity=v;
  rave::Logger::Level ravelevel=(rave::Logger::Level) (v/5);
  rave::Logger::setLogLevel ( ravelevel );
  if ( silent ) return;
  edm::LogInfo("LoopSettings") << "Verbosity is " << v << " (" << ravelevel << " in rave units).";
  ostringstream ostr;
  ostr << "Will log ImportantInfo";
  if ( ravelevel >= 0 )
  {
    ostr << ", Error";
    if ( ravelevel >= 1 )
    {
      ostr << ", Warning";
      if ( ravelevel >= 2 )
      {
        ostr << ", Info";
        if ( ravelevel >= 3 )
        {
          ostr << ", Debug";
        }
      }
    }
  }
  edm::LogInfo("LoopSettings") << ostr.str() << "."; 
}

int LoopSettings::min() const
{
  return theMin;
}

int LoopSettings::max() const
{
  return theMax;
}

bool LoopSettings::useBeamSpot() const
{
  return theUseBeamSpot;
}

bool LoopSettings::performFlavorTagging() const
{
  return thePerformFlavorTagging;
}

void LoopSettings::setPerformFlavorTagging( bool p )
{
  thePerformFlavorTagging=p;
}

LoopSettings::LoopSettings() : theSource(""), theMethod(""), theFlavorTagMethod(""),
                               theMin(-1), theMax(-1), theVerbosity(10), 
                               theUseBeamSpot ( false ), thePerformFlavorTagging ( false ),
                               theVtxFactory ( 0 ), theFlavorTagFactory ( 0 ),
                               theEventFactory ( EventFactory ("") ), theNThreads ( 1 ),
                               theFancyCounter ( true ), theBarLength ( 50 ), theExpect ( 0 ),
                               theLogToFile ( false )
{
  setVerbosity ( theVerbosity, true );
}

void LoopSettings::setBarLength ( int n )
{
  theBarLength=n;
}

void LoopSettings::setLogToFile ( bool n )
{
  theLogToFile=n;
}

int LoopSettings::nThreads() const
{
  return theNThreads;
}

const rave::VertexFactory * LoopSettings::vertexFactory() const
{
  return theVtxFactory;
}

rave::VertexFactory * LoopSettings::nonConstVertexFactory() const
{
  return const_cast < rave::VertexFactory * > (theVtxFactory);
}

rave::FlavorTagFactory * LoopSettings::flavorTagFactory() const
{
  return theFlavorTagFactory;
}

void LoopSettings::setVertexFactory ( const rave::VertexFactory * v )
{
  theVtxFactory=v;
}

void LoopSettings::setFlavorTagFactory ( rave::FlavorTagFactory * f )
{
  theFlavorTagFactory=f;
}

void LoopSettings::setEventFactory ( const EventFactory & f )
{
  theEventFactory=f;
}

const EventFactory & LoopSettings::eventFactory() const
{
  return theEventFactory;
}

int LoopSettings::expect() const
{
  return theExpect;
}

bool LoopSettings::logToFile() const
{
  return theLogToFile;
}

void LoopSettings::set ( const std::string & source, const std::string & method,
    const string & flavortagmethod, int min, int max, int verbosity, 
    bool usebeamspot, int nthreads, int expect )
{
  theSource=source;
  theMethod=method;
  theFlavorTagMethod=flavortagmethod;
  theMin=min;
  theMax=max;
  setVerbosity ( verbosity );
  theUseBeamSpot = usebeamspot;
  thePerformFlavorTagging=false;
  if ( flavortagmethod != "" && flavortagmethod !="none" ) thePerformFlavorTagging=true;
  theNThreads=nthreads;
  theExpect=expect;
}

bool LoopSettings::fancyCounter() const
{
  return theFancyCounter;
}

int LoopSettings::barLength() const
{
  return theBarLength;
}

LoopSettings & LoopSettings::Instance()
{
  static LoopSettings ret;
  return ret;
}


