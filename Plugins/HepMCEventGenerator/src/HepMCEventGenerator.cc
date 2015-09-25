#include "initPythia.h"
#include "HepMCEventGenerator.h"
#include "HepMC/PythiaWrapper.h"
#include "Environment/Utilities/interface/VertigoStreamers.h"
#include "Environment/MCObjects/interface/PDGCodes.h"
#include "RaveBase/Converters/interface/RaveStreamers.h"
#include <rave/impl/TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h>
#include <rave/impl/RaveTools/Converters/interface/MagneticFieldSingleton.h>

using namespace std;
using namespace HepMC;

namespace {
  struct Equals {
    bool operator() ( const rave::Point3D & p1, const rave::Point3D & p2 )
    {
      if ( ( fabs ( p1.x() - p2.x() ) < 1e-6  ) &&
           ( fabs ( p1.y() - p2.y() ) < 1e-6  ) &&
           ( fabs ( p1.z() - p2.z() ) < 1e-6  ) )
      {
        return true;
      }
      return false;
    }
  };

  vertigo::SimParticle fromHepMC ( const GenParticle & p )
  {
    int pdgid=p.pdg_id();
    float vx=0., vy=0., vz=0.;
    if ( p.production_vertex() )
    {
      vx=p.production_vertex()->point3d().x();
      vy=p.production_vertex()->point3d().y();
      vz=p.production_vertex()->point3d().z();
    }
    rave::Vector7D state ( vx, vy, vz,
                     p.momentum().x(),
                     p.momentum().y(),
                     p.momentum().z(),
                     p.generated_mass() );
    int charge = pdgid < 0 ? -1 : 1;
    return vertigo::SimParticle ( state, charge, 0, pdgid );
  }

  void discuss ( const HepMC::GenParticle & p, int level=0 )
  {
    string prefix="";
    if ( level>0)
    {
      for ( int i=0; i< level ; i++ )
        prefix+="    ";
    }

    int pdgid=p.pdg_id();
    int status=p.status();
    if (level==0)
      cout << endl << endl << "-----" << endl;
    else 
      cout << prefix << " level " << level << endl;

    cout << prefix << "[HepMCEventGenerator] analyse: " << PDGCodes::name ( pdgid ) << "[s" << status << "]" << endl;
    GenVertex * v1 = p.end_vertex();
    cout << prefix << " prod_vertex:" << p.production_vertex() << endl;
    cout << prefix << "  end_vertex:" << v1 << endl;
    if ( v1 )
    {
      cout << prefix << "    end_kids:" << v1->particles_out_size() << endl;
      cout << prefix << "        kids: ";
      for ( GenVertex::particles_out_const_iterator i=v1->particles_out_const_begin(); 
            i!=v1->particles_out_const_end() ; ++i )
      {
        cout << PDGCodes::name ( (**i).pdg_id() )  << "[s" << (**i).status() << "] ";
        if ( (**i).pdg_id() == p.pdg_id() )
          discuss ( **i, level+1 );
      }
      cout << endl;
    }
  }
}

bool HepMCEventGenerator::hasBeamSpot() const
{
  return false;
}

void HepMCEventGenerator::addParticle ( vertigo::Event & event, 
    const GenParticle & p ) const
{
  if ( !p.production_vertex() ) return;
  int pdgid=p.pdg_id();
  int status=p.status();
  // if ( abs ( pdgid ) == 24 || abs ( pdgid ) == 25 )
  if ( status == 3 )
  {
    discuss ( p, 0 );
  }
  if ( status != 3 ) return;
  if ( p.momentum().e() < 2.0 ) return;
  vertigo::SimParticle track = fromHepMC ( p );
  add ( event, track );
}

rave::Ellipsoid3D HepMCEventGenerator::beamSpot() const
{
  return rave::Ellipsoid3D();
}

HepMCEventGenerator::HepMCEventGenerator( const string & filename ) :
  config_ ( "" ), genevt_(0), evtnr_(0), pythia_(false), file_(false)
{
  source ( filename );
}


std::string HepMCEventGenerator::describe() const
{
  return "HepMCEventGenerator";
}

vertigo::Event HepMCEventGenerator::fromGenEvent( GenEvent * evt ) const
{
  evt->use_units(Units::GEV, Units::CM);
  evt->set_cross_section( getPythiaCrossSection() );
  // evt->print ( cout );
  vertigo::Event event ( evtnr_++, 0, "hepmc" );
  for ( GenEvent::particle_const_iterator i = evt->particles_begin() ; 
        i!=evt->particles_end()  ; i++ )
  {
    addParticle ( event, **i );
  }
  event.add ( vertices_ );
  delete evt;
  return event;
}

