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

#include <machinelearning.h>

using namespace machinelearning;


int main(int argc, char* argv[]) {
    
    if (argc < 2)
        throw std::runtime_error("you need at least one parameter as input. first newsgroup server and optional second parameter for the group name");

    // connect to server
    tools::sources::nntp news( argv[1] );

    // on one parameter we return the group list
    if (argc == 2) {
        // read group list
        std::map<std::string, std::size_t> groups = news.getGroupList();
        for (std::map<std::string, std::size_t>::iterator it = groups.begin(); it != groups.end(); ++it)
            std::cout << it->first << "     (" << it->second << ")" << std::endl;
        std::cout << "\n===================================================================================" << std::endl;
    
    } else {
    
        // sets which part of a messages is read
        news.setContent( tools::sources::nntp::full );
    
        // sets the newsgroup for browsing 
        news.setGroup( argv[2] );
    
        // browse each article in the group
        for(tools::sources::nntp::iterator it=news.begin(); it != news.end(); ++it) {
        
            // splits header and body of the message (should be only used with full articles)
            std::string header, body;
            tools::sources::nntp::separateHeaderBody( *it, header, body );
        
            std::cout << header << "\n-----------------------------------------------------------------------------------\n";
            std::cout << body   << "\n===================================================================================" << std::endl;
        }
    }
    
    return EXIT_SUCCESS;
}