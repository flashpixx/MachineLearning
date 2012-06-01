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

# -*- coding: utf-8 -*-
import os
Import("*")


conf = Configure(env)


# OSX system information
if "CXX" in os.environ:
    conf.env.Replace(CXX = os.environ["CXX"])
    print("Using compiler " + os.environ["CXX"])
    
if os.environ.has_key("CPPPATH") :
    conf.env.Append(CPPPATH = os.environ["CPPPATH"].split(os.pathsep))
    print("Appending custom C++ path (CPPPATH)")

if "CXXFLAGS" in os.environ:
    conf.env.Append(CXXFLAGS = os.environ["CXXFLAGS"])
    print("Appending custom build flags (CXXFLAGS)")
 
if "LDFLAGS" in os.environ:
    conf.env.Append(LINKFLAGS = os.environ["LDFLAGS"])
    print("Appending custom link flags (LDFLAG)")

if "DYLD_LIBRARY_PATH" in os.environ :
    conf.env.Append(LIBPATH = os.environ["DYLD_LIBRARY_PATH"].split(os.pathsep)) 
    print("Appending custom OSX dynamic library path (DYLD_LIBRARY_PATH)")
elif "LD_LIBRARY_PATH" in os.environ :
    conf.env.Append(LIBPATH = os.environ["LD_LIBRARY_PATH"].split(os.pathsep)) 
    print("Appending custom library path (LD_LIBRARY_PATH)")
elif "LIBRARY_PATH" in os.environ :
    conf.env.Append(LIBPATH = os.environ["LIBRARY_PATH"].split(os.pathsep)) 
    print("Appending custom library path (LIBRARY_PATH)")


# main configuration
conf.env.Append(CXXFLAGS = "-fopenmp -pipe -D BOOST_FILESYSTEM_NO_DEPRECATED -D BOOST_NUMERIC_BINDINGS_BLAS_CBLAS")    
conf.env.Append(LINKFLAGS = "-pthread -fopenmp")


libraries  = [  "boost_system",
                "boost_iostreams",
                "boost_regex"
             ]
cheaders   = [  "omp.h"
             ]
cppheaders = [  "map",
                "algorithm",
                "limits",
                "iostream",
                "numeric",
                "string",
                "sstream",
                "ostream",
                "fstream",
                "stdexcept",
                "vector",
                "ctime",
                "iterator",
                "typeinfo",
                "cxxabi.h",
                
                "boost/numeric/ublas/matrix.hpp",
                "boost/numeric/ublas/vector.hpp", 
                "boost/numeric/ublas/matrix_sparse.hpp",
                "boost/numeric/ublas/vector_proxy.hpp",
                "boost/numeric/ublas/matrix_proxy.hpp",
                "boost/numeric/ublas/storage.hpp",
                "boost/numeric/ublas/io.hpp",
                "boost/numeric/ublas/symmetric.hpp", 
                "boost/numeric/bindings/blas.hpp",
                "boost/numeric/bindings/ublas/vector.hpp",
                "boost/numeric/bindings/ublas/matrix.hpp",
                "boost/numeric/bindings/lapack/driver/geev.hpp",
                "boost/numeric/bindings/lapack/driver/ggev.hpp",
                "boost/numeric/bindings/lapack/driver/gesv.hpp",
                "boost/numeric/bindings/lapack/driver/gesvd.hpp",
                "boost/numeric/bindings/lapack/computational/hseqr.hpp",
                "boost/iostreams/filter/gzip.hpp",
                "boost/iostreams/filter/bzip2.hpp",
                "boost/iostreams/stream.hpp",
                "boost/iostreams/device/null.hpp",
                "boost/iostreams/filtering_streambuf.hpp",
                "boost/iostreams/filtering_stream.hpp",
                "boost/iostreams/filter/counter.hpp",
                "boost/iostreams/concepts.hpp",
                "boost/iostreams/operations.hpp",
                "boost/iostreams/copy.hpp",
                "boost/algorithm/string.hpp", 
                "boost/algorithm/string/erase.hpp",
                "boost/accumulators/accumulators.hpp",
                "boost/accumulators/statistics/stats.hpp",
                "boost/accumulators/statistics/min.hpp",
                "boost/accumulators/statistics/max.hpp",
                "boost/accumulators/statistics/mean.hpp",
                "boost/accumulators/statistics/variance.hpp",
                "boost/math/distributions/beta.hpp",
                "boost/math/distributions/students_t.hpp",
                "boost/math/distributions/weibull.hpp",
                "boost/math/distributions/rayleigh.hpp",
                "boost/math/distributions/pareto.hpp",
                "boost/math/distributions/chi_squared.hpp",
                "boost/math/distributions/triangular.hpp",
                "boost/iterator/counting_iterator.hpp",
                "boost/iterator/permutation_iterator.hpp",
                "boost/lambda/lambda.hpp",
                "boost/foreach.hpp",
                "boost/random.hpp",
                "boost/shared_ptr.hpp",
                "boost/static_assert.hpp",
                "boost/bind.hpp",
                "boost/ref.hpp",
                "boost/regex.hpp",
                "boost/filesystem.hpp",
                "boost/lexical_cast.hpp",
                "boost/thread.hpp"
             ]


