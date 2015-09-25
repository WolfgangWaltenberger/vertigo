#include "Environment/Utilities/interface/Console.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <unistd.h>
#include "Vertigo/Event/interface/Event.h"
#include "Environment/Configuration/interface/SimpleConfigurable.h"
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include <rave/Track.h>
#include <rave/impl/DataFormats/GeometryVector/interface/GlobalPoint.h>

using namespace std;

vertigo::Console::Console() : useColors (
    SimpleConfigurable<int> ( 1, "Console:Colors" ).value() )
//      1 )
{
  /* if we dont write to a tty ( redirections! ),
   * then we also dont want any colors
   */
  if ( useColors == 1 && (!isatty (1)) )
  {
    useColors = 0;
  };
}

string vertigo::Console::set ( const kColor & col, const kMode mode ) const {
  ostringstream ret;
  if ( useColors ) {
    ret << "\033[" << (int) mode << ";3" << (int) col << ";11m";
  };
  return ret.str();
}

string vertigo::Console::set ( const kColor & foreground,
    const kColor & background, const kMode mode ) const {
  ostringstream ret;
  if ( useColors ) {
    ret << "\033[" << (int) mode << ";4" << (int) background << ";3"
        << (int) foreground << ";11m";
  };
  return ret.str();
}

string vertigo::Console::point ( const GlobalPoint & gp ) const {
  ostringstream ret;
  ret.setf ( ios::fixed );
  ret << "( " << setw(8) << setprecision(5) << gp.x()
      << ", " << setw(8) << gp.y()
      << ", " << setw(9) << setprecision(5) << gp.z()
      << " )";
  return ret.str();
}

string vertigo::Console::reset () const {
  if ( useColors ) {
    return "\033[0;0m";
  } else {
    return "";
  };
}

string vertigo::Console::vtxstring ( int n ) const {
  if ( n==1 ) {
    return "Vertex";
  } else {
    return "Vertices";
  };
}

string vertigo::Console::spaces ( const int n ) const {
  string t(n,' ');
  return t;
}

string vertigo::Console::topSeparator( kColor col , int length ) const {
  ostringstream ret;
  string m3 ( (int) (length * 3 / 36), '-' );
  string m2 ( (int) (length * 2 / 36), '-' );
  string m7 ( (int) (length * 7 / 36), '-' );
  string m12 ( (int) (length * 12 / 36), '-' );
  string m11 ( (int) (length * 11 / 36), '-' );
  string pA ( (int) (length * 1 / 36), '<' );
  string pE ( (int) (length * 1 / 36), '>' );

  ret << set ( kBlack, kHilight ) << pA << m12
      << set ( col, kNormal ) << m11
      << set ( col, kHilight ) << m11
      << set ( col ) << m3
      << set ( kBlack, kHilight ) << m2 << pE
      << endl; // << '\0';
  return ret.str();
}

string vertigo::Console::bottomSeparator( kColor col , int length ) const {
  ostringstream ret;
  string m12 ( (int) (length / 12), '-' );
  string m7 ( (int) (length *7 / 36), '-' );
  string m6 ((int) (length * 6 / 36), '-' );
  string m5 ( (int) (length * 5 / 36), '-' );
  string m9 ( (int) (length * 9 / 36), '-' );
  string p1 ( (int) (length * 1 / 36), '·' );
  ret << set ( kBlack, kHilight ) << p1 << m9
      << set ( col ) << m6 << set ( col, kHilight )
      << m12 << set ( col , kHilight ) << m7 << set ( col )
      << m9 << set ( kBlack, kHilight ) << m5 << p1 << endl
      << reset(); // << '\0';
  return ret.str();
}

void vertigo::Console::eventFooter () const {
  cout << spaces(10) << topSeparator( kBlue) << set ( kWhite, kNormal );
}

void vertigo::Console::eventHeader ( const int event, const int run ) const {

  cout << endl
       << spaces(10) << bottomSeparator() << set (kGreen, kNormal )
       << spaces(10);
  if (run) {
    cout << "   Event " << set (kGreen, kHilight ) << "Nr. "
         << set ( kGreen, kHilight ) << event
         << " , Run Nr. " << run;
  } else {
    cout << "           Event " << set (kGreen, kHilight ) << "Nr. "
         << set ( kGreen, kHilight ) << event;
  };

  cout << endl
       << spaces(10) << bottomSeparator() << reset();
}

void vertigo::Console::eventDescription ( const int simtracks,
    const int rectracks, const int simvertices ) const {

  cout << spaces(10) << "  " << set (kGreen, kHilight )
       << setw(3) << simtracks
       << set ( kGreen, kNormal ) << " SimTracks, "
       << set (kGreen, kHilight ) << simvertices
       << set ( kGreen, kNormal ) << " SimVertices." << endl
       << spaces(10) << "  " << setw(3) << set (kGreen, kHilight )
       << rectracks
       << set ( kGreen, kNormal ) << " RecTracks." << endl
       << spaces(10) << bottomSeparator() << reset();
}

void vertigo::Console::eventDescription ( const int simtracks,
    const int rectracks, const int simvertices, const int simtracks_f,
    const int rectracks_f, const int simvertices_f ) const
{
  cout << spaces(10) << "  " << set (kGreen, kHilight )
       << setw(3) << simtracks << "(" << simtracks_f << ")"
       << set ( kGreen, kNormal ) << " SimTracks, "
       << set (kGreen, kHilight ) << simvertices << "(" << simvertices_f << ")"
       << set ( kGreen, kNormal ) << " SimVertices." << endl
       << spaces(10) << "  " << setw(3) << set (kGreen, kHilight )
       << rectracks << "(" << rectracks_f << ")"
       << set ( kGreen, kNormal ) << " RecTracks." << endl
       << spaces(10) << bottomSeparator() << reset();
}

std::string vertigo::Console::prettyPrint ( float s ) const
{
  // print distance, given in cm.
  ostringstream ostr;
  s=s/100.;
  signed d=(signed) floor ( log10(s) / 3 );
  switch (d)
  {
    case -3:
      ostr << s * 1.e9 << "n";
      break;
    case -2:
      ostr << s * 1.e6 << "u"; // "µ"; // "mu"
      break;
    case -1:
      ostr << s * 1.e3 << "m";
      break;
    case 0:
      ostr << s;
      break;
    case 1:
      ostr << s / 1.e3 << "K";
      break;
    case 2:                                                                                                           ostr << s/1.e6 << "M";
      break;                                                                                                        case 3:
      ostr << s/1.e9 << "G";
      break;
    case 4:
    default:
      ostr << s/1.e12 << "T";
      break;
  };
  return ostr.str();
}

void vertigo::Console::eventDescription ( const vertigo::Event & e ) const
{
  int st = e.simTracks ().size();
  int rt = e.tracks ().size();
  int sv = e.simVertices().size();
  eventDescription ( st, rt, sv );
}
