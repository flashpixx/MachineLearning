/** 
 @cond
 #########################################################################
 # GPL License                                                           #
 #                                                                       #
 # This file is part of the Machine Learning Framework.                  #
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
 @endcond
 **/

/**
 * @mainpage Machine Learning Framework
 *
 * @section requirements Requirements
 * <ul>
 * <li>ATLAS (http://math-atlas.sourceforge.net/)</li>
 * <li>Boost (http://www.boost.org/) (support for iostreams with gzip and bzip2 support and thread support musst be compiled, random and MPI support are optional)</li>
 * <li>Boost Bindings (SVN http://svn.boost.org/svn/boost/sandbox/numeric_bindings / ZIP http://mathema.tician.de/software/boost-numeric-bindings)</li>
 * <li>LAPACK (http://www.netlib.org/lapack/)</li>
 * <li>HDF (http://www.hdfgroup.org/)</li>
 * <li><i>optional Message-Passing-Interface-Support:</i> Open MPI (http://www.open-mpi.org/) for unix systems / MS MPI (Microsoft Cluster Pack) for Windows system</li>
 * <li><i>optional GetText:</i> (http://www.gnu.org/software/gettext) for including multilanguage support</li>
 * </ul>
 *
 * @section compileroptions Compiler Option
 * global toolbox compilerflags
 * <ul>
 * <li><pre>RANDOMDEVICE</pre> for using the Boost Device Random support (required Boost Random Device Support), otherwise a Mersenne Twister is used</li>
 * <li><pre>MULTILANGUAGE</pre> option for compiling the framework with multilanguage support (uses gettext)</li>
 * <li><pre>MPI</pre> enable MPI Support for the toolbox (required Boost MPI support)</li>
 * </ul>
 * The following global compiler commands should be set
 * <ul>
 * <li><pre>NDEBUG</pre> for disabling Boost and local debugging</li>
 * <li><pre>BOOST_UBLAS_NDEBUG</pre> for disabling Boost UBlas support</li>
 * <li><pre>BOOST_NUMERIC_BINDINGS_BLAS_CBLAS</pre> add LAPACK support for the Boost Bindings</li>
 * </ul>
 *
 * @section License
 * The framework is supported under the @subpage license
 *
 * @section defs definitions / specifications
 * <ul>
 * <li>data points should be matrix data and the matrix is row-orientated, so for K data points with every point dimension P, we have a K x P matrix (prototype matrices are equal)</li>
 * </ul>
 *
 * @section ex advanced documentation
 * <ul>
 * <li>@subpage classifier</li>
 * <li>@subpage clustering</li>
 * <li>@subpage distances</li>
 * <li>@subpage dimreduce</li>
 * </ul>
 *
 *
 *
 * @page license GNU General Public License
 * @verbinclude license.txt
 *
 *
 *
 * @page classifier classifier
 * @section lazy lazy learner
 * @code
    ublas::matrix<double> data = / fill data (row orientated) /;
    std::vector<std::string> labels = / fill label for each row /;
 
    // create distance object
    distances::euclid<double> d;
    // neighbourhood k-nearest-neighbour object with 4 neighbours
    neighbourhood::knn<double> k(d, 4);
    // create lazy learner object with inverse-distance-weightening 
    classifier::lazylearner<double, std::string> ll(k, classifier::lazylearner<double, std::string>::inversedistance);
 
    // insert data
    ll.setDatabase( data,  labels );
 
 * @endcode
 *
 *
 *
 * @page clustering clustering
 * The clustering classes are templates classes, there are two main structures <i>supervised</i> and <i>nonsupervised</i>. The namespaces are named in the same way machinelearning::clustering::nonsupervised or 
 * machinelearning::clustering::supervised. All classes are subclasses of their supervised / unsupervised base class. Especially the supervised classes have two template parameters, first for the data and second
 * for their labels, the nonsupervised uses only one template parameter. The examples show the different using.
 *
 * @section kmeans k-Means
 * @code
    ublas::matrix<double> data = / fill data (row orientated) /;

    // create euclidian distance object
    distances::euclid<double> d;
    // create kmeans with 11 prototypes and column dimension of data
    clustering::nonsupervised::kmeans<double> kmeans(d, 11, data.size2());
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
 * @section ng neural gas
 * @code
    ublas::matrix<double> data = / fill data (row orientated) /;
 
    // create distance object
    distances::euclid<double> d;
    // create neural gas object with 11 prototypes
    clustering::nonsupervised::neuralgas<double> ng(d, 11, data.size2());
    // enabled logging
    ng.setLogging(true);
    // clustering with 15 iterations
    ng.train(data, 15);
 
    // if logging is enabled
    if (ng.getLogging()) {
 
        // get prototypes after every iteration 
        std::vector< ublas::matrix<double> > p = ng.getLoggedPrototypes();
 
        for(std::size_t i=0; i < p.size(); ++i)
            std::cout << p[i] << std::endl;
    }
 
    // get row index for the prototype of every data point
    ublas::matrix<double> unkown = / create matrix with unkown data /;
    ublas::indirect_array< std::vector<std::size_t> > protoidx = ng.use(unkown);
 * @endcode
 *
 * @section rlvq relevance vector quantization (RLVQ)
 * @code
    ublas::matrix<double> data = / fill data (row orientated) /;
    std::vector<std::string> labels = / fill label for each row /;
    std::vector<std::string> prototypelabel = / fill label for each prototype /;
 
    // create distance object
    distances::euclid<double> d;
    // create rlvq object (the size of label vector is the number of prototypes)
    clustering::supervised::rlvq<double, std::string> rlvq(d, prototypelabel, data.size2())
    // enabled logging
    rlvq.setLogging(true);
 
    // train prototypes in 15 iterations
    rlvq.train(data, labels, 15);
 * @endcode
 *
 * @section spcl spectral clustering
 * @code
    ublas::matrix<double> data = / similarity matrix /;
 
    // create spectral clustering object with 3 cluster
    clustering::nonsupervised::spectralclustering<double> spectral(3, data.size1());
    // run clustering with 15 iteration
    spectral.train(data, 15);
 * @endcode
 *
 *
 *
 * @page dimreduce dimension reducing
 * The dimension reducing classes are in the namespace machinelearning::dimensionreduce and can be used for dimension reduction. The are two subnamespaces machinelearning::dimensionreduce::nonsupervised and 
 * machinelearning::dimensionreduce::supervised. Supervised uses two template parameter first one for datatype, second one for labeling. The nonsupervised is used only one parameter (for datatype). The examples
 * show the different option
 *
 * @section pca principle component analysis (PCA)
 * @code
    ublas::matrix<double> data = / fill data (row orientated) /;
 
    // create a pca object for reduce data to 2 dimensions
    dimensionreduce::nonsupervised::pca<double> p(2);
    
    // create reduce data matrix
    ublas::matrix<double> reduce = pca.map(data);
 * @endcode
 *
 * @section lda linear discriminant analysis (LDA)
 * @code
    ublas::matrix<double> data      = / fill data (row orientated) /;
    std::vector<std::string> labels = / fill label for each row /;
 
    // create lda object for reduce data to 2 dimensions
    dimensionreduce::supervised::lda<double, std::string> lda(2);
 
    // create reduce data
    ublas::matrix<double> reduce = lda.map(data, labels);
 * @endcode
 *
 * @section lle local linear embedding (LLE)
 * @code
 * @endcode
 *
 *
 *
 * @page distances distances
 * The namespace machinelearning::distances holds all types of distances. Every distance function is a subclass of <i>distance</i> and calculates distances values for vector- and matrixdata. The class must be implementated
 * as a template class and must hold some special functions for using the distance operation. In the namespace is also the ncd-class that creates a symmetric/asymmetric dissimilarity matrix of string- or filedata with the
 * <i>normalized compression distance</i>, that based on an approximation of the the Kolmogorov complexity. The example show how to use these classes.
 *
 * @section ncd normalize compression distance (NCD)
 * @code
    std::vector< std:: string > val;
    val.push_back( / add file path or string data / );
     
    distances::ncd ncd1( distances::ncd::gzip );
    ncd1.setCompressionLevel( ncd::bestspeed );
    std::cout << "read data as file and use gzip" << std::endl; 
    std::cout << "unsymmetric: " << ncd1.unsymmetric<double>(val, true) << std::endl;
    std::cout << "symmetric: " << ncd1.symmetric<double>(val, true) << std::endl;
 
    distances::ncd ncd2( distances::ncd::bzip2 );
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
#include "neuronalnetwork/neuronalnetwork.h"
#include "neighborhood/neighborhood.h"
#include "distances/distances.h"
#include "tools/tools.h"
#include "functionoptimization/functionoptimization.h"

#endif