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
 * <li>Automatically Tuned Linear Algebra Software ( http://math-atlas.sourceforge.net/ ) [version newer than 3.9.40]</li>
 * <li>Boost ( http://www.boost.org/ ) with following components
 *     <ul>
 *         <li>IOStreams with ZLib and BZip2 support</li>
 *         <li>Thread support</li>
 *         <li>Math support</li>
 *         <li>Regular Expression support</li>
 *         <li>Filesystem support</li>
 *         <li>System support</li>
 *         <li><i>optional Random Device support</i></li>
 *         <li><i>optional MPI support</i></li>
 *     </ul>
 * </li>
 * <li>Boost Bindings (SVN http://svn.boost.org/svn/boost/sandbox/numeric_bindings )</li>
 * <li>GiNaC ( http://www.ginac.de/ ) with CLN ( http://www.ginac.de/CLN/ ) [version newer or equal 1.6]</li>
 * <li>Linear Algebra PACKage ( http://www.netlib.org/lapack/ ) [Version newer 3.3]</li>
 * <li><i>optional Hierarchical Data Format (HDF)</i> ( http://www.hdfgroup.org/ ) [version newer or equal 5.1.8.4]</li>
 * <li><i>optional Message-Passing-Interface-Support</i>
 *      <ul>
 *          <li>Open MPI ( http://www.open-mpi.org/ )</li>
 *          <li>MPICH2 ( http://www.mcs.anl.gov/research/projects/mpich2/ )</li>
 *          <li>Microsoft High Performance Computing Pack 2008 R2 SDK ( http://www.microsoft.com/downloads/en/details.aspx?FamilyID=bc671b22-f158-4a5f-828b-7a374b881172 )</li>
 *      </ul>
 * </li>
 * <li><i>optional GetText</i> ( http://www.gnu.org/software/gettext ) for including multilanguage support</li>
 * <li><i>optional LibXML2</i> ( http://xmlsoft.org/ ) (used by wikipedia support)</li>
 * <li><i>optional Scons</i> ( http://www.scons.org/ )</li>
 * <li><i>optional Doxygen</i> ( http://www.doxygen.org/ )</li>
 * </ul>
 * For manual installation of the library you can follow the short @subpage installationnotes
 *
 * @section def definition / style guide
 * <ul>
 * <li>the framework is only header-based so you don't need compile any files</li>
 * <li>data points should be matrix data and the matrix is row-orientated, so for K data points with every point dimension P, we have a K x P matrix (prototype matrices are equal)</li>
 * <li>all template classes / algorithms must have a floating point datatype on instantiation, because the datastructurs need this types for correct value calculating. The (base) classes use a static assertion for checking the value type</li>
 * <li>all compiler flags start with <dfn>MACHINELEARNING_</dfn></li>
 * <li>all include defines for header files are set in this way <dfn>MACHINELEARNING_DIRECTORY_FILE_H/HPP</dfn>
 * <li>all structures are in the namespace <dfn>machinelearning</dfn></li>
 * <li>all messages should get the structure <dfn>_("<message>")</dfn>, because the underline prefix is the support for different languages</li>
 * </ul>
 *
 * The source code uses some style guides options:
 * <ul>
 * <li>all variables use a prefix for their scope (derivated from <a href="http://en.wikipedia.org/wiki/Hungarian_notation">hungarian notation</a>): 
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
 * <li>the svn keyword LastChangeDate should be set for all files</li>
 * <li>all parameters should be a call-by-reference parameter</li>
 * <li>matrix structures should be row-oriantated, so loops over the elements are first over the rows and columns on the second / inner loop</li>
 * <li>template parameter should be asserted for the correct type</li>
 * </ul>
 *
 * @section compileroptions compiler option
 * toolbox compilerflags
 * <ul>
 * <li><dfn>MACHINELEARNING_RANDOMDEVICE</dfn> for using the Boost Device Random support (requires Boost Random Device Support), otherwise a Mersenne Twister is used</li>
 * <li><dfn>MACHINELEARNING_MULTILANGUAGE</dfn> option for compiling the framework with multilanguage support (uses gettext)</li>
 * <li><dfn>MACHINELEARNING_FILES</dfn> adds the support for file reading and writing (default CSV). Special file support can be set with the following flags<ul>
 * <li><dfn>MACHINELEARNING_FILES_HDF</dfn> Hierarchical Data Format support</li>
 * </ul></li>
 * <li><dfn>MACHINELEARNING_SOURCES</dfn> compiles sources in that way, that e.g. NNTP / Wikipedia data can be read directly</li>
 * <li><dfn>MACHINELEARNING_MPI</dfn> enable MPI Support for the toolbox (requires Boost MPI support)</li>
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
 * <li><dfn>--create-documentation</dfn> creates the Doxygen documentation (Doxygen must be called, so it should be within the path)</li>
 * <li><dfn>--create-language</dfn> creates the language files / adds new messages to the existing files (xgettext must be called, so it should be within the path)</li>
 * <li><dfn>--compile-language</dfn> compiles all language files (msgfmt must be called, so it should be within the path)</li>
 * </ul>
 * For compiling the test cpp files in the test directory the following option can be set (Hierarchical Data Format is needed)
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
 * <li><dfn>other</dfn> this target build all other examples, following options must be set: <dfn>--with-files</dfn>, <dfn>--with-sources</dfn> (optional <dfn>--with-mpi</dfn>) </li>
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
 * <li>@subpage other</li>
 * </ul>
 *
 *
 *
 * @page installationnotes Installation Notes
 * All configure scripts have a <dfn>--prefix=</dfn> option for setting a target installation directory. It is recommand to use this option for seperating the manually installation
 * in opinion to the system libraries.
 *
 * @section nix Linux / Mac OS X
 * In both OS (Linux & Mac OS X) the libraries can be build with the following steps within the extracted source directory:
 * @code
    ./configure
    make
    make install
   @endcode
   For the single packages are some notes:
 *
 * @subsection nixzip BZip2 and ZLib support 
 * Within the most systems <a href="http://www.bzip.org/">BZip2 sources</a> and <a href="http://zlib.net/">ZLib sources</a> are installed. Sources can be installed from the package
 * tree or manually from the source packages.
 *
 * @subsection nixboost Boost
 * First bJam must be build in the command line (terminal), so in the extracted source path the command
 * @code bootstrap.sh @endcode
 * must be run. After that Boost can be build with (The MPI support can be enabled with <dfn>--with-mpi</dfn>, but it requires MPI sources and libraries. The configuration for MPI use can be found on
 * <dfn>www.boost.org/doc/libs/-release number-/doc/html/mpi.html</dfn>)
 * @code bjam --with-filesystem --with-math --with-random --with-regex --with-thread --with-system --with-serialization --with-iostreams threading=multi runtime-link=shared variant=release toolset=gcc|darwin install @endcode
 * The <dfn>toolset</dfn> option must be:
 * <ul>
 * <li><dfn>gcc</dfn> for unix systems</li>
 * <li><dfn>darwin</dfn> for Mac OS X</li>
 * </ul>
 * The numerical bindings for LAPack are needed, so the SVN must be checked out.
 *
 * @subsection nixhdf HDF
 * The HDF libraray must be build with C++ support, so the configure call must be:
 * @code configure --enable-cxx @endcode
 *
 * @subsection nixatlas Atlas with full LAPack
 * The framework need a full LAPack support, so a GFortran compiler is needed. The fortran compiler can be downloaded on http://gcc.gnu.org/wiki/GFortranBinaries . The configure call should be
 * @code configure --dylibs --with-netlib-lapack-tarfile=-path to lapack.tgz- @endcode
 * In some cases the pointer bitwidth (<dfn>-b</dfn>) must be set. The flag <dfn>--nof77</dfn> should not be set, because some LAPack routines are not built. The configure call must be run into
 * a temporary directory, so in the first step the temporary directory must be created (information within the Atlas errata)
 *
 *
 * <hr>
 * @section windows Microsoft Windows
 * Windows does not supported all depending libraries. In this steps are some information about the installation depending libraries. All compiler tools are used under <a href="http://www.cygwin.com/">
 * Cygwin</a>. You need <a href="http://www.python.org">Python for Windows (MSI installer)</a> for using Scons. The sequence of the build process should be carried out as indicated here.
 *
 * @subsection winzip BZip2 and ZLib support
 * Boost IOStream and HDF require BZip2 and ZLib support. Information for IOStream is found on <dfn>www.boost.org/doc/libs/-release number-/libs/iostreams/doc/installation.html</dfn>.
 * <a href="http://www.bzip.org/">BZip2 sources</a> can compiled with Visual Studio (see readme). After extracting the source BZip2 can compiled on the Visual Studio Command Line
 * with (it is recommend to use an <dfn>include</dfn> directory for the header files and a <dfn>lib</dfn> directory for the libraries)
 * @code nmake -f makefile.msc @endcode 
 * After compiling all files in the directory can be deleted except
 * <ul>
 * <li><dfn>bzip2.exe</dfn></li>
 * <li><dfn>bzip2.exe.manifest</dfn></li>
 * <li><dfn>bzip2recover.exe</dfn></li>
 * <li><dfn>bzip2recover.exe.manifest</dfn></li>
 * <li><dfn>bzlib.h</dfn></li>
 * <li><dfn>libbz2.lib</dfn></li>
 * <li><dfn>msvcr90.dll</dfn></li>
 * <li><dfn>msvcr90d.dll</dfn></li>
 * </ul>
 * <a href="http://zlib.net/">ZLib sources</a> can be compiled also with
 * @code nmake -f win32/Makefile.msc @endcode 
 * The following files are needed
 * <ul>
 * <li><dfn>crc32.h</dfn></li>
 * <li><dfn>deflate.h</dfn></li>
 * <li><dfn>inftrees.h</dfn></li>
 * <li><dfn>trees.h</dfn></li>
 * <li><dfn>gzguts.h</dfn></li>
 * <li><dfn>zconf.h</dfn></li>
 * <li><dfn>inffast.h</dfn></li>
 * <li><dfn>zlib.h</dfn></li>
 * <li><dfn>inffixed.h</dfn></li>
 * <li><dfn>zutil.h</dfn></li>
 * <li><dfn>inflate.h</dfn></li>
 * <li><dfn>zdll.lib</dfn></li>
 * <li><dfn>zlib.lib</dfn></li>
 * <li><dfn>zlib1.dll.manifest</dfn></li>
 * </ul>
 * The Boost IOStream can be compiled only with the environmental variables for BZip2 and ZLib:
 * <ul>
 * <li><dfn>BZIP2_BINARY = libbz2</dfn></li>
 * <li><dfn>BZIP2_INCLUDE = installation directory</dfn></li>
 * <li><dfn>BZIP2_LIBPATH = library directory</dfn></li>
 * <li><dfn>ZLIB_BINARY = zdll</dfn></li>
 * <li><dfn>ZLIB_INCLUDE = installation directory</dfn></li>
 * <li><dfn>ZLIB_LIBPATH = library directory</dfn></li>
 * </ul>
 *
 * @subsection winboost Boost
 * First bJam must be build in the Visual Studio Command Line, so in the extracted source path the command
 * @code bootstrap.bat @endcode
 * must be run. After that Boost can be build with (The MPI support can be enabled with <dfn>--with-mpi</dfn>, but it requires MPI sources and libraries. The configuration for MPI use can be found on
 * <dfn>www.boost.org/doc/libs/-release number-/doc/html/mpi.html</dfn>)
 * @code bjam --with-filesystem --with-math --with-random --with-regex --with-thread --with-system --with-serialization --with-iostreams threading=multi runtime-link=shared variant=release toolset=msvc install @endcode
 * The numerical bindings for LAPack are needed, so the SVN must be checked out.
 *
 * @subsection winhdf HDF
 * It is recommand to use <a href="http://www.cmake.org">CMake</a> to perfom the building and installing. After extracting the files in the sources directory the following command must be
 * called within the Visual Studio Command Line (for using ZLib or SLib support see HDF install instructions):
 * @code cmake-gui . @endcode
 * Generate the compiler option and set the following option
 * <ul>
 * <li><dfn>BUILD_SHARED_LIBS</dfn> enabled</li>
 * <li><dfn>HDF5-BUILD_CPP_LIB</dfn> enabled</li>
 * <li><dfn>CMAKE_ARCHIVE_OUTPUT_DIRECTORY</dfn> set to a build directory</li>
 * <li><dfn>CMAKE_LIBRARY_OUTPUT_DIRECTORY</dfn> set to a build directory</li>
 * <li><dfn>CMAKE_RUNTIME_OUTPUT_DIRECTORY</dfn> set to a build directory</li>
 * <li><dfn>CMAKE_INSTALL_PREFIX</dfn> set to the installation / target directory</li>
 * </ul>
 * After generating the configuration files, the <dfn>ALL_BUILD.vcproj</dfn> must be opened. Set the build type to <dfn>Release</dfn> and create the <dfn>ALL_BUILD</dfn> target, after compiling
 * the <dfn>INSTALL</dfn> target must be run.
 *
 * @subsection winatlas Atlas with full LAPack
 * Cygwin must first installed with Gcc, G++ and GFortran. It is recommend that you take a look into the Atlas errata for more information about installing Atlas under Windows. It is recommend to move the 
 * Atlas source and the <dfn>lapack.tgz</dfn> into the Cygwin users home directory (eg. sources are in <dfn>~/ATLAS</dfn> and lapack in <dfn>~/lapack.tgz</dfn>).
 * After that create a new directory in the home (eg. <dfn>mkdir ~/tmp</dfn>) and step into. Than call the configure script with (take a look to the flags <dfn>-b 32|64</dfn> for using 32 or 64 bit)
 * @code ~/ATLAS/configure --dylibs -b 32 --with-netlib-lapack-tarfile=~/lapack.tgz  @endcode
 *
 * @subsection winginac GiNac with CLN
 * Both libraries must be compiled with Cygwin (see @ref winatlas). Extract the CLN source first, switch in the Cygwin command line into the source directory and run the command
 * @code ./configure @endcode
 * If you would like to set the installation directory use. After configure run
 * @code make @endcode 
 * and
 * @code make install @endcode 
 * GiNaC requires the CLN, which can be accessed with <a href="http://www.freedesktop.org/wiki/Software/pkg-config">PKG-Config</a> or environmental variables can be set with:
 * @code 
    export CLN_LIBS="-L-path to CLN-/lib -lcln"
    export CLN_CFLAGS="-I-path to CLN-/include"
 * @endcode
 * After setting the variables GiNaC can be build with the same commands like CLN.
 *
 * @subsection winxml LibXML2
 * The XML2 library can be build with the same steps like GiNaC. The webpage references binary packages too.
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
 * @section png patch neural gas
 * @include examples/clustering/patch_neuralgas.cpp
 *
 * @section rng relational neural gas
 * @include examples/clustering/relational_neuralgas.cpp
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
 * The toolbox implements some structures for geting datasets. The compile option <dfn>MACHINELEARNING_SOURCES</dfn> must be set for compiling the namespace
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
    MPI::Finalize();
 * @endcode
 *
 *
 *
 * @page files file support
 * The toolbox supports different file formats. The compile option <dfn>MACHINELEARNING_FILES</dfn> must be set for
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
 * <dfn>MACHINELEARNING_MULTILANGUAGE</dfn> must be set for using the support. The message files can be edited with Poedit ( http://www.poedit.net/ ).
 * For using the language support, a call for setting the correct languages must be written wihin the main program:
 * @code
    tools::language::bind(<name of the language file, should be "ml">, <path to the language directory, shoudl be "./tools/language/">);
 * @endcode
 *
 *
 *
 * @page other other examples
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
                datacell(i)	  = [];
                labelcolor(i) = [];
                label(i)	  = [];
                maxcount(i)	  = [];
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
 *
 * @file machinelearning.h main header for including in a project
 *
 * @file examples/distance/ncd.cpp testprogram for the normalized compression distance
 * @file examples/clustering/neuralgas.cpp testprogram for the neural gas algorithm
 * @file examples/clustering/patch_neuralgas.cpp testprogram for the patch neural gas algorithm
 * @file examples/clustering/relational_neuralgas.cpp testprogram for the relational neural gas algorithm
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
 * @file examples/other/mds_nntp.cpp program for reading newsgroup articles, stopword reduction, distance calculating and MDS plotting
 * @file examples/other/mds_wikipedia.cpp program for reading Wikipedia articles, stopword reduction, distance calculating and MDS plotting
 *
 * @file classifier/classifier.h main header for all classifier structurs
 * @file classifier/classifier.hpp header for the abstract class implementation of the classifiers
 * @file classifier/lazylearner.hpp lazy learner algorithm
 *
 * @file clustering/clustering.h main header for all clustering algorithms
 * @file clustering/clustering.hpp header for abstract clustering classes
 * @file clustering/nonsupervised/kmeans.hpp k-means implementation
 * @file clustering/nonsupervised/neuralgas.hpp neuralgas implemention for real vector space
 * @file clustering/nonsupervised/relational_neuralgas.hpp neuralgas implemention for distance / relational data
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
 
 * @file textprocess/textprocess.h main header for text processing algorithms
 * @file textprocess/termfrequency.h class for creating a term frequency structur of input text
 * @file textprocess/stopwordreduction.h class for stopword reduction
 *
 *
 * @todo thinking about a SSE support in the framework http://en.wikipedia.org/wiki/SSE3
 * @todo thinking about CILK support http://supertech.csail.mit.edu/cilk/
 * @todo adding genetic algorithms
 * @todo adding simulated annealing
**/

#ifndef MACHINELEARNING_H
#define MACHINELEARNING_H


/** main namespace of the framework
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
#include "textprocess/textprocess.h"

#endif