#include "Analysis/Observer/interface/Skeleton.h"

void Skeleton::operator() ( const vertigo::Event & e )
{
}

void Skeleton::operator() ( const std::exception & e )
{
}

void Skeleton::configure ( const std::string & conf )
{
}

Skeleton::~Skeleton() {}

Skeleton * Skeleton::clone() const
{
  return new Skeleton ( *this );
}

#include "Analysis/Observer/interface/ObserverBuilder.h"
namespace {
  ObserverBuilder<Skeleton> t( "Skeleton",
      "Skeleton code of an observer", false );
}

