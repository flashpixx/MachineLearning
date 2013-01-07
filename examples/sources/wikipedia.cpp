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


/** output for one article
 * @param p_wiki wikipedia object
 **/
void output( tools::sources::wikipedia& p_wiki ) {
    // extract article data (if article exists read data else get acronyms)
    std::vector<std::string> l_label;
    if (p_wiki.isArticle()) {
        std::cout << p_wiki.getArticleContent() << "\n-----------------------------------------------------------------------------------" << std::endl;
        l_label = p_wiki.getArticleLabel();
    } else
        l_label = p_wiki.getArticleAcronym();


    // show label data
    for(std::size_t i=0; i < l_label.size(); ++i)
        std::cout << l_label[i] << std::endl;
}



/** main program
 * @param p_argc number of arguments
 * @param p_argv arguments
 **/
int main(int p_argc, char* p_argv[])
{
    #ifdef MACHINELEARNING_MULTILANGUAGE
    tools::language::bindings::bind();
    #endif
    
    std::string l_lang;

    // create CML options with description
    po::options_description l_description("allowed options");
    l_description.add_options()
        ("help", "produce help message")
        ("search", po::value< std::vector<std::string> >()->multitoken(), "returns the artice of the keyword / returns list of articles / if empty, you will get a random article")
        ("lang", po::value<std::string>(&l_lang)->default_value("en"), "language code (iso 639-1 or -3) [default: en]")
    ;

    po::variables_map l_map;
    po::positional_options_description l_input;
    po::store(po::command_line_parser(p_argc, p_argv).options(l_description).positional(l_input).run(), l_map);
    po::notify(l_map);

    if (l_map.count("help")) {
        std::cout << l_description << std::endl;
        return EXIT_SUCCESS;
    }


    // create wikipedia object
    tools::sources::wikipedia wiki( tools::language::fromString(l_lang)  );

    if (!l_map.count("search")) {
        wiki.getRandomArticle();
        output(wiki);
    } else {
        const std::vector<std::string> l_search = l_map["search"].as< std::vector<std::string> >();

        for(std::size_t i=0; i < l_search.size(); ++i) {
            wiki.getArticle( l_search[i] );
            output(wiki);
            std::cout << "\n===================================================================================" << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
