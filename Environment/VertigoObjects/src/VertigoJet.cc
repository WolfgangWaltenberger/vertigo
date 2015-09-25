#include "Environment/VertigoObjects/interface/VertigoJet.h"
#include <rave/impl/TrackingTools/TransientTrack/interface/TransientTrack.h>
#include <rave/impl/RaveBase/RaveEngine/interface/RaveVertexReconstructor.h>
#include <rave/impl/RaveBase/RaveEngine/interface/RaveId.h>
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>
#include <rave/impl/RaveBase/Converters/interface/RaveToCmsObjects.h>
#include <iostream>
#include <limits>

using namespace std;

namespace {
  rave::Point3D point ( const vector < rave::Track > & t )
  {
    if ( t.size() == 0 )
    {
      edm::LogWarning ("VertigoJet" ) << "Jet has no tracks.";
      return rave::Point3D ();
    }
    if ( t.size() < 2 )
    {
      edm::LogWarning ( "VertigoJet" ) << "Jet has fewer than two tracks - " 
           << "             Returning initial state.";
      return t[0].state().position();
    }
    // vector < ModeFinder3d::PointAndDistance > pts;
    vector < reco::TransientTrack > trks;
    RaveToCmsObjects converter;
    for ( vector< rave::Track >::const_iterator i=t.begin(); i!=t.end() ; ++i )
    {
      // reco::TransientTrack trk ( *i );
      trks.push_back ( converter.tTrack (*i ) );
      // GlobalPoint pt = trk.impactPointState().globalPosition();
      // ModeFinder3d::PointAndDistance p ( pt, 1. );
      // pts.push_back ( p );
    }
    rave::VertexReconstructor rtor ( "avf" );
    vector < TransientVertex > vtces = rtor.vertices ( trks );
    float NaN = numeric_limits<float>::quiet_NaN();
    GlobalPoint value ( NaN, NaN, NaN );
    if ( vtces.size() ) value = vtces[0].position();
    // GlobalPoint value = FsmwModeFinder3d() ( pts );
    return rave::Point3D ( value.x(), value.y(), value.z() );
  }

  rave::Vector3D sum ( const vector < rave::Track > & t )
  {
    float px=0.;
    float py=0.;
    float pz=0.;
    for ( vector< rave::Track >::const_iterator i=t.begin(); i!=t.end() ; ++i )
    {
      px+=i->state().momentum().x();
      py+=i->state().momentum().y();
      pz+=i->state().momentum().z();
    }
    return rave::Vector3D ( px, py, pz );
  }
}

vertigo::Jet::Jet () : tag_(-1.), mctag_('?'), mcphystag_('?'), isTagged_(false),
  hasmctag_ ( false), hasmcphystag_ (false), ghosthaserror_ (false),id_( RaveId::uniqueId() ),
  angle_ ( 0.3 ), needsCompute_ ( true ), time_(0.)
{}

vertigo::Jet::Jet ( const std::vector < rave::Track > & tracks )
  : tracks_(tracks), tag_(-1.), mctag_('?'), mcphystag_('?'),  isTagged_(false), hasmctag_(false),
  hasmcphystag_ (false), ghosthaserror_ (false), id_( RaveId::uniqueId() ), 
  angle_ ( 0.3 ), needsCompute_ ( true ), time_(0.)
{}

vertigo::Jet::Jet ( const std::vector < rave::Track > & tracks,
   const rave::Point3D & pos, const rave::Vector3D & mom )
  : tracks_(tracks), tag_(-1.), mctag_('?'), mcphystag_('?'), isTagged_(false), hasmctag_(false),
  hasmcphystag_ (false), ghosthaserror_ (false), id_( RaveId::uniqueId() ), position_ (pos), momentum_(mom), angle_(.3),
  needsCompute_ ( false ), time_(0.)
{}

vertigo::Jet::Jet ( const std::vector < rave::Track > & tracks,
   const rave::Point3D & pos, const rave::Vector3D & mom,
   const rave::Covariance6D & error )
  : tracks_(tracks), tag_(-1.), mctag_('?'), mcphystag_('?'), isTagged_(false), hasmctag_(false),
  hasmcphystag_ (false), ghosthaserror_ (true), id_( RaveId::uniqueId() ), 
  position_ (pos), momentum_(mom), error_(error), angle_(.3),
  needsCompute_ ( false ), time_(0.)
{}

float vertigo::Jet::time() const
{
  return time_;
}

void vertigo::Jet::setTime ( float f )
{
  time_=f;
}

rave::Track vertigo::Jet::ghost() const
{
  rave::Vector6D track ( position_, momentum_);
  rave::Track ghost_track ( track, error_, 0, 0., 0. );
  return ghost_track;
}

void vertigo::Jet::add ( const rave::Track & t )
{
  if ( find ( tracks_.begin(), tracks_.end(), t ) == tracks_.end() )
  {
    tracks_.push_back ( t );
    needsCompute_=true;
  }
}

bool vertigo::Jet::isTagged() const
{
  return isTagged_;
}

void vertigo::Jet::tag( double t )
{
  tag_=t;
  isTagged_=true;
}

double vertigo::Jet::getTag() const
{
  if (!isTagged_)
  {
    edm::LogWarning ( "VertigoJet") << 
      "trying to get jet tag of non-tagged jet!";
    return -1.;
  }
  return tag_;
}

void vertigo::Jet::compute() const
{
  momentum_=sum ( tracks_ );
  position_=point ( tracks_ );
  needsCompute_=false;
}

std::vector < rave::Track > vertigo::Jet::tracks() const
{
  return tracks_;
}

std::vector < rave::Vertex > vertigo::Jet::vertices() const
{
  return vertices_;
}

void vertigo::Jet::add ( const rave::Vertex & v )
{
  vertices_.push_back ( v );
}

int vertigo::Jet::id() const
{
  return id_;
}

rave::Point3D vertigo::Jet::position() const
{
  if ( needsCompute_ ) compute();
  return position_;
}

rave::Vector3D vertigo::Jet::momentum() const
{
  if ( needsCompute_ ) compute();
  return momentum_;
}

rave::Covariance6D vertigo::Jet::error() const
{
  return error_;
}

double vertigo::Jet::eta() const
{
  if ( needsCompute_ ) compute();
  edm::LogWarning ("VertigoJet") 
    << "not yet written!";
  return numeric_limits<double>::quiet_NaN();
}

bool vertigo::Jet::hasMCTag() const
{
  return hasmctag_;
}

bool vertigo::Jet::hasMCPhysicsTag() const
{
  return hasmctag_;
}

bool vertigo::Jet::ghostHasError() const
{
  return ghosthaserror_;
}

char vertigo::Jet::getMCTag() const
{
  return mctag_;
}

char vertigo::Jet::getMCPhysicsTag() const
{
  return mcphystag_;
}

void vertigo::Jet::mcTag ( char d )
{
  mctag_=d;
}

void vertigo::Jet::mcPhysicsTag ( char d )
{
  mcphystag_=d;
}


double vertigo::Jet::angle() const
{
  return angle_;
}
