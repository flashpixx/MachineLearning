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

namespace tl        = machinelearning::tools;
namespace dist      = machinelearning::distances;
namespace ndim      = machinelearning::dimensionreduce::nonsupervised;
namespace dim       = machinelearning::dimensionreduce::supervised;
namespace sl        = machinelearning::clustering::supervised;
namespace nsl       = machinelearning::clustering::nonsupervised;
namespace nd        = machinelearning::neighborhood;
namespace cl        = machinelearning::classifier;
namespace func      = machinelearning::functionaloptimization;
namespace linalg    = boost::numeric::bindings::lapack;
namespace ublas     = boost::numeric::ublas;

int main(int argc, char* argv[]) {

    std::vector< std:: string > val;
    val.push_back("main.cpp");
    val.push_back("tools/files/hdf.hpp");
    val.push_back("distances/ncd.hpp");
    val.push_back("clustering/supervised/rlvq.hpp");

    val.push_back("tools/lapack.hpp");
    val.push_back("tools/random.hpp");
    val.push_back("tools/matrix.hpp");
    val.push_back("tools/vector.hpp");

    dist::ncd<double> ncd(dist::ncd<double>::bzip2);
    //ncd.setCompressionLevel( dist::ncd::bestspeed );
    //std::cout << "unsymmetric: " << ncd.unsymmetric(val, true) << std::endl;
    std::cout << "symmetric: " << ncd.symmetric(val, true) << std::endl;
        
    return EXIT_SUCCESS;
}