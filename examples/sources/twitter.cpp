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

#include <map>
#include <ctime>
#include <cstdlib>

#include <machinelearning.h>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>

using namespace machinelearning;



/** main program
 * @param argc number of arguments
 * @param argv arguments
 **/
int main(int argc, char* argv[]) {
    
    tools::sources::twitter tw;
    
    
    tools::sources::twitter::searchparameter s;
    /*
    tools::sources::twitter::searchparameter::geoposition g;
    
    s.setLanguage( tools::language::BR );
    s.setResultType( tools::sources::twitter::searchparameter::mixed );
    
    g.longitude = 20;
    g.latitude = 40;
    g.radius = 1.3;
    s.setGeoPosition(g);
    
    std::time_t rawtime;
    time ( &rawtime );
    struct tm* timeinfo = localtime ( &rawtime );
    s.setUntilDate(*timeinfo);
    */
     
    s.setResultCount(115);
    
    tw.search("netzpolitik", s);
    
    return EXIT_SUCCESS;
}