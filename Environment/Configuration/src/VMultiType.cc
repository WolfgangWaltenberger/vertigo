#include "Environment/Configuration/interface/VMultiType.h"
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;
using namespace vertigo;

namespace {
  vector < string > split ( string source )
  {
    vector < string > ret;
    while ( source.find(",") != string::npos )
    {
      string sub = source.substr ( 0, source.find(",") );
      if ( sub.size() ) ret.push_back ( source.substr ( 0, source.find(",") ) );
      source.replace ( 0, source.find(",")+1, "" );
      while ( source.size() && source[0]==' ' )
      {
        source.replace ( 0, 1, "" );
      };
    };
    if ( source.size() ) ret.push_back ( source );
    return ret;
  }

  VMultiType::Type floatType()
  {
    static VMultiType::Type ret = VMultiType::kNone;
    if ( ret != VMultiType::kNone ) return ret;
    switch ( sizeof ( float ) )
    {
      case 4:
        ret=VMultiType::kDouble32; break;
      case 8:
        ret=VMultiType::kDouble64; break;
        break;
      default:
        ret=VMultiType::kNone;
    };
    return ret;
  }

  VMultiType::Type doubleType()
  {
    static VMultiType::Type ret = VMultiType::kNone;
    if ( ret != VMultiType::kNone ) return ret;
    switch ( sizeof ( double ) )
    {
      case 4:
        ret=VMultiType::kDouble32; break;
      case 8:
        ret=VMultiType::kDouble64; break;
        break;
      default:
        ret=VMultiType::kNone;
    };
    return ret;
  }

  VMultiType::Type doubleVecType()
  {
    static VMultiType::Type ret = VMultiType::kNone;
    if ( ret != VMultiType::kNone ) return ret;
    switch ( doubleType() )
    {
      case VMultiType::kDouble32:
        ret=VMultiType::kDouble32Vec; break;
      case VMultiType::kDouble64:
        ret=VMultiType::kDouble64Vec; break;
        break;
      default:
        ret=VMultiType::kNone;
    };
    return ret;
  }
  
  VMultiType::Type longType()
  {
    static VMultiType::Type ret = VMultiType::kNone;
    if ( ret != VMultiType::kNone ) return ret;
    switch ( sizeof ( long ) )
    {
      case 4:
        ret=VMultiType::kInt32; break;
      case 8:
        ret=VMultiType::kInt64; break;
        break;
      default:
        ret=VMultiType::kNone;
    };
    return ret;
  }

  VMultiType::Type intType()
  {
    static VMultiType::Type ret = VMultiType::kNone;
    if ( ret != VMultiType::kNone ) return ret;
    switch ( sizeof ( int ) )
    {
      case 4:
        ret=VMultiType::kInt32; break;
      case 8:
        ret=VMultiType::kInt64; break;
        break;
      default:
        ret=VMultiType::kNone;
    };
    return ret;
  }

  VMultiType::Type longVecType()
  {
    static VMultiType::Type ret = VMultiType::kNone;
    if ( ret != VMultiType::kNone ) return ret;
    switch ( longType() )
    {
      case VMultiType::kInt32:
        ret=VMultiType::kInt32Vec; break;
      case VMultiType::kInt64:
        ret=VMultiType::kInt64Vec; break;
      default:
        ret=VMultiType::kNone;
    };
    return ret;
  }
}

VMultiType::VMultiType ( const vector < double > & data )
{
  clear();
  hasDoubleVec=true; theDoubleVec=data;
  theType=doubleVecType();
}

VMultiType::VMultiType ( const vector < long > & data )
{
  clear();
  hasIntVec=true; theIntVec=data;
  theType=longVecType();
}

VMultiType::VMultiType ( const string & data )
{
  clear();
  theString=data; hasString=true; 
  theType=kString;
}

VMultiType::VMultiType ( const char * data )
{
  clear();
  theString=data; hasString=true; 
  theType=kString;
}

VMultiType::VMultiType ( double data )
{
  clear();
  theDouble=data; hasDouble=true;
  theType=doubleType();
}

VMultiType::VMultiType ( float data )
{
  clear();
  theDouble=data; hasDouble=true;
  theType = floatType();
}

VMultiType::VMultiType()
{
  clear();
  theType=VMultiType::kNone;
}

VMultiType::VMultiType ( signed long data )
{
  clear();
  theInt = data; hasInt=true;
  theType=longType();
}

VMultiType::VMultiType ( int data )
{
  clear();
  theInt=data; hasInt = true;
  theType=intType();
}


VMultiType::VMultiType ( bool data )
{
  clear(); 
  theInt = (long) data; theType=kBool; 
  hasInt=true;
}

VMultiType::Type VMultiType::isType() const
{
  return theType;
}

bool 
VMultiType::hasValue() const
{
  return (hasDouble || hasInt || hasString || hasDoubleVec || hasIntVec);
}

