/** 
 @cond
 ############################################################################
 # LGPL License                                                             #
 #                                                                          #
 # This file is part of the Machine Learning Framework.                     #
 # Copyright (c) 2010-2012, Philipp Kraus, <philipp.kraus@flashpixx.de>     #
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
 * @mainpage Base Information
 * @author Philipp Kraus <philipp.kraus@flashpixx.de> 
 *
 * @section licenseSection License
 * The framework is supported under the <a href="http://www.gnu.org/licenses/lgpl-3.0.en.html">GNU Lesser General Public License</a>
 * 
 * @section requirements Requirements
 * <ul>
 * <li>Automatically Tuned Linear Algebra Software ( http://math-atlas.sourceforge.net/ ) [Version newer than 3.9.40]</li>
 * <li>Boost ( http://www.boost.org/ ) with following components
 *     <ul>
 *         <li>IOStreams with ZLib and BZip2 support</li>
 *         <li>Exception support</li>
 *         <li>Math support</li>
 *         <li>Regular Expression support</li>
 *         <li>System support</li>
 *         <li><i>optional Date-Time support</i> (used by Twitter support)</li>
 *         <li><i>optional Program Options</i> (only used by the examples)</li>
 *         <li><i>optional Filesystem support</i> (only used by the examples)</li>
 *         <li><i>optional Thread support</i> (only used by the framework logger)</li>
 *         <li><i>optional Serialization</i> (only used by MPI use)</li>
 *         <li><i>optional Program options</i> (only used by the examples)</li>
 *         <li><i>optional Random Device support</i></li>
 *         <li><i>optional MPI support</i></li>
 *     </ul>
 * </li>
 * <li>Boost Bindings ( SVN http://svn.boost.org/svn/boost/sandbox/numeric_bindings/ )</li>
 * <li>Linear Algebra PACKage ( http://www.netlib.org/lapack/ ) [Version newer than 3.3]</li>
 * <li>OpenMP ( http://www.openmp.org )</li>
 * <li><i>optional Hierarchical Data Format (HDF)</i> ( http://www.hdfgroup.org/ ) [Version newer or equal 5.1.8.4]</li>
 * <li><i>optional Message-Passing-Interface-Support</i>
 *      <ul>
 *          <li>Open MPI ( http://www.open-mpi.org/ )</li>
 *          <li>MPICH2 ( http://www.mcs.anl.gov/research/projects/mpich2/ )</li>
 *          <li>Microsoft High Performance Computing Pack SDK ( http://www.microsoft.com/download/en/search.aspx?q=HPC%20Pack%20SDK )</li>
 *      </ul>
 * </li>
 * <li><i>optional GiNaC</i> ( http://www.ginac.de/ ) with CLN ( http://www.ginac.de/CLN/ ) [Version newer or equal 1.6]</li>
 * <li><i>optional GetText</i> ( http://www.gnu.org/software/gettext/ ) (used by multilanguage support)</li>
 * <li><i>optional LibXML2</i> ( http://xmlsoft.org/ ) (used by wikipedia support)</li>
 * <li><i>optional LibJSONCPP</i> ( http://sourceforge.net/projects/jsoncpp/ ) (used by twitter support)</li>
 * <li><i>optional Java Runtime Environment (JRE) / Java Developer Kit (JDK)</i> ( http://www.java.com/ / http://www.oracle.com/technetwork/java/javase/downloads/index.html ) </li>
 * <li><i>optional Scons</i> ( http://www.scons.org/ )</li>
 * <li><i>optional Swig</i> ( http://www.swig.org/ )</li>
 * <li><i>optional Doxygen</i> ( http://www.doxygen.org/ )</li>
 * <li><i>optional CMake</i> ( http://www.cmake.org/ ) needed by MSYS / MinGW use</li>
 * </ul>
 * A default library installation can be created with the commands
 * @code
    scons librarydownload
    scons librarybuild
 * @endcode
 * After compiling the directory <dfn>library/build_*</dfn> is created with the libraries. The * can be <dfn>debug</dfn> or <dfn>release</dfn> for different build options.
 * The framework has been tested under
 * <ul>
 * <li>Mac OS X Leopard (10.5)</li>
 * <li>Mac OS X Snow Leopard (10.6)</li>
 * <li>Mac OS X Lion (10.7)</li>
 * <li>Ubuntu Lucid Lynx (10.04)</li>
 * <li>Linux Mint Maya (13)</li>
 * <li>Gentoo Linux (stable / unstable)</li>
 * <li>Windows 7 with <a href="http://www.cygwin.com/">Cygwin</a> with the buildin Python installation [in some cases you need to rebase all libraries in Cygwin (eg fork errors):
 * Close all Cygwin windows, start the <dfn>dash.exe</dfn> from Cygwins bin directory and run the command <dfn>/bin/rebaseall -v</dfn>]</li>
 * <li>Windows 7 with <a href="http://www.mingw.org">MinGW / MSYS</a> with a native <a href="http://www.python.org">Python installation</a></li>
 * </ul>
 * On projects the main header <dfn>machinelearning.h</dfn> and the cpp file <dfn>machinelearning.cpp</dfn> are needed.
 *
 *
 * @section def Definition / Style Guide
 * <ul>
 * <li>the framework is only header-based so you don't need compile any files (except logger or random device support is needed, than the <dfn>machinelearnig.cpp</dfn> must compiled and linked)</li>
 * <li>data points should be matrix data and the matrix is row-orientated, so for K data points with every point dimension P, we have a K x P matrix (prototype matrices are equal)</li>
 * <li>all template classes / algorithms must have a floating point datatype on instantiation, because the datastructurs need this types for correct value calculating. The (base) classes use a static 
 * assertion for checking the value type</li>
 * <li>all compiler flags start with <dfn>MACHINELEARNING_</dfn></li>
 * <li>all include defines for header files are set in this way <dfn>__MACHINELEARNING_DIRECTORY_FILE_H/HPP</dfn>
 * <li>all structures are in the namespace <dfn>machinelearning</dfn></li>
 * <li>all messages should get the structure <dfn>_("<message>")</dfn>, because the underline prefix is the support for different languages</li>
 * <li>all subbuild scripts are stored within the directory that stores the main build files, the build file is named <dfn>build.py</dfn></li>
 * <li>platform environmanetal build scripts are stores under <dfn>buildenvironment</dfn></li>
 * </ul>
 *
 * The source code uses some style guides options:
 * <ul>
 * <li>all variables use a prefix for their scope (inspired by <a href="http://en.wikipedia.org/wiki/Hungarian_notation">hungarian notation</a>): 
 *      <ul>
 *          <li><dfn>l_</dfn> local variable / scope within a methode</li>
 *          <li><dfn>m_</dfn> member variable / scope within an object</li>
 *          <li><dfn>p_</dfn> parameter variable within a method / function declaration</li>
 *      </ul>
 *      The excepts are counting variables eg. <dfn>i</dfn>, <dfn>j</dfn>, <dfn>n</dfn> or iterator variables <dfn>it</dfn>
 * </li>
 * <li>classes, that implementates algorithms with numerical structures, should be written as template class</li>
 * <li>methods, properties, classes and namespaces should be documentated with Doxygen</li>
 * <li>each subdirectory of the framework, should have a header file with the same directoryname for including the whole directory. This header file must be appended to the main header file of the framework</li>
 * <li>all parameters should be a call-by-reference parameter</li>
 * <li>matrix structures should be row-oriantated, so loops over the elements are first over the rows and columns on the second / inner loop</li>
 * <li>template parameter should be asserted for the correct type</li>
 * <li>header inclusion (forward declaration) and implementation are split into two files (if needed), header file (.h/.hpp) and the implementation file with name <dfn>name.implementation.hpp/.h/.cpp</dfn></li>
 * </ul>
 *
 *
 * @section compileroptions Compileroption
 * toolbox compilerflags
 * <ul>
 * <li><dfn>MACHINELEARNING_NDEBUG</dfn> remove any debug information (eg framework individual asserts)</li> 
 * <li><dfn>MACHINELEARNING_RANDOMDEVICE</dfn> for using the Boost Device Random support (requires Boost Random Device Support), otherwise a Mersenne Twister is used</li>
 * <li><dfn>MACHINELEARNING_MULTILANGUAGE</dfn> option for compiling the framework with multilanguage support (uses gettext)</li>
 * <li><dfn>MACHINELEARNING_LOGGER</dfn> option for using a own logger</li>
 * <li><dfn>MACHINELEARNING_FILES</dfn> adds the support for file reading and writing (default CSV). Special file support can be set with the following flags<ul>
 * <li><dfn>MACHINELEARNING_FILES_HDF</dfn> Hierarchical Data Format support</li>
 * </ul></li>
 * <li><dfn>MACHINELEARNING_SYMBOLICMATH</dfn> flag for using GiNaC library for creating symbolic expression (eg. gradient descent)</li>
 * <li><dfn>MACHINELEARNING_SOURCES</dfn> compiles sources in that way, that e.g. NNTP / Wikipedia data can be read directly<ul>
 * <li><dfn>MACHINELEARNING_SOURCES_TWITTER</dfn> twitter support</li>
 * </ul></li>
 * <li><dfn>MACHINELEARNING_MPI</dfn> enable MPI Support for the toolbox (requires Boost MPI support)</li>
 * </ul>
 * The following compiler commands should / must be set
 * <ul>
 * <li><dfn>NDEBUG</dfn> for disabling Boost and local debugging</li>
 * <li><dfn>BOOST_UBLAS_NDEBUG</dfn> for disabling Boost Ublas support</li>
 * <li><dfn>BOOST_NUMERIC_BINDINGS_BLAS_CBLAS</dfn> add CBLAS / LAPACK support for the Boost Bindings (need not be set under MSYS / MinGW)</li>
 * <li><dfn>BOOST_FILESYSTEM_NO_DEPRECATED</dfn> disabled deprecated code</li>
 * </ul>
 *
 *
 * @subsection scons Scons Build Environment
 * The framework supports Scons for compiling. The script reads the environment (Linux, OS X & Microsoft) and uses the
 * plattform specified options for compiling the sources [see options and default values with <dfn>scons -Q -h</dfn>]. On
 * default the script tries to detect the distcc environment and try to use it
 * <ul>
 * <li><dfn>withrandomdevice</dfn> adds the compilerflag for random device support</li>
 * <li><dfn>withmpi</dfn> adds the compilerflag for cluster / MPI support (on the target <dfn>librarybuild</dfn> MPI support is compiled into the Boost)</li>
 * <li><dfn>withmultilanguage</dfn> adds the multilanguage support with gettext</li>
 * <li><dfn>withsources</dfn> support for the namespace machinelearning::tools::sources</li>
 * <li><dfn>withfiles</dfn> support for the namespace machinelearning::tools::files</li>
 * <li><dfn>withlogger</dfn> compiles a own logger class within the framework</li>
 * <li><dfn>withsymbolicmath</dfn> support for symbolic math (eg: machinelearning::functionaloptimization::gradientdescent )</li>
 * </ul><ul>
 * <li><dfn>buildtype</dfn> build type [allowed valus: debug | release, default value is set to release]</li>
 * <li><dfn>uselocallibrary</dfn> uses only the libraries which are stores within the library directory</li>
 * <li><dfn>jnipath</dfn> optional path to the <dfn>jni.h</dfn> file</li>
 * <li><dfn>usedistcc</dfn> the build process forces distcc on building</li>
 * <li><dfn>usecolorcompiler</dfn> enable / disable searching for color compiler</li>
 * <li><dfn>copylibrary</dfn> copy the dynamic libraries into the build directory</li>
 * <li><dfn>atlaslink</dfn> link option for the atlas library [allowed values: multi | single] </li>
 * <li><dfn>winversion</dfn> Windows version (only used on MinGW / MinGW) [allowed values: win7 | srv2008 | vista | srv2003sp1 | xpsp2 | srv2003 | xp | w2000]</li>
 * </ul>
 * Option flag, that are used during building the library package:
 * <ul>
 * <li><dfn>skiplibrary</dfn> disables / skips the automatic build of a library [allowd values: all, none, lapack, boost, hdf, ginac, json, xml]</li> 
 * <li><dfn>zipsupport</dfn> enable / disable locale zip support on the Boost. BZip2 and GZip library can be downloaded</li>
 * <li><dfn>boostbuild</dfn> the Boost build options sets the required Boost libraries on the build process:
 * <ul>
 *      <li><dfn>required</dfn> builds only the required library (no additional support like logger, Wikipedia)</li>
 *      <li><dfn>requiredoptional</dfn> builds the libraries except the librarie, that are needed by the examples</li>
 *      <li><dfn>full</dfn> builds all libraries</li>
 * </ul>
 * <li><dfn>atlasblaspointerwidth</dfn> parameter for setting the ATLAS / GotoBlas pointer width during running the library build target (default empty for system default)</li>
 * <li><dfn>atlasversion</dfn> type of the atlas version. The default option uses the latest developer version, otherwise the latest stable version is used [allowed values: devel | stable]</li>
 * <li><dfn>jsoncppversion</dfn> type of the Json-Cpp version. The default option uses the latest developer version, otherwise the latest stable version is used [allowed values: devel | stable]</li>
 * The MPI option (<dfn>withmpi</dfn>) is independed of this options.
 * </ul>
 * The <dfn>SConstruct</dfn> script, which is the entry point to the compiler calls, is stored in the framework main directory. Under the <dfn>buildenvironment</dfn> directory all platform / environment specialized 
 * scripts are stored. All build scripts are named <dfn>build.py</dfn> and stored within the subdirectories. The script tries to find a color compiler (a compiler with the prefix <dfn>color</dfn> or <dfn>color-</dfn>
 * is searched and if found used)
 *
 *
 * @subsection targets Buildtargets
 * The scons script supports different targets for building the examples. The call can be supplemented with the parameters above. Each subdirectory within the example directory
 * show a build target, but some targets will be build first if different parameters are set
 * <ul>
 * <li><dfn>documentation</dfn> create the documentation with Doxygen (Doxygen must be installed and be reached within the path)</li>
 * <li><dfn>librarydownload</dfn> downloads each library</li>
 * <li><dfn>librarybuild</dfn> compiles each library with the default option. Compiled libraries are installed under the subdirectory <dfn>library/build_*</dfn>.</li>
 * <li><dfn>language</dfn> creates the language files / adds new messages to the existing files (xgettext must be called, so it should be within the path)</li>
 * <li><dfn>clean / -c</dfn> target that cleans framework files</li>
 * </ul><ul>
 * <li><dfn>sources</dfn> sources are eg. Wikipedia, NNTP, but the parameter <dfn>withsources</dfn> must be set for compiling and for the cloud example the files parameter must be set additionally</li>
 * <li><dfn>clustering</dfn> this target build all clustering algorithms, but the <dfn>withfiles</dfn> parameter must be set</li>
 * <li><dfn>classifier</dfn> this target build all classifier algorithms, but the <dfn>withfiles</dfn> parameter must be set</li>
 * <li><dfn>reducing</dfn> this target build all dimension reduce algorithms, but the <dfn>withfiles</dfn> parameter must be set</li>
 * <li><dfn>distance</dfn> this target build all distance algorithms, but the <dfn>withfiles</dfn> parameter must be set</li>
 * <li><dfn>other</dfn> this target build all other examples, <dfn>withfiles</dfn> options must be set, <dfn>withsources</dfn> can be set (includes nntp and wikipedia examples) and optional 
 * <dfn>withmpi</dfn> </li>
 * <li><dfn>ga</dfn> target for building genetic algorithms</li>
 * </ul><ul>
 * <li><dfn>java</dfn> create the the C/C++ stub files of each Java class, create the shared library and add all to the Jar file. With the system environment variable (<dfn>MACHINELEARNING_DLL_OVERWRITE</dfn>
 * on java run (option flag <dfn>-D</dfn>), the DLLs are written on each call to the temporary directory)</li>
 * <li><dfn>javaclustering</dfn> build the java examples of the clustering subpackage</li>
 * <li><dfn>javareduce</dfn> build the java examples of the reducing algorithms (java library jar file must build first and stored under the build directory)</li>
 * <li><dfn>javatools</dfn> build the java examples of the util subpackage</li>
 * </ul>
 *
 *
 * @section ex Examples
 * <ul>
 * <li>@subpage frameworkassert</li>
 * <li>@subpage lang</li>
 * <li>@subpage classifier</li>
 * <li>@subpage clustering</li>
 * <li>@subpage sources</li>
 * <li>@subpage distances</li>
 * <li>@subpage dimreduce</li>
 * <li>@subpage files</li>
 * <li>@subpage logger</li>
 * <li>@subpage other</li>
 * <li>@subpage java</li>
 * </ul>
 *
 *
 *
 * @page frameworkassert Framework Asserts
 * The framework supports an own assert method. The asserts can be defined via preprocessor call and support the framework logger, so each assert is logged. With the option
 * <dfn>MACHINELEARNING_NDEBUG</dfn> the assert values can be disabled. An assert statement can be used with
 * @code
    MACHINELEARNING_ASSERT( expression, message )
 * @endcode
 * If the <dfn>expression</dfn> is set to false the assert is called with <dfn>message</dfn>. Filename and line number are added automatically.
 *
 *
 *
 * @page classifier Example Classifier
 * The classifier algorithms are tempalte classes.
 * @section lazy Lazy Learner
 * @include examples/classifier/lazy.cpp
 *
 *
 *
 * @page clustering Example Clustering
 * The clustering classes are templates classes, there are two main structures <i>supervised</i> and <i>nonsupervised</i>. The namespaces are named in the same way machinelearning::clustering::nonsupervised or 
 * machinelearning::clustering::supervised. All classes are subclasses of their supervised / unsupervised base class. Especially the supervised classes have two template parameters, first for the data and second
 * for their labels, the nonsupervised uses only one template parameter. The examples show the different using.
 *
 * @section kmeans k-Means
 * @include examples/clustering/kmeans.cpp
 *
 * @section ng Neural Gas
 * @include examples/clustering/neuralgas.cpp
 *
 * @section png Patch Neural Gas
 * @include examples/clustering/patch_neuralgas.cpp
 *
 * @section rng Relational Neural Gas
 * @include examples/clustering/relational_neuralgas.cpp
 *
 * @section rlvq Relevance Learning Vector Quantization (RLVQ)
 * @include examples/clustering/rlvq.cpp
 *
 * @section spcl Spectral Clustering
 * @include examples/clustering/spectral.cpp
 *
 *
 *
 * @page dimreduce Example Dimensionreduce
 * The dimension reducing classes are in the namespace machinelearning::dimensionreduce::nonsupervised and machinelearning::dimensionreduce::supervised can be used
 * for dimension reduction. Supervised uses two template parameter first one for datatype, second one for labeling. The nonsupervised is used only one parameter
 * (for datatype). The examples show the different option
 *
 * @section pca Principle Component Analysis (PCA)
 * @include examples/reducing/pca.cpp
 *
 * @section lda Linear Discriminant Analysis (LDA)
 * @include examples/reducing/lda.cpp
 *
 * @section mds Multidimensional Scaling (MDS)
 * @include examples/reducing/mds.cpp
 *
 * @section lle Local Linear Embedding (LLE)
 * @code
 * @endcode
 *
 *
 *
 * @page distances Example Distances
 * The namespace machinelearning::distances holds all types of distances. Every distance function is a subclass of <i>distance</i> and calculates distances values for vector- and matrixdata.
 * The class must be implementated as a template class and must hold some special functions for using the distance operation. In the namespace is also the ncd-class that creates a
 * symmetric/asymmetric dissimilarity matrix of string- or filedata with the <i>normalized compression distance</i>, that based on an approximation of the the Kolmogorov complexity. The
 * example show how to use these classes.
 *
 * @section ncd Normalize Compression Distance (NCD)
 * @include examples/distance/ncd.cpp
 *
 *
 *
 * @page sources Example Data Sources
 * The toolbox implements some structures for geting datasets. The compile option <dfn>MACHINELEARNING_SOURCES</dfn> must be set for compiling the namespace
 * machinelearning::tools::sources .
 *
 * @section nntp Newsgroups
 * The class allows to browse newsgroups in a naitve way for extraction the message content. The class throws different exception if there
 * are connection or stream errors during sending or receiving data (see exceptions within the class).
 * @include examples/sources/newsgroup.cpp
 *
 * @section wiki Wikipedia
 * The wikipedia class can be used for read the article data of wikipedia articles in different languages
 * @include examples/sources/wikipedia.cpp
 *
 * @section twitter Twitter
 * The twitter class can be used for reading tweet data with different search tags
 * @include examples/sources/twitter.cpp
 *
 * @section cloud Cloud
 * The cloud class creates a multimodal n-dimensional data set with normal distribution. The n-dimensional cube is sampled in equidistant
 * steps and on the cross-points a normal distribution is created.
 * @include examples/sources/cloud.cpp
 * 
 *
 *
 * @page logger Examples Logger
 * Within the toolbox is a logger class which implements a thread-safe and optional MPI logger. The logger create a singleton object that create
 * a file access for writing messages. The MPI component sends all messages with non-blocking communication to the CPU 0. See in the logger class
 * for log states, which must be used for writing the messages.
 * @section normal Normal Use
 * @code
    // create the logger singleton instance
    tools::logger::createInstance();
 
    // sets the log level for writing messages 
    tools::logger::getInstance()->setLevel( tools::logger::info );
 
    // creates a message
    tools::logger::getInstance()->write( tools::logger::warn, "test message" );
 
    // shows the filename in which the messages are write down
    std::cout << tools::logger::getInstance()->getFilename() << std::endl;
 
    // release the logger instance
    tools::logger::releaseInstance();
 * @endcode
 *
 * @section mpi MPI Use
 * <strong>The MPI library must be compiled with thread-support and must be initialized manually</strong>
 * @code
    MPI::Init_thread( argc, argv, MPI_THREAD_SERIALIZED )
    boost::mpi::communicator l_mpi;
 
    // create the logger singleton instance
    tools::logger::createInstance();
 
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
    MPI::Finalize();
 
    // release the logger instance
    tools::logger::releaseInstance();
 * @endcode
 *
 *
 *
 * @page files Example File Support
 * The toolbox supports different file formats. The compile option <dfn>MACHINELEARNING_FILES</dfn> must be set for
 * using the support and all components are within the namespace machinelearning::tools::files
 * @section csv Comma Separated Values (CSV)
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
 * @section hdf Hierarchical Data Format (HDF)
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
 * @page lang Multilanguage Support
 * The toolbox uses <dfn>gettext</dfn> to translate the messages. The source code must include the language header file and all messages,
 * which should be supported multilanguage must be written in this way <dfn>_("messages")</dfn>. The mesages will be collected with the
 * gettext tools and safed under <dfn>tools/language/&quot;language eg. de_DE.UTF-8&quot;/LC_MESSAGES/&quot;language eg de_DE&quot;.po</dfn>. The compiler flag
 * <dfn>MACHINELEARNING_MULTILANGUAGE</dfn> must be set for using the support. The message files can be edited with Poedit ( http://www.poedit.net/ ).
 * For using the language support, a call for setting the correct languages must be written wihin the main program:
 * @code
    tools::language::bind(<name of the language file, should be "ml">, <path to the language directory, shoudl be "./tools/language/">);
 * @endcode
 *
 *
 *
 * @page other Examples Other
 *
 * @section toc Table of contents
 * <ul>
 * <li>@ref mdsnntp</li>
 * <li>@ref mdsnntpmatlab</li>
 * <li>@ref mdswiki</li>
 * <li>@ref mdswikimatlab</li>
 * </ul>
 *
 * @section mdsnntp Distance analyse of newsgroups articles and visualization with MDS
 * The program collects some newsgroup articles (of the groups, that are set with --groups or --groups rand n, read n groups randomaly or the n biggest groups with --groups n). The articles are
 * filtered with the term frequency option and the filtered articles are used for creating a distance matrix with is projected via MDS and written to a HDF file. The program can compiled also
 * with MPI support, so each process get their own articles and matrix data. 
 * @include examples/other/mds_nntp.cpp
 *
 * @subsection mdsnntpmatlab Matlab code for plotting NNTP distance data
 * The Matlab code on the bottom can be used for reading the HDF file and create a colored plot with all data.
 * @code
     function plotnntp( pcfile )
         pmarkersize=5;
         
         % create colors
         textlabel  = hdf5read( pcfile, '/uniquegroup');
         label      = cell(size(textlabel,1),1);
         labelcolor = cell(size(textlabel,1),1);
         
         col        = jet(size(textlabel,1));
         %col       = hsv(size(textlabel,1));
         for i=1:size(labelcolor,1)
            label{i}      = char(textlabel(i).data);
            labelcolor{i} = col(i, :);
         end
         
         % we create for each label group a data matrix
         data       = hdf5read( pcfile, '/project');
         if (size(data,2) ~= 2) && (size(data,2) ~= 3)
            error('plot only with 2D or 3D');
         end
         
         datalabel  = hdf5read( pcfile, '/group');
         
         datacell   = cell(size(label,1),1);
         maxcount   = zeros(size(label,1),1);
         
         for i=1:size(textlabel,1)
            datacell{i} = zeros( size(data,1), size(data,2) );
         end
         
         for i=1:size(data,1)
            pos   = strmatch(char(datalabel(i).data), label, 'exact');
            point = datacell{pos};
         
            point(maxcount(pos)+1,:) = data(i,:);
         
            datacell{pos} = point;
            maxcount(pos) = maxcount(pos) + 1;
         end
         
         % remove non-existing datasets
         emptycell = maxcount == 0;
         for i=1:numel(emptycell)
            if emptycell(i)
                datacell(i)   = [];
                labelcolor(i) = [];
                label(i)      = [];
                maxcount(i)   = [];
            end
         end
         
         % create plot
         figure;
         grid on;
         hold on;
         
         phandle = zeros(numel(datacell),1);
         for i=1:numel(datacell)
         
            point = datacell{i};
            point = point(1:maxcount(i), :);
         
            if size(point,2) == 2
                phandle(i) = plot(point(:,1), point(:,2), '.', 'Color', labelcolor{i}, 'DisplayName', label{i}, MarkerSize',pmarkersize);
            else
                phandle(i) = plot3(point(:,1), point(:,2), point(:,3), '.', 'Color', labelcolor{i}, 'DisplayName', label{i}, 'MarkerSize',pmarkersize);
            end
         end
         
         legend(phandle);
 * @endcode
 *
 * @section mdswiki Distance analyse of Wikipedia articles and visualization with MDS
 * @include examples/other/mds_wikipedia.cpp
 *
 * @subsection mdswikimatlab Matlab code for plotting Wikipedia distance data
 * @code
     function plotwiki( pcfile, ptext)
     
         if nargin < 2 || isempty(ptext)
            ptext = true;
         end
         pmarkersize=5;
         
         % get data
         data  = hdf5read( pcfile, '/project');
         if (size(data,2) ~= 2) && (size(data,2) ~= 3)
            error('plot only with 2D or 3D');
         end
         label = hdf5read( pcfile, '/label');
         
         % create plot
         figure;
         grid on;
         hold on;
         
         if size(data,2) == 2
            plot(data(:,1), data(:,2), '.', 'MarkerSize',pmarkersize);
         else
            plot3(data(:,1), data(:,2), data(:,3), '.', 'MarkerSize',pmarkersize);
         end
         
         for i=1:size(data,1)
            if ptext
                if size(data,2) == 2
                    text( data(i,1)+0.005, data(i,2), char(label(i).data), 'FontSize', 8);
                else
                    text( data(i,1)+0.005, data(i,2), data(i,3), char(label(i).data), 'FontSize', 8);
                end
            end
         end
 * @endcode
 * 
 *
 *
 * @page java Examples Java
 * The Java examples can use with the <dfn>machinelearning.jar</dfn> only. So first the Jar must be build with the <dfn>javac</dfn> build target and stored under the <dfn>build</dfn> directory.
 * After that the Java targets can be build or the Jar file can be used for own projects. For using the Jar file in a own project, compile your classes with the Java compiler and add the
 * Jar to your classpath. Take a look to the examples to use the algorithms and data structures or see the documentation
 *
 * @section toc Table of contents
 * <ul>
 * <li>@ref javaeigen</li>
 * <li>@ref javasvd</li>
 * <li>@ref javarandom</li>
 * <li>@ref javapca</li>
 * <li>@ref javamds</li>
 * </ul>
 *
 * @section javaeigen Eigenvalues
 * @include examples/java/tools/eigen.java
 *
 * @section javasvd Singular Value Decomposition
 * @include examples/java/tools/svd.java
 *
 * @section javarandom Random Java Distribution
 * @include examples/java/tools/random.java
 *
 * @section javapca Principal Component Analysis (PCA)
 * @include examples/java/reducing/pca.java
 *
 * @section javamds Multidimensional Scaling (MDS)
 * @include examples/java/reducing/mds.java
 *
 *
 *
 * @file machinelearning.h main header for including in a project
 * @file machinelearning.cpp main implementation file of the project, that must be compiled and added to the compile targets
 *
 * @file classifier/classifier.h main header for all classifier structurs
 * @file classifier/classifier.hpp header for the abstract class implementation of the classifiers
 * @file classifier/lazylearner.hpp lazy learner algorithm
 *
 * @file clustering/clustering.h main header for all clustering algorithms
 * @file clustering/nonsupervised/clustering.hpp header for nonsupervised abstract clustering classes
 * @file clustering/nonsupervised/kmeans.hpp k-means implementation
 * @file clustering/nonsupervised/neuralgas.hpp neuralgas implemention for real vector space
 * @file clustering/nonsupervised/relational_neuralgas.hpp neuralgas implemention for distance / relational data
 * @file clustering/nonsupervised/spectralclustering.hpp implementation of the spectral clustering
 * @file clustering/supervised/clustering.hpp header for supervised abstract clustering classes
 * @file clustering/supervised/rlvq.hpp implementation of relevance vector quantization
 *
 * @file dimensionreduce/dimensionreduce.h main header of dimension reducing algorithms
 * @file dimensionreduce/nonsupervised/reduce.hpp  abstract class for nonsupervised dimension reducing classes
 * @file dimensionreduce/nonsupervised/lle.hpp local linear embedding implementation
 * @file dimensionreduce/nonsupervised/pca.hpp principal component analysis implementation
 * @file dimensionreduce/nonsupervised/mds.hpp multidimensional scaling implementation
 * @file dimensionreduce/supervised/reduce.hpp  abstract class for supervised dimension reducing classes
 * @file dimensionreduce/supervised/lda.hpp lineare discriminant analysis implementation
 * 
 * @file distances/distances.h main header for all distance structures
 * @file distances/distance.hpp abstract class for distance algorithms
 * @file distances/norm/euclid.hpp class for euclidian distances
 * @file distances/ncd.hpp implementation of the normalize compression distance
 *
 * @file errorhandling/exception.hpp header file for exceptions with implemention (forward declaration)
 * @file errorhandling/exception.implementation.hpp file with the exception implementation
 * @file errorhandling/assert.h header file for framework asserts
 *
 * @file functionoptimization/functionoptimization.h main header for function optimization
 * @file functionoptimization/gradientdescent.hpp gradient descent implementation
 *
 * @file geneticalgorithm/geneticalgorithm.h main header for all genetic algorithms
 * @file geneticalgorithm/population.hpp population class
 * @file geneticalgorithm/fitness/fitness.h main header file for all fitness classes
 * @file geneticalgorithm/fitness/fitness.hpp abstract class of the fitness function
 * @file geneticalgorithm/individual/individual.h main header file for all individual classes
 * @file geneticalgorithm/individual/individual.hpp abstract class of an individual
 * @file geneticalgorithm/individual/binaryindividual.hpp implementation of a binary individual
 * @file geneticalgorithm/crossover/crossover.h main header file for all crossover classes
 * @file geneticalgorithm/crossover/crossover.hpp abstract class of the crossover function
 * @file geneticalgorithm/crossover/kcrossover.hpp k-crossover implementation
 * @file geneticalgorithm/selection/selection.h main header file for all selection classes
 * @file geneticalgorithm/selection/selection.hpp abstract class of the selection function
 * @file geneticalgorithm/selection/roulettewheel.hpp class with roulette-wheel-selection
 * @file geneticalgorithm/selection/bestof.hpp abstract class with best-of-selection
 *
 * @file neighborhood/neighborhood.h main header for neighborhood structurs
 * @file neighborhood/neighborhood.hpp abstract class for neighborhood implementation
 * @file neighborhood/kapproximation.hpp k-approximation class
 * @file neighborhood/knn.hpp k-nearest-neighborhood implementation
 *
 * @file tools/tools.h main header for tools algorithms
 * @file tools/function.hpp different functions eg. numerical limit checking
 * @file tools/logger.hpp logger implementation (forward declaration)
 * @file tools/logger.implementation.hpp logger implementation
 * @file tools/lapack.hpp wrapper class for LAPack calls
 * @file tools/matrix.hpp implementation of matrix operations
 * @file tools/vector.hpp implementation of vector operations
 * @file tools/random.hpp random implementation 
 * @file tools/typeinfo.h implemention of the typeinfo interface
 *
 * @file tools/sources/sources.h main header for all sources
 * @file tools/sources/nntp.h NNTP client
 * @file tools/sources/wikipedia.h wikipedia client
 * @file tools/sources/twitter.h twitter support
 * @file tools/sources/cloud.hpp implementation of cloud datasets
 *
 * @file tools/language/language.h multilanguage includes
 * @file tools/language/bindings.h class for gettext calls
 * @file tools/language/iso639.h include with macro definition of the language codes
 *
 * @file tools/files/files.h main header for file structurs
 * @file tools/files/csv.hpp implementation for reading and writing csv files
 * @file tools/files/hdf.hpp implementation for reading and writing hdf files
 
 * @file textprocess/textprocess.h main header for text processing algorithms
 * @file textprocess/termfrequency.h class for creating a term frequency structur of input text
 * @file textprocess/stopwordreduction.h class for stopword reduction
 *
 * @file tools/iostreams/iostreams.h main header for iostreams includes
 * @file tools/iostreams/urlencoder.h encoder for url
 *
 **/

