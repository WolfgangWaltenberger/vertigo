#ifndef VertigoConsole_H
#define VertigoConsole_H

#include <rave/impl/DataFormats/GeometryVector/interface/GlobalPoint.h>
#include <string>

namespace vertigo {
class Event;

/**
 *  \class VertigoConsole
 *  helper class that should provide a few code snippets
 *  that are handy for output to cout.
 *  E.g. color escape sequences ( can be globally turned off
 *  via VertigoConsole:Colors ), a method to turn on and off
 *  _all_ output to cout and cerr, simple
 *  "headers" and "footers" for Vertex event description,
 *  ...
 *
 */

class Console {
private:
  int useColors;
public:
  Console();

  enum kColor { kBlack=0, kRed=1, kGreen=2, kBrown=3, kBlue=4, kMagenta=5,
                kCyan=6, kWhite=7 };
  enum kMode { kNormal=0, kHilight=1 };

  std::string set ( const kColor & col, const kMode mode = kNormal ) const;

  std::string set ( const kColor & foreground,
      const kColor & background, const kMode mode = kNormal ) const;

  std::string reset() const;

  std::string topSeparator( kColor = kGreen, int length=36 ) const;

  std::string bottomSeparator( kColor = kGreen, int length=36 ) const;
  void eventHeader ( const int event, const int run=0 ) const;
  void eventFooter () const;

  void eventDescription ( const int simtracks, const int rectracks,
      const int simvertices ) const;
  void eventDescription ( const int simtracks, const int rectracks,
      const int simvertices, const int simtracks_filtered,
      const int rectracks_filtered, const int simvertices_filtered ) const;

  std::string spaces ( const int n ) const;
  std::string vtxstring ( int n ) const;
  std::string point ( const GlobalPoint & gp ) const;

  std::string prettyPrint ( float ) const;

  void eventDescription ( const Event & r ) const;
};
}

#endif
