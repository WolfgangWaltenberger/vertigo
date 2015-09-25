#include "Analysis/Score/interface/SimpleScore.h"
#include <iostream>

using namespace std;


namespace {
  int debug() { return 0; }
}

SimpleScore::SimpleScore() : theFound(0), theFake(0), theN(0),
    theMethod ("")
{}

void SimpleScore::operator() ( const vertigo::Event & e )
{
  if ( theN == 0 ) theMethod = e.algorithm();
  vector < vertigo::SimVertex > svtces = e.simVertices();

  for ( vector< vertigo::SimVertex >::const_iterator i=svtces.begin(); 
        i!=svtces.end() ; ++i )
  {
    if ( e.isReconstructible ( *i ) )
    {
      theN++; // count rectible simvtces
      if ( debug() )
      {
        std::cout << "[SimpleScore::debug] " << i->id() << " reconstructible. n=" << theN
             << " now." << std::endl;
      }

      /*
      vector < rave::Vertex > ass = e.associatedVertices ( *i );
      if ( ass.size() == 0 )
      {
        //  sim vertex has associated rec vertex
        theFake++;
      } else {
        theFound++;
      }*/
    }
  }

  set < vertigo::SimVertex > ass_simvtces;
  vector < rave::Vertex > vtces = e.vertices();
  for ( vector< rave::Vertex >::const_iterator i=vtces.begin(); 
        i!=vtces.end() ; ++i )
  {
    vector < vertigo::SimVertex > ass = e.associatedVertices ( *i );
    if ( ass.size() == 0 )
    {
      // rec vertex has no associated sim vertex
      theFake++;
      if ( debug() )
      {
        cout << "[SimpleScore::debug] " << i->id() << " has no ass sim vtx."
             << " fake=" << theFake << " now." << endl;
      }
    } else {
      if ( !(e.isReconstructible( ass[0] ) ) )
      {
        // association to a non-rectible sim vertex is punished, as well.
        theFake++;
        if ( debug() )
        {
          cout << "[SimpleScore::debug] " << i->id() << " has no rectible ass sim vtx."
               << " fake=" << theFake << " now." << endl;
        }
      } else {
        // a reconstructible sim vertex.
        // lets see if we have associated it already.
        if ( ass_simvtces.count( ass[0] )  )
        {
          // we have. so we have a fake.
          theFake++;
          if ( debug() )
          {
            cout << "[SimpleScore::debug] " << i->id() << " associated sim vtx " << ass[0].id()
                 << " has been associated before."
                 << " fake=" << theFake << " now." << endl;
          }
        } else {
          // no, a newly associated sim vertex.
          ass_simvtces.insert ( ass[0] );
          if ( debug() )
          {
            cout << "[SimpleScore::debug] " << i->id() << " is good."
                 << " found=" << theFound << " now." << endl;
          }
          theFound++;
        }
      }
    }
  }

  // now add all non-assigned simvtces.
  // theFake+=svtces.size()-ass_simvtces.size();
}

const string & SimpleScore::algorithm() const
{
  return theMethod;
}

Measurement1D SimpleScore::efficiency() const
{
  float eff = (float) ( theFound ) / (float) (theN);
  float sig = sqrt ( eff * ( 1. - eff ) / (float) (theN) );
  return Measurement1D ( eff, sig );
}

Measurement1D SimpleScore::fake() const
{
  float n = theN + theFake; // or without + theFake, if you wish.
  float fake = (float) ( theFake ) / n;
  float sig = sqrt ( fake * ( 1. - fake ) / n );
  return Measurement1D ( fake, sig );
}

SimpleScore * SimpleScore::clone() const
{
  return new SimpleScore ( *this );
}

SimpleScore::~SimpleScore() {}
