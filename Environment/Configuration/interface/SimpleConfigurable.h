#ifndef _SimpleConfigurable_H_
#define _SimpleConfigurable_H_

#include "Environment/Configuration/interface/SimpleConfiguration.h"
#include <string>

template < class T >
class SimpleConfigurable
{
    /**
     *  Simple implementation of CARF's SimpleConfigurable.
     *  Is only used in the standalone version.
     */
public:
    SimpleConfigurable( T value, const std::string & name ) : theName(name)
    {
        SimpleConfiguration::current()->setDefault( name, value );
    };

    T value()
    {
        return SimpleConfiguration::current()->value(theName);
    };
private:
    std::string theName;
};

#endif
