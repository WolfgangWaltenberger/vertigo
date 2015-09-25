/** @file
 *
 * PDG Codes FIXME
 * there must be an official thing for this somewhere
 */

#ifndef PDGCodes_H
#define PDGCodes_H

#include <string>

class PDGCodes
{
public:
  static std::string name ( int pid );
  static std::string name ( const std::string & pid );
  static int pDGCode ( const std::string & pid );
  static int pDGCode ( char pid );
};

#endif
