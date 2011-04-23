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
#include <boost/numeric/ublas/matrix.hpp>

namespace ublas = boost::numeric::ublas;
namespace dim   = machinelearning::dimensionreduce::nonsupervised;
namespace tools  = machinelearning::tools;



bool cliArguments( int argc, char* argv[], std::map<std::string, boost::any>& p_args ) {
    
    if (argc < 2) {
        std::cout << "--inputfile" << "\t" << "input HDF5 file" << std::endl;
        std::cout << "--inputpath" << "\t" << "path to dataset" << std::endl;
        std::cout << "--outfile" << "\t" << "output HDF5 file" << std::endl;
        std::cout << "--outpath" << "\t" << "output path within the HDF5 file" << std::endl;
        std::cout << "--dimension" << "\t" << "number of project dimensions" << std::endl;
        
        return false;
    }
    
    
    // set default arguments
    std::map<std::string, std::vector<std::string> > l_argmap;
    l_argmap["inputfile"] = std::vector<std::string>();
    l_argmap["inputpath"] = std::vector<std::string>();
    l_argmap["outfile"]   = std::vector<std::string>();
    l_argmap["outpath"]   = std::vector<std::string>();
    l_argmap["dimension"] = std::vector<std::string>();
    
    
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
    if ((l_argmap["dimension"].size() != 1) || 
        (l_argmap["outfile"].size() != 1) || 
        (l_argmap["outpath"].size() != 1) || 
        (l_argmap["inputfile"].size() != 1) || 
        (l_argmap["inputpath"].size() != 1)
        )
        throw std::runtime_error("number of arguments are incorrect");
    
    p_args["inputfile"]     = l_argmap["inputfile"][0];
    p_args["inputpath"]     = l_argmap["inputpath"][0];
    p_args["outfile"]       = l_argmap["outfile"][0];
    p_args["outpath"]       = l_argmap["outpath"][0];
    
    try {
        p_args["dimension"]  = boost::lexical_cast<std::size_t>( l_argmap["dimension"][0] );
    } catch (...) {
        throw std::runtime_error("numerical data can not extracted");
    }  
    
    return true;
}



int main(int argc, char* argv[]) {

    std::map<std::string, boost::any> l_args;
    if (!cliArguments(argc, argv, l_args))
        return EXIT_FAILURE;
    
    
    // read source hdf file
    tools::files::hdf source( boost::any_cast<std::string>(l_args["inputfile"]) );
    
    // create pca object and map the data
    dim::pca<double> pca( boost::any_cast<std::size_t>(l_args["dimension"]) );
    ublas::matrix<double> project = pca.map( source.readBlasMatrix<double>(boost::any_cast<std::string>(l_args["inputpath"]), H5::PredType::NATIVE_DOUBLE) );

    
    // create file and write data to hdf
    tools::files::hdf target(boost::any_cast<std::string>(l_args["outfile"]), true);
    target.writeBlasMatrix<double>( boost::any_cast<std::string>(l_args["outpath"]),  project, H5::PredType::NATIVE_DOUBLE );
    
    return EXIT_SUCCESS;
}