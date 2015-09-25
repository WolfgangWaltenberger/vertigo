#include "Environment/MCObjects/interface/PDGCodes.h"
#include <sstream>
#include <limits>
#include <cstdlib>
using namespace std;

string PDGCodes::name ( const string & pid )                                                            
{                                                                                                       
  if ( pid == "undef" ) return pid;                                                                     
  return name ( atoi ( pid.c_str() ) );                                                                 
}                                                                                                      

int PDGCodes::pDGCode ( const string & pid )
{
  int sgn=1;
  string mpid = pid;
  while ( mpid.find("-")!=string::npos )
  {
    mpid.replace(mpid.find("-"),1,"" );
    sgn=-1;
  }
  while ( mpid.find("+")!=string::npos )
  {
    mpid.replace(mpid.find("+"),1,"" );
  }
  while ( mpid.find("bar")!=string::npos )
  {
    mpid.replace(mpid.find("bar"),3,"" );
    sgn=-1;
  }
  if (mpid.size()==1 )
  {
    char p = mpid[0];
    return sgn*pDGCode(p);
  }
  if (mpid=="e") return sgn*11;
  if (mpid=="mu") return sgn*13;
  if (mpid=="tau") return sgn*15;
  if (mpid=="gamma") return 22;
  if (mpid=="nu_e") return sgn*12;
  if (mpid=="nu_mu") return sgn*14;
  if (mpid=="nu_tau") return sgn*16;
  if (mpid=="pi0") return 111;
  if (mpid=="rho0") return 113;
  if (mpid=="rho") return sgn*213;
  if (mpid=="pi") return sgn*211;
  if (mpid=="eta") return 211;
  if (mpid=="K0s") return sgn*310;
  if (mpid=="K0") return sgn*311;
  if (mpid=="K0+") return sgn*313;
  if (mpid=="K") return sgn*321;
  if (mpid=="p") return sgn*2212;
  if (mpid=="n") return sgn*2112;
  if (mpid=="~chi10") return sgn*1000022;
  if (mpid=="~g") return sgn*1000021;
  if (mpid=="~chi20") return sgn*1000023;
  if (mpid=="~chi1+") return sgn*1000024;
  if (mpid=="~chi2+") return sgn*1000037;
  if (mpid=="~d_L") return sgn*1000001;
  if (mpid=="~d_R") return sgn*2000001;
  if (mpid=="~u_L") return sgn*1000002;
  if (mpid=="~u_R") return sgn*2000002;
  if (mpid=="~s_L") return sgn*1000003;
  if (mpid=="~s_R") return sgn*2000003;
  if (mpid=="~c_L") return sgn*1000004;
  if (mpid=="~c_R") return sgn*2000004;
  if (mpid=="~b_L") return sgn*1000005;
  if (mpid=="~b_R") return sgn*2000005;
  if (mpid=="~b_1") return sgn*1000005;
  if (mpid=="~b_2") return sgn*2000005;
  if (mpid=="~t_L") return sgn*1000006;
  if (mpid=="~t_R") return sgn*2000006;
  if (mpid=="~t_1") return sgn*1000006;
  if (mpid=="~t_2") return sgn*2000006;
  if (mpid=="~e_L") return sgn*1000011;
  if (mpid=="~e_R") return sgn*2000011;
  if (mpid=="~mu_L") return sgn*1000013;
  if (mpid=="~mu_R") return sgn*2000013;
  if (mpid=="~tau_L") return sgn*1000015;
  if (mpid=="~tau_R") return sgn*2000015;
  return std::numeric_limits<int>::quiet_NaN();
}

int PDGCodes::pDGCode ( char pid )
{
  switch ( pid )
  {
    case 'q': return 0;
    case 'd': return 1;
    case 'u': return 2;
    case 's': return 3;
    case 'c': return 4;
    case 'b': return 5;
    case 't': return 6;
    case 'e': return 11;
    case 'g': return 21;
    case 'Z': return 23;
    case 'W': return 24;
    case 'H': return 25;
  }
  return std::numeric_limits<int>::quiet_NaN();
}

