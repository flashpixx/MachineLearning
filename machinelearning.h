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

/** 
 * @mainpage Machine Learning Framework
 * @author Philipp Kraus <philipp.kraus@flashpixx.de> 
 *
 * @section licenseSection license
 * The framework is supported under the @subpage license
 * 
 * @section requirements requirements
 * <ul>
 * <li>Automatically Tuned Linear Algebra Software ( http://math-atlas.sourceforge.net/ )</li>
 * <li>Boost ( http://www.boost.org/ ) (iostreams with gzip and bzip2 support and thread support musst be compiled within, random device and MPI support are optional)</li>
 * <li>Boost Bindings (SVN http://svn.boost.org/svn/boost/sandbox/numeric_bindings )</li>
 * <li>GiNaC ( http://www.ginac.de/ )</li>
 * <li>Linear Algebra PACKage ( http://www.netlib.org/lapack/ )</li>
 * <li><i>optional Hierarchical Data Format (HDF)</i> ( http://www.hdfgroup.org/ )</li>
 * <li><i>optional Message-Passing-Interface-Support</i>
 *      <ul>
 *          <li>Open MPI ( http://www.open-mpi.org/ )</li>
 *          <li>MPICH2 ( http://www.mcs.anl.gov/research/projects/mpich2/ )</li>
 *          <li>Microsoft High Performance Computing Pack 2008 R2 SDK ( http://www.microsoft.com/downloads/en/details.aspx?FamilyID=bc671b22-f158-4a5f-828b-7a374b881172 )</li>
 *      </ul>
 * <li><i>optional GetText</i> ( http://www.gnu.org/software/gettext ) for including multilanguage support</li>
 * <li><i>optional LibXML2</i> ( http://xmlsoft.org/ ) (used by wikipedia support)</li>
 * <li><i>optional Scons</i> ( http://www.scons.org/ )</li>
 * <li><i>optional Doxygen</i> ( http://www.doxygen.org/ )</li>
 * </ul>
 *
 * @section def definition / style guide
 * <ul>
 * <li>data points should be matrix data and the matrix is row-orientated, so for K data points with every point dimension P, we have a K x P matrix (prototype matrices are equal)</li>
 * <li>all compiler flags start with <dfn>ML_</dfn></li>
 * <li>all structures are in the namespace <dfn>machinelearning</dfn></li>
 * <li>all messages should get the structure <dfn>_("<message>")</dfn>, because the underline prefix is the support for different languages</li>
 * </ul>
 *
 * The source code uses some style guides options:
 * <ul>
 * <li>all variables use a prefix for their scopde: 
 *      <ul>
 *          <li><dfn>l_</dfn> local variable / scope within a methode</li>
 *          <li><dfn>m_</dfn> member variable / scope within an object</li>
 *          <li><dfn>g_</dfn> globale variable / scope within the global code / namespace</li>
 *      </ul>
 *      The excepts are counting variables eg. <dfn>i</dfn>, <dfn>j</dfn>, <dfn>n</dfn> or iterator variables <dfn>it</dfn>
 * </li>
 * <li>classes, that implementates algorithms with numerical structures, should be written as template class</li>
 * <li>methods, properties, classes and namespaces should be documentated with Doxygen</li>
 * <li>each subdirectory of the framework, should have a header file with the same directoryname for including the whole directory. This header file must be appended to the main header file of the framework</li>
 * <li>the svn keyword LastChangeDate should be set for all files</li>
 * <li>all parameters should be a call-by-reference parameter</li>
 * </ul>
 *
 * @section compileroptions compiler option
 * toolbox compilerflags
 * <ul>
 * <li><dfn>ML_RANDOMDEVICE</dfn> for using the Boost Device Random support (required Boost Random Device Support), otherwise a Mersenne Twister is used</li>
 * <li><dfn>ML_MULTILANGUAGE</dfn> option for compiling the framework with multilanguage support (uses gettext)</li>
 * <li><dfn>ML_FILES</dfn> adds the support for HDF & CSV file reading and writing</li>
 * <li><dfn>ML_SOURCES</dfn> compiles sources in that way, that e.g. NNTP / Wikipedia data can be read directly</li>
 * <li><dfn>ML_CLUSTER</dfn> enable MPI Support for the toolbox (required Boost MPI support)</li>
 * </ul>
 * The following compiler commands should / must be set
 * <ul>
 * <li><dfn>NDEBUG</dfn> for disabling Boost and local debugging</li>
 * <li><dfn>BOOST_UBLAS_NDEBUG</dfn> for disabling Boost Ublas support</li>
 * <li><dfn>BOOST_NUMERIC_BINDINGS_BLAS_CBLAS</dfn> add CBLAS / LAPACK support for the Boost Bindings</li>
 * </ul>
 *
 * @subsection scons compiling with scons
 * The framework supports Scons for compiling. The script reads the environment (Linux, OS X & Microsoft) and uses the
 * plattform specified options for compiling the sources. The Scons script supports some different parameters:
 * <ul>
 * <li><dfn>--create-documentation</dfn> creates the doxygen documentation (doxygen must be called, so it should be within the path)</li>
 * <li><dfn>--create-language</dfn> creates the language files / adds new messages to the existing files (xgettext must be called, so it should be within the path)</li>
 * <li><dfn>--compile-language</dfn> compiles all language files (msgfmt must be called, so it should be within the path)</li>
 * </ul>
 * For compiling the test cpp files in the test directory the following option can be set
 * <ul>
 * <li><dfn>--with-randomdevice</dfn> adds the compilerflag for random device support</li>
 * <li><dfn>--with-mpi</dfn> adds the compilerflag for cluster / MPI support</li>
 * <li><dfn>--with-multilanguage</dfn> adds the multilanguage support with gettext</li>
 * <li><dfn>--with-sources</dfn> support for the namespace machinelearning::tools::sources</li>
 * <li><dfn>--with-files</dfn> support for the namespace machinelearning::tools::files</li>
 * </ul>
 *
 * @subsection targets build targets
 * The scons script supports different targets for building the examples. The call can be supplemented with the parameters above. Each subdirectory within the example directory
 * show a build target, but some targets will be build first if different parameters are set
 * <ul>
 * <li><dfn>source</dfn> sources are eg. Wikipedia, NNTP, but the parameter <dfn>--with-sources</dfn> must be set for compiling and for the cloud example the files parameter must be set additionally</li>
 * <li><dfn>clustering</dfn> this target build all clustering algorithms, but the <dfn>--with-files</dfn> parameter must be set</li>
 * <li><dfn>classifier</dfn> this target build all classifier algorithms, but the <dfn>--with-files</dfn> parameter must be set</li>
 * <li><dfn>reducing</dfn> this target build all dimension reduce algorithms, but the <dfn>--with-files</dfn> parameter must be set</li>
 * <li><dfn>distance</dfn> this target build all distance algorithms, but the <dfn>--with-files</dfn> parameter must be set</li>
 * </ul>
 *
 * @section ex advanced documentation
 * <ul>
 * <li>@subpage classifier</li>
 * <li>@subpage clustering</li>
 * <li>@subpage sources</li>
 * <li>@subpage distances</li>
 * <li>@subpage dimreduce</li>
 * <li>@subpage files</li>
 * <li>@subpage logger</li>
 * <li>@subpage lang</li>
 * </ul>
 *
 *
 *
 * @page license GNU Lesser General Public License
 * @verbinclude license.txt
 *
 *
 *
 * @page classifier classifier
 * The classifier algorithms are tempalte classes.
 * @section lazy lazy learner
 * @include examples/classifier/lazy.cpp
 *
 *
 *
 * @page clustering clustering
 * The clustering classes are templates classes, there are two main structures <i>supervised</i> and <i>nonsupervised</i>. The namespaces are named in the same way machinelearning::clustering::nonsupervised or 
 * machinelearning::clustering::supervised. All classes are subclasses of their supervised / unsupervised base class. Especially the supervised classes have two template parameters, first for the data and second
 * for their labels, the nonsupervised uses only one template parameter. The examples show the different using.
 *
 * @section kmeans k-Means
 * @include examples/clustering/kmeans.cpp
 *
 * @section ng neural gas
 * @include examples/clustering/neuralgas.cpp
 *
 * @section rlvq relevance learning vector quantization (RLVQ)
 * @include examples/clustering/rlvq.cpp
 *
 * @section spcl spectral clustering
 * @include examples/clustering/spectral.cpp
 *
 *
 *
 * @page dimreduce dimension reducing
 * The dimension reducing classes are in the namespace machinelearning::dimensionreduce::nonsupervised and machinelearning::dimensionreduce::supervised can be used
 * for dimension reduction. Supervised uses two template parameter first one for datatype, second one for labeling. The nonsupervised is used only one parameter
 * (for datatype). The examples show the different option
 *
 * @section pca principle component analysis (PCA)
 * @include examples/reducing/pca.cpp
 *
 * @section lda linear discriminant analysis (LDA)
 * @include examples/reducing/lda.cpp
 *
 * @section mds multidimensional scaling (MDS)
 * @include examples/reducing/mds.cpp
 *
 * @section lle local linear embedding (LLE)
 * @code
 * @endcode
 *
 *
 *
 * @page distances distances
 * The namespace machinelearning::distances holds all types of distances. Every distance function is a subclass of <i>distance</i> and calculates distances values for vector- and matrixdata.
 * The class must be implementated as a template class and must hold some special functions for using the distance operation. In the namespace is also the ncd-class that creates a
 * symmetric/asymmetric dissimilarity matrix of string- or filedata with the <i>normalized compression distance</i>, that based on an approximation of the the Kolmogorov complexity. The
 * example show how to use these classes.
 *
 * @section ncd normalize compression distance (NCD)
 * @include examples/distance/ncd.cpp
 *
 *
 *
 * @page sources data sources
 * The toolbox implements some structures for geting datasets. The compile option <dfn>ML_SOURCES</dfn> must be set for compiling the namespace
 * machinelearning::tools::sources .
 *
 * @section nntp newsgroups
 * The class allows to browse newsgroups in a naitve way for extraction the message content. The class throws different exception if there
 * are connection or stream errors during sending or receiving data (see exceptions within the class).
 * @include examples/sources/newsgroup.cpp
 *
 * @section wiki wikipedia
 * The wikipedia class can be used for read the article data of wikipedia articles in different languages
 * @include examples/sources/wikipedia.cpp
 *
 * @section cloud cloud
 * The cloud class creates a multimodal n-dimensional data set with normal distribution. The n-dimensional cube is sampled in equidistant
 * steps and on the cross-points a normal distribution is created.
 * @include examples/sources/cloud.cpp
 * 
 *
 *
 * @page logger logger
 * Within the toolbox is a logger class which implements a thread-safe and optional MPI logger. The logger create a singletone object that create
 * a file access for writing messages. The MPI component sends all messages with non-blocking communication to the CPU 0. See in the logger class
 * for log states, which must be used for writing the messages.
 * @section normal normal use
 * @code
    // sets the log level for writing messages 
    tools::logger::getInstance()->setLevel( tools::logger::info );
 
    // creates a message
    tools::logger::getInstance()->write( tools::logger::warn, "test message" );
 
    // shows the filename in which the messages are write down
    std::cout << tools::logger::getInstance()->getFilename() << std::endl;
 * @endcode
 *
 * @section mpi mpi use
 * <strong>The MPI library must be compiled with thread-support and must be initialized manually</strong>
 * @code
    MPI::Init_thread( argc, argv, MPI_THREAD_SERIALIZED )
    boost::mpi::communicator l_mpi;
 
    // create the listener 
    tools::logger::getInstance()->startListener( l_mpi );
 
 
    // sets the log level for writing messages (it is seperated for each CPU)
    tools::logger::getInstance()->setLevel( tools::logger::info );
 
    // creates a message in the local log
    tools::logger::getInstance()->write( tools::logger::warn, "test message" );
 
    // creates a message with MPI use
    tools::logger::getInstance()->write( l_mpi, tools::logger::warn, "test message with MPI" );
 
    // close the listener
    tools::logger::getInstance()->shutdownListener( l_mpi );
 
    // shows the filename of each CPU (only CPU 0 collectes all messages with MPI support)
    std::cout << tools::logger::getInstance()->getFilename() << std::endl;
    MPI::Finalize()
 * @endcode
 *
 *
 *
 * @page files file support
 * The toolbox supports different file formats. The compile option <dfn>ML_FILES</dfn> must be set for
 * using the support and all components are within the namespace machinelearning::tools::files
 * @section csv comma separated values (csv)
 * @code
    tools::sources::csv csv;
 
    // read matrix data (optional second parameter is a string with separator characters)
    // the file content must be followed this order:
    // <number or rows> <number of columns>
    // 1. row: 1. column, 2.column ....
    // ....
    boost::numeric::ublas::matrix<double> matrix = csv.readBlasMatrix<double>("<filename>");
 
    // read vector data
    // the file content must be followd this order:
    // <number of elements>
    // 1. element
    // ....
    boost::numeric::ublas::vector<double> vector = csv.readBlasVector<double>("<filename>");
 
    // read any vector data
    // the file content must be followd this order:
    // 1. element
    // 2. element
    // ....
    std::vector<T> vec = csv::readVector<T>("<filename>");
 
 
    // write data
    csv.write<double>("<filename>", vector);
    // third parameter set the separater character
    csv.write<double>(""<filename>", matrix);
 * @endcode
 *
 * @section hdf hierarchical data format (hdf)
 * HDF files need on reading and writing own data formats that description can found on http://www.hdfgroup.org/HDF5/doc/cpplus_RM/classH5_1_1PredType.html 
 * @code
    // only read access
    tools::files::hdf source("<path to hdf file>");
 
    // read matrix data
    boost::numeric::ublas::matrix<double> matrix = source.readMatrix<double>("<path to dataset>", H5::PredType::NATIVE_DOUBLE);
    // read vector data
    boost::numeric::ublas::vector<unsigned int> vector = source.readVector<unsigned int>("<path to dataset>", H5::PredType::NATIVE_UINT);
    // read string
    std::string str = source.readString("<path to data>");
 
    // create a new & empty hdf file (read / write access)
    tools::files::hdf target("path to hdf file", true);
    target.write<double>( "<path for dataset>",  matrix, H5::PredType::NATIVE_DOUBLE );
    target.write<double>( "<path for dataset>",  vector, H5::PredType::NATIVE_DOUBLE );
 
    // hdf file will be closed and flushed if variable lost the scope
 * @endcode
 *
 *
 *
 * @page lang multilanguage support
 * The toolbox uses <dfn>gettext</dfn> to translate the messages. The source code must include the language header file and all messages,
 * which should be supported multilanguage must be written in this way <dfn>_("messages")</dfn>. The mesages will be collcted with the
 * gettext tools and safed under <dfn>tools/language/#language eg. de_DE.UTF-8#/LC_MESSAGES/#language eg de_DE#.po</dfn>. The compiler flag
 * <dfn>ML_MULTILANGUAGE</dfn> must be set for using the support. The message files can be edited with Poedit ( http://www.poedit.net/ ).
 * For using the language support, a call for setting the correct languages must be written wihin the main program:
 * @code
    tools::language::bind(<name of the language file, should be "ml">, <path to the language directory, shoudl be "./tools/language/">);
 * @endcode
 *
 *
 *
 * @file machinelearning.h main header for including in a project
 *
 * @file examples/distance/ncd.cpp testprogram for the normalized compression distance
 * @file examples/clustering/neuralgas.cpp testprogram for the neural gas algorithm
 * @file examples/clustering/kmeans.cpp testprogram for the k-means algorithm
 * @file examples/clustering/spectral.cpp testprogram for spectral clustering
 * @file examples/clustering/rlvq.cpp testprogram for the rlvq algorithm
 * @file examples/classifier/lazy.cpp testprogram for the lazy-learner algorithm
 * @file examples/reducing/pca.cpp testprogram for the pca
 * @file examples/reducing/lda.cpp testprogramm for lda
 * @file examples/reducing/mds.cpp testprogramm for mds
 * @file examples/sources/newsgroup.cpp testprogram for NNTP using
 * @file examples/sources/wikipedia.cpp testprogram for using Wikipedia
 * @file examples/sources/cloud.cpp testprogram for create n-dimensional normal distribution
 *
 * @file classifier/classifier.h main header for all classifier structurs
 * @file classifier/classifier.hpp header for the abstract class implementation of the classifiers
 * @file classifier/lazylearner.hpp lazy learner algorithm
 *
 * @file clustering/clustering.h main header for all clustering algorithms
 * @file clustering/clustering.hpp header for abstract clustering classes
 * @file clustering/nonsupervised/kmeans.hpp k-means implementation
 * @file clustering/nonsupervised/neuralgas.hpp neuralgas implemention for real vector space
 * @file clustering/nonsupervised/spectralclustering.hpp implementation of the spectral clustering
 * @file clustering/supervised/rlvq.hpp implementation of relevance vector quantization
 *
 * @file dimensionreduce/dimensionreduce.h main header of dimension reducing algorithms
 * @file dimensionreduce/dimensionreduce.hpp abstract class for dimension reducing classes
 * @file dimensionreduce/nonsupervised/lle.hpp local linear embedding implementation
 * @file dimensionreduce/nonsupervised/pca.hpp principal component analysis implementation
 * @file dimensionreduce/nonsupervised/mds.hpp multidimensional scaling implementation
 * @file dimensionreduce/supervised/lda.hpp lineare discriminant analysis implementation
 * 
 * @file distances/distances.h main header for all distance structures
 * @file distances/distance.hpp abstract class for distance algorithms
 * @file distances/norm/euclid.hpp class for euclidian distances
 * @file distances/ncd.hpp implementation of the normalize compression distance
 *
 * @file exception/exception.cpp implementation of the exceptions
 * @file exception/exception.h header file for exceptions
 *
 * @file functionoptimization/functionoptimization.h main header for function optimization
 * @file functionoptimization/gradientdescent.hpp gradient descent implementation
 *
 * @file neighborhood/neighborhood.h main header for neighborhood structurs
 * @file neighborhood/neighborhood.hpp abstract class for neighborhood implementation
 * @file neighborhood/knn.hpp k-nearest-neighborhood implementation
 *
 * @file tools/tools.h main header for tools algorithms
 * @file tools/function.hpp different functions eg. numerical limit checking
 * @file tools/logger.cpp logger static implementation
 * @file tools/logger.hpp logger implementation
 * @file tools/lapack.hpp wrapper class for LAPack calls
 * @file tools/matrix.hpp implementation of matrix operations
 * @file tools/vector.hpp implementation of vector operations
 * @file tools/random.hpp random implementation 
 * @file tools/random.cpp random static implementation
 *
 * @file tools/sources/sources.h main header for all sources
 * @file tools/sources/nntp.h NNTP client
 * @file tools/sources/wikipedia.h wikipedia client
 * @file tools/sources/cloud.hpp implementation of cloud datasets
 *
 * @file tools/language/language.h multilanguage calls
 *
 * @file tools/files/files.h main header for file structurs
 * @file tools/files/csv.hpp implementation for reading and writing csv files
 * @file tools/files/hdf.hpp implementation for reading and writing hdf files
**/

#ifndef MACHINELEARNING_H
#define MACHINELEARNING_H


/** main namespace for the framework
 * $LastChangedDate$
 **/
namespace machinelearning {};


#include "exception/exception.h"
#include "dimensionreduce/dimensionreduce.h"
#include "classifier/classifier.h"
#include "clustering/clustering.h"
#include "neighborhood/neighborhood.h"
#include "distances/distances.h"
#include "tools/tools.h"
#include "functionoptimization/functionoptimization.h"

#endif