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

# -*- coding: utf-8 -*-

import os
Import("*")



# setup the environment
def tarparse( line ) :
    return line.split()[-1]
    
conf.env["EXTRACT_PARSE"]   = tarparse
conf.env["EXTRACT_LIST"]    = "tar tvfz $SOURCE"
conf.env["EXTRACT_CMD"]     = "tar oxfvz $SOURCE"
conf.env["EXTRACT_CMDBZ"]   = "tar oxfvj $SOURCE"
conf.env["extractsuffix"]   = " -C "


# OSX system information
if "CXX" in os.environ:
    conf.env.Replace(CXX = os.environ["CXX"])
    print("Using compiler " + os.environ["CXX"])

if "CPPPATH" in os.environ :
    conf.env.Append(CPPPATH = os.environ["CPPPATH"].split(os.pathsep))
    print("Appending custom C++ path (CPPPATH)")

if "CXXFLAGS" in os.environ:
    conf.env.Append(CXXFLAGS = os.environ["CXXFLAGS"].split(" "))
    print("Appending custom build flags (CXXFLAGS)")
 
if "LDFLAGS" in os.environ:
    conf.env.Append(LINKFLAGS = os.environ["LDFLAGS"].split(" "))
    print("Appending custom link flags (LDFLAG)")

if "DYLD_LIBRARY_PATH" in os.environ :
    conf.env.Append(LIBPATH = os.environ["DYLD_LIBRARY_PATH"].split(os.pathsep)) 
    print("Appending custom OSX dynamic library path (DYLD_LIBRARY_PATH)")
elif "LD_LIBRARY_PATH" in os.environ :
    conf.env.Append(LIBPATH = os.environ["LD_LIBRARY_PATH"].split(os.pathsep)) 
    print("Appending custom posix library path (LD_LIBRARY_PATH)")
elif "LIBRARY_PATH" in os.environ :
    conf.env.Append(LIBPATH = os.environ["LIBRARY_PATH"].split(os.pathsep)) 
    print("Appending custom posix library path (LIBRARY_PATH)")


# main configuration
conf.env.AppendUnique(LINKFLAGS   = ["fopenmp", "-pthread", "--as-needed"])
conf.env.AppendUnique(CXXFLAGS    = ["fopenmp", "-pthread", "-pipe", "BOOST_FILESYSTEM_NO_DEPRECATED", "BOOST_NUMERIC_BINDINGS_BLAS_CBLAS"])

if conf.env["buildtype"] == "release" :
    conf.env.AppendUnique(CPPDEFINES     = ["NDEBUG", "BOOST_UBLAS_NDEBUG", "MACHINELEARNING_NDEBUG"])
    
if conf.env["withoptimize"] :
    conf.env.AppendUnique(CXXFLAGS    = ["-O2", "-fomit-frame-pointer", "-finline-functions"])




# library defintion
localconf = {
    "cpplibraries" : [  "stdc++",
                        "boost_system",
                        "boost_regex",
                        "boost_iostreams"
    ],
    
    "staticlinkonly"     : [ "boost_exception" ],
    
    "cppdylibrariesonly" : [ ], 
    
    "cdylibrariesonly"   : [ ], 
    
    "clibraries"         : [ ],
    
    "cheaders"   :  [ "omp.h" ],
    
    "cppheaders" :  [ "map",
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
                      "boost/lexical_cast.hpp",
                      
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
                      
                      "boost/numeric/bindings/blas.hpp",
                      "boost/numeric/bindings/ublas/vector.hpp",
                      "boost/numeric/bindings/ublas/matrix.hpp",
                      "boost/numeric/bindings/lapack/driver/geev.hpp",
                      "boost/numeric/bindings/lapack/driver/ggev.hpp",
                      "boost/numeric/bindings/lapack/driver/gesv.hpp", 
                      "boost/numeric/bindings/lapack/driver/gesvd.hpp", 
                      "boost/numeric/bindings/lapack/computational/hseqr.hpp"
                      
                   ]
}
      

if not("javac" in COMMAND_LINE_TARGETS) :
    localconf["cpplibraries"].extend(["boost_program_options", "boost_filesystem"])
    localconf["cppheaders"].extend( ["cstdlib", "boost/program_options/parsers.hpp", "boost/program_options/variables_map.hpp", "boost/filesystem.hpp", "boost/program_options/options_description.hpp"] )


if conf.env["staticlink"] :
    localconf["clibraries"].append("atlas")
    localconf["cdylibrariesonly"].extend(["z", "bz2"])
elif not(conf.env["staticlink"]) and conf.env["atlaslink"] == "multi" :
    localconf["clibraries"].append("tatlas")
elif not(conf.env["staticlink"]) and conf.env["atlaslink"] == "single" :
    localconf["clibraries"].append("satlas")


if conf.env["withsources"] :
    conf.env.Append(CXXFLAGS = "-D MACHINELEARNING_SOURCES -D MACHINELEARNING_SOURCES_TWITTER")
    localconf["clibraries"].append("xml2")
    localconf["cpplibraries"].append("json")
    localconf["cheaders"].extend(["libxml/parser.h", "libxml/tree.h", "libxml/xpath.h", "libxml/xpathInternals.h"]);
    localconf["cppheaders"].extend(["locale", "json/json.h", "boost/asio.hpp", "boost/xpressive/xpressive.hpp", "boost/date_time/time_facet.hpp", "boost/date_time/gregorian/gregorian.hpp", "boost/date_time/local_time/local_time.hpp"] )


if conf.env["withrandomdevice"] :
    conf.env.Append(CXXFLAGS = "-D MACHINELEARNING_RANDOMDEVICE")
    localconf["cpplibraries"].append("boost_random")
    localconf["cppheaders"].append("boost/nondet_random.hpp")

    
if conf.env["withmpi"] :
    conf.env.Append(CXXFLAGS = "-D MACHINELEARNING_MPI")
    localconf["cpplibraries"].extend([ "boost_mpi", "boost_serialization" ])
    localconf["cppheaders"].append("boost/mpi.hpp")
    conf.env.Replace(CXX = "mpic++")

    
if conf.env["withmultilanguage"] :
    conf.env.Append(CXXFLAGS = "-D MACHINELEARNING_MULTILANGUAGE")
    localconf["clibraries"].append("intl")
    localconf["cheaders"].append("libintl.h")

    
if conf.env["withfiles"] :
    conf.env.Append(CXXFLAGS = "-D MACHINELEARNING_FILES -D MACHINELEARNING_FILES_HDF")
    localconf["cppheaders"].append("H5Cpp.h")
    localconf["clibraries"].extend(["hdf5", "hdf5_hl"])
    localconf["cpplibraries"].append("hdf5_cpp")
    

if conf.env["withsymbolicmath"] :
    conf.env.Append(CXXFLAGS = "-D MACHINELEARNING_SYMBOLICMATH")
    localconf["cppheaders"].extend(["boost/multi_array.hpp", "boost/variant.hpp", "ginac/ginac.h"])
    localconf["cpplibraries"].append("ginac")


if conf.env["withlogger"] :
    conf.env.Append(CXXFLAGS = "-D MACHINELEARNING_LOGGER")
    localconf["cpplibraries"].append("boost_thread")
    

#help.checkConfiguratin( conf, localconf )
