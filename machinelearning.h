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
 * <li>Automatically Tuned Linear Algebra Software ( http://math-atlas.sourceforge.net/ ) [version newer than 3.9.40]</li>
 * <li>Boost ( http://www.boost.org/ ) with following components
 *     <ul>
 *         <li>IOStreams with ZLib and BZip2 support</li>
 *         <li>Exception support</li>
 *         <li>Date-Time support</li>
 *         <li>Math support</li>
 *         <li>Program Options</li>
 *         <li>Regular Expression support</li>
 *         <li>Filesystem support</li>
 *         <li>System support</li>
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
 * <li><i>optional Hierarchical Data Format (HDF)</i> ( http://www.hdfgroup.org/ ) [version newer or equal 5.1.8.4]</li>
 * <li><i>optional Message-Passing-Interface-Support</i>
 *      <ul>
 *          <li>Open MPI ( http://www.open-mpi.org/ )</li>
 *          <li>MPICH2 ( http://www.mcs.anl.gov/research/projects/mpich2/ )</li>
 *          <li>Microsoft High Performance Computing Pack SDK ( http://www.microsoft.com/download/en/search.aspx?q=HPC%20Pack%20SDK )</li>
 *      </ul>
 * </li>
 * <li><i>optional GiNaC</i> ( http://www.ginac.de/ ) with CLN ( http://www.ginac.de/CLN/ ) [version newer or equal 1.6]</li>
 * <li><i>optional GetText</i> ( http://www.gnu.org/software/gettext/ ) (used by multilanguage support)</li>
 * <li><i>optional LibXML2</i> ( http://xmlsoft.org/ ) (used by wikipedia support)</li>
 * <li><i>optional LibJSONCPP</i> ( http://sourceforge.net/projects/jsoncpp/ ) (used by twitter support)</li>
 * <li><i>optional Java Runtime Environment (JRE) / Java Developer Kit (JDK)</i> ( http://www.java.com/ / http://www.oracle.com/technetwork/java/javase/downloads/index.html ) </li>
 * <li><i>optional Scons</i> ( http://www.scons.org/ )</li>
 * <li><i>optional Swig</i> ( http://www.swig.org/ )</li>
 * <li><i>optional Doxygen</i> ( http://www.doxygen.org/ ) with Graphviz support ( http://www.graphviz.org )</li>
 * </ul>
 * For manual installation of the library you can follow the short @subpage installationnotes / A default library installation can be created with the command <dfn>scons librarybuild</dfn>
 * with the optional ATLAS building parameters. After compiling the directory <dfn>library/build</dfn> must be moved to a directory in that all libraries should be stored, because
 * a clean-target removes the <dfn>library/build</dfn> directory. The default installation need the make-tools (make), gcc / g++ (C and C++ compiler) and gfortran (Fortran compiler).
 * The framework was tested under: Mac OS X Leopard (10.5), Mac OS X Snow Leopard (10.6), Mac OS X Lion (10.7), Ubuntu Lucid Lynx (10.04), Gentoo Linux (stable / unstable) and Microsoft
 * Windows 7 (with Cygwin). The main header file <dfn>machinelearning.h</dfn> must be added to the <dfn>CPPPATH</dfn> variable with the prefix directory <dfn>machinelearning</dfn>, if the examples
 * external libraries (Java / Python ...) should be build.
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
 * <li>the svn keyword LastChangeDate should be set for all files</li>
 * <li>all parameters should be a call-by-reference parameter</li>
 * <li>matrix structures should be row-oriantated, so loops over the elements are first over the rows and columns on the second / inner loop</li>
 * <li>template parameter should be asserted for the correct type</li>
 * <li>header inclusion (forward declaration) and implementation are split into two files (if needed), header file (.h/.hpp) and the implementation file with name <dfn>name.implementation.hpp/.h/.cpp</dfn></li>
 * </ul>
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
 * <li><dfn>BOOST_NUMERIC_BINDINGS_BLAS_CBLAS</dfn> add CBLAS / LAPACK support for the Boost Bindings</li>
 * <li><dfn>BOOST_FILESYSTEM_NO_DEPRECATED</dfn> disabled deprecated code</li>
 * </ul>
 *
 * @subsection scons Scons Build Environment
 * The framework supports Scons for compiling. The script reads the environment (Linux, OS X & Microsoft) and uses the
 * plattform specified options for compiling the sources. For compiling the test cpp files in the test directory the following option can
 * be set (Hierarchical Data Format is needed) [see options and default values with <dfn>scons -Q -h</dfn>]
 * <ul>
 * <li><dfn>atlaslink</dfn> adds the option which ATLAS library is linked (multi = multithreadding [tatlas], single = singlethreadding [satlas])</li>
 * <li><dfn>staticlink</dfn> links all libraries static (default option yes), overrides <dfn>atlaslink</dfn> flag</li>
 * <li><dfn>math</dfn> compiler math option (sse3, sse, 387 for floating-point arithmetics) (used if optimazation is enabled)</li>
 * <li><dfn>withframeworkdebug</dfn> enables the <dfn>MACHINELEARNING_NDEBUG</dfn> flag</li>
 * <li><dfn>withrandomdevice</dfn> adds the compilerflag for random device support</li>
 * <li><dfn>withmpi</dfn> adds the compilerflag for cluster / MPI support (on the target <dfn>librarybuild</dfn> MPI support is compiled into the Boost)</li>
 * <li><dfn>withmultilanguage</dfn> adds the multilanguage support with gettext</li>
 * <li><dfn>withlogger</dfn> compiles a own logger class within the framework</li>
 * <li><dfn>withsources</dfn> support for the namespace machinelearning::tools::sources</li>
 * <li><dfn>withfiles</dfn> support for the namespace machinelearning::tools::files</li>
 * <li><dfn>withsymbolicmath</dfn> support for symbolic math (eg: machinelearning::functionaloptimization::gradientdescent )</li>
 * <li><dfn>withdebug</dfn> compile with debug symbols</li>
 * <li><dfn>withoptimize</dfn> optimization like O2</li>
 * <li><dfn>cputype</dfn> cpu type, see GCC documentation http://gcc.gnu.org/onlinedocs/gcc/i386-and-x86_002d64-Options.html [default: native]</li>
 * <li><dfn>showconfig</dfn> creates a printable output of the build configuration</li>
 * </ul><ul>
 * <li><dfn>atlaspointerwidth</dfn> parameter for setting the ATLAS pointer width during running the library build target (default empty for system default)</li>
 * <li><dfn>atlascputhrottle</dfn> enable / disable detection of ATLAS CPU throtteling during runngin the library build target (default disable)</li>
 * <li><dfn>skipbuild</dfn> disables / skips the automatic build of a library [allowd values: all, none, atlas, boost, hdf, ginac, json , xml (only available on Cygwin)]</li> 
 * <li><dfn>skipbuilderror</dfn> if an error occures during the building process, a commandline input for aborting or continuing is needed, with this option the input can be suppressed (default disable)</li>
 * </ul>
 * On Cygwin the option
 * <ul>
 * <li><dfn>winver</dfn> is set for the Windows version [default: win7, other allowed values: srv2008, vista, srv2003sp1, xpsp2, srv2003, xp, w2000)</li>
 * </ul>
 * The <dfn>SConstruct</dfn> script, which is the entry point to the compiler calls, is stored in the framework main directory. Under the <dfn>scons</dfn> directory all other script (helper, platform)
 * are stored. For each target a own <dfn>SConscript</dfn> script exists under the directory, which should be build. For each platform a own script is stored under <dfn>scons/platform</dfn> that is used for setting the
 * platform default compile option like linked libraries, pathes, ...
 *
 * @subsection targets Buildtargets
 * The scons script supports different targets for building the examples. The call can be supplemented with the parameters above. Each subdirectory within the example directory
 * show a build target, but some targets will be build first if different parameters are set
 * <ul>
 * <li><dfn>documentation</dfn> create the documentation with Doxygen (Doxygen must be installed and be reached within the path)</li>
 * <li><dfn>librarybuild</dfn> download the librarys and compile each library with the default option. Compiled libraries are installed under the subdirectory <dfn>library/build</dfn>.
 * Requirements for the automatic build installation process: <ul>
 * <li>tar, make, gfortran, gcc & g++ must be installed and within the search path</li>
 * <li>all files are downloaded under <dfn>library/</dfn>, each file can be replaced with another version, so the script downloads the packages only, if the file does not exists</li>
 * <li>the framework must be stored in a directory without any spaces, spaces create errors on the build process</li>
 * </ul>
 * <li><dfn>createlanguage</dfn> creates the language files / adds new messages to the existing files (xgettext must be called, so it should be within the path)</li>
 * <li><dfn>updatelanguage</dfn> compiles all language files (msgfmt must be called, so it should be within the path)</li>
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
 * <li><dfn>javac</dfn> create the the C/C++ stub files of each Java class, create the shared library and add all to the Jar file</li>
 * <li><dfn>javareduce</dfn> build the java examples of the reducing algorithms (java library jar file must build first and stored under the build directory)</li>
 * <li><dfn>javautil</dfn> build the java examples of the util subpackage</li>
 * </ul>
 *
 * @section ex Examples
 * <ul>
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
 * @page installationnotes Installation Notes
 * For building all libraries with a set of default options the target <dfn>librarybuild</dfn> with the parameters <dfn>atlaspointerwidth</dfn>, <dfn>atlascputhrottle</dfn>, <dfn>skipbuild</dfn>
 * and <dfn>skipbuilderror</dfn> can be used.<br/>
 * All configure scripts have a <dfn>--prefix=</dfn> option for setting a target installation directory. It is recommand to use this option for seperating the manually installation
 * in contrast to the system libraries. This steps discribe the manually compilation of each library, but you can use the devel packages of your distribution (and precompiled libraries).
 * This tutorial is a short excerpt for the install process only, so it is recommand, that you know the system details for installing. 
 *
 * @section nix Linux / Mac OS X
 * In both OS (Linux & Mac OS X) the libraries can be build with the following steps within the extracted source directory:
 * @code
    ./configure
    make
    make install
 * @endcode
 * For the single packages are some notes:
 *
 * @subsection nixzip BZip2 and ZLib support 
 * Within the most systems <a href="http://www.bzip.org/">BZip2 sources</a> and <a href="http://zlib.net/">ZLib sources</a> are installed. Sources can be installed from the package
 * tree or manually from the source packages.
 *
 * @subsection nixboost Boost
 * First bJam must be build in the command line (terminal), so in the extracted source path the command
 * @code bootstrap.sh @endcode
 * must be run. After that Boost can be build with (The MPI support can be enabled with <dfn>withmpi</dfn>, but it requires MPI sources and libraries. The configuration for MPI use can be found on
 * <dfn>www.boost.org/doc/libs/-release number-/doc/html/mpi.html</dfn>)
 * @code bjam --with-exception --with-filesystem --with-math --with-random --with-regex --with-date_time --with-thread --with-system --with-program_options --with-serialization --with-iostreams --disable-filesystem2 threading=multi runtime-link=shared variant=release toolset=gcc|darwin install @endcode
 * In newer Boost version (since 1.47.0) the <dfn>bjam</dfn> command is replaced with <dfn>b2</dfn>. The <dfn>toolset</dfn> option must be:
 * <ul>
 * <li><dfn>gcc</dfn> for unix systems (Cygwin too)</li>
 * <li><dfn>darwin</dfn> for Mac OS X</li>
 * </ul>
 * The numerical bindings for LAPack are needed, so the SVN direcotry http://svn.boost.org/svn/boost/sandbox/numeric_bindings must be checked out.
 *
 * @subsection nixhdf HDF
 * The HDF libraray must be build with C++ support:
 * @code configure --enable-cxx @endcode
 *
 * @subsection nixatlas Atlas with full LAPack
 * The framework need a full LAPack support, so a GFortran compiler is needed. The fortran compiler can be downloaded on http://gcc.gnu.org/wiki/GFortranBinaries or use the compiler within the distribution.
 * The configure call should be
 * @code configure --dylibs --with-netlib-lapack-tarfile=-path to lapack.tgz- @endcode
 * In some cases the pointer bitwidth (<dfn>-b</dfn>) must be set. The flag <dfn>--nof77</dfn> should not be set, because some LAPack routines are not built (a full LAPack support is required).
 * The configure call must be run into a temporary directory, so in the first step the temporary directory must be created. It is recommend that you take a look into the Atlas errata for more
 * information about installing Atlas under Windows (see http://math-atlas.sourceforge.net/errata.html ). 
 * @subsubsection nixsoname Linux library with soname
 * If you want build the Java packages with the Atlas and LAPack library, you must compile the library and after compiling you should change the line in <dfn>working directory/lib/Makefile</dfn>
 * @code
    (LD) $(LDFLAGS) -shared -soname $(LIBINSTdir)/$(outso) -o $(outso) \
 * @endcode
 * to
 * @code
    (LD) $(LDFLAGS) -shared -soname $(outso) -o $(outso) \
 * @endcode
 * for setting the internal soname of the library. After changing you should run on the working directory
 * @code
    make shared
 * @endcode
 * Now the library can installed with the default option.
 * 
 *
 * @subsection nixpath path under Linux
 * Under Linux some environmantal variables must be set, if the libraries are installed into a non-default directory. The variable <dfn>CPPPATH</dfn> must be set to the include
 * directories and <dfn>LD_LIBRARY_PATH</dfn> and <dfn>LIBRARY_PATH</dfn> must be pointed to the library path. The variables can be set within the <dfn>/etc/profile</dfn> or <dfn>~/.profile</dfn> with
 * @code
    export CPPPATH=first_path:second_path
    export LIBRARY_PATH=first_path:second_path
    export LD_LIBRARY_PATH=$LIBRARY_PATH
 * @endcode
 *
 * @subsection macpath path under Mac OS X
 * The path data should be set in the same way as in Linux, but OS X needs a own configuration file to use the variables in all OSX programs. Create a blank file in the hidden directory <dfn>~/.MacOSX</dfn>
 * with the name <dfn>environment.plist</dfn> and set the variable <dfn>CPPPATH</dfn> to the include directories and  <dfn>LIBRARY_PATH</dfn> to the library subdirectories. If you run a program a variable
 * <dfn>DYLD_LIBRARY_PATH</dfn> is also needed, but it can't set with the environmental file (because of security reasons) so it is recommand that the following line is added to the <dfn>/etc/profile</dfn>:
 * @code
    export DYLD_LIBRARY_PATH=$LIBRARY_PATH
 * @endcode
 *
 *
 * <hr>
 * @section windows Microsoft Windows
 * The building process with Windows is very difficult, because not all libraries support native Windows building scripts. It is recommand to use <a href="http://www.cygwin.com/">Cygwin</a> for building.  
 * Install Cygwin with the tools (but in some cases dependend libraries are needed, Cygwin creates errors if the libraries not exist):
 * <ul>
 * <li><dfn>devel/make</dfn></li>
 * <li><dfn>devel/gcc4-core</dfn></li>
 * <li><dfn>devel/gcc4-g++</dfn></li>
 * <li><dfn>devel/gcc4-gfortran</dfn></li>
 * <li><dfn>python/python</dfn></li>
 * </ul>
 * Here all libraries are installed under <dfn>C:\\opt\\library</dfn> with the version number of the library is set to a subdirectory, other the tools under <dfn>C:\\opt</dfn>. On Windows the 
 * <a href="http://www.bzip.org/">BZip2</a> and <a href="http://www.zlib.net/">ZLib</a> must be installed for using in the Boost.Iostreams. BZip2 sources must be extracted and run the commands
 * @code
    make
    make install PREFIX=/cygdrive/c/opt/library/bzip2/<version>
 * @endcode
 * The ZLib library is installed with
 * @code
    configure --prefix=/cygdrive/c/opt/library/zlib/<version>
    make
    make install
 * @endcode
 * For Boost.Iostreams in the Cygwin install directory in the file <dfn>etc/profile</dfn> or <dfn>~/.bashrc</dfn> the following variables must be set:
 * @code
    export BZIP2_BINARY=bz2
    export BZIP2_INCLUDE=/cygdrive/c/opt/library/bzip2/<version>/include
    export BZIP2_LIBPATH=/cygdrive/c/opt/library/bzip2/<version>/lib
 
    export ZLIB_BINARY=z
    export ZLIB_INCLUDE=/cygdrive/c/opt/library/zlib/<version>/include
    export ZLIB_LIBPATH=/cygdrive/c/opt/library/zlib/<version>/lib
 * @endcode
 * It is also recommend, that the linker flag is set with, because all examples are console applications
 * @code
    export LDFLAGS="-mconsole"
 * @endcode
 *
 * The libraries can be installed with the same steps as in Linux.
 * The path to the libraries directories must be set / added to the system or user path variable <dfn>PATH</dfn> (the Cygwin <dfn>bin</dfn> directory must be added, because all libraries are linked to the
 * <dfn>cygwin.dll</dfn>, which is stored in the <dfn>bin</dfn> directory), eg:
 * @code
    C:\opt\cygwin\bin;C:\opt\library\atlas\<version>\lib;C:\opt\library\hdf\<version>\lib;C:\opt\library\xml2\<version>\bin;C:\opt\library\boost\<version>\bin;C:\opt\library\boost\<version>\lib
 * @endcode
 * Scons can be installed under Cygwin with the command within the extracted source:
 * @code
    python setup.py install --prefix=/cygdrive/c/opt/scons
 * @endcode
 * The variable <dfn>CPPPATH</dfn> of the include directories is set in the file within the Cygwin install directory <dfn>etc/profile</dfn> (and to the path variable is added the scons directory)
 * @code
    PATH=$PATH:/cygdrive/c/opt/scons/bin
    export CPPPATH=/cygdrive/c/opt/library/atlas/<version>/include:/cygdrive/c/opt/library/boost/<version>/include:/cygdrive/c/opt/library/boost/sandbox/numeric_bindings:/cygdrive/c/opt/library/hdf/<version>/include:/cygdrive/c/opt/library/xml2/<version>/include/libxml2
 * @endcode
 * 
 * @subsection winversion Windows Version
 * The Windows version must be set with the scons parameter <dfn>winver</dfn>. The values are:
 * <ul>
 * <li><dfn>win7</dfn> Windows 7 [default]</li>
 * <li><dfn>srv2008</dfn> Windows Server 2008</li>
 * <li><dfn>vista</dfn> Windows Vista</li>
 * <li><dfn>srv2003sp1</dfn> Windows Server 2003 with SP1</li>
 * <li><dfn>xpsp2</dfn> Windows XP with SP2</li>
 * <li><dfn>srv2003</dfn> Windows Server 2003</li>
 * <li><dfn>xp</dfn> Windows XP</li>
 * <li><dfn>w2000</dfn> Windows 2000</li>
 * </ul>
 * The description and hexdecimal values can be found on http://msdn.microsoft.com/en-us/library/aa383745%28v=vs.85%29.aspx
 *
 * @subsection winjavac using Java builds under Cygwin
 * For using the Java builds all libraries must be installed and the JDK must be installed also and within the path directory. The include directory must be expanded with the subdirectory
 * <dfn>include</dfn> and <dfn>include/win32</dfn>. The GCC compiler under Cygwin does not have a buildin type <dfn>__int64</dfn>, so within the file <dfn>include/win32/jni_md.h</dfn> 
 * the line 
 * @code
    typedef long long jlong;
 * @endcode
 * must be changed to
 * @code
    #ifdef __GNUC__
    typedef long long jlong;
    #else
    typedef __int64 jlong;
    #endif
 * @endcode
 * Now the Java parts can be compiled with the Cygwin G++
 *
 * @subsection winscons Error with Scons
 * In some cases if Scons is running, the python would be create a memory error like:
 * @code
    6976514 [main] python 3456 C:\opt\cygwin\bin\python.exe: *** fatal error - unable to remap \\?\C:\opt\cygwin\lib\python2.6\lib-dynload\time.dll to same address as parent: 0x260000 != 0x2C0000
    Stack trace:
    Frame     Function  Args
    00228748  6102796B  (00228748, 00000000, 00000000, 00000000)
    00228A38  6102796B  (6117EC60, 00008000, 00000000, 61180977)
    00229A68  61004F1B  (611A7FAC, 61243BA4, 00260000, 002C0000)
    End of stack trace
 * @endcode
 * Following steps are solving the problem
 * <ol>
 * <li>close all Cygwin windows and Cygwin programs</li>
 * <li>open the <dfn>dash.exe</dfn> within the <dfn>bin</dfn> directory of the Cygwin installation</li>
 * <li>run the command @code /bin/rebaseall -v @endcode</li>
 * </ol>
 * If you get an error that the temporary directory is not writable, run first in a Cygwin shell the command
 * @code
    cd -to your Windows Home directory eg /cygdrive/c/Users/username-
    cd AppData/Local
    chmod 777 Temp
 * @endcode
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
 * @include examples/java/util/eigen.java
 *
 * @section javasvd Singular Value Decomposition
 * @include examples/java/util/svd.java
 *
 * @section javarandom Random Java Distribution
 * @include examples/java/util/random.java
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
 * @file examples/distance/ncd.cpp testprogram for the normalized compression distance
 * @file examples/clustering/neuralgas.cpp testprogram for the neural gas algorithm
 * @file examples/clustering/patch_neuralgas.cpp testprogram for the patch neural gas algorithm
 * @file examples/clustering/relational_neuralgas.cpp testprogram for the relational neural gas algorithm
 * @file examples/clustering/kmeans.cpp testprogram for the k-means algorithm
 * @file examples/clustering/spectral.cpp testprogram for spectral clustering
 * @file examples/clustering/rlvq.cpp testprogram for the rlvq algorithm
 * @file examples/classifier/lazy.cpp testprogram for the lazy-learner algorithm
 * @file examples/reducing/pca.cpp testprogram for the pca
 * @file examples/reducing/lda.cpp testprogram for lda
 * @file examples/reducing/mds.cpp testprogram for mds
 * @file examples/geneticalgorithm/knapsack.cpp testprogram for genetic algorithm (knapsack problem)
 * @file examples/sources/newsgroup.cpp testprogram for NNTP using
 * @file examples/sources/wikipedia.cpp testprogram for using Wikipedia
 * @file examples/sources/twitter.cpp testprogram for using Twitter tweets
 * @file examples/sources/cloud.cpp testprogram for create n-dimensional normal distribution
 * @file examples/other/mds_nntp.cpp program for reading newsgroup articles, stopword reduction, distance calculating and MDS plotting
 * @file examples/other/mds_wikipedia.cpp program for reading Wikipedia articles, stopword reduction, distance calculating and MDS plotting
 * @file examples/other/mds_file.cpp program for reading (text) files, stopword reduction, distance calculating and MDS plotting
 * @file examples/other/mds_twitter.cpp program for reading Twitter data and calculating plot with MDS
 * @file examples/java/reducing/pca.java example java program for using PCA
 * @file examples/java/reducing/mds.java example java program for using MDS
 * @file examples/java/util/eigen.java example for eigenvalue algorithms
 * @file examples/java/util/svd.java example for SVD algorithms
 * @file examples/java/util/random.java example for distribution algorithms
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
 * $LastChangedDate$
 * @todo split class declaration and implementation in different files for solving guard dependcy problems
 * @todo adding LUA ( http://en.wikipedia.org/wiki/Lua_(programming_language) ) support for dynamic programming ( http://en.wikipedia.org/wiki/Dynamic_programming )
 * @todo adding Twiddle Algorithm ( http://www.youtube.com/watch?v=2uQ2BSzDvXs )
 * @todo adding SFDP support (placment for lage graph data) http://www2.research.att.com/~yifanhu/SOFTWARE/SFDP/index.html / http://www2.research.att.com/~yifanhu/PUB/graph_draw_small.pdf
 * @todo adding GSL support http://www.gnu.org/software/gsl/
 * @todo adding PAPI support http://icl.cs.utk.edu/papi/
 * @todo adding stemming support ( http://en.wikipedia.org/wiki/Stemming )
 * @todo thinking about a SSE support in the framework http://en.wikipedia.org/wiki/SSE3
 * @todo adding GTML with http://en.wikipedia.org/wiki/Expectation–maximization_algorithm
 * @todo adding http://en.wikipedia.org/wiki/Apriori_algorithm
 * @todo adding http://en.wikipedia.org/wiki/Hierarchical_clustering
 * @todo adding http://en.wikipedia.org/wiki/DBSCAN
 * @todo adding http://en.wikipedia.org/wiki/OPTICS_algorithm
 * @todo adding http://en.wikipedia.org/wiki/Local_outlier_factor
 * @todo thinking about CILK support http://supertech.csail.mit.edu/cilk/
 * @todo adding simulated annealing (see GSL http://www.gnu.org/software/gsl/)
 * @todo adding some generic support for http://headmyshoulder.github.com/odeint-v2/
 * @todo adding voronoi tessellation (with CGAL http://http://www.cgal.org/ ) and other CGAL algorithms
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