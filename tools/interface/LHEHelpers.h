#ifndef LHEHelpers_H
#define LHEHelpers_H

#include <map>
#include <string>

/**
 * @class LHEHelpers helper methods for dealing with lhe event sources
 */

class LHEHelpers 
{
  public:
    static std::string getModelName ( const std::string & t );
    /** method that parses the sms description string, returns particle
     * masses */
    static std::map < std::string, float > getSMSMasses 
      ( const std::string & t, bool debug=false );

};

#endif