void 
VMultiType::toDefault()
{
  // type info is left intact.
  theDouble=0.;         hasDouble=false;
  theInt=0;             hasInt=false;
  theString="undef";    hasString=false;
  theDoubleVec.clear(); hasDoubleVec=false;
  theIntVec.clear();    hasIntVec=false;
}

string VMultiType::isA() const
{
  return typeName ( theType );
}

string VMultiType::typeName( Type t ) const
{
  switch ( t )
  {
    case kString:
      return "string";
    case kBool:
      return "bool";
    case kInt32:
    case kInt64:
      return "int";
    case kDouble32:
    case kDouble64:
      return "double";
    case kDouble32Vec:
    case kDouble64Vec:
      return "doublevec";
    case kInt32Vec:
    case kInt64Vec:
      return "intvec";
    case kNone:
    default:
      return "undef";
  };
}

bool VMultiType::operator== ( const char * strg ) const
{
  return ( ((string) (*this)) == ( (string) (strg) ) );
}

bool VMultiType::operator== ( const VMultiType & mt ) const
{
  return ( ( static_cast< const string &> (*this)) == ( (string) (mt) ) );
}

bool VMultiType::operator== ( const vector < double > & v ) const
{
  cout << "[VMultiType] operator == doublevec!!!" << endl;
  if ( v.size() != this->asDoubleVector().size() ) 
    return false;
  // FIXME sorting?
  for ( unsigned int i=0; i< v.size() ; i++ )
  {
    if ( v[i] != theDoubleVec[i] ) return false;
  };
  return true;
}


bool VMultiType::operator!= ( const char * strg ) const
{
  return !(this->operator== (strg) );
}

double VMultiType::asDouble() const
{
  if ( !hasDouble )
  {
    if ( hasString )
    {
      theDouble = atof ( theString.c_str() );
    } else if ( hasInt ) {
      theDouble = theInt;
    } else {
      theDouble = 0.;
    };
    hasDouble = true;
  };
  // return 1.e-12* ( int ( 1.e12 * theDouble ) );
  return theDouble;
}

long signed VMultiType::asLong() const
{
  if ( !hasInt )
  {
    if ( hasDouble )
    {
      theInt=(long) (theDouble);
    } else if ( hasString ) {
      theInt=atoi ( theString.c_str() );
    } else {
      theInt=0;
    };
    hasInt=true;
  };
  return theInt;
}

vector < double > VMultiType::asDoubleVector() const
{
  if ( !hasDoubleVec )
  {
    theDoubleVec.clear();
    if ( hasString ) {
      // need to ask for string first.  because otherwise we would _always_ end
      // up with one element only!
      vector < string > tokens = split ( theString );
      for ( vector< string >::const_iterator i=tokens.begin(); 
            i!=tokens.end() ; ++i )
      {
        theDoubleVec.push_back ( atof ( i->c_str() ) );
      };
    } else if ( hasDouble ) {
      theDoubleVec.push_back ( theDouble );
    } else if ( hasInt ) {
      theDoubleVec.push_back ( (double) theInt );
    } else {
    };
   hasDoubleVec=true;
  };
  /*
  cout << endl
       << "asDoubleVector=";
  for ( vector< double >::const_iterator i=theDoubleVec.begin(); 
        i!=theDoubleVec.end() ; ++i )
  {
    cout << *i << " ";
  };
  cout << endl;
  */
  return theDoubleVec;
}

vector < long > VMultiType::asIntVector() const
{
  if ( !hasIntVec )
  {
    theIntVec.clear();
    if ( hasString ) {
      vector < string > tokens = split ( theString );
      for ( vector< string >::const_iterator i=tokens.begin(); 
            i!=tokens.end() ; ++i )
      {
        theIntVec.push_back ( atoi ( i->c_str() ) );
      };
    } else if ( hasInt ) {
      theIntVec.push_back ( theInt );
    } else if ( hasDouble ) {
      theIntVec.push_back ( (long) theDouble );
    } else {
    };
   hasIntVec=true;
  };
  return theIntVec;
}


int VMultiType::asInt() const
{
  return (int) asLong();
}

bool VMultiType::asBool() const
{
  /*
  if ( asLong() )
  {
    return true;
  } else {
    return false;
  }*/
  return (bool) asLong();
}

string VMultiType::asString() const
{
  if ( !hasString )
  {
    ostringstream ostr;
    if ( hasDouble )
    {
      ostr << setprecision ( goodPrecision() );
      ostr << theDouble;
      if ( ostr.str().find(".")==string::npos )
      {
        ostr << ".";
      }
    } else if ( hasInt ) {
      ostr << theInt;
    } else if ( hasDoubleVec ) {
      ostr << setprecision ( goodPrecision() );
      for ( vector< double >::const_iterator i=theDoubleVec.begin(); 
            i!=theDoubleVec.end() ; ++i )
      {
        if ( i != theDoubleVec.begin() ) ostr << ", ";
        ostr << *i;
      };
    } else {
      ostr << "undef";
    };
    theString=ostr.str();
    hasString=true;
  };
  return theString;
}

