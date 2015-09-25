/** @file
 *  Recreate CMS beamspot from data.
 */

#ifndef CmsBeamSpotBuilder_H
#define CmsBeamSpotBuilder_H

#include "TTree.h"
#include <rave/Ellipsoid3D.h>

class CmsBeamSpotBuilder
{
public:
  static rave::Ellipsoid3D build( TTree * tree );
};

#endif
