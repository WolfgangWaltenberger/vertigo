#ifdef WITH_HARVESTER
#include "VertigoHarvest/interface/ObserverTuples.h"
#include <rave/impl/CommonTools/Statistics/interface/ChiSquaredProbability.h>
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>
#include "Environment/MCObjects/interface/PDGCodes.h"

using namespace dataharvester;
using namespace std;

void ObserverTuples::addSimVertex ( const vertigo::SimVertex & sv, Tuple & vtx )
{
  vtx["simx"]=sv.position().x();
  vtx["simy"]=sv.position().y();
  vtx["simz"]=sv.position().z();
  vtx["primary"]=sv.isPrimary();
  vtx["tag"]=string("SV")+sv.tag();
  vtx["pid"]=sv.pid();
  vtx["n_simtracks (number of sim tracks)"]=sv.tracks().size();
  vtx["simvtxid"]= sv.id();
}

void ObserverTuples::addVertex ( const rave::Vertex & rv, Tuple & vtx, float w )
{
  vtx["recx"]=rv.position().x();
  vtx["recy"]=rv.position().y();
  vtx["recz"]=rv.position().z();
  vtx["dxx"]=rv.error().dxx();
  vtx["dyy"]=rv.error().dyy();
  vtx["dzz"]=rv.error().dzz();
  vtx["simvtxid"]=rv.id();
  float chi2 = rv.chiSquared();
  float ndf = rv.ndf();
  vtx["chi2"]=chi2;
  vtx["ndf"]=ndf;
  vtx["w"]=w;
  vtx["ntrks"]=rv.tracks().size();
  float chi2p=-1.;
  float nchi=-1.;
  if ( !finite ( chi2 ) || !finite (ndf ) || chi2 < 0. || ndf <= 0. )
  {
    edm::LogWarning("ObserverTuples") << "Cannot compute chi squared probability: "
         << "chi2=" << chi2 << "  ndf=" << ndf;
    if ( finite( chi2) && finite(ndf) && ndf < 0. ) nchi=chi2/ndf; // this is numerically ok, so we do it ...
  } else {
    chi2p=ChiSquaredProbability ( chi2 , ndf );
    nchi=chi2/ndf;
  };

  vtx["nchi"]=nchi;
  vtx["probchi"]=chi2p;
  vtx["ncomponents"]=rv.components().size();
}

void ObserverTuples::addAssociatedVertex ( const vertigo::SimVertex & sv,
    const rave::Vertex & rv, dataharvester::Tuple & t )
{
  addSimVertex ( sv, t );
  addVertex ( rv, t );
  float dx = sv.position().x() - rv.position().x();
  float dy = sv.position().y() - rv.position().y();
  float dz = sv.position().z() - rv.position().z();
  t["dx"]=dx;
  t["dy"]=dy;
  t["dz"]=dz;
  t["d"]=sqrt ( dx*dx + dy*dy + dz *dz );
  t["dt"]=sqrt ( dx*dx + dy*dy );

  float pullx = dx / sqrt ( rv.error().dxx() );
  float pully = dy / sqrt ( rv.error().dyy() );
  float pullz = dz / sqrt ( rv.error().dzz() );
  t["stx"]=pullx;
  t["sty"]=pully;
  t["stz"]=pullz;
  t["st"]=sqrt ( pullx*pullx + pully*pully + pullz *pullz );
  t["stt"]=sqrt ( pullx*pullx + pully*pully );
}

void ObserverTuples::addJet ( const vertigo::Jet & j, dataharvester::Tuple & t )
{
  t["ntracks"]=j.tracks().size();
  t["nvertices"]=j.vertices().size();
  t["id"]=j.id();
  t["angle"]=j.angle();
  // t["eta"]=j.eta();
  t["time"]=j.time();
  string mctag;
  mctag+=j.getMCTag();
  t["mctag_name"]=mctag;
  string mcphystag;
  mcphystag+=j.getMCPhysicsTag();
  t["mcphysicstag_name"]=mcphystag;

  t["mctag"]=PDGCodes::pDGCode ( j.getMCTag() );
  t["mcphystag"]=PDGCodes::pDGCode ( j.getMCPhysicsTag() );

  t["istagged"]=j.isTagged();
  float tag=-9999.;
  if ( j.isTagged() ) tag=j.getTag();
  t["rectag"]=tag;

  float x=-9999.;
  float y=-9999.;
  float z=-9999.;
  float px=-9999.;
  float py=-9999.;
  float pz=-9999.;

  if ( j.tracks().size() > 1 )
  {
    x=j.position().x();
    y=j.position().y();
    z=j.position().z();
    px=j.momentum().x();
    py=j.momentum().y();
    pz=j.momentum().z();
  }

  t["x"]=x;
  t["y"]=y;
  t["z"]=z;
  t["px"]=px;
  t["py"]=py;
  t["pz"]=pz;
}

#endif
