#include "MagneticFieldWriter.h"
#include <rave/impl/RaveTools/Converters/interface/MagneticFieldSingleton.h>
#include <dataharvester/Tuple.h>
#include <dataharvester/Writer.h>

using namespace std;

MagneticFieldWriter::MagneticFieldWriter() :
    theFileName ("magneticfield.txt;magneticfield.root" ),
    hasProcessed ( false )
{ }

MagneticFieldWriter::MagneticFieldWriter ( const MagneticFieldWriter & o ) :
    theFileName ( o.theFileName ), hasProcessed ( o.hasProcessed )
{}

MagneticFieldWriter::~MagneticFieldWriter()
{
  dataharvester::Writer::close();
}

void MagneticFieldWriter::configure ( const string & f )
{
    theFileName=f;
    hasProcessed=false;
}

void MagneticFieldWriter::process ( const vertigo::Event & e )
{
  write();
}

void MagneticFieldWriter::process ( const exception & e, const string & )
{
}

void MagneticFieldWriter::write()
{
  if ( hasProcessed ) return; // write only once!
  {
    vector < GlobalPoint > points;
    for ( int x=0; x< 10  ; x++ )
      for ( int y=0; y< 10  ; y++ )
        for ( int z=0; z< 10  ; z++ )
          points.push_back ( GlobalPoint ( (float) (x*10.), \
                (float) (y*10.), (float) (z*10.) ) );

    writePoints ( points, "volumescan" );
  }

  {
    vector < GlobalPoint > points;
    for ( int z=0; z< 1000  ; z++ )
      points.push_back ( GlobalPoint ( 0., 0., (float) (z*1.) ) );

    writePoints ( points, "beamline" );
  }

  hasProcessed=true;
}

void MagneticFieldWriter::writePoints( const vector < GlobalPoint > & points, const string & name ) const
{
  for ( vector< GlobalPoint >::const_iterator i=points.begin(); i!=points.end() ; ++i )
  {
    GlobalVector vec = MagneticFieldSingleton::Instance()->inTesla ( *i );
    writePoint ( vec, *i, name );
  }
}

void MagneticFieldWriter::writePoint ( const GlobalVector & vec,
    const GlobalPoint & pt, const string & name ) const
{
  dataharvester::Tuple t( name );
  t["ptx"]=pt.x();
  t["pty"]=pt.y();
  t["ptz"]=pt.z();
  t["vecx"]=vec.x();
  t["vecy"]=vec.y();
  t["vecz"]=vec.z();
  dataharvester::Writer::file ( theFileName ) << t ;
}

MagneticFieldWriter * MagneticFieldWriter::clone() const
{
  return new MagneticFieldWriter ( *this );
}

#include "Analysis/Observer/interface/ObserverBuilder.h"
namespace {
  ObserverBuilder<MagneticFieldWriter> t( "MagneticFieldWriter", "Analyses the specified fitter", false );
}