string PDGCodes::name ( int pid )
{
  switch ( pid )
  {
    case 1:
      return "d";
    case -1:
      return "dbar";
    case 2:
      return "u";
    case -2:
      return "ubar";
    case 3:
      return "s";
    case -3:
      return "sbar";
    case 4:
      return "c";
    case -4:
      return "cbar";
    case 5:
      return "b";
    case -5:
      return "bbar";
    case 6:
      return "t";
    case -6:
      return "tbar";
    case 11:
      return "e-";
    case -11:
      return "e+";
    case 13:
      return "mu-";
    case -13:
      return "mu+";
    case 12:
    case 14:
    case 16:
    case -12:
    case -14:
    case -16:
      return "nu";
    case 15:
      return "tau-";
    case -15:
      return "tau+";
    case 21:
      return "g";
    case 22:
      return "gamma";
    case 23:
      return "Z";
    case 24:
      return "W+";
    case -24:
      return "W-";
    case 25:
      return "H";
    case -25:
      return "H";
    case 111:
      return "pi0";
    case 113:
      return "rho0";
    case 213:
      return "rho+";
    case -213:
      return "rho-";
    case 211:
      return "pi+";
    case -211:
      return "pi-";
    case 221:
      return "eta";
    case 310:
      return "K0s";
    case -310:
      return "K0sbar";
    case 311:
      return "K0";
    case -311:
      return "K0bar";
    case 313:
      return "K0*";
    case -313:
      return "K0*bar";
    case 321:
      return "K+";
    case -321:
      return "K-";
    case 323:
      return "K*+";
    case -323:
      return "K*-";
    case 333:
      return "phi";
    case -333:
      return "phibar";
    case 331:
      return "eta'";
    case 411:
      return "D+";
    case -411:
      return "D-";
    case 413:
      return "D*+";
    case -413:
      return "D*-";
    case 421:
      return "D0";
    case -421:
      return "D0bar";
    case 423:
      return "D0*";
    case -423:
      return "-D0*";
    case 431:
      return "Ds+";
    case -431:
      return "Ds-";
    case 441:
      return "etac";
    case -441:
      return "etacbar";
    case 511:
      return "B0";
    case -511:
      return "B0bar";
    case 521:
      return "B+";
    case -521:
      return "B-";
    case 531:
      return "B0s";
    case -531:
      return "B0sbar";
    case 2112:
      return "n";
    case 2212:
      return "p";
    case -2212:
      return "p-";
    case 2214:
      return "Delta+";
    case -2214:
      return "Delta-";
    case 3112:
      return "Sigma-";
    case -3112:
      return "Sigma+";
    case 3122:
      return "Lambda";
    case -3122:
      return "Lambdabar";
    case 3222:
      return "Sigma+";
    case -3222:
      return "Sigma-";
    case 1000021:                                                               
      return "~g";                                                              
    case -1000021:                                                              
      return "~gbar";                                                           
    case 1000022:                                                               
      return "~chi10";                                                          
    case -1000022:                                                              
      return "~chi10bar";                                                       
    case 1000023:                                                               
      return "~chi20";                                                          
    case -1000023:                                                              
      return "~chi20bar";                                                       
    case 1000024:                                                               
      return "~chi1+";                                                          
    case -1000024:                                                              
      return "~chi1+bar";                                                       
    case 1000025:                                                               
      return "~chi30";                                                          
    case -1000025:                                                              
      return "~chi30bar";                                                       
    case 1000037:                                                               
      return "~chi2+";                                                          
    case -1000037:                                                              
      return "~chi2+bar";                                                       
    case 1000001:                                                               
      return "~d_L";                                                            
    case 2000001:                                                               
      return "~d_R";                                                                
    case 1000002:                                                               
      return "~u_L";                                                            
    case 2000002:                                                               
      return "~u_R";        
    case 1000003:                                                               
      return "~s_L";                                                            
    case 2000003:                                                               
      return "~s_R";                                                            
    case 1000004:                                                               
      return "~c_L";                                                            
    case 2000004:                                                               
      return "~c_R";                                                            
    case 1000005:                                                               
      return "~b_1";                                                            
    case 2000005:                                                               
      return "~b_2";                                                            
    case 1000006:                                                               
      return "~t_1";                                                            
    case 2000006:                                                               
      return "~t_2";                                                            
    case 1000011:                                                               
      return "~e_L";                                                            
    case 2000011:                                                               
      return "~e_R";         
    case -1000001:                                                              
      return "~d_Lbar";                                                         
    case -2000001:                                                              
      return "~d_Rbar";                                                         
    case -1000002:                                                              
      return "~u_Lbar";                                                         
    case -2000002:                                                              
      return "~u_Rbar";                                                         
    case -1000003:                                                              
      return "~s_Lbar";                                                         
    case -2000003:                                                              
      return "~s_Rbar";                                                         
    case -1000004:                                                              
      return "~c_Lbar";                                                         
    case -2000004:                                                              
      return "~c_Rbar";                                                         
    case -1000005:                                                              
      return "~b_1bar";                                                         
    case -2000005:                                                              
      return "~b_2bar";                                                         
    case -1000006:                                                              
      return "~t_1bar";                                                         
    case -2000006:                                                              
      return "~t_2bar";                                                         
    case -1000011:                                                              
      return "~e_Lbar";                 
    case -2000011:                                                              
      return "~e_Rbar";    

    default:
      ostringstream o;
      o << "??? (" <<  pid << ")";
      return o.str();
  }
}
