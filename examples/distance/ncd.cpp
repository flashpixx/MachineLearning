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
#include <string>
#include <map>
#include <boost/any.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/symmetric.hpp>

using namespace boost::numeric;
using namespace machinelearning;


bool cliArguments( int argc, char* argv[], std::map<std::string, boost::any>& p_args ) {
    
    if (argc < 2) {
        std::cout << "--inputfile" << "\t" << "input HDF5 file" << std::endl;
        std::cout << "--outfile" << "\t" << "optional output HDF5 file" << std::endl;
        std::cout << "--compress" << "\t" << "compression level (allowed values are: default, bestspeed or bestcompression)" << std::endl;
        std::cout << "--algorithm" << "\t" << "compression algorithm (allowed values are: gzip, bzip)" << std::endl;
        std::cout << "--matrix" << "\t" << "structure of the matrix (allowed values are: symmetric or unsymmetric" << std::endl;
        return false;
    }
    
    
    // set default arguments
    std::map<std::string, std::vector<std::string> > l_argmap;
    l_argmap["inputfile"] = std::vector<std::string>();
    l_argmap["outfile"]   = std::vector<std::string>();
    l_argmap["compress"] = std::vector<std::string>();
    l_argmap["algorithm"] = std::vector<std::string>();
    l_argmap["matrix"] = std::vector<std::string>();
    
    
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
    if (l_argmap["inputfile"].size() < 2)
        throw std::runtime_error("number of input files must be equal or greater than two");
    
    //set default arguments
    p_args["inputfile"]   = l_argmap["inputfile"];
    p_args["outfile"]     = std::string();

    if (l_argmap["outfile"].size() > 0)
        p_args["outfile"]     = l_argmap["outfile"][0];

    // check input arguments and convert them
    p_args["algorithm"] = distances::ncd<double>::bzip2;
    if (l_argmap["algorithm"].size() > 0) {
        boost::to_lower(l_argmap["algorithm"][0]);
        if (l_argmap["algorithm"][0] == "gzip")
            p_args["algorithm"]   = distances::ncd<double>::gzip;
        if ((l_argmap["algorithm"].size() > 0) && (l_argmap["algorithm"][0] == "bzip"))
            p_args["algorithm"]   = distances::ncd<double>::bzip2;
    }

    p_args["compress"] = distances::ncd<double>::defaultcompression;
    if (l_argmap["compress"].size() > 0) {
        boost::to_lower(l_argmap["compress"][0]);
        if (l_argmap["compress"][0] == "default")
            p_args["compress"]    = distances::ncd<double>::defaultcompression;
        if (l_argmap["compress"][0] == "bestspeed")
            p_args["compress"]    = distances::ncd<double>::bestspeed;
        if (l_argmap["compress"][0] == "bestcompression")
            p_args["compress"]    = distances::ncd<double>::bestcompression;
    }

    p_args["matrix"] = std::string("unsymmetric");
    if (l_argmap["matrix"].size() > 0) {
        boost::to_lower(l_argmap["matrix"][0]);
        if ( (l_argmap["matrix"][0] == "symmetric") || (l_argmap["matrix"][0] == "unsymmetric"))
            p_args["matrix"] = l_argmap["matrix"][0];
    }
    
    return true;
}


int main(int argc, char* argv[]) {

    std::map<std::string, boost::any> l_args;
    if (!cliArguments(argc, argv, l_args))
        return EXIT_FAILURE;
    
    // create ncd object
    distances::ncd<double> ncd( boost::any_cast< distances::ncd<double>::compresstype >(l_args["algorithm"]) );
    ncd.setCompressionLevel( boost::any_cast< distances::ncd<double>::compresslevel >(l_args["compress"]) );
    
    // create the distance matrix and use the each element of the vector as a filename
    ublas::matrix<double> distancematrix;
    
    
    if (boost::any_cast<std::string>(l_args["matrix"]) == "unsymmetric")
        distancematrix = ncd.unsymmetric( boost::any_cast< std::vector<std::string> >(l_args["inputfile"]), true);
    else
        distancematrix = ncd.symmetric( boost::any_cast< std::vector<std::string> >(l_args["inputfile"]), true);

    
    if (boost::any_cast<std::string>(l_args["outfile"]).empty())
        std::cout << distancematrix << std::endl;
    else {
        // create hdf file and write data
        tools::files::hdf file(boost::any_cast<std::string>(l_args["outfile"]), true);
        file.writeBlasMatrix<double>( "/ncd",  distancematrix, H5::PredType::NATIVE_DOUBLE );
        std::cout << "structure of the output file" << std::endl;
        std::cout << "/ncd" << "\t\t" << "distance matrix" << std::endl;
    }
    
    return EXIT_SUCCESS;
}