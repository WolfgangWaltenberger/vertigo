%module(directors="1") vertigo
%{
#ifndef RAVE
#define RAVE
#endif
#include "Vertigo/EventGeneration/interface/EventGenerator.h"
#include "Vertigo/EventGeneration/interface/EventFactory.h"
#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include "Vertigo/main/Version.h"
#include "tools/interface/XSection.h"
#include "tools/interface/BayesianUpperLimit.h"
#include "tools/interface/LHEHelpers.h"
#include "Vertigo/EventLoop/interface/VertexMetaFactory.h"
#include "Vertigo/Converters/interface/VertigoToCmsObjects.h"
#include "VertigoHarvest/interface/VertigoHarvest.h"
#include "Environment/VertigoObjects/interface/VertigoJet.h"
#include "Environment/MCObjects/interface/SimParticle.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Vertigo/Event/interface/Event.h"
#include "Environment/MCObjects/interface/PDGCodes.h"
#include "Environment/Utilities/interface/VertigoStreamers.h"
#include "Environment/Configuration/interface/SimpleConfiguration.h"
#include "Environment/Configuration/interface/VMultiType.h"
#include "Analysis/Observer/interface/Observer.h"
#include "Analysis/Observer/interface/ObserverManager.h"
#include "Vertigo/TrackSmearing/interface/RecTrackSmearer.h"
#include "Vertigo/TrackSmearing/interface/GaussianRecTrackSmearer.h"
#include <rave/impl/FWCore/ParameterSet/interface/ParameterSet.h>
#include <rave/impl/RaveBase/RaveEngine/interface/RaveParameterSetBuilder.h>
#include <rave/MagneticField.h>
#include <rave/impl/RecoVertex/VertexTools/interface/GeometricAnnealing.h>
#include <rave/impl/RecoVertex/VertexTools/interface/DeterministicAnnealing.h>
#include <rave/impl/CommonTools/Statistics/interface/ChiSquaredProbability.h>

typedef std::ostream ErrorObj;
%}

%ignore vertigo::VMultiType::operator=;
%ignore vertigo::VMultiType::operator double;
%ignore vertigo::VMultiType::operator float;
%ignore vertigo::VMultiType::operator long;
%ignore vertigo::VMultiType::operator std::string;
%ignore vertigo::VMultiType::operator int;
%include "exception.i"

%exception EventFactory::next {
    try {
        $action
    }
    catch (...) {
        PyErr_SetString (PyExc_StopIteration , "harvest stop" );
        return NULL;
    }
}

%include <rave/swig/rave_autoconf.i>

%rename(VertigoSimTrack) vertigo::SimTrack;
%rename(VertigoVersion) vertigo::Version;
%rename(VertigoEvent) vertigo::Event;
%rename(VertigoSimParticle) vertigo::SimParticle;
%rename(VertigoSimVertex) vertigo::SimVertex;
%rename(VertigoLShift) edm::operator<<;

%include "std_map.i"
%include "std_pair.i"

%feature("nodirector") ObserverManager;
%feature("director") Observer;
%feature("nodirector") EventGenerator;
%feature("nodirector") EventFactory;


#define RAVE
%include "Vertigo/main/Version.h"
%include "Vertigo/EventGeneration/interface/EventGenerator.h"
%include "Vertigo/EventGeneration/interface/EventFactory.h"
%include "Vertigo/Converters/interface/VertigoToCmsObjects.h"
%include "Vertigo/EventLoop/interface/LoopSettings.h"
%include "Environment/MCObjects/interface/PDGCodes.h"
%include "Vertigo/EventLoop/interface/VertexMetaFactory.h"
%include "VertigoHarvest/interface/VertigoHarvest.h"
%include "Environment/VertigoObjects/interface/VertigoJet.h"
%include "Environment/MCObjects/interface/SimParticle.h"
%include "Environment/MCObjects/interface/SimVertex.h"
%include "Environment/MCObjects/interface/SimTrack.h"
%include "Vertigo/Event/interface/Event.h"
%include "Environment/Utilities/interface/VertigoStreamers.h"
%include "Environment/Configuration/interface/SimpleConfiguration.h"
%include "Environment/Configuration/interface/VMultiType.h"
%include "Analysis/Observer/interface/Observer.h"
%include "Analysis/Observer/interface/ObserverManager.h"
%include "Vertigo/TrackSmearing/interface/RecTrackSmearer.h"
%include "Vertigo/TrackSmearing/interface/GaussianRecTrackSmearer.h"
%include "tools/interface/XSection.h"
%include "tools/interface/BayesianUpperLimit.h"
%include "tools/interface/LHEHelpers.h"
%include <rave/MagneticField.h>
%include <rave/impl/FWCore/ParameterSet/interface/ParameterSet.h>
%include <rave/impl/CommonTools/Statistics/interface/ChiSquaredProbability.h>
%include <rave/impl/RaveBase/RaveEngine/interface/RaveParameterSetBuilder.h>
%include <rave/impl/RecoVertex/VertexTools/interface/AnnealingSchedule.h>
%include <rave/impl/RecoVertex/VertexTools/interface/GeometricAnnealing.h>
%include <rave/impl/RecoVertex/VertexTools/interface/DeterministicAnnealing.h>

%template(getStringParameter) edm::ParameterSet::getParameter<std::string>;
%template(getIntParameter) edm::ParameterSet::getParameter<int>;
%template(getFloatParameter) edm::ParameterSet::getParameter<float>;
%template(getDoubleParameter) edm::ParameterSet::getParameter<double>;
%template(getNestedParameter) edm::ParameterSet::getParameter<edm::ParameterSet>;
%template(SMSMassesNames) std::map < std::string, float >;

namespace std {
   %template(VertigoSimTrackContainer) vector<vertigo::SimTrack>;
   %template(VertigoSimParticleContainer) vector<vertigo::SimParticle>;
   %template(VertigoSimVertexContainer) vector<vertigo::SimVertex>;
   %template(VertigoJetContainer) vector<vertigo::Jet>;
   %template(VectorFloat) vector < float >;
   %template(VectorString) vector < string >;
};

%extend rave::Point3D {
   rave::Point3D copy ( )
   {
       return rave::Point3D ( *self );
   }
   rave::Point3D __mul__ ( double scale  )
   {
     rave::Point3D ret ( *self );
     ret.multiply ( scale );
     return ret;
   }
   rave::Point3D __add__ ( const rave::Point3D & other )
   {
     return ((*self)+other);
   }

   rave::Point3D __sub__ ( const rave::Point3D & other )
   {
     return ((*self)-other);
   }
}

%extend rave::Vector4D {
   rave::Vector4D copy ( )
   {
       return rave::Vector4D ( *self );
   }

   rave::Vector4D __add__ ( const rave::Vector4D & other )
   {
     return ((*self)+other);
   }

   rave::Vector4D __sub__ ( const rave::Vector4D & other )
   {
     return ((*self)-other);
   }
}

%extend rave::Vector6D {
   rave::Vector6D copy ( )
   {
       return rave::Vector6D ( *self );
   }
}

%extend rave::Vector7D {
   rave::Vector7D copy ( )
   {
       return rave::Vector7D ( *self );
   }
}

%extend edm::ParameterSet {
   %pythoncode {
      def __str__ ( self ):
        return self.toString();
   }
}

STRINGHELPER(vertigo::SimTrack);
STRINGHELPER(vertigo::SimParticle);
STRINGHELPER(vertigo::SimVertex);
