#include "L3TrackChecker.h"
#include "dataharvester/Writer.h"
#include <cmath>

namespace {
  void addCovMatrix ( dataharvester::Tuple & t, const EVENT::Track * trk )
  {
    t["cov0"]=trk->getCovMatrix()[0];
    t["cov1"]=trk->getCovMatrix()[1];
    t["cov2"]=trk->getCovMatrix()[2];
    t["cov3"]=trk->getCovMatrix()[3];
    t["cov4"]=trk->getCovMatrix()[4];
    t["cov5"]=trk->getCovMatrix()[5];
    t["cov6"]=trk->getCovMatrix()[6];
    t["cov7"]=trk->getCovMatrix()[7];
    t["cov8"]=trk->getCovMatrix()[8];
    t["cov9"]=trk->getCovMatrix()[9];
    t["cov10"]=trk->getCovMatrix()[10];
    t["cov11"]=trk->getCovMatrix()[11];
    t["cov12"]=trk->getCovMatrix()[12];
    t["cov13"]=trk->getCovMatrix()[13];
    t["cov14"]=trk->getCovMatrix()[14];
    t["cov15"]=trk->getCovMatrix()[15];
  }
}

void L3TrackChecker::check ( const EVENT::Track * trk,
    EVENT::MCParticle * m ) const
{
  dataharvester::Tuple t("L3Trk");
  t["chi2"]=trk->getChi2();
  double d0=trk->getD0();
  double phi=trk->getPhi();
  double omega=trk->getOmega();
  double z0=trk->getZ0();
  double tanlambda=trk->getTanLambda();

  double refx=trk->getReferencePoint()[0];
  double refy=trk->getReferencePoint()[1];
  double refz=trk->getReferencePoint()[2];

  t["d0"]=d0;
  t["phi"]=phi;
  t["omega"]=omega;
  t["z0"]=z0;
  t["tanlambda"]=tanlambda;
  float covd0d0 = trk->getCovMatrix()[0];
  float covphiphi = trk->getCovMatrix()[2];
  float covomegaomega = trk->getCovMatrix()[5];
  float covz0z0 = trk->getCovMatrix()[9];
  float covtanlambdatanlambda = trk->getCovMatrix()[14];

  t["std0"]=d0/sqrt(covd0d0);
  t["stphi"]=phi/sqrt(covphiphi);
  t["stomega"]=omega/sqrt(covomegaomega);
  t["stz0"]=z0/sqrt(covz0z0);
  t["sttanlambda"]=tanlambda/sqrt(covtanlambdatanlambda);

  t["refx"]=refx;
  t["refy"]=refy;
  t["refz"]=refz;

  addCovMatrix ( t, trk );

  t.fill();
  dataharvester::Writer::file("l3.root") << t;
}
