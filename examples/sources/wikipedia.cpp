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
        std::cout << "--lang \t\t language (values: en, de [default])" << std::endl;
        std::cout << "--search \t returns the artice of the keyword / returns list of articles / if empty, you will get a random article" << std::endl;
        return false;
    }
    
    // set default arguments
    std::map<std::string, std::vector<std::string> > l_argmap;
    l_argmap["lang"]       = std::vector<std::string>();
    l_argmap["search"]     = std::vector<std::string>();
    
    
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
    
    
    p_args["search"]    = l_argmap["search"];
    
    p_args["lang"] = tools::language::de;
    if (l_argmap["lang"].size() > 0) {
        std::string lc = l_argmap["lang"][0];
        boost::to_lower(lc);
        
        if (lc == "en")
            p_args["lang"] = tools::language::en;
    }
    
    
    return true;
}


/** output for one article
 * @param p_wiki wikipedia object
 **/
void output( tools::sources::wikipedia& p_wiki ) {
    // extract article data (if article exists read data else get acronyms)
    std::vector<std::string> label;
    if (p_wiki.isArticle()) {
        std::cout << p_wiki.getArticleContent() << "\n-----------------------------------------------------------------------------------" << std::endl;
        label = p_wiki.getArticleLabel();
    } else
        label = p_wiki.getArticleAcronym();
    
    
    // show label data
    for(std::size_t i=0; i < label.size(); ++i)
        std::cout << label[i] << std::endl;
}



/** main program
 * @param argc number of arguments
 * @param argv arguments
 **/
int main(int argc, char* argv[]) {
    
    std::map<std::string, boost::any> l_args;
    if (!cliArguments(argc, argv, l_args))
        return EXIT_FAILURE;
    

    // create wikipedia object
    tools::sources::wikipedia wiki( boost::any_cast<tools::language::code>(l_args["lang"]) );
    
    // get article data
    std::vector<std::string> lsearch = boost::any_cast< std::vector<std::string> >(l_args["search"]);
    
    if (lsearch.size() == 0) {
        wiki.getRandomArticle();
        output(wiki);
    } else
        for(std::size_t i=0; i < lsearch.size(); ++i) {
            wiki.getArticle( lsearch[i] );
            output(wiki);
            std::cout << "\n===================================================================================" << std::endl;
        }
    

    
    return EXIT_SUCCESS;
}