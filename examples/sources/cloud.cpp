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

#include "../../machinelearning.h"
#include <boost/lexical_cast.hpp>

using namespace machinelearning;


int main(std::size_t argc, char* argv[]) {
    
    // set default values
    std::size_t dim                 = 0;
    std::size_t sampling            = 10;
    std::size_t pointsmin           = 50;
    std::size_t pointsmax           = 500;
    double rangemin                 = 0;
    double rangemax                 = 1;
    double variancemin              = 0.1;
    double variancemax              = 0.8;
    
    // check min arguments
    if (argc < 2) 
        throw std::runtime_error("number of parameters not correct");

    // try to convert string arguments to numeric values
    try {
        dim             = boost::lexical_cast<std::size_t>(argv[1]);
        
        if (argc > 2) 
            sampling    = boost::lexical_cast<std::size_t>(argv[2]);
        
        if (argc > 3)
            pointsmin   = boost::lexical_cast<std::size_t>(argv[3]);
        if (argc > 4)
            pointsmax   = boost::lexical_cast<std::size_t>(argv[4]);
        
        if (argc > 5)
            rangemin    = boost::lexical_cast<double>(argv[5]);
        if (argc > 6)
            rangemax    = boost::lexical_cast<double>(argv[6]);
        
        if (argc > 7)
            variancemin = boost::lexical_cast<double>(argv[7]);
        if (argc > 8)
            variancemax = boost::lexical_cast<double>(argv[8]);
        
    } catch (...) {
        throw std::runtime_error("numerical parameter not correct");
    }    

    
    
    // create cloud object and set the data
    tools::sources::cloud<double> cloud(dim);

    // set range of the points
    cloud.setPoints(pointsmin, pointsmax);
    
    // set variance, range and samples
    cloud.setVariance(variancemin, variancemax);
    for(std::size_t i=0; i < dim; ++i)
        cloud.setRange(i, rangemin, rangemax, sampling);
    
    // create hdf file and generate data
    tools::files::hdf hdf("cloud.hdf5", true);
    hdf.write<double>( "/data",  cloud.generate(), H5::PredType::NATIVE_DOUBLE );
    
    std::cout << "create HDF file \"cloud.hdf5\" with dataset \"/data\"" << std::endl;
    return EXIT_SUCCESS;
}
