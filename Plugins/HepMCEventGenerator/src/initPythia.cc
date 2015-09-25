//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, April 2007
//
// Initialize pythia
// Using a separate cc file allows us to test issues with PythiaWrapper.h
//
//////////////////////////////////////////////////////////////////////////

#include "initPythia.h"
#include <iostream>
#include <cstdlib>

using namespace std;

namespace {
  inline void Wgamma()
  {
    cout << "[initPythia] Wgamma!" << endl;
    // Select W+gamma process (process number 20) 
    // (here we have to be careful of C/F77 differences: arrays in C 
    //  start at 0, F77 at 1, so we need to subtract 1 from the process #)
    pysubs.msub[20-1] = 1; // W+gamma

    // Call pythia initialization
  }

  inline void H4mu()
  {
    cout << "[initPythia] H4mu!" << endl;
    pysubs.msub[123-1]=0; // H -> tau tau ??  // H -> Z Z
    pysubs.msub[124-1]=0; // H -> b b ??
    pysubs.msub[24-1]=0; // H -> b b ??
    pysubs.msub[26-1]=0; // -> W h
    pysubs.msub[3-1]=1; // -> h
    pysubs.msub[102-1]=0; // H -> tau tau ??
    pydat3.mdme[1-1][213-1] = 1;  // H  -> c c
    pydat3.mdme[1-1][225-1] = 0;  // H  -> Z Z
    pydat3.mdme[1-1][174-1] = 0;  //
    pydat3.mdme[1-1][175-1] = 0;  //
    pydat3.mdme[1-1][176-1] = 0;  //
    pydat3.mdme[1-1][177-1] = 0;  //
    pydat3.mdme[1-1][178-1] = 0;  //
    pydat3.mdme[1-1][179-1] = 0;  //
    pydat3.mdme[1-1][180-1] = 0;  //
    pydat3.mdme[1-1][181-1] = 0;  //
    pydat3.mdme[1-1][182-1] = 0;  // H  -> Z Z -> 4e
    pydat3.mdme[1-1][183-1] = 0;  //
    pydat3.mdme[1-1][184-1] = 0;  // H  -> Z Z -> 4mu
    pydat3.mdme[1-1][185-1] = 0;  //
    pydat3.mdme[1-1][186-1] = 0;  //
    pydat3.mdme[1-1][187-1] = 0;  //
    pydat3.mdme[1-1][188-1] = 0;  //
    pydat3.mdme[1-1][189-1] = 0;  //
  }
}

void initPythia( const std::string & name )
{
    // (Some platforms may require the initialization of pythia PYDATA block 
    //  data as external - if you get pythia initialization errors try 
    //  commenting in/out the below call to initpydata() )
    // initpydata();
    pysubs.msel=0; 
    
    bool hit=false;

    if ( name == "Wgamma" )
    {
      Wgamma();
      hit=true;
    }

    if ( name == "H4mu" )
    {
      H4mu();
      hit=true;
    }

    if (!hit)
    {
      cout << "[initPythia] didnt recognize config " << name 
           << endl;
      exit(0);
    }

    // set random number seed (mandatory!)
    pydatr.mrpy[0]=55122 ;
    // Tell Pythia not to write multiple copies of particles in event record.
    pypars.mstp[128-1] = 2;
    // Example of setting a Pythia parameter: set the top mass 
    pydat2.pmas[1-1][6-1]= 175;  
    pydat2.pmas[25-1][1-1]= 140;   // higgs mass
    //
    // Call pythia initialization
    call_pyinit( "CMS", "p", "p", 7000. );
}
