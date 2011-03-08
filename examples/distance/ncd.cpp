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
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/symmetric.hpp>

using namespace boost::numeric;
using namespace machinelearning;


int main(int argc, char* argv[]) {

    if (argc < 3)
        throw std::runtime_error("you need at least two files as input");
    
    // create ncd object (set optional compression level)
    distances::ncd<double> ncd( distances::ncd<double>::bzip2 );
    //ncd.setCompressionLevel( dist::ncd::bestspeed );
    
    // copy parameters to a std::vector
    std::vector<std::string> val;
    for(std::size_t i=1; i < argc; i++)
        val.push_back( argv[i] );
    
    // create the distance matrix and use the each element of the vector as a filename
    // optional can be created a symmetric distance matrix (it's a little bit faster) 
    ublas::matrix<double> distancematrix = ncd.unsymmetric(val, true);
    //ublas::matrix<double> distancematrix = ncd.symmetric(val, true);
    
    // create hdf file and write data
    tools::files::hdf file("ncd.hdf5", true);
    file.write<double>( "/data",  distancematrix, H5::PredType::NATIVE_DOUBLE );
        
    std::cout << "distance matrix (create HDF file \"ncd.hdf5\" with dataset \"/data\"): \n" << std::endl;
    return EXIT_SUCCESS;
}