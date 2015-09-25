#ifndef _CalibrationMerger_H_
#define _CalibrationMerger_H_

#include <string>
#include <vector>

/*! \class CalibrationMerger
 *  merges two or more calibration files.
 */

class CalibrationMerger {

public:
  /// add file name to be merged
  void add ( const std::string & filename );

  /// merge and write to out
  void merge( const std::string & out );
private:
  std::vector < std::string > files_;
};

#endif // _CalibrationMerger_H_
