#ifndef Workspace_Primitives_XSection_H
#define Workspace_Primitives_XSection_H

#include <map>
#include <iostream>
#include <string>

class XSection
{
public:
  XSection();

  /// if we need to interpolate, how do we interpolate?
  /// Lower: take xsec of lower particle mass ( i.e. higher! xsec)
  enum InterpolationType { Lower=0, Interpolate=1, Higher=2 };
  /**
   *  get the leading order cross section [in pb] of sample named "sample"
   *  \paramname sample Samplename, e.g. LM1
   */
  double getLOXsec( std::string sample );
  std::map<std::string, double> getLOXsecs() const;

  /**
   *  get the event generator's filter efficiency of "sample"
   */
  double getFeff( std::string sample );
  std::map<std::string, double> getFeffs() const;

  /**
   *  get the k-factor of "sample"
   */
  double getKfct( std::string sample );
  std::map<std::string, double> getKfcts() const;

  double getNLOEffXsec( std::string name );

  /** returns the weight that normalizes to lumi pb^-1, given nevents
   */
  double getWeight( const std::string & name, double lumi, int nevents );

  void setEntry( std::string name, double xsec, double feff, double kfct );

  /// gg cross section, given the gluino mass
  float xSecGG( float mgl, InterpolationType i = Interpolate ) const;

  /// get the gluino mass, given the ~g~g cross section
  float mGluino ( float xsecGG ) const;
  
  /// get the squark mass, given the ~q~q cross section
  float mSquark ( float xsecQQ ) const;
  
  /// get the stop mass, given the ~t~t cross section
  float mStop ( float xsecTT ) const;

  /// ~g~q cross section, given the gluino and the squark masses
  float xSecGQ( float mgl, float msq, InterpolationType i = Interpolate ) const;
  /// ~q~q cross section, given the squark mass
  float xSecQQ( float msq, InterpolationType i = Interpolate ) const;
  
  /// ~t~t cross section, given the stop mass
  float xSecTT( float mst, InterpolationType i = Interpolate ) const;

  /// ~b~b cross section, given the sbottom mass
  float xSecBB( float msb, InterpolationType i = Interpolate ) const;

private:
  std::map<std::string, double> xsec_;
  std::map<std::string, double> feff_;
  std::map<std::string, double> kfct_;

};

std::ostream& operator<<( std::ostream& out, const XSection& xsec );

#endif
