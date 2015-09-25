/*
 * RaveParticleVertexing.cc
 */

// Plugin includes
#include "RaveParticleVertexing.h"
#include "L3LcioToRaveObjects.h"
#include "RaveToL3LcioObjects.h"
#include "LDCMagneticField.h"
#include "LDCPropagator.h"

// LCIO includes
#include <EVENT/LCIO.h>
#include <EVENT/ReconstructedParticle.h>
#include <IMPL/ReconstructedParticleImpl.h>
#include <IMPL/LCCollectionVec.h>
#include <IMPL/LCRelationImpl.h>
#include <UTIL/PIDHandler.h>

// CLHEP includes
#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Matrix/SymMatrix.h"

// Rave includes
#include <rave/Track.h>
#include <rave/Vertex.h>

// HistFile io
#include <fstream>
#include <iomanip>

//BCR>> apparently needed to access the magentic field
// Include Marlin
#include <marlin/Global.h>
#include "marlin/Processor.h"
// Include Gear header files
#include <gear/BField.h>
#include <gearimpl/Vector3D.h> 

#include <CLHEP/Vector/LorentzVector.h>

using namespace marlin;
//BCR<<


marlinrave::RaveParticleVertexing RaveParticleVertexingInstance;

namespace marlinrave {
  
  RaveParticleVertexing::RaveParticleVertexing() : Processor("RaveParticleVertexing")
						   , _inputParticleCollectionName("ParticleCanditates")
						   , _outputParticleCollectionName("refittedParticles")
						   , _relationsCandFitName("relationsCandFit")    
						   , _outputVertexCollName("fittedVertices")
						   , _outputTrackCollName("refittedTracks")
						   , _outputDaughterCollName("decayDaughters")
						   , _method("default"), _verbose(0) {
    
    _description
      = "RaveParticleVertexing does vertex finding and fitting using a wide variety of algorithms";
    
    registerInputCollection(EVENT::LCIO::RECONSTRUCTEDPARTICLE,
			    "InputParticles",
			    "Particles containing tracks. All tracks contained "
			    "in a particle are going to be fitted to a common vertex",
			    _inputParticleCollectionName, _inputParticleCollectionName);
    
    registerOutputCollection(EVENT::LCIO::RECONSTRUCTEDPARTICLE,
			     "OutputParticles",
			     "Particles containing refitted tracks and vertex information",
			     _outputParticleCollectionName, _outputParticleCollectionName);
    registerOutputCollection(
			     EVENT::LCIO::LCRELATION,
			     "RelationsCandFit",
			     "Name of the collection to hold the weighted relations between "
			     "the input Candiate and the sucessfully fitted decays ",
			     _relationsCandFitName, _relationsCandFitName);
    

    registerOutputCollection(EVENT::LCIO::VERTEX,
			     "OutputVertices",
			     "name of the collection containing the fitted vertices of the decay candidates",
			     _outputVertexCollName, _outputVertexCollName);
    registerOutputCollection(EVENT::LCIO::TRACK,
			     "OutputTracks",
			     "name of the collection containing the refitted tracks of the decay candidates",
			     _outputTrackCollName, _outputTrackCollName);
    registerOutputCollection(EVENT::LCIO::RECONSTRUCTEDPARTICLE,
			     "OutputDaughters",
			     "name of the collection containing the daughter particles of the decay candidates",
			     _outputDaughterCollName, _outputDaughterCollName);



    registerProcessorParameter("Method",
			       "Method used for vertex finding and fitting",
			       _method, _method);
    registerOptionalParameter("Verbose", "Print results to stdout",
			      _verbose, _verbose);
  }
  
  RaveParticleVertexing::~RaveParticleVertexing() {
    end();
  }
  
  RaveParticleVertexing* RaveParticleVertexing::newProcessor() {
    return new RaveParticleVertexing;
  }
  
  void RaveParticleVertexing::init() {
    streamlog_out(DEBUG) << "RaveParticleVertexing::init ..." << std::endl;
    printParameters();


    int raveVerbosity = 1;
    if (_verbose)
		raveVerbosity = _verbose;
    
    _factory = boost::shared_ptr<rave::VertexFactory>(new rave::VertexFactory(
									      LDCMagneticField(), LDCPropagator(), _method, raveVerbosity));
  }
  
