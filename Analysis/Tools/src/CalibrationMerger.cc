#include "Analysis/Tools/interface/CalibrationMerger.h"
#include <rave/impl/RaveBase/Converters/interface/RaveAlgorithmConverter.h>
#include <rave/impl/RaveBase/Converters/interface/AlgorithmToRaveConverter.h>
#include <rave/impl/RecoBTag/CombinedSV/interface/BTagAlgorithmCalibration.h>
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>
#include "VertigoHarvest/interface/CalibrationWriter.h"
#include "VertigoHarvest/interface/CalibrationBuilder.h"

using namespace std;

namespace {
  typedef std::vector < std::pair < combsv::CombinedSVCalibrationCategory *,                                                                        combsv::CombinedSVCalibrationHistogram * > > HistoCat;

  void fill ( const combsv::CombinedSVCalibrationHistogram * from,
              combsv::CombinedSVCalibrationHistogram * to )
  {
    unsigned sze = from->upperLimits().size();
    if ( sze != to->upperLimits().size() )
    {
      edm::LogError("CalibrationMerger") << "Different bin sizes!";
      return;
    }

    for ( unsigned i=0; i< sze ; i++ )
    {
      int fromcount = from->binCount( i );
      if ( fromcount > 0 ) to->addBinCount ( i, fromcount );
    }
  }

  BTagAlgorithmCalibration mergeTwo ( const HistoCat & hc1, const HistoCat & hc2 )
  {
    BTagAlgorithmCalibration calib ( hc1 );

    for ( HistoCat::const_iterator i=hc2.begin(); i!=hc2.end() ; ++i )
    {
      // add information to calib
      combsv::CombinedSVCalibrationCategory * cat = i->first;
      combsv::CombinedSVCalibInput inp ( cat->vertexType(), cat->partonType(),
          cat->taggingVar(), ( cat->jetPtMin() + cat->jetPtMax()) / 2.,
          ( cat->jetEtaMin() + cat->jetEtaMax() ) / 2. );
      combsv::CombinedSVCalibrationHistogram * histo = 
        const_cast < combsv::CombinedSVCalibrationHistogram * > (calib.getCalibData  ( inp ));
      if ( histo )
      {
        fill ( i->second, histo );
      } else {
        edm::LogError("CalibrationMerger") << "Cannot find histogram for " << inp;
      }
    }
    return calib;
  }

  void addMeta ( rave::Calibration & calib, const vector< rave::CalibrationRun > & meta )
  {
    for ( vector< rave::CalibrationRun >::const_iterator i=meta.begin(); i!=meta.end() ; ++i )
    {
      calib.add ( *i );
    }
  }

  void copy ( const std::string & in, const std::string & out )
  {
    FILE * fin = fopen ( in.c_str(), "r" );
    FILE * fout = fopen ( out.c_str(), "w" );
    int c;
    while ((c = fgetc(fin)) != EOF)
    {
      fputc(c, fout );
    }

    fclose( fin );
    fclose( fout );        
  }
}

void CalibrationMerger::add ( const std::string & f )
{
  files_.push_back ( f );
}

void CalibrationMerger::merge( const std::string & out )
{
  if ( files_.size() == 0 ) return;
  if ( files_.size() == 1 )
  {
    copy ( files_[0], out );
    return;
  }
  RaveAlgorithmConverter conv;
  vector < rave::Calibration > calibrations;
  BTagAlgorithmCalibration calib;

  for ( vector< string >::const_iterator i=files_.begin(); i!=files_.end() ; ++i )
  {
    edm::LogInfo("CalibrationMerger") << "Read " << *i;
    rave::Calibration c1=CalibrationBuilder::create ( *i );
    calibrations.push_back ( c1 );
    HistoCat hc1 = conv.convert ( c1 );
    if ( i == files_.begin() )
    {
      calib = BTagAlgorithmCalibration ( hc1 );
    } else {
      calib = mergeTwo ( calib.dumpCalibration(), hc1 );
    }
  }

  AlgorithmToRaveConverter bconv;
  rave::Calibration newc = bconv.convert ( calib.dumpCalibration() );

  edm::LogInfo("CalibrationMerger") << "Add meta information";
  for ( vector< rave::Calibration >::const_iterator i=calibrations.begin(); 
        i!=calibrations.end() ; ++i )
  {
     addMeta ( newc, i->runs() );
  }
  edm::LogInfo("CalibrationMerger") << "Write " << out;
  CalibrationWriter ( out ).save ( newc );
}

