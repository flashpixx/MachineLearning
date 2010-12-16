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
#include <string>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/io.hpp>


namespace tools     = machinelearning::tools;
namespace distance  = machinelearning::distances;
namespace ublas     = boost::numeric::ublas;


int main(std::size_t argc, char* argv[]) {

    if (argc < 3)
        throw std::runtime_error("you need at least two files as input");
    
    
    distance::ncd<double> ncd( distance::ncd<double>::bzip2 );
    //ncd.setCompressionLevel( dist::ncd::bestspeed );
    
    std::vector<std::string> val;
    for(std::size_t i=1; i < argc; i++)
        val.push_back( argv[i] );
    
    ublas::matrix<double> distances = ncd.unsymmetric(val, true);
    //ublas::matrix<double> distances = ncd.symmetric(val, true);
    
    
    tools::files::hdf f("ncd.hdf5", true);
    f.write<double>( "/data",  distances, H5::PredType::NATIVE_DOUBLE );
        
    std::cout << distances << std::endl;

        
    return EXIT_SUCCESS;
}