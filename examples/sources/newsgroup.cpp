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

    std::string l_content;
    bool l_cancel;

    // create CML options with description
    po::options_description l_description("allowed options");
    l_description.add_options()
        ("help", "produce help message")
        ("server", po::value<std::string>(), "IP / address of the NNTP server")
        ("groups", po::value< std::vector<std::string> >()->multitoken(), "list of groups / not set = show group list")
        ("content", po::value<std::string>(&l_content)->default_value("body"), "content of articles (values: full, body [default], header)")
        ("canceled", po::value<bool>(&l_cancel)->default_value(false), "show canceled articles (values: false / 0 [default], true)")
    ;

    po::variables_map l_map;
    po::positional_options_description l_input;
    po::store(po::command_line_parser(p_argc, p_argv).options(l_description).positional(l_input).run(), l_map);
    po::notify(l_map);

    if (l_map.count("help")) {
        std::cout << l_description << std::endl;
        return EXIT_SUCCESS;
    }

    if (!l_map.count("server")) {
        std::cerr << "[--server] option must be set" << std::endl;
        return EXIT_FAILURE;
    }




    // connect to server
    tools::sources::nntp news( l_map["server"].as<std::string>() );

    if (!l_map.count("groups")) {

        // read group list
        std::map<std::string, std::size_t> l_groups = news.getGroupList();
        for (std::map<std::string, std::size_t>::iterator it = l_groups.begin(); it != l_groups.end(); ++it)
            std::cout << it->first << "     (" << it->second << ")" << std::endl;

    } else {

        news.setContent( tools::sources::nntp::body );
        if (l_content == "full")
            news.setContent( tools::sources::nntp::full );
        if (l_content == "header")
            news.setContent( tools::sources::nntp::header );


        const std::vector<std::string> l_groups = l_map["groups"].as< std::vector<std::string> >();

        for(std::size_t i=0; i < l_groups.size(); ++i) {
            // sets the newsgroup for browsing
            news.setGroup( l_groups[i] );

            // browse each article in the group
            for(tools::sources::nntp::iterator it=news.begin(); it != news.end(); ++it) {
                if ( (it->isArticleCanceled()) && l_cancel )
                    continue;

                std::cout << it->getArticle()  << "\n===================================================================================" << std::endl;
            }
        }
    }

    return EXIT_SUCCESS;
}
