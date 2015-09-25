#ifndef _MultipleVertexGun_H_
#define _MultipleVertexGun_H_

#include "VertexGun.h"
#include <rave/Track.h>
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimVertex.h"

/*! \class MultipleVertexGun
 *  "Collects" the tracks from other VertexGuns into one
 *  object. MultipleVertexGun itself is a VertexGun.
 *
 * \author Wolfgang Waltenberger
 */

class MultipleVertexGun : public VertexGun {

public:
  /**
   *  Default constructor.
   *  \param merge If true, the SimVertices are merged into one 
   *  SimVertex, if the distance between the vertices is smaller
   *  than 1 micron.
   */
  MultipleVertexGun( bool merge = false, const std::string & name="MultipleVertexGun" );
  MultipleVertexGun( const MultipleVertexGun & );

  ~MultipleVertexGun();

  /// instantiate object with n_times VertexGuns.

  /// Add VertexGun to the object.
  void add( const VertexGun & );

  vertigo::Event shoot();

  void clear();
  virtual std::string name() const;

  MultipleVertexGun * clone() const;

private:
  std::vector < VertexGun *> theGuns;
  vertigo::Event theEvent;
  std::string theName;
  bool theMerge; //< merge close simvertices? (not yet used)
};

#endif // _MultipleVertexGun_H_