#ifndef __MACHINELEARNING_H
#define __MACHINELEARNING_H


/** main namespace of the framework
 * @todo change vector / datatypes for memory mapped files ( http://en.wikipedia.org/wiki/Mmap / http://en.wikipedia.org/wiki/Memory-mapped_file / http://www.boost.org/doc/libs/1_52_0/libs/iostreams/doc/classes/mapped_file.html )
 *
 * @todo split class declaration and implementation in different files for solving guard dependcy problems
 * @todo define testcases http://www.boost.org/doc/libs/1_51_0/libs/test/doc/html/index.html
 * @todo adding LUA ( http://en.wikipedia.org/wiki/Lua_(programming_language) ) support for dynamic programming ( http://en.wikipedia.org/wiki/Dynamic_programming )
 * @todo adding Twiddle Algorithm ( http://www.youtube.com/watch?v=2uQ2BSzDvXs )
 * @todo adding SFDP support (placment for lage graph data) http://www2.research.att.com/~yifanhu/SOFTWARE/SFDP/index.html / http://www2.research.att.com/~yifanhu/PUB/graph_draw_small.pdf
 * @todo adding GSL support http://www.gnu.org/software/gsl/
 * @todo adding http://en.wikipedia.org/wiki/Ford–Fulkerson_algorithm / http://en.wikipedia.org/wiki/Edmonds–Karp_algorithm / http://en.wikipedia.org/wiki/Dinic%27s_algorithm
 * @todo adding PAPI support http://icl.cs.utk.edu/papi/
 * @todo adding Cuda support http://thrust.github.com/
 * @todo adding NN sparse encoding http://en.wikipedia.org/wiki/Sparse_coding
 * @todo adding LSH for distance calculation ( http://en.wikipedia.org/wiki/Locality-sensitive_hashing )
 * @todo adding optimization algorithmus http://en.wikipedia.org/wiki/Levenberg–Marquardt_algorithm
 * @todo thinking about http://en.wikipedia.org/wiki/Conceptual_graph
 * @todo adding minimum spanning tree clustering http://zachjorgensen.com/ictai06.pdf
 * @todo adding stemming support ( http://en.wikipedia.org/wiki/Stemming )
 * @todo thinking about a SSE support in the framework http://en.wikipedia.org/wiki/SSE3
 * @todo adding SVM components ( http://www.csie.ntu.edu.tw/~cjlin/libsvm/ )
 * @todo adding GTML with http://en.wikipedia.org/wiki/Expectation–maximization_algorithm
 * @todo adding http://en.wikipedia.org/wiki/Apriori_algorithm
 * @todo adding http://en.wikipedia.org/wiki/Hierarchical_clustering
 * @todo adding http://en.wikipedia.org/wiki/DBSCAN
 * @todo adding http://en.wikipedia.org/wiki/OPTICS_algorithm
 * @todo adding http://en.wikipedia.org/wiki/Local_outlier_factor
 * @todo thinking about CILK support http://supertech.csail.mit.edu/cilk/
 * @todo adding simulated annealing (see GSL http://www.gnu.org/software/gsl/)
 * @todo adding some generic support for http://headmyshoulder.github.com/odeint-v2/
 * @todo adding voronoi tessellation (with CGAL http://http://www.cgal.org/ ) and other CGAL algorithms / http://en.wikipedia.org/wiki/Fortune%27s_algorithm
 * @todo adding support for SuperLU ( http://crd-legacy.lbl.gov/~xiaoye/SuperLU/ )
 * @todo create a PHP & Python modul ( use Swig )
 * @todo add unit tests with Google ( http://code.google.com/p/googletest/ )
 * @todo implementating graph algorithms (use boost graph): http://en.wikipedia.org/wiki/A*_search_algorithm / http://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm
 * @todo add celluar algorithms: http://en.wikipedia.org/wiki/Cellular_automaton
 * @todo add http://en.wikipedia.org/wiki/RANSAC
 * @todo thinking about additional datatypes http://en.wikipedia.org/wiki/GNU_Multiple_Precision_Arithmetic_Library
 **/
namespace machinelearning {}


#include "errorhandling/assert.h"
#include "errorhandling/exception.hpp"
#include "dimensionreduce/dimensionreduce.h"
#include "classifier/classifier.h"
#include "clustering/clustering.h"
#include "geneticalgorithm/geneticalgorithm.h"
#include "neighborhood/neighborhood.h"
#include "distances/distances.h"
#include "tools/tools.h"
#include "functionoptimization/functionoptimization.h"
#include "textprocess/textprocess.h"

#endif
