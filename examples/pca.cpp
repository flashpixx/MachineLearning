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
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/matrix.hpp>

namespace ublas = boost::numeric::ublas;
namespace dim   = machinelearning::dimensionreduce::nonsupervised;
namespace tl   = machinelearning::tools;


int main(int argc, char* argv[]) {
    if (argc < 4)
        throw std::runtime_error("you need at least three parameter as input. first HDF file, second path to dataset, third number of projected dimensions");
    
    tl::files::hdf source( argv[1] );
    tl::files::hdf target("pca.hdf5", true);

    dim::pca<double> pca( boost::lexical_cast<std::size_t>(argv[4]) );
    target.write<double>( "/data",  pca.map( source.readMatrix<double>(argv[1], H5::PredType::NATIVE_DOUBLE) ), H5::PredType::NATIVE_DOUBLE );
    
    std::cout << "create HDF file \"pca.hdf5\" with dataset \"/data\"" << std::endl;
    
    
    return EXIT_SUCCESS;
}