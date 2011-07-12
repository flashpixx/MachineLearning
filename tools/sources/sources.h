/** 
 @cond
 ############################################################################
 # LGPL License                                                             #
 #                                                                          #
 # This file is part of the Machine Learning Framework.                     #
 # Copyright (c) 2010, Philipp Kraus, <philipp.kraus@flashpixx.de>          #
 # This program is free software: you can redistribute it and/or modify     #
 # it under the terms of the GNU Lesser General Public License as           #
 # published by the Free Software Foundation, either version 3 of the       #
 # License, or (at your option) any later version.                          #
 #                                                                          #
 # This program is distributed in the hope that it will be useful,          #
 # but WITHOUT ANY WARRANTY; without even the implied warranty of           #
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
 # GNU Lesser General Public License for more details.                      #
 #                                                                          #
 # You should have received a copy of the GNU Lesser General Public License #
 # along with this program. If not, see <http://www.gnu.org/licenses/>.     #
 ############################################################################
 @endcond
 **/

#ifdef MACHINELEARNING_SOURCES

#ifndef MACHINELEARNING_TOOLS_SOURCES_H
#define MACHINELEARNING_TOOLS_SOURCES_H

namespace machinelearning { 
    namespace tools { 
        
        /** namespace with classes for generating example data
         * $LastChangedDate$
         * @todo adding support for OpenStreetMap http://wiki.openstreetmap.org/wiki/API / http://libosmscout.sourceforge.net
         * @todo adding support for Apache Cassandra http://en.wikipedia.org/wiki/Apache_Cassandra
         * @todo adding support for mySQL / MS SQL / Postgres SQL / Oracle via eg http://www.sqlapi.com/
         * @todo adding twitter support https://dev.twitter.com/docs and https://dev.twitter.com/docs/streaming-api/methods
         * @todo adding support for reading HTML data with WGet / Curl
         **/
        namespace sources {};
    
    };
};

#include "nntp.h"
#include "wikipedia.h"
#include "cloud.hpp"

#endif
#endif
