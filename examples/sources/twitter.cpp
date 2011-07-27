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

#include <iostream>
#include <machinelearning.h>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>

using namespace machinelearning;


#include <sstream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filtering_stream.hpp>
namespace io = boost::iostreams;


/** main program
 * @param argc number of arguments
 * @param argv arguments
 **/
int main(int argc, char* argv[]) {
  
    io::filtering_streambuf< io::output > out;

    out.push(tools::iostreams::urlencoder());
    out.push( std::cout );
    
    std::istringstream l_sstream( "abcd_ efgh tjg     abc* xxxx\ndhasidhasdbaldl", std::stringstream::binary);
    l_sstream >> std::noskipws;
    
    std::copy( std::istream_iterator<char>(l_sstream), std::istream_iterator<char>(), std::ostreambuf_iterator<char>(&out) );
    std::cout << "\n\n\n" << l_sstream.str() << std::endl;
    
    /*
    tools::sources::twitter tw;
    tools::sources::twitter::searchparameter s;
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
    
    tw.search("xxx", s);
    */
    //std::cout << tools::function::urlencode("http://www.example.com/~user/?test=1&test1=2") << std::endl;

    return EXIT_SUCCESS;
}