  void RaveParticleVertexing::processEvent(EVENT::LCEvent * event) {

    
    streamlog_out(DEBUG) << "processEvent ..." << std::endl;
    
    // BCR>>
    // obtain magentic field, allow that it might change on event-by-event basis 
    const gear::BField & bField = Global::GEAR->getBField();
    double zBField = (bField.at(gear::Vector3D(0., 0., 0.))).z();
    // Track curvature to transverse momentum conversion factor
    double OmegaToPt = 2.99755e-4; // BCR ---------------------->   HACK ALERT:  should be defined in some header common to MakeChargedParticle and this class
    //                                                                              or even better we write a utilty which takes tracks and returns a double mom[3] 
    // BCR<<
    
    // Fetch the specified collection (parameter Tracks)
    EVENT::LCCollection* inputParticles 
      = event->getCollection(_inputParticleCollectionName);
    
    streamlog_out(MESSAGE) << "Found " << inputParticles->getNumberOfElements()
			   << " Particles, now searching vertices ... " << std::endl;
    
    
    // Convert the found tracks to Rave objects
    L3LcioToRaveObjects inputConverter;
    RaveToL3LcioObjects outputConverter;
    
    // creating output collections
    IMPL::LCCollectionVec* outputParticles = new LCCollectionVec(
								 EVENT::LCIO::RECONSTRUCTEDPARTICLE);
    
    IMPL::LCCollectionVec* outputRelations = new IMPL::LCCollectionVec(
								       EVENT::LCIO::LCRELATION);

    // additional output collection for memory management
    IMPL::LCCollectionVec* outputTracks    = new LCCollectionVec(
								 EVENT::LCIO::TRACK);
    IMPL::LCCollectionVec* outputVertices  = new LCCollectionVec(
								 EVENT::LCIO::VERTEX);
    IMPL::LCCollectionVec* outputDaughters = new LCCollectionVec(
								 EVENT::LCIO::RECONSTRUCTEDPARTICLE);

    
    // prepare the PID handling
    UTIL::PIDHandler in_handlerPID(inputParticles);

    UTIL::PIDHandler out_handlerPID(outputParticles);
    out_handlerPID.addAlgorithm("REC", StringVec());  // contains ID as which was reconstructed
    out_handlerPID.addAlgorithm("GEN", StringVec());  // contains ID of Generator level truth 

    
    //loop over all particles in the input collection
    for (int j = 0; j < inputParticles->getNumberOfElements(); j++) {
	  
      
      EVENT::ReconstructedParticle
	*lcioParticle =
	dynamic_cast<EVENT::ReconstructedParticle*> (inputParticles->getElementAt(j));
      
      std::vector<rave::Track> raveTracks;
      
      const ReconstructedParticleVec daughters = lcioParticle->getParticles();
	  
      for(ReconstructedParticleVec::const_iterator daughter = daughters.begin();
	  daughter != daughters.end(); daughter++) {
	const TrackVec trackVec = (*daughter)->getTracks();
	for(TrackVec::const_iterator track = trackVec.begin();
	    track != trackVec.end(); track++) {
	  
	  try {
	    raveTracks.push_back(inputConverter.convert(*track));
	  } catch (std::string msg) {
	    streamlog_out(ERROR) << "Track could not be converted: " << msg
				 << std::endl;
	  } catch (...) {
	    streamlog_out(ERROR) << "Track could not be converted!"
				 << std::endl;
	  }
	}
      }

      
      streamlog_out(DEBUG) << "Calling the VertexFactory ..." << std::endl;
      // Do the reconstruction using the method given (parameter Method)
      std::vector<rave::Vertex> raveVertices = _factory->create(raveTracks);
      
      streamlog_out(DEBUG) << "Found " << raveVertices.size() << " vertices"
			   << std::endl;
      
      if (raveVertices.size() > 1) {
	streamlog_out(DEBUG4) << "INFO: candidate split into several vertices, reject this candidate " 
				<< raveVertices.size() << std::endl;	  
	continue;  // loop over input particles 
      }
      
      for (std::vector<rave::Vertex>::iterator raveVertex =
	     raveVertices.begin(); raveVertex != raveVertices.end(); raveVertex++) {
	
	IMPL::VertexImpl* outputVertex = outputConverter.convert(*raveVertex);
	outputVertex->setAlgorithmType(_method);
	
	
	if (raveVertex->hasRefittedTracks()) {
	  
	  if (raveVertex->refittedTracks().size()
	      != lcioParticle->getParticles().size()) {
	    streamlog_out(DEBUG4) << "INFO: ignore candidate with missmatching N(daughters):" 
				    << "  Input "  << lcioParticle->getParticles().size()
				    << "  Output " << raveVertex->refittedTracks().size() 
				    << std::endl;
	  } else {
	    
	    
	    // only now book output particle
	    lcio::ReconstructedParticleImpl* outputParticle =
	      new ReconstructedParticleImpl();
	    


	    // some properties of the input particle can be directly transfered to the output particle
	    outputParticle->setType(lcioParticle->getType());
	    outputParticle->setCharge(lcioParticle->getCharge());
	    outputParticle->setReferencePoint(lcioParticle->getReferencePoint()); // << actually should be validated
	    
	    
	    double sum_px = 0.;
	    double sum_py = 0.;
	    double sum_pz = 0.;
	    double sum_e = 0.;
	    
	    
	    // Add the assigned refitted tracks  
	    std::vector<rave::Track> raveTracks =
	      raveVertex->refittedTracks();
	    // still also need original tracks to map refitted tracks to input particles
	    std::vector<rave::Track> origRaveTracks =
	      raveVertex->tracks();
	    // TODO  make the refitted track remember from which original object it was derived
	    
	    for (unsigned int ld=0; ld < lcioParticle->getParticles().size(); ld++) {
	      
	      IMPL::TrackImpl* outputTrack = NULL;
	      int icTrack =0;  // index for some bookkeeping
	      for (unsigned int ii = 0; ii <origRaveTracks.size(); ii++) {		
		// TODO reinterpret_cast is dangerous !!!! maybe switch to boost::any ??
		EVENT::LCObject* testTrack = reinterpret_cast<EVENT::LCObject *> (origRaveTracks[ii].originalObject());
		if (testTrack->id() == lcioParticle->getParticles()[ld]->getTracks()[0]->id()) {
		  outputTrack = outputConverter.convert(
							raveTracks[ii], raveVertex->position());
		  icTrack = ii; 
		  break;
		}
	      }
	      if (outputTrack==NULL) {
		streamlog_out(MESSAGE4) << "### FATAL ERROR no refitted track for daughter " << ld << " found" << std::endl;  
		break; // continue on next daughter
	      }
	      
	      outputParticle->addTrack(outputTrack);
	      
	      
	      
	      // COMPILES ?????
	      //	  ReconstructedParticleImpl* newDaughter =
	      //	      new ReconstructedParticleImpl(lcioParticle->getParticles()[ld]);
	      // NO
	      //	      newDaughter = (ReconstructedParticleImpl*) lcioParticle->getParticles()[ld].clone();

	      

	      
	      ReconstructedParticleImpl* newDaughter =  new ReconstructedParticleImpl();
	      	   	      
	      newDaughter->setType(
				     lcioParticle->getParticles()[ld]->getType());
	      newDaughter->setCharge(
				     lcioParticle->getParticles()[ld]->getCharge());
	      newDaughter->setMass(
				   lcioParticle->getParticles()[ld]->getMass());
	      
	      // calculate momentum vector from track
	      double tr_mass  = lcioParticle->getParticles()[ld]->getMass();
	      double tr_omega = outputTrack->getOmega();
	      double tr_tanlam = outputTrack->getTanLambda();
	      double tr_phi0 = outputTrack->getPhi();
	      double tr_pt = OmegaToPt * fabs(zBField / tr_omega);
	      double tr_px = tr_pt * cos(tr_phi0);
	      double tr_py = tr_pt * sin(tr_phi0);
	      double tr_pz = tr_pt * tr_tanlam;
	      
	      const double tr_mom[3] = { tr_px, tr_py, tr_pz };
	      double tr_e = sqrt(tr_px * tr_px + tr_py * tr_py
				 + tr_pz * tr_pz + tr_mass * tr_mass);
	      
	      
	      streamlog_out(DEBUG4) << "  Input = " << ld 
				    << "  MASS = " << lcioParticle->getParticles()[ld]->getMass()
				    << "  E = " << lcioParticle->getParticles()[ld]->getEnergy()
				    << "  Pt = " << sqrt(lcioParticle->getParticles()[ld]->getMomentum()[0]*
							 lcioParticle->getParticles()[ld]->getMomentum()[0]
							 +
							 lcioParticle->getParticles()[ld]->getMomentum()[1]*
							 lcioParticle->getParticles()[ld]->getMomentum()[1])
				    << "  Pz = " << lcioParticle->getParticles()[ld]->getMomentum()[2]
				    << "  phi = " << atan2(lcioParticle->getParticles()[ld]->getMomentum()[1],
							   lcioParticle->getParticles()[ld]->getMomentum()[0])
				    << "  Px = " << lcioParticle->getParticles()[ld]->getMomentum()[0]
				    << "  Py = " << lcioParticle->getParticles()[ld]->getMomentum()[1]
				    << std::endl;
	      
	      streamlog_out(DEBUG4) << " Output = " << icTrack 
				    << "  mass = " << tr_mass
				    << "  e = " << tr_e
				    << "  pt = " << tr_pt
				    << "  pz = " << tr_pz
				    << "  phi = " << tr_phi0
				    << "  px = " << tr_px
				    << "  py = " << tr_py
		
				    << std::endl;
	      
	      
	      
	      
	      streamlog_out(DEBUG4) << "  RefPoint Output " << ld 
				    << "  x = " << outputTrack->getReferencePoint()[0]
				    << "  y = " << outputTrack->getReferencePoint()[1]
				    << "  z = " << outputTrack->getReferencePoint()[2]
				    << std::endl;
	      streamlog_out(DEBUG4) << "  RefPoint Input  " << ld 
				    << "  x = " << lcioParticle->getParticles()[ld]->getTracks()[0]->getReferencePoint()[0]
				    << "  y = " << lcioParticle->getParticles()[ld]->getTracks()[0]->getReferencePoint()[1]
				    << "  z = " << lcioParticle->getParticles()[ld]->getTracks()[0]->getReferencePoint()[2]
				    << std::endl;
	      
	      // track parameter sanity checks
	      streamlog_out(DEBUG4) << ld <<":  " 
				      << " InputTrk: " 
				      << "  id = "     << lcioParticle->getParticles()[ld]->getTracks()[0]->id()
				      << "  Omega = "  << lcioParticle->getParticles()[ld]->getTracks()[0]->getOmega()
				      << "  TanLam = " << lcioParticle->getParticles()[ld]->getTracks()[0]->getTanLambda()
				      << "  Phi = "    << lcioParticle->getParticles()[ld]->getTracks()[0]->getPhi()
				      << "  Z0 = "     << lcioParticle->getParticles()[ld]->getTracks()[0]->getZ0()*1000.
				      << "  D0 = "     << lcioParticle->getParticles()[ld]->getTracks()[0]->getD0()*1000.
				      << std::endl;
	      streamlog_out(DEBUG4) << icTrack <<":  " 
				      << "OutputTrk: " 
				      << "  id = "     << outputTrack->id()
				      << "  Omega = "  << outputTrack->getOmega()
				      << "  TanLam = " << outputTrack->getTanLambda()
				      << "  Phi = "    << outputTrack->getPhi()
				      << "  Z0 = "     << outputTrack->getZ0()*1000.
				      << "  D0 = "     << outputTrack->getD0()*1000.
				      << std::endl;
	      
	      
	      newDaughter->setMomentum(tr_mom);
	      sum_px += tr_px;
	      sum_py += tr_py;
	      sum_pz += tr_pz;
	      sum_e += tr_e;
	      
	      // reserve original track associated with the daughter particle
	      newDaughter->addTrack(lcioParticle->getParticles()[ld]->getTracks()[0]);
	      // 
	      outputParticle->addParticle(newDaughter);
	      

	      outputDaughters->addElement(newDaughter);
	      outputTracks->addElement(outputTrack);
	      
	      //	      // BCR >> should one delete the daugther after use?
	      //	      delete newDaughter; 
	      //	      newDaughter = NULL;
	      //	      delete outputTrack;
	      //	      outputTrack = NULL;

	      // BCR <<

	    } // endfor loop over daughters
	    
	    
	    const double sum_mom[3] = { sum_px, sum_py, sum_pz };
	    double sum_mass = sqrt(sum_e * sum_e - (sum_px * sum_px
							+ sum_py * sum_py + sum_pz * sum_pz));
	    outputParticle->setMomentum(sum_mom);
	    outputParticle->setMass(sum_mass);
	    outputParticle->setEnergy(sum_e);
		
	    streamlog_out(DEBUG4) << "   Input Decay  " 
				  << "  MASS = " << lcioParticle->getMass()
				  << "  E = "    << lcioParticle->getEnergy()
				  << "  Pt = "   << sqrt(lcioParticle->getMomentum()[0]*
							 lcioParticle->getMomentum()[0]
							 +
							 lcioParticle->getMomentum()[1]*
							 lcioParticle->getMomentum()[1])
				  << "  Pz = " << lcioParticle->getMomentum()[2]
				  << "  phi = "<< atan2(lcioParticle->getMomentum()[1],
							lcioParticle->getMomentum()[0])
				  << "  Px = " << lcioParticle->getMomentum()[0]
				  << "  Py = " << lcioParticle->getMomentum()[1]
				  << std::endl;
	    streamlog_out(DEBUG4) << "  Output Decay  " 
				  << "  MASS = " << sum_mass
				  << "  E = "    << sum_e
				  << "  Pt = "   << sqrt(sum_px*sum_px + sum_py*sum_py)
				  << "  Pz = "   << sum_pz
				  << "  phi = "  << atan2(sum_py,sum_px)
				  << "  Px = "   << sum_px
				  << "  Py = "   << sum_py
				  << std::endl;
	    
	    
	    //create the output particle from the tracks and the vertex
	    outputParticle->setStartVertex(outputVertex);

 	    // transfere PID info
	    
	    int rec_PDG_ID = in_handlerPID.getParticleID(lcioParticle,
							 in_handlerPID.getAlgorithmID("REC")).getPDG();
	    int gen_PDG_ID = in_handlerPID.getParticleID(lcioParticle,
							 in_handlerPID.getAlgorithmID("GEN")).getPDG();
	    
	    out_handlerPID.setParticleID(outputParticle, 0, rec_PDG_ID, 1.0,
					 out_handlerPID.getAlgorithmID("REC"), FloatVec());
	    out_handlerPID.setParticleID(outputParticle, 1, gen_PDG_ID, 1.0,
					 out_handlerPID.getAlgorithmID("GEN"), FloatVec());

	    outputParticle->setParticleIDUsed(outputParticle->getParticleIDs()[0]);		

	    outputParticles->addElement(outputParticle);


	    IMPL::LCRelationImpl * relCandFit = new IMPL::LCRelationImpl(
									 lcioParticle, outputParticle, 1.);
	    outputRelations->addElement(relCandFit);
	    
	    outputVertices->addElement(outputVertex);

	    //// BCR>> remove local info after it was handed over to LCIO
	    //delete outputVertex;
	    //outputVertex = NULL;
	    //// BCR<<
	    
	    
	  } // endif else ( !(nTracks(vertex)!=ntracks(input)) 
	  
	} // endif  vertex  has fitted tracks 
	
      }  // endfor loop over rave Vertices
    }  // endfor loop over input candidates
 

    event->addCollection(outputRelations, _relationsCandFitName);
    event->addCollection(outputParticles, _outputParticleCollectionName);
    
    event->addCollection(outputTracks,    _outputTrackCollName);
    event->addCollection(outputVertices,  _outputVertexCollName);
    event->addCollection(outputDaughters, _outputDaughterCollName);
    
    
  }
  

  void RaveParticleVertexing::check(EVENT::LCEvent * event) {
    // nothing to check here - could be used to fill checkplots
    // in reconstruction processor
  }


  void RaveParticleVertexing::end() {


  }




} // namespace marlinrave

