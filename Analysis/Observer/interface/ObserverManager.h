#ifndef _ObserverManager_H_
#define _ObserverManager_H_

#include "Analysis/Observer/interface/Observer.h"
#include <map>
#include <string>

/*! \class ObserverManager
 *  Class that manages all observers 
 *  (eventually they will have to register here)
 *
 *  The ObserverManager guarantees thread safety
 *  by locking the whole manager while
 *  dispatching the event to the observers.
 */

class ObserverManager : public Observer {

public:
  static ObserverManager & Instance();
  void registerObserver ( const std::string & name, Observer * o,
                          const std::string & description,
                          bool enable );
  bool disable ( const std::string & );
  void disableAll();
  void configure ( const std::string & );


  /**
   *  Configure observer "name", with the configuration string "conf".
   */
  void configure ( const std::string & name, const std::string & conf );

  /** 
   *  Enable observer
   */
  bool enable ( const std::string & );
  std::string describe ( const std::string & );
  void process ( const vertigo::Event & );
  void process ( const std::exception &, const std::string & type = "std" );
  // void process ( const edm::ParameterSet & );

  /**
   *  get an observer.
   *  \paramname newinstance Always ask for a new instance?
   *  Default is to have only one Instance per Observer.
   */
  Observer * get ( const std::string &, bool verbose=true, bool newinstance=false );

  std::map < std::string, Observer * > get ();

  ~ObserverManager();
  ObserverManager * clone() const;
  /** calls VertigoHarvest::close() */
  void close();
  void destroy();

private:
  ObserverManager ( const ObserverManager & );
  ObserverManager ();
  std::map < std::string, Observer * > theObservers;
  std::map < std::string, bool > theIsEnabled;
  std::map < std::string, std::string > theDescription;
};

#endif // _ObserverManager_H_