if not("javac" in COMMAND_LINE_TARGETS) :
    libraries.extend(["boost_program_options", "boost_exception", "boost_filesystem"])
    cppheaders.extend( ["cstdlib", "boost/program_options/parsers.hpp", "boost/program_options/variables_map.hpp", "boost/program_options/options_description.hpp",] )

if env["atlaslink"] == "multi" :
    libraries.extend( ["ptcblas", "ptf77blas", "gfortran"] )
elif env["atlaslink"] == "single" :
    libraries.extend( ["cblas", "f77blas", "gfortran"] )

if env["staticlink"] :
    libraries.append("atlas")
else :
    if env["atlaslink"] == "multi" :
        libraries.append("tatlas")
    elif env["atlaslink"] == "single" :
        libraries.append("satlas")

if env["withdebug"] :
    conf.env.Append(CXXFLAGS = "-g")
else :
    conf.env.Append(CXXFLAGS = "-D NDEBUG -D BOOST_UBLAS_NDEBUG")
    
if env["withsources"] :
    conf.env.Append(CXXFLAGS = "-D MACHINELEARNING_SOURCES" "-D MACHINELEARNING_SOURCES_TWITTER")
    libraries.extend( ["xml2", "json"] )
    cppheaders.extend( ["locale", "json/json.h", "boost/asio.hpp", "boost/xpressive/xpressive.hpp", "boost/date_time/time_facet.hpp", "boost/date_time/gregorian/gregorian.hpp", "boost/date_time/local_time/local_time.hpp"] )
    cheaders.extend( ["libxml/parser.h", "libxml/tree.h", "libxml/xpath.h", "libxml/xpathInternals.h"] )

if env["withrandomdevice"] :
    conf.env.Append(CXXFLAGS = "-D MACHINELEARNING_RANDOMDEVICE")
    libraries.append("boost_random");
    cppheaders.append("boost/nondet_random.hpp")
    
if env["withmpi"] :
    conf.env.Append(CXXFLAGS = "-D MACHINELEARNING_MPI")
    libraries.extend( ["boost_mpi", "boost_serialization"] )
    cppheaders.append("boost/mpi.hpp")
    conf.env.Replace(CXX = "mpic++")
    
if env["withmultilanguage"] :
    conf.env.Append(CXXFLAGS = "-D MACHINELEARNING_MULTILANGUAGE")
    libraries.append("intl");
    cheaders.append("libintl.h")
    
if env["withfiles"] :
    conf.env.Append(CXXFLAGS = "-D MACHINELEARNING_FILES -D MACHINELEARNING_FILES_HDF")
    libraries.extend( ["hdf5_cpp", "hdf5"] )
    cppheaders.append("H5Cpp.h")
    
if env["withsymbolicmath"] :
    conf.env.Append(CXXFLAGS = "-D MACHINELEARNING_SYMBOLICMATH")
    libraries.append("ginac")
    cppheaders.extend( ["boost/multi_array.hpp", "boost/variant.hpp", "ginac/ginac.h"] )

if env["withlogger"] :
    conf.env.Append(CXXFLAGS = "-D MACHINELEARNING_LOGGER")
    libraries.append("boost_thread")
    
if env["withoptimize"] :
    conf.env.Append(CXXFLAGS = "-O2 -fomit-frame-pointer -finline-functions")
    if env["math"] == "sse3" :
        conf.env.Append(CXXFLAGS = "-mfpmath=sse -msse3")
    elif env["math"] == "sse" :
        conf.env.Append(CXXFLAGS = "-mfpmath=sse -msse")
    else :
        conf.env.Append(CXXFLAGS = "-mfpmath=387")



# uniquify all lists
cheaders   = help.unique(cheaders)
cppheaders = help.unique(cppheaders)
libraries  = help.unique(libraries)

# check header data
for i in cheaders :
    if not conf.CheckCHeader(i) :
        Exit(1)

for i in cppheaders :
    if not conf.CheckCXXHeader(i) :
        Exit(1)


# check library data (Lapack is linked always static)
if env["staticlink"] :
    libraries = [env["LIBPREFIX"]+i+env["LIBSUFFIX"] for i in libraries]
    libraries.extend( ["z", "bz2"] )
libraries.append(env["LIBPREFIX"]+"lapack"+env["LIBSUFFIX"])

for i in libraries :
    if not conf.CheckLib(i) :
        Exit(1)
    #conf.env.Append(LIBS = i)

env = conf.Finish()


#http://scons.org/wiki/SavingVariablesToAFile
#vars = Variables("config.osx")

#vars.Add("CXX")
#vars.Add("CPPPATH")
#vars.Add("CCFLAGS")
#vars.Add("LINKFLAGS")
#vars.Add("LIBPATH")

#vars.Update(env)

#vars.Save("config.osx", env)




