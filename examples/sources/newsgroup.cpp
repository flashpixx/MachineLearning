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
        std::cout << "--server \t\t IP / address of the NNTP server" << std::endl;
        std::cout << "--groups \t\t list of groups or empty for getting group list" << std::endl;
        std::cout << "--content \t\t content of articles (values: full, body [default], header)" << std::endl;
        std::cout << "--canceled \t\t show canceled articles (vales: off [default], on)" << std::endl;
        return false;
    }
    
    // set default arguments
    std::map<std::string, std::vector<std::string> > l_argmap;
    l_argmap["server"]     = std::vector<std::string>();
    l_argmap["groups"]     = std::vector<std::string>();
    l_argmap["content"]    = std::vector<std::string>();
    l_argmap["canceled"]   = std::vector<std::string>();
    
    
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
    
    
    //check map values and convert them
    if ( (l_argmap["server"].size() != 1) )
        throw std::runtime_error("number of arguments are incorrect");
    
    p_args["server"]      = l_argmap["server"][0];
    p_args["groups"]      = l_argmap["groups"];
    p_args["canceled"]    = false;
    
    p_args["content"]     = tools::sources::nntp::body;
    if (l_argmap["content"].size() > 0) {
        std::string lc = l_argmap["content"][0];
        boost::to_lower(lc);
    
        if (lc == "full")
            p_args["content"]     = tools::sources::nntp::full;
        if (lc == "header")
            p_args["content"]     = tools::sources::nntp::header;
    }
    
    if (l_argmap["canceled"].size() > 0) {
        std::string lc = l_argmap["canceled"][0];
        boost::to_lower(lc);
        p_args["canceled"] = lc == "on";
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
            
            
    // connect to server
    tools::sources::nntp news( boost::any_cast<std::string>(l_args["server"]) );

    // on one parameter we return the group list
            if ( (boost::any_cast< std::vector<std::string> >(l_args["groups"])).size() == 0) {
        // read group list
        std::map<std::string, std::size_t> groups = news.getGroupList();
        for (std::map<std::string, std::size_t>::iterator it = groups.begin(); it != groups.end(); ++it)
            std::cout << it->first << "     (" << it->second << ")" << std::endl;
    
    } else {
    
        // sets which part of a messages is read
        news.setContent( boost::any_cast<tools::sources::nntp::content>(l_args["content"]) );
    
        std::vector<std::string> lgroups = boost::any_cast< std::vector<std::string> >(l_args["groups"]);
        for(std::size_t i=0; i < lgroups.size(); ++i) {
            // sets the newsgroup for browsing 
            news.setGroup( lgroups[i] );
    
            // browse each article in the group
            for(tools::sources::nntp::iterator it=news.begin(); it != news.end(); ++it) {
                if ( (it->isArticleCanceled()) && (!boost::any_cast<bool>(l_args["canceled"])) )
                    continue;
                
                std::cout << it->getArticle()  << "\n===================================================================================" << std::endl;
            }
        }
    }
    
    return EXIT_SUCCESS;
}