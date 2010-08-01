/** 
 \verbatim
 #########################################################################
 # GPL License                                                           #
 #                                                                       #
 # Copyright (c) 2010, Philipp Kraus, <philipp.kraus@flashpixx.de>       #
 # This program is free software: you can redistribute it and/or modify  #
 # it under the terms of the GNU General Public License as published by  #
 # the Free Software Foundation, either version 3 of the License, or     #
 # (at your option) any later version.                                   #
 #                                                                       #
 # This program is distributed in the hope that it will be useful,       #
 # but WITHOUT ANY WARRANTY; without even the implied warranty of        #
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
 # GNU General Public License for more details.                          #
 #                                                                       #
 # You should have received a copy of the GNU General Public License     #
 # along with this program.  If not, see <http://www.gnu.org/licenses/>. #
 #########################################################################
 \endverbatim
 **/

/**
 * @mainpage Machine Learning Framework
 *
 * @section requirements Requirements
 * <ul>
 * <li>ATLAS (http://math-atlas.sourceforge.net/)</li>
 * <li>Boost (http://www.boost.org/) (support for iostreams with gzip and bzip2 support musst be compiled, random and MPI support are optional)</li>
 * <li>Boost Bindings (SVN http://svn.boost.org/svn/boost/sandbox/numeric_bindings / ZIP http://mathema.tician.de/software/boost-numeric-bindings)</li>
 * <li>LAPACK (http://www.netlib.org/lapack/)</li>
 * <li>HDF (http://www.hdfgroup.org/)</li>
 * <li><i>optional MPI-Support:</i> Open MPI (http://www.open-mpi.org/) for unix systems / MS MPI (Microsoft Cluster Pack) for Windows system</li>
 * </ul>
 *
 * @section compileroptions Compiler Option
 * global toolbox compilerflags
 * <ul>
 * <li><pre>RANDOMDEVICE</pre> for using the Boost Device Random support (required Boost Random Device Support), otherwise a Mersenne Twister is used
 * <li><pre>MPI</pre> enable MPI Support for the toolbox (required Boost MPI support)</li>
 * </ul>
 * The following global compiler commands should be set
 * <ul>
 * <li><pre>NDEBUG</pre> for disabling Boost and local debugging</li>
 * <li><pre>BOOST_UBLAS_NDEBUG</pre> for disabling Boost UBlas support</li>
 * <li><pre>BOOST_NUMERIC_BINDINGS_BLAS_CBLAS</pre> add LAPACK support for the Boost Bindings</li>
 * </ul>
 *
 * @section defs definitions / assumption
 * <ul>
 * <li>data points should be matrix data and the matrix is row-orientated, so for K data points with every point dimension P, we have a K x P matrix (prototype matrices are equal)</li>
 * </ul>
 *
 * @section ex examples
 * <ul>
 * <li>@subpage clustering</li>
 * <li>@subpage distances</li>
 * </ul>
 *
 *
 * @page clustering clustering examples
 * @section kmeans k-Means
 * @code
    ublas::matrix<double> data = / fill data (row orientated) /;

    // create euclidian distance object
    tools::distances::euclid<double> d;

    // create kmeans with 11 prototypes and column dimension of data
    kmeans<double> kmeans(d, 11, data.size2());
    // enabled logging
    kmeans.setLogging(true);
    // clustering with 15 iterations
    kmeans.train(data, 15);

    // if logging is enabled
    if (kmeans.getLogging()) {
        
        // get prototypes after every iteration 
        std::vector< ublas::matrix<double> > p = kmeans.getLoggedPrototypes();

        for(std::size_t i=0; i < p.size(); ++i)
            std::cout << p[i] << std::endl;
    }
 
    // get row index for the prototype of every data point
    ublas::matrix<double> unkown = / create matrix with unkown data /;
    ublas::indirect_array< std::vector<std::size_t> > protoidx = kmeans.use(unkown);
 * @endcode
 *
 *
 * @page distances distance examples 
 * @section ncd normalize compression distance (NCD)
 * The namespace machinelearning::distances holds all types of distances. Every distance function is a subclass of <i>distance</i> and calculates distances values for vector- and matrixdata. The class must be implementated
 * as a template class and must hold some special functions for using the distance operation. In the namespace is also the ncd-class that creates a symmetric/asymmetric dissimilarity matrix of string- or filedata with the
 * <i>normalized compression distance</i>, that based on an approximation of the the Kolmogorov complexity. The example show how to use this class:
 * @code
    std::vector< std:: string > val;
    val.push_back( / add file path or string data / );
     
    ncd ncd1(ncd::gzip);
    ncd1.setCompressionLevel( ncd::bestspeed );
    std::cout << "read data as file and use gzip" << std::endl; 
    std::cout << "unsymmetric: " << ncd1.unsymmetric<double>(val, true) << std::endl;
    std::cout << "symmetric: " << ncd1.symmetric<double>(val, true) << std::endl;
 
    ncd ncd2(ncd::bzip2);
    ncd2.setCompressionLevel( ncd::bestcompression );
    std::cout << "read data as string and use bzip2" << std::endl; 
    std::cout << "unsymmetric: " << ncd1.unsymmetric<double>(val) << std::endl;
    std::cout << "symmetric: " << ncd1.symmetric<double>(val) << std::endl;
 * @endcode
 
**/

#ifndef MACHINELEARNING_H
#define MACHINELEARNING_H


#include "exception/exception.h"
#include "dimensionreduce/dimensionreduce.h"
#include "classifier/classifier.h"
#include "clustering/clustering.h"
#include "neighborhood/neighborhood.h"
#include "distances/distances.h"
#include "tools/tools.h"


#endif