"""
flags = {}


flags["CPPPATH"] = []
if os.environ.has_key("CPPPATH") :
    flags["CPPPATH"].extend(os.environ["CPPPATH"].split(os.pathsep))
elif os.environ.has_key("CPATH") :
    flags["CPPPATH"].extend(os.environ["CPATH"].split(os.pathsep))
    
flags["LIBPATH"] = []
if os.environ.has_key("DYLD_LIBRARY_PATH") :
    flags["LIBPATH"].extend(os.environ["DYLD_LIBRARY_PATH"].split(os.pathsep))
elif os.environ.has_key("LIBRARY_PATH") :
    flags["LIBPATH"].extend(os.environ["LIBRARY_PATH"].split(os.pathsep))
elif os.environ.has_key("LD_LIBRARY_PATH") :
    flags["LIBPATH"].extend(os.environ["LD_LIBRARY_PATH"].split(os.pathsep))
    
flags["CXXFLAGS"] = []
if os.environ.has_key("CXXFLAGS") :
    flags["CXXFLAGS"].extend(os.environ["CXXFLAGS"].split(" "))

flags["LINKFLAGS"] = []
if os.environ.has_key("LDFLAGS") :
    flags["LINKFLAGS"].extend(os.environ["LDFLAGS"].split(" "))
    


flags["CXXFLAGS"].extend(["-fopenmp", "-pipe", "-Wall", "-D BOOST_FILESYSTEM_NO_DEPRECATED", "-D BOOST_NUMERIC_BINDINGS_BLAS_CBLAS"])    
flags["LINKFLAGS"].extend(["-pthread", "-fopenmp"])



flags["LIBS"]         = ["boost_system", "boost_iostreams", "boost_regex"]
flags["SHLINKFLAGS"]  = ["-install_name ${TARGET.file} -headerpad_max_install_name"]



if not("javac" in COMMAND_LINE_TARGETS) :
    flags["LIBS"].extend(["boost_program_options", "boost_exception", "boost_filesystem"])

if env["atlaslink"] == "multi" :
    flags["LIBS"].extend( ["ptcblas", "ptf77blas", "gfortran"] )
elif env["atlaslink"] == "single" :
    flags["LIBS"].extend( ["cblas", "f77blas", "gfortran"] )

if env["staticlink"] :
    flags["LIBS"].append("atlas")
else :
    if env["atlaslink"] == "multi" :
        flags["LIBS"].append("tatlas")
    elif env["atlaslink"] == "single" :
        flags["LIBS"].append("satlas")
        

#if env["withdebug"] :
    flags["CXXFLAGS"].append("-g")
else :
    flags["CXXFLAGS"].extend(["-D NDEBUG", "-D BOOST_UBLAS_NDEBUG"])

#if env["withmpi"] :
    flags["CXXFLAGS"].append("-D MACHINELEARNING_MPI")
    flags["LIBS"].extend( ["boost_mpi", "boost_serialization"] )

#if env["withrandomdevice"] :
    flags["CXXFLAGS"].append("-D MACHINELEARNING_RANDOMDEVICE")
    flags["LIBS"].append("boost_random");

#if env["withmultilanguage"] :
    flags["CXXFLAGS"].append("-D MACHINELEARNING_MULTILANGUAGE")
    flags["LIBS"].append("intl");

#if env["withsources"] :
    flags["CXXFLAGS"].extend(["-D MACHINELEARNING_SOURCES", "-D MACHINELEARNING_SOURCES_TWITTER"])
    flags["LIBS"].extend( ["xml2", "json"] )

#if env["withfiles"] :
    flags["CXXFLAGS"].extend(["-D MACHINELEARNING_FILES", "-D MACHINELEARNING_FILES_HDF"])
    flags["LIBS"].extend( ["hdf5_cpp", "hdf5"] )

if env["withsymbolicmath"] :
    flags["CXXFLAGS"].append("-D MACHINELEARNING_SYMBOLICMATH")
    flags["LIBS"].append("ginac")

if env["withoptimize"] :
    flags["CXXFLAGS"].extend(["-O2", "-fomit-frame-pointer", "-finline-functions"])
    if env["math"] == "sse3" :
        flags["CXXFLAGS"].extend(["-mfpmath=sse", "-msse3"])
    elif env["math"] == "sse" :
        flags["CXXFLAGS"].extend(["-mfpmath=sse", "-msse"])
    else :
        flags["CXXFLAGS"].extend(["-mfpmath=387"])

if env["withlogger"] :
    flags["CXXFLAGS"].append("-D MACHINELEARNING_LOGGER")
    flags["LIBS"].append("boost_thread")


# if the static option is set, replace all libraries with the static version
if env["staticlink"] :
    dylink = ["z", "bz2"]
    for i in flags["LIBS"] :
        found = env.FindFile(env["LIBPREFIX"]+i+env["LIBSUFFIX"], flags["LIBPATH"])
        if found <> None :
            flags["LINKFLAGS"].append(found)
        else :
            dylink.append(i);
    flags["LIBS"] = dylink
    
#LAPack link always static
found = env.FindFile(env["LIBPREFIX"]+"lapack"+env["LIBSUFFIX"], flags["LIBPATH"])
if found <> None :
    flags["LINKFLAGS"].append(found)

env.MergeFlags(flags)
"""