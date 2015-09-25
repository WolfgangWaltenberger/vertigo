/*!
 * @file HarvestTest.cpp
 * @brief Test programm for the RaveObjectHarvester / -Seeder
 *
 * This program serialises Rave objects, via the DataHarvester
 * framework. It then reads the objects back into objects.
 * And compares original objects with copies.   
 *
 */

#include "RaveBase/RaveInterface/interface/RaveTrack.h"
#include "Vertigo/VertigoHarvester/interface/RaveObjectHarvester.h"
#include "Vertigo/VertigoHarvester/interface/RaveObjectSeeder.h"
#include "Vertigo/VertigoHarvester/interface/RaveEvent.h"
#include "Geometry/Vector/interface/GlobalPoint.h"
#include "Geometry/Vector/interface/GlobalVector.h"
#include "TerminateException.h"

#include <vector>
#include <iostream>

namespace {
  void write()
  {
    GlobalPoint pos ( 1., 1., 1. );
    GlobalVector mom ( 2., 2., 2. );
    RaveCovariance6D err ( .1, .0, .0, .1, .0, .1, 0., 0., 0., 0., 0., 0.,
        0., 0. 0., .1, .0 , .0 , .1 , 0. , 1. );
    RaveVector6D st ( 1., 1., 1., 2., 2., 2. );
    rave::Track t ( st, err, 1, 0, "test" );
    RaveEvent evt ( 23 );
    evt.add ( t );
    RaveObjectHarvester harvest ( "test.txt" );
    harvest.save ( evt );
  }

  void readEvent( RaveObjectSeeder & seed )
  {
      RaveEvent e=seed.next();
      std::vector < rave::Track > trks=e.tracks();
      std::cout << "[HarvestTest] received event #" << e.id()
                << ":  " << trks.size() << " tracks." << endl;
  }

  void read()
  {
    RaveObjectSeeder seed ( "test.txt" );
    while ( true )
    {
      try {
        readEvent ( seed );
      } catch ( harvest::TerminateException & ) {
        cout << "[HarvestTest] no more events." << endl;
        break;
      }
    }
  }
}

int main ( int argc, char * argv[] )
{
  write();
  read();
}
