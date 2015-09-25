#ifndef __VMultiType__H
#define __VMultiType__H

#include <string>
#include <vector>
#include <iostream>

namespace vertigo {
class VMultiType {
public:
  /**
   *  \class VMultiType
   *  Weak typing in a strongly typed programming language
   */
  VMultiType ( const std::string & data );
  VMultiType ( const char * );
  VMultiType ( long signed data );
  VMultiType ( int data );
  VMultiType ( double data );
  VMultiType ( float data );
  VMultiType ( bool data );
  VMultiType ( const std::vector < double > & data );
  VMultiType ( const std::vector < long > & data );

  VMultiType ();
  operator double() const;
  /* operator vector < double > () const;
  operator vector < long > () const; */
  operator float() const;
  operator std::string() const;
  operator long() const;
  operator int() const;
  operator bool() const;

  std::string asString() const;
  double asDouble() const;
  long asLong() const;
  int asInt() const;
  bool asBool() const;
  std::vector < double > asDoubleVector() const;
  std::vector < long > asIntVector() const;

  //* try to rethink the assigned type
  void trimType();

  VMultiType & operator= ( const std::string & );
  VMultiType & operator= ( long signed );
  VMultiType & operator= ( int );
  VMultiType & operator= ( size_t );
  VMultiType & operator= ( double );
  // unimplemented by now, using standard C++ cast currently
  //VMultiType & operator= ( float );
  VMultiType & operator= ( bool );
  VMultiType & operator= ( const char * );
  VMultiType & operator= ( const std::vector < double > & );
  VMultiType & operator= ( const std::vector < long > & );

  bool operator== ( const char * ) const;
  bool operator== ( const VMultiType &) const;
  bool operator== ( const std::vector < double > & ) const;
  bool operator!= ( const char * ) const;

  /**
   *  returns "double", "int", "string", or "bool". Note that isType gives more
   *  detailed info.
   */
  std::string isA() const;

  enum Type { kString, kInt32, kInt64, kDouble32, kDouble64,
              kBool, kDouble32Vec, kDouble64Vec, 
              kInt32Vec, kInt64Vec, kNone };

  // returns the typename of a given type
  std::string typeName ( Type ) const;

  Type isType() const; //< returns the exact type
  bool hasValue() const;
  void toDefault();
  void defineAs( Type ); //< fix the type
private:
  void clear();
  Type stringIs() const; //< get the most restrictive type for the string
  int goodPrecision() const; //< work out a good precision for floats and doubles
private:
  /**
   *  make all private members mutable.  That way the VMultiType can be treated
   *  like any other type (const correctness ...)
   *  Note that conversions are "kept"
   */
  mutable double theDouble;
  mutable std::string theString;
  mutable long signed theInt;
  mutable std::vector < double > theDoubleVec;
  mutable std::vector < long > theIntVec;

  /**
   *  is it a double or has it been converted
   *  into a double before?
   */
  mutable bool hasDouble;
  /** 
   *  is it a string or has it been converted
   *  to a string before?
   */
  mutable bool hasString;
  /**
   *  Is it an int or has it been converted
   *  to an int before?
   */
  mutable bool hasInt;
  /**
   *  Is it a double vector or has it 
   *  been converted to a double vector before?
   */
  mutable bool hasDoubleVec;
  /**
   *  Is it a double vector or has it 
   *  been converted to a int vector before?
   */
  mutable bool hasIntVec;
  Type theType; //< What _is_ it?
};
}

std::ostream & operator << ( std::ostream & s, vertigo::VMultiType & rt );

#endif