void HepMCEventGenerator::addVertex ( 
    vertigo::Event & event,
    vertigo::SimTrack & track ) const
{
  rave::Point3D p ( track.state().x(), track.state().y(), track.state().z() );
  Equals equals;
  // vector < vertigo::SimVertex > vtces = event.simVertices();
  /* cout << "[HepMCEventGenerator] adding " << vtces.size()+1 
       << "nd mc vertex at " << p << endl; */
  for ( vector< vertigo::SimVertex >::iterator i=vertices_.begin(); 
        i!=vertices_.end() ; ++i )
  {
    if ( equals ( i->position(), p ) )
    {
      track.setVertex ( &(*i) );
      i->add ( track );
      return;
    }
  }
  vector < vertigo::SimTrack > trks;
  trks.push_back ( track );
  bool isprimary= (vtxid_ == 0 );
  vertigo::SimVertex vtx ( p, trks, vtxid_++, "", isprimary );
  vertices_.push_back ( vtx );
}

void HepMCEventGenerator::add ( vertigo::Event & event,
    const vertigo::SimParticle & ptcl ) const
{
    /* add a simparticle, plus its sim track, produce a rec track
     * and associate the rec track with the sim track
     */
    event.add ( ptcl );
    GlobalPoint p ( ptcl.state().x(), ptcl.state().y(), ptcl.state().z() );
    GlobalVector m ( ptcl.state().px(), ptcl.state().py(), ptcl.state().pz() );
    GlobalTrajectoryParameters gtp ( p, m, ptcl.charge(),
       MagneticFieldSingleton::Instance() );
    rave::Track rtrack = smearer_.recTrack ( gtp );
    event.add ( rtrack );
    vertigo::SimTrack track ( pid_++, (rave::Vector6D) ptcl.state(), 
        ptcl.charge(), 0, ptcl.pid()  );
    addVertex ( event, track );
    event.add ( track );
    event.associate ( track , rtrack );
    // cout << "SimTrack: " << track << endl;

}

vertigo::Event HepMCEventGenerator::next() const
{
  pid_=0;
  vtxid_=0;
  vertices_.clear();
  GenEvent * evt = 0;
  if ( pythia_ )
  {
    call_pyevnt();
    call_pyhepc( 1 );
    evt = hepevtio_.read_next_event();
  }

  if ( file_ )
  {
    evt = genevt_->read_next_event();
  }

  if ( !evt )
  {
    throw std::string ( "no more hepmc events" );
  }
  return fromGenEvent ( evt );
}

HepMCEventGenerator::HepMCEventGenerator() :
  config_(""), genevt_ ( 0 ), pythia_ ( false ), file_ ( 0 )
{}

void HepMCEventGenerator::source ( const std::string & config )
{
  generator_=config;
  config_="";
  string::size_type separator=config.find("-");
  if ( separator!=string::npos )
  {
    generator_=config.substr(0,separator);
    config_=config.substr(separator+1,string::npos );
  }
  /* cout << "[HepMCEventGenerator] configure ``" << generator_ 
       << "-" << config_ << "''" << endl; */
  HepMC::HEPEVT_Wrapper::set_max_number_entries(4000);
  HepMC::HEPEVT_Wrapper::set_sizeof_real(8);
  if ( generator_ == "pythia" )
  {
    pythia_=true;
  }

  if ( generator_ == "file" )
  {
    file_=true;
    genevt_ = new HepMC::IO_GenEvent (config_.c_str(),std::ios::in);
    if ( genevt_->rdstate() != 0 )
    {
      cout << "[HepMCEventGenerator] could not open " << config_ << endl;
      exit(0);
    }
  }

  if ( pythia_ ) initPythia( config_ );
}

HepMCEventGenerator::HepMCEventGenerator( const HepMCEventGenerator & o )
{
  cout << "[HepMCEventGenerator] not implemented!!" << endl;
  exit(0);
}

HepMCEventGenerator::~HepMCEventGenerator()
{
  if ( genevt_ ) delete genevt_;
}

int HepMCEventGenerator::skip ( int n ) const
{
  for ( int i=0; i< n ; i++ ) next();
  return n;
}

#include "Vertigo/EventGeneration/interface/GeneratorBuilder.h"
namespace {
    GeneratorBuilder<HepMCEventGenerator> t( "hepmc", "hepmc file reader" );
}

