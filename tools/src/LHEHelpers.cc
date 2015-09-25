#include "tools/interface/LHEHelpers.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <numeric>

using namespace std;

string LHEHelpers::getModelName ( const string & t )
{
  size_t found = t.find("T");
  size_t found_ = t.find("_");
  string mmodel = t.substr(found,found_-found);
  return mmodel;
}

map < string, float > LHEHelpers::getSMSMasses( const string & t, bool debug )
{
  map < string, float > smsParams;
  size_t foundLength = t.size();
  size_t found = t.find("T");
  size_t found_ = t.find("_");

  if (debug)
  {
    cout << "[ModelParameters] interpreting ``"<< t.substr(0,t.size()-1) << "'' as an SMS model string"
         << endl;
  }

  string smaller = t.substr(found+1,foundLength);
  string mmodel = t.substr(found,found_-found);
  found = smaller.find("_");
  smaller = smaller.substr(found+1,smaller.size());

  istringstream iss(smaller);
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
  smsParams["mgl"]=mGL;
  smsParams["msq"]=mSQ;
  smsParams["mn"]=mLSP;
  smsParams["xfrac"]=xCHI;
  smsParams["mc"]= mchi;
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
  return smsParams;
}
