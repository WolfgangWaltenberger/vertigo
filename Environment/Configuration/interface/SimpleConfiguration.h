#ifndef _SimpleConfiguration_H_
#define _SimpleConfiguration_H_

#include "Environment/Configuration/interface/VMultiType.h"
#include <string>

class SimpleConfiguration
{
    /**
     *  Simple implementation of CARF's SimpleConfiguration.
     */
public:
    static SimpleConfiguration* current();
    void status() const;
    void modify( const std::string & name, const vertigo::VMultiType & value );
    void setDefault( const std::string & name, const vertigo::VMultiType & value );
    vertigo::VMultiType value( const std::string & name );

private:
    SimpleConfiguration();
};

#endif
