//
// C++ Interface: LcioWriter
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LCIOOBSERVER_H
#define LCIOOBSERVER_H

#include "Analysis/Observer/interface/Observer.h"

#include "lcio.h"
#include "IO/LCWriter.h"

#include "boost/shared_ptr.hpp"

/**
 * @author Fabian Moser <fabiamos@gmail.com>
 */
class LcioWriter :
    public Observer
{
  public:
    LcioWriter( const std::string & filename = "" );
    ~LcioWriter();
    void configure ( const std::string & filename );
    void process ( const vertigo::Event & );
    void process ( const std::exception &, const std::string & );
    LcioWriter * clone() const;
  private:
    boost::shared_ptr<lcio::LCWriter> theLCWriter;

};

#endif
