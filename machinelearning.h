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
 * <li>Boost (http://www.boost.org/)</li>
 * <li>Boost Bindings (SVN http://svn.boost.org/svn/boost/sandbox/numeric_bindings / ZIP http://mathema.tician.de/software/boost-numeric-bindings)</li>
 * <li>LAPACK (http://www.netlib.org/lapack/)</li>
 * <li>HDF (http://www.hdfgroup.org/)</li>
 * <li>=></li><i>optional MPI-Support:</i> Open MPI (http://www.open-mpi.org/) for unix systems / MS MPI (Microsoft Cluster Pack) for Windows system
 * </ul>
 *
 * @section compileroptions Compiler Option
 * The follingen global compiler commands must be set
 * <ul>
 * <li><pre>NDEBUG</pre> for disabling Boost and local debugging</li>
 * <li><pre>BOOST_UBLAS_NDEBUG</pre> for disabling Boost UBlas support</li>
 * <li><pre>BOOST_NUMERIC_BINDINGS_BLAS_CBLAS</pre> add LAPACK support for the Boost Bindings</li>
 * <li><pre>DEVICERANDOM</pre> for using the Boost Device Random support, otherwise a Mersenne Twister is used
 * <li><pre>MPI</pre> enable MPI Support for the toolbox (required Boost MPI support)</li>
 * </ul>
 *
 * @section defs definitions / assumption
 * <ul>
 * <li>every similarity / dissimilarity matrix that is a method parameter must be a dissimilarity matrix with diagonal elements zero (otherwise it must be specified)</li>
 * </ul>
 *
 * @page libaray Library Installation 
 * @section atlaslapack Atlas with LAPACK support
 * We need only ATLAS and LAPACK working on C/C++ compiler. For unix systems we need only to download both library and unpack only the ATLAS
 * library. Than we create a tempory directory, change to the directory and call the command:
 * <pre>atlas_source_dir/configure --nof77 --prefix=installation_dir --with-netlib-lapack-tarfile=path_lapack_tar_gzip/lapack.tgz </pre>
 * After configure run make and make install for installing.
 *
 *
 *
 * @section boost Boost Libarary
 * Boost can be installed normally with bJam (see Boost page). If you want to MPI support, you must compiled the Boost with MPI support
 * 
 * @subsection boostio Boost I/O Support
 * The normalized-compression-distance (NCD) depends on Boost I/O calls and the GZIP and BZIP-2 routines, so both must be compiled
 * and linked. For windows users 
 *
 *
 * @subsection boostrandom Boost Random Support
 * The random support is compiled with a Mersenne Twister on default. With the compilerfalg <i>DEVICERANDOM</i> all random calls are compiled
 * against the Boost Device Random support
 * 
 *
 *
 * @section bindings Boost Bindings
 * The Boost Bindings can download from the SVN or use the ZIP. You must only add to the include path
 *
 *
 *
 * @section hdf HDF Library
 * For unix system call the following command:
 * <pre>configure --enable-cxx --prefix=install_dir</pre>
 * The <i>enbale-cxx</i> option adds the C++ headers into the include directory
 *
 *
 *
 * @page examples Code Examples
 * @section k-Means
 * <pre>
        ublas::matrix<double> data = / fill data (row orientated) /;
 
        // create euclidian distance object
        tools::distances::euclid<double> d;
 
        // create kmeans with 11 prototypes and dimension 2
        kmeans<double> kmeans(d, 11, 2);
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
 * </pre>
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