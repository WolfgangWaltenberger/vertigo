#ifndef _VertexMetaFactory_H_
#define _VertexMetaFactory_H_

  /**
   *
   *  \class VertexMetaFactory
   *  a "meta" factory that produces the VertexFactory for a particular
   *  experiment.
   */

#include <string>
#include <map>
#include <rave/VertexFactory.h>

class VertexMetaFactory {
public:

  static VertexMetaFactory & Instance();

  /**
   *  retrieve a factory
   */
  const rave::VertexFactory * get ( 
      const std::string & skin, const std::string & method="default",
      int verbosity=0 );

  /** ask if a skin is available, trigger loading the plugin,
   *  if necessary
   */
  bool query ( const std::string & skin );

  /** 
   *  register a vertex factory, usually called
   *  by VertexFactoryBuilder
   */
  void registerFactory ( const std::string & name, rave::VertexFactory *,
     const std::string & description = "" ); 

  /**
   *  get the description of a factory
   */
  std::string getDescription ( const std::string & skin );

  /** get a description of all available factories
   */
  std::map < std::string, std::string > getDescriptions();

private:
  VertexMetaFactory(); //< private constructor - singleton
  std::map < std::string, rave::VertexFactory * > theFactories;
  std::map < std::string, std::string > theDescriptions;
};

#endif // _VertexMetaFactory_H_
