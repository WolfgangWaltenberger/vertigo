#ifndef _VertigoJet_H_
#define _VertigoJet_H_

#include <vector>
#include <rave/Point3D.h>
#include <rave/Vector3D.h>
#include <rave/Track.h>
#include <rave/Vertex.h>

/**
 *  \class VertigoJet a jet, contains mostly the set of tracks,
 *  plus reconstructed and(!!) simulated b-tag
 */

namespace vertigo {
  class Jet 
  {
    public:
      Jet ();
      Jet ( const std::vector < rave::Track > & tracks );
      Jet ( const std::vector < rave::Track > & tracks,
            const rave::Point3D & pos, const rave::Vector3D & mom );
      Jet(const std::vector < rave::Track > & tracks,
	  const rave::Point3D & pos, const rave::Vector3D & mom,
	  const rave::Covariance6D & error );
      void tag ( double ); //< set the "real" flavor tag
      double getTag() const; //< get the "real" flavor tag
      void mcTag( char ); //< set the monte carlo flavor tag
      char getMCTag() const; //< get the monte carlo flavor tag

      void mcPhysicsTag ( char ); //< set the monte carlo "physics" flavor tag
      char getMCPhysicsTag() const; //< get the physics monte carlo tag

      bool isTagged() const;
      bool hasMCTag() const;
      bool hasMCPhysicsTag() const;
      bool ghostHasError() const;

      std::vector < rave::Track > tracks() const;
      std::vector < rave::Vertex > vertices() const;
      void add ( const rave::Track & t );
      void add ( const rave::Vertex & );
      int id() const;

      rave::Point3D position() const; //< average position
      rave::Vector3D momentum() const; //< total momentum
      rave::Covariance6D error() const;
      double eta() const; //< jet eta
      double angle() const; //< opening angle

      float time() const; //< time spent on this jet (with e.g. flavor tagging)
      void setTime ( float );
      rave::Track ghost() const;

    private:
      void compute() const;

      std::vector < rave::Track > tracks_;
      std::vector < rave::Vertex > vertices_;
      double tag_;
      char mctag_;
      char mcphystag_;
      bool isTagged_;
      bool hasmctag_;
      bool hasmcphystag_;
      bool ghosthaserror_;
      int id_;

      mutable rave::Point3D position_;
      mutable rave::Vector3D momentum_;
      mutable rave::Covariance6D error_;
      mutable double angle_;
      mutable bool needsCompute_;

      float time_;
  };
}

#endif
