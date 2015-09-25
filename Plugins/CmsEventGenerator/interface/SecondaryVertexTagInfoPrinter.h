/** @file
 *  Recreate CMS beamspot from data.
 */

#ifndef SecondaryVertexTagInfoPrinter_H
#define SecondaryVertexTagInfoPrinter_H

#include "TTree.h"

class SecondaryVertexTagInfoPrinter
{
public:
  static void print( TTree * tree, int event );
};

#endif