int VMultiType::goodPrecision() const
{
  switch ( theType )
  {
    case kDouble32:
    case kDouble32Vec:
      return 8; // 7 plus 1
    case kDouble64:
    case kDouble64Vec:
      return 18; // 16 plus 2
    default:
      return 10; // what do i know?
      // should never be used, anyways
  };
}

void VMultiType::trimType()
{
  if (theType!=kString) return;
  theType=stringIs();
  // cout << endl << " ->" << *this << "<- is a " << isA() << endl;
}

void VMultiType::defineAs( VMultiType::Type t )
{
  theType=t;
}

VMultiType::Type VMultiType::stringIs() const
{
  Type ret = longType();    // assumed default type
  if ( theString == "nan" || theString == "NaN" )
  {
    ret = doubleType();
    theDouble=numeric_limits<double>::quiet_NaN();
    hasDouble=true;
    return ret;
  };
  if ( theString == "infinity" || theString == "inf" || theString == "+inf" )
  {
    ret = doubleType();
    theDouble=numeric_limits<double>::infinity();
    hasDouble=true;
    return ret;
  };

  if ( theString == "-infinity" || theString == "-inf" )
  {
    ret = doubleType();
    theDouble=-numeric_limits<double>::infinity();
    return ret;
  };

  if ( theString == "true" )
  {
    ret = kBool;
    hasInt = true;
    theInt=1;
    return ret;
  };

  if ( theString == "false" )
  {
    ret = kBool;
    hasInt = true;
    theInt=0;
    return ret;
  }

  int n_dots=0;
  for ( unsigned int i=0; i< theString.size() ; i++ )
  {
    char t = theString[i];
    switch (t)
    {
      case ' ': // space is too vague
      case '0':
      case '-':
      case '+':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      {
        break;
      }
      case '.':
        n_dots++;
      case 'e':
      {
        // no int no more
        // FIXME argh! if double vector, we can have more than one dot!
        if ( n_dots > 1 && ret !=doubleVecType() )
        {
          // two dots or more. that makes it a string
          // (maybe an IPv4 address or sth?)
          ret = VMultiType::kString;
          break;
        };
        if ( ret == longType() ) ret = doubleType();
        if ( ret == longVecType() ) ret = doubleVecType();
        break;
      };
      case ',':
      {
        // can only be a vector
        if ( ret == longType() ) ret = longVecType();
        if ( ret == doubleType() ) ret = doubleVecType();
        break;
      };
      default:
        ret = kString;
    };
  };

  return ret;
}

VMultiType::operator double() const
{
  return (double) this->asDouble();
}

VMultiType::operator float() const
{
  return (float) this->asDouble();
}

VMultiType::operator long() const
{
  return this->asLong();
}

VMultiType::operator int() const
{
  return (int) (this->asLong() );
}

VMultiType::operator bool() const
{
  return (bool) (this->asLong() );
}

VMultiType::operator string() const
{
  return this->asString();
}

void VMultiType::clear()
{
  // destroy type info
  theType=kNone;

  toDefault();
}

VMultiType & VMultiType::operator= ( const string & o )
{
  clear();
  theString=o; hasString=true;
  theType=kString;
  return *this;
}

VMultiType & VMultiType::operator= ( const char * o )
{
  clear();
  theString=o; hasString=true;
  theType=kString;
  return *this;
}


VMultiType & VMultiType::operator= ( long signed o )
{
  clear();
  theInt=o; hasInt=true;
  theType=longType();
  return *this;
}

VMultiType & VMultiType::operator= ( const vector < double > & o )
{
  clear();
  theType=doubleVecType();
  hasDoubleVec=true; theDoubleVec=o;
  return *this;
}

VMultiType & VMultiType::operator= ( const vector < long > & o )
{
  clear(); theType=longVecType();
  hasIntVec=true; theIntVec=o;
  return *this;
}


VMultiType & VMultiType::operator= ( int o )
{
  return this->operator= ( (long signed) o );
}

VMultiType & VMultiType::operator= ( bool o )
{
  clear();
  theInt=(long) o; hasInt=true;
  theType=kBool;
  return *this;
}


VMultiType & VMultiType::operator= ( size_t o )
{
  return this->operator= ( (long signed) o );
}

VMultiType & VMultiType::operator= ( double o )
{
  clear();
  theDouble=o; hasDouble=true;
  theType=doubleType();
  return *this;
}

std::ostream & operator <<  ( ostream & s, vertigo::VMultiType & rt )
{
  s << (string) rt;
  return s;
}
