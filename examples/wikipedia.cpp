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

#include "../machinelearning.h"

using namespace machinelearning;


int main(std::size_t argc, char* argv[]) {

    // create wikipedia object
    tools::sources::wikipedia wiki;
    
    // check if arguments exists, argument is set => try to get this article else get random article
    if (argc < 2)
        wiki.getRandomArticle();
    else
        wiki.getArticle( argv[1] );
    
    
    // extract article data (if article exists read data else get acronyms)
    std::vector<std::string> label;
    if (wiki.isArticle()) {
        std::cout << wiki.getArticleContent() << "\n==========================================================" << std::endl;
        label = wiki.getArticleLabel();
    } else
        label = wiki.getArticleAcronym();
    
    
    // show label data
    for(std::size_t i=0; i < label.size(); ++i)
        std::cout << label[i] << std::endl;
    
    return EXIT_SUCCESS;
};