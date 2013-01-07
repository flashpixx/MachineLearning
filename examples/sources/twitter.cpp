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

#include <cstdlib>
#include <machinelearning.h>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>


using namespace machinelearning;
namespace po = boost::program_options;

/** main program
 * @param p_argc number of arguments
 * @param p_argv arguments
 **/
int main(int p_argc, char* p_argv[])
{
    #ifdef MACHINELEARNING_MULTILANGUAGE
    tools::language::bindings::bind();
    #endif
    
    // default values
    std::size_t l_rpp  = 10;
    std::size_t l_max  = 0;
    std::size_t l_page = 1;

    // create CML options with description
    po::options_description l_description("allowed options");
    l_description.add_options()
        ("help", "produce help message")
        ("search", po::value< std::vector<std::string> >()->multitoken(), "search keyword / keyword list ['tm' returns the actuall timeline tweets, 'trend' returns the daily trends and other options are ignored]")
        ("lang", po::value<std::string>(), "language code (iso 639-1 or -3)")
        ("geo", po::value< std::vector<std::string> >()->multitoken(), "geographic position (format: latitude longitude radius radiuslength [km = kilometer, mi = miles])")
        ("max", po::value<std::size_t>(&l_max)->default_value(0), "maximum number of tweets [default: 0 = maximum]")
        ("rpp", po::value<std::size_t>(&l_rpp)->default_value(15), "number of tweets on each call [default: 15]")
        ("page", po::value<std::size_t>(&l_page)->default_value(1), "number of starting page [default: 1]")
        ("until", po::value< std::vector<std::string> >()->multitoken(), "date value (format: YYYY MM DD)")
    ;

    po::variables_map l_map;
    po::positional_options_description l_input;
    po::store(po::command_line_parser(p_argc, p_argv).options(l_description).positional(l_input).run(), l_map);
    po::notify(l_map);

    if (l_map.count("help")) {
        std::cout << l_description << std::endl;
        return EXIT_SUCCESS;
    }

    if (!l_map.count("search")) {
        std::cerr << "[--search] option must be set" << std::endl;
        return EXIT_FAILURE;
    }




    // create search parameters
    tools::sources::twitter::searchparameter l_params;
    l_params.setNumberResults(l_rpp, l_page);


    if (l_map.count("lang"))
        try {
            l_params.setLanguage( tools::language::fromString(l_map["lang"].as<std::string>()) );
        } catch (...) {}

    if (l_map.count("geo")) {
        const std::vector<std::string> l_geoparam = l_map["geo"].as< std::vector<std::string> >();

        if (l_geoparam.size() >= 4)
            try {
                tools::sources::twitter::searchparameter::geoposition l_geo;

                l_geo.latitude      = boost::lexical_cast<double>(l_geoparam[0]);
                l_geo.longitude     = boost::lexical_cast<double>(l_geoparam[1]);
                l_geo.radius        = boost::lexical_cast<double>(l_geoparam[2]);
                if (l_geoparam[3] == "mi")
                    l_geo.length  = tools::sources::twitter::searchparameter::miles;

                l_params.setGeoPosition( l_geo );
            } catch (...) {}
    }

    if (l_map.count("until")) {
        const std::vector<std::string> l_dateparam = l_map["until"].as< std::vector<std::string> >();

        if (l_dateparam.size() >= 3)
            try {
                boost::gregorian::date l_until(
                                               boost::lexical_cast<std::size_t>(l_dateparam[0]),
                                               boost::lexical_cast<std::size_t>(l_dateparam[1]),
                                               boost::lexical_cast<std::size_t>(l_dateparam[2])
                                               );
                l_params.setUntilDate( l_until );
            } catch (...) {}
    }



    // do twitter run
    tools::sources::twitter l_twitter;

    const std::vector<std::string> l_search = l_map["search"].as< std::vector<std::string> >();

    std::string l_searchfirst = l_search[0];
    boost::to_lower(l_searchfirst);
    if (l_searchfirst == "tm") {
        std::vector<tools::sources::twitter::timelinetweet> l_data = l_twitter.getPublicTimeline();
        for(std::size_t j=0; j < l_data.size(); ++j)
            std::cout << l_data[j] << std::endl;
    } else
        if (l_searchfirst == "trend") {
            std::vector<std::string> l_data = l_twitter.getDailyTrends();
            for(std::size_t j=0; j < l_data.size(); ++j)
                std::cout << l_data[j] << std::endl;
        } else
            for(std::size_t i=0; i < l_search.size(); ++i) {
                std::vector<tools::sources::twitter::searchtweet> l_data = l_twitter.search( l_search[i], l_params, l_max );

                for(std::size_t j=0; j < l_data.size(); ++j)
                    std::cout << l_data[j] << std::endl;

                std::cout << "===================================================================================" << std::endl;
            }

    return EXIT_SUCCESS;
}
