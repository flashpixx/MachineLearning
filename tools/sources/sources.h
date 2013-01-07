/** 
 @cond
 ############################################################################
 # LGPL License                                                             #
 #                                                                          #
 # This file is part of the Machine Learning Framework.                     #
 # Copyright (c) 2010-2012, Philipp Kraus, <philipp.kraus@flashpixx.de>     #
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

#ifndef __MACHINELEARNING_TOOLS_SOURCES_SOURCES_H
#define __MACHINELEARNING_TOOLS_SOURCES_SOURCES_H

namespace machinelearning { 
    namespace tools { 
        
        /** namespace with classes for generating example data
         * @todo adding support for OpenStreetMap http://wiki.openstreetmap.org/wiki/API / http://wiki.openstreetmap.org/wiki/Overpass_API
         * @todo adding support for Apache Cassandra http://en.wikipedia.org/wiki/Apache_Cassandra
         * @todo adding support for Hadoop http://hadoop.apache.org/
         * @todo adding support for MangoDB http://www.mongodb.org/
         * @todo adding support for MonetDB http://www.monetdb.org/
         * @todo adding support for graph databases http://en.wikipedia.org/wiki/Graph_database 
         * @todo adding support for mySQL / MS SQL / Postgres SQL / Oracle via eg http://www.sqlapi.com/
         * @todo adding twitter streaming support https://dev.twitter.com/docs/streaming-api/methods
         * @todo adding support for reading HTML data with WGet / Curl
         * @todo adding support for http://en.wikipedia.org/wiki/CouchDB
         * @todo adding support for web-sockets http://www.whatwg.org/specs/web-socket-protocol/
         * @todo adding support for differnt source of http://datacatalogs.org/ 
         * @todo adding support for http://www.data.gov/
         **/
        namespace sources {}
    
    }
}

#include "nntp.h"
#include "wikipedia.h"
#include "cloud.hpp"
#include "twitter.h"

#endif
#endif
