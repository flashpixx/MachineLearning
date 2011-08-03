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



/** read all input arguments 
 * @param argc number of arguments of "main"
 * @param argv arguments of "main"
 * @param p_args map with argument values (default values)
 * @return bool if all is correct
 **/
bool cliArguments( int argc, char* argv[], std::map<std::string, boost::any>& p_args ) {
    
    if (argc < 2) {
        std::cout << "--search \t search content" << std::endl;
        std::cout << "--lang \t\t language" << std::endl;
        std::cout << "--geo \t\t geographic position (latitude, longitude, radius, radiuslength [km = kilometer, mi = miles])" << std::endl;
        std::cout << "--max \t\t maximum number of tweets (0 = maximum)" << std::endl;
        std::cout << "--rpp \t\t number of tweets on each call" << std::endl;
        std::cout << "--page \t\t number of starting page" << std::endl;
        std::cout << "--until \t\t date value (format YYYY MM DD)" << std::endl;
        return false;
    }
     
    // set default arguments
    std::map<std::string, std::vector<std::string> > l_argmap;
    l_argmap["search"]     = std::vector<std::string>();
    l_argmap["lang"]       = std::vector<std::string>();
    l_argmap["geo"]        = std::vector<std::string>();
    l_argmap["max"]        = std::vector<std::string>();
    l_argmap["rpp"]        = std::vector<std::string>();
    l_argmap["page"]       = std::vector<std::string>();
    l_argmap["until"]      = std::vector<std::string>();
    
    
    // read all arguments
    std::size_t n=1;
    for(int i=1; i < argc; i+=n) {
        n = 1;
        std::string lc(argv[i]);
        if (lc.length() < 2)
            continue;
        
        lc = lc.substr(2);
        boost::to_lower( lc );
        
        if (l_argmap.find(lc) != l_argmap.end()) {
            std::vector<std::string> lv;
            
            for(int l=i+1; l < argc; ++l) {
                std::string lc2(argv[l]);
                if ( (lc2.length() >= 2) && (lc2.substr(0,2) == "--") )
                    break;
                
                lv.push_back(lc2);
                n++;
            }
            
            l_argmap[lc] = lv;
        }
        
    }
    
    if ( (l_argmap["search"].size() < 1) )
        throw std::runtime_error("number of arguments are incorrect");
    
    p_args["search"]    = l_argmap["search"];
    
    if (l_argmap["lang"].size() > 0)
        p_args["lang"] = tools::language::fromString(l_argmap["lang"][0]);
    
    if ( (l_argmap["geo"].size() == 4) ) {
        
        p_args["geo_latitude"]  = boost::lexical_cast<double>(l_argmap["geo"][0]);
        p_args["geo_longitude"] = boost::lexical_cast<double>(l_argmap["geo"][1]);
        p_args["geo_radius"]    = boost::lexical_cast<double>(l_argmap["geo"][2]);
        
        p_args["geo_length"] = tools::sources::twitter::searchparameter::kilometer;
        if (l_argmap["geo"][2] == "mi")
            p_args["geo_length"] = tools::sources::twitter::searchparameter::miles;
    }
    
    if (l_argmap["rpp"].size() > 0)
        p_args["rpp"] = boost::lexical_cast<std::size_t>(l_argmap["rpp"][0]);
    
    if (l_argmap["page"].size() > 0)
        p_args["page"] = boost::lexical_cast<std::size_t>(l_argmap["page"][0]);
    
    p_args["max"]       = std::size_t(0);
    if (l_argmap["max"].size() > 0)
        p_args["max"] = boost::lexical_cast<std::size_t>(l_argmap["max"][0]);
    
    if (l_argmap["until"].size() > 3) {
        boost::gregorian::date l_until(
                boost::lexical_cast<std::size_t>(l_argmap["until"][0]),
                boost::lexical_cast<std::size_t>(l_argmap["until"][1]),
                boost::lexical_cast<std::size_t>(l_argmap["until"][2])
        );
        p_args["until"] = l_until;
    }
    
    return true;
}



/** main program
 * @param argc number of arguments
 * @param argv arguments
 **/
int main(int argc, char* argv[]) {
    
    std::map<std::string, boost::any> l_args;
    if (!cliArguments(argc, argv, l_args))
        return EXIT_FAILURE;
    
    
    tools::sources::twitter::searchparameter l_params;
    
    try {
        l_params.setLanguage( boost::any_cast<tools::language::code>(l_args["lang"]) ); 
    } catch (...) {}

    
    try {
        tools::sources::twitter::searchparameter::geoposition l_geo;
        
        l_geo.longitude = boost::any_cast<double>(l_args["geo_longitude"]);
        l_geo.latitude  = boost::any_cast<double>(l_args["geo_latitude"]);
        l_geo.radius    = boost::any_cast<double>(l_args["geo_radius"]);
        l_geo.length    = boost::any_cast<tools::sources::twitter::searchparameter::radiuslength>(l_args["geo_length"]);
    } catch (...) {}
    
    try {
        l_params.setUntilDate( boost::any_cast<boost::gregorian::date>(l_args["until"]) );
    } catch (...) {}


    tools::sources::twitter l_twitter;
    const std::vector<std::string> l_search = boost::any_cast< std::vector<std::string> >(l_args["search"]);
    
    for(std::size_t i=0; i < l_search.size(); ++i) {
        std::vector<tools::sources::twitter::tweet> l_data = l_twitter.search( l_search[i], l_params, boost::any_cast<std::size_t>(l_args["max"]) );
        
        for(std::size_t j=0; j < l_data.size(); ++j)
            std::cout << l_data[j] << std::endl;
        
        std::cout << "===================================================================================" << std::endl;
    }
    
    return EXIT_SUCCESS;
}