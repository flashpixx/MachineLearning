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
namespace ublas = boost::numeric::ublas;


/** read all input arguments 
 * @param argc number of arguments of "main"
 * @param argv arguments of "main"
 * @param p_args map with argument values (default values)
 * @return bool if all is correct
 **/
bool cliArguments( int argc, char* argv[], std::map<std::string, boost::any>& p_args ) {
    
    if (argc < 2) {
        std::cout << "--outfile \t\t output HDF5 file" << std::endl;
        std::cout << "--outpath \t\t output path within the HDF5 file" << std::endl;
        std::cout << "--dimension \t\t number of dimension (default: 2)" << std::endl;
        std::cout << "--sampling \t\t sampling of the intervall (default: 10)" << std::endl;
        std::cout << "--pointsmin \t\t minimal number of points (default: 50)" << std::endl;
        std::cout << "--pointsmax \t\t maximal number of points (default: 500)" << std::endl;
        std::cout << "--rangemin \t\t minimal range value (default: 0)" << std::endl;
        std::cout << "--rangemax \t\t maximal range value (default: 100)" << std::endl;
        std::cout << "--variancemin \t\t minimal variance value (default: 0.1)" << std::endl;
        std::cout << "--variancemax \t\t maximal variance value (default: 0.8)" << std::endl;
        std::cout << "--create \t\t type for creating clouds (values are: all [default], alternate, random)" << std::endl;
        std::cout << "--probability \t\t probability for the random create option (default: 0.5)" << std::endl;
        std::cout << "--shuffle \t\t shuffle the vectors of the clouds (default: off)" << std::endl;
        
        return false;
    }
    
    // set default arguments
    std::map<std::string, std::vector<std::string> > l_argmap;
    l_argmap["outfile"]     = std::vector<std::string>();
    l_argmap["outpath"]     = std::vector<std::string>();
    l_argmap["dimension"]   = std::vector<std::string>();
    l_argmap["sampling"]    = std::vector<std::string>();
    l_argmap["pointsmin"]   = std::vector<std::string>();
    l_argmap["pointsmax"]   = std::vector<std::string>();
    l_argmap["rangemin"]    = std::vector<std::string>();
    l_argmap["rangemax"]    = std::vector<std::string>();
    l_argmap["variancemin"] = std::vector<std::string>();
    l_argmap["variancemax"] = std::vector<std::string>();
    l_argmap["create"]      = std::vector<std::string>();
    l_argmap["probability"] = std::vector<std::string>();
    l_argmap["shuffle"]     = std::vector<std::string>();
    
    
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
    if ( (l_argmap["outfile"].size() != 1) ||
        (l_argmap["outpath"].size() != 1)
        )
        throw std::runtime_error("number of arguments are incorrect");
    
    p_args["outfile"]       = l_argmap["outfile"][0];
    p_args["outpath"]       = l_argmap["outpath"][0];
    
    // default values
    p_args["dimension"]     = std::size_t(2);
    p_args["sampling"]      = std::size_t(10);
    p_args["pointsmin"]     = std::size_t(50);
    p_args["pointsmax"]     = std::size_t(500);
    
    p_args["rangemin"]      = double(0);
    p_args["rangemax"]      = double(100.0);
    p_args["variancemin"]   = double(0.1);
    p_args["variancemax"]   = double(0.8);
    
    p_args["probability"]   = double(0.5);
    
    p_args["create"]        = tools::sources::cloud<double>::all;
    if (l_argmap["create"].size() == 1) {
        boost::to_lower(l_argmap["create"][0]);
        if (l_argmap["create"][0] == "random")
            p_args["create"]        = tools::sources::cloud<double>::random;
        if (l_argmap["create"][0] == "alternate")
            p_args["create"]        = tools::sources::cloud<double>::alternate;
    }
    
    p_args["shuffle"] = false;
    if (l_argmap["shuffle"].size() == 1) {
        boost::to_lower(l_argmap["shuffle"][0]);
        p_args["shuffle"] = l_argmap["shuffle"][0] == "on";
    }
    
    try {
        
        if (l_argmap["dimension"].size() == 1)
            p_args["dimension"] = boost::lexical_cast<std::size_t>(l_argmap["dimension"][0]);
        
        if (l_argmap["sampling"].size() == 1)
            p_args["sampling"] = boost::lexical_cast<std::size_t>(l_argmap["sampling"][0]);
        
        if (l_argmap["pointsmin"].size() == 1)
            p_args["pointsmin"] = boost::lexical_cast<std::size_t>(l_argmap["pointsmin"][0]);
        if (l_argmap["pointsmax"].size() == 1)
            p_args["pointsmax"] = boost::lexical_cast<std::size_t>(l_argmap["pointsmax"][0]);
        
        if (l_argmap["rangemin"].size() == 1)
            p_args["rangemin"] = boost::lexical_cast<double>(l_argmap["rangemin"][0]);
        if (l_argmap["rangemax"].size() == 1)
            p_args["rangemax"] = boost::lexical_cast<double>(l_argmap["rangemax"][0]);
        
        if (l_argmap["variancemin"].size() == 1)
            p_args["variancemin"] = boost::lexical_cast<double>(l_argmap["variancemin"][0]);
        if (l_argmap["variancemax"].size() == 1)
            p_args["variancemax"] = boost::lexical_cast<double>(l_argmap["variancemax"][0]);
        
        if (l_argmap["probability"].size() == 1)
            p_args["probability"] = boost::lexical_cast<double>(l_argmap["probability"][0]);

        
    } catch (...) {
        throw std::runtime_error("numerical data can not extracted");
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
    
    
    // create cloud object and set the data
    tools::sources::cloud<double> cloud( boost::any_cast<std::size_t>(l_args["dimension"]) );
    
    // set range of the points
    cloud.setPoints( boost::any_cast<std::size_t>(l_args["pointsmin"]), boost::any_cast<std::size_t>(l_args["pointsmax"]) );
    
    // set variance, range and samples
    cloud.setVariance( boost::any_cast<double>(l_args["variancemin"]), boost::any_cast<double>(l_args["variancemax"]) );
    for(std::size_t i=0; i < boost::any_cast<std::size_t>(l_args["dimension"]); ++i)
        cloud.setRange(i, boost::any_cast<double>(l_args["rangemin"]), boost::any_cast<double>(l_args["rangemax"]), boost::any_cast<std::size_t>(l_args["sampling"]));   
    
    // create file and write data to hdf
    tools::files::hdf target(boost::any_cast<std::string>(l_args["outfile"]), true);
    target.writeBlasMatrix<double>( boost::any_cast<std::string>(l_args["outpath"]),  cloud.generate( boost::any_cast< tools::sources::cloud<double>::cloudcreate >(l_args["create"]), boost::any_cast<double>(l_args["probability"]), boost::any_cast<bool>(l_args["shuffle"]) ), H5::PredType::NATIVE_DOUBLE );
    
    return EXIT_SUCCESS;
}
