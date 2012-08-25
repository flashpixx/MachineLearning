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


# === default configuration ================================================
localconf = {
    "cpplibraries"          : [  "stdc++",
                                 "boost_system",
                                 "boost_regex",
                                 "boost_iostreams"
    ],
    
    "staticlinkonly"        : [  "boost_exception"
    ],
    
    "cppdylibrariesonly"    : [ ], 
    
    "cdylibrariesonly"      : [ ], 
    
    "clibraries"            : [ ],
    
    "cheaders"              :  [ "omp.h"
    ],
    
    "cppheaders"            :  [ "map",
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

                                 os.path.join("boost", "numeric", "ublas", "matrix.hpp"),
                                 os.path.join("boost", "numeric", "ublas", "vector.hpp"), 
                                 os.path.join("boost", "numeric", "ublas", "matrix_sparse.hpp"),
                                 os.path.join("boost", "numeric", "ublas", "vector_proxy.hpp"),
                                 os.path.join("boost", "numeric", "ublas", "matrix_proxy.hpp"),
                                 os.path.join("boost", "numeric", "ublas", "storage.hpp"),
                                 os.path.join("boost", "numeric", "ublas", "io.hpp"),
                                 os.path.join("boost", "numeric", "ublas", "symmetric.hpp"), 
                            
                                 os.path.join("boost", "numeric", "bindings", "blas.hpp"),
                                 os.path.join("boost", "numeric", "bindings", "ublas", "vector.hpp"),
                                 os.path.join("boost", "numeric", "bindings", "ublas", "matrix.hpp"),
                                 os.path.join("boost", "numeric", "bindings", "lapack", "driver", "geev.hpp"),
                                 os.path.join("boost", "numeric", "bindings", "lapack", "driver", "ggev.hpp"),
                                 os.path.join("boost", "numeric", "bindings", "lapack", "driver", "gesv.hpp"),
                                 os.path.join("boost", "numeric", "bindings", "lapack", "driver", "gesvd.hpp"),
                                 os.path.join("boost", "numeric", "bindings", "lapack", "computational", "hseqr.hpp"),
                            
                                 os.path.join("boost", "algorithm", "string.hpp"), 
                                 os.path.join("boost", "algorithm", "string", "erase.hpp"),
                            
                                 os.path.join("boost", "accumulators", "accumulators.hpp"),
                                 os.path.join("boost", "accumulators", "statistics", "stats.hpp"),
                                 os.path.join("boost", "accumulators", "statistics", "min.hpp"),
                                 os.path.join("boost", "accumulators", "statistics", "max.hpp"),
                                 os.path.join("boost", "accumulators", "statistics", "mean.hpp"),
                                 os.path.join("boost", "accumulators", "statistics", "variance.hpp"),
                                 os.path.join("boost", "math", "distributions", "beta.hpp"),
                                 os.path.join("boost", "math", "distributions", "students_t.hpp"),
                                 os.path.join("boost", "math", "distributions", "weibull.hpp"),
                                 os.path.join("boost", "math", "distributions", "rayleigh.hpp"),
                                 os.path.join("boost", "math", "distributions", "pareto.hpp"),
                                 os.path.join("boost", "math", "distributions", "chi_squared.hpp"),
                                 os.path.join("boost", "math", "distributions", "triangular.hpp"),
                            
                                 os.path.join("boost", "iterator", "counting_iterator.hpp"),
                                 os.path.join("boost", "iterator", "permutation_iterator.hpp"),
                            
                                 os.path.join("boost", "lambda", "lambda.hpp"),
                            
                                 os.path.join("boost", "foreach.hpp"),
                                 os.path.join("boost", "random.hpp"),
                                 os.path.join("boost", "shared_ptr.hpp"),
                                 os.path.join("boost", "static_assert.hpp"),
                                 os.path.join("boost", "bind.hpp"),
                                 os.path.join("boost", "ref.hpp"),
                                 os.path.join("boost", "regex.hpp"),
                                 os.path.join("boost", "lexical_cast.hpp"),
                                  
                                 os.path.join("boost", "iostreams", "filter", "gzip.hpp"), 
                                 os.path.join("boost", "iostreams", "filter", "bzip2.hpp"), 
                                 os.path.join("boost", "iostreams", "stream.hpp"), 
                                 os.path.join("boost", "iostreams", "device", "null.hpp"), 
                                 os.path.join("boost", "iostreams", "filtering_streambuf.hpp"), 
                                 os.path.join("boost", "iostreams", "filtering_stream.hpp"), 
                                 os.path.join("boost", "iostreams", "filter", "counter.hpp"), 
                                 os.path.join("boost", "iostreams", "concepts.hpp"), 
                                 os.path.join("boost", "iostreams", "operations.hpp"), 
                                 os.path.join("boost", "iostreams", "copy.hpp"),
                                  
                                 os.path.join("boost", "numeric", "bindings", "blas.hpp"),
                                 os.path.join("boost", "numeric", "bindings", "ublas", "vector.hpp"),
                                 os.path.join("boost", "numeric", "bindings", "ublas", "matrix.hpp"),
                                 os.path.join("boost", "numeric", "bindings", "lapack", "driver", "geev.hpp"),
                                 os.path.join("boost", "numeric", "bindings", "lapack", "driver", "ggev.hpp"),
                                 os.path.join("boost", "numeric", "bindings", "lapack", "driver", "gesv.hpp"), 
                                 os.path.join("boost", "numeric", "bindings", "lapack", "driver", "gesvd.hpp"), 
                                 os.path.join("boost", "numeric", "bindings", "lapack", "computational", "hseqr.hpp")
    ]
}
# ==========================================================================



# === OSX environment configuration ========================================
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
conf.env.AppendUnique(CXXFLAGS    = ["fopenmp", "-pthread", "-pipe"])
conf.env.AppendUnique(CPPDEFINES  = ["BOOST_FILESYSTEM_NO_DEPRECATED", "BOOST_NUMERIC_BINDINGS_BLAS_CBLAS"])

if conf.env["buildtype"] == "release" :
    conf.env.AppendUnique(CPPDEFINES  = ["NDEBUG", "BOOST_UBLAS_NDEBUG", "MACHINELEARNING_NDEBUG"])
    conf.env.AppendUnique(CXXFLAGS    = ["-O2", "-fomit-frame-pointer", "-finline-functions"])
    
if conf.env["atlaslink"] == "multi" :
    localconf["clibraries"].append("tatlas")
elif conf.env["atlaslink"] == "single" :
    localconf["clibraries"].append("satlas")
    
if not("javac" in COMMAND_LINE_TARGETS) :
    localconf["cpplibraries"].extend([
                            "boost_program_options", "boost_filesystem"
    ])
    localconf["cppheaders"].extend([
                            "cstdlib",
                            os.path.join("boost", "program_options", "parsers.hpp"),
                            os.path.join("boost", "program_options", "variables_map.hpp"),
                            os.path.join("boost", "filesystem.hpp"),
                            os.path.join("boost", "program_options", "options_description.hpp")
    ])
# ==========================================================================



# === build configuration ==================================================
if conf.env["withrandomdevice"] :
    conf.env.AppendUnique(CPPDEFINES  = ["MACHINELEARNING_RANDOMDEVICE"])
    localconf["cpplibraries"].append(
                            "boost_random"
    )
    localconf["cppheaders"].append(
                            os.path.join("boost", "nondet_random.hpp")
    )
    
    
if conf.env["withmpi"] :
    conf.env.Replace(CXX = "mpic++")
    conf.env.AppendUnique(CPPDEFINES  = ["MACHINELEARNING_MPI"])
    localconf["cpplibraries"].extend([
                            "boost_mpi",
                            "boost_serialization"
    ])
    localconf["cppheaders"].append(
                            os.path.join("boost", "mpi.hpp")
    )
    

if conf.env["withmultilanguage"] :
    conf.env.AppendUnique(CPPDEFINES  = ["MACHINELEARNING_MULTILANGUAGE"])
    localconf["clibraries"].append(
                            "intl"
    )
    localconf["cheaders"].append(
                            "libintl.h"
    )

    
if conf.env["withfiles"] :
    conf.env.AppendUnique(CPPDEFINES  = ["MACHINELEARNING_FILES", "MACHINELEARNING_FILES_HDF"])
    localconf["cppheaders"].append(
                            "H5Cpp.h"
    )
    localconf["clibraries"].extend([
                            "hdf5",
                            "hdf5_hl"
    ])
    localconf["cpplibraries"].append(
                            "hdf5_cpp"
    )
    

if conf.env["withsymbolicmath"] :
    conf.env.AppendUnique(CPPDEFINES  = ["MACHINELEARNING_SYMBOLICMATH"])
    localconf["cppheaders"].extend([
                            os.path.join("boost", "multi_array.hpp"),
                            os.path.join("boost", "variant.hpp"),
                            os.path.join("ginac", "ginac.h")
    ])
    localconf["cpplibraries"].append(
                            "ginac"
    )


if conf.env["withlogger"] :
    conf.env.AppendUnique(CPPDEFINES  = ["MACHINELEARNING_LOGGER"])
    localconf["cpplibraries"].append(
                            "boost_thread"
    )    


if conf.env["withsources"] :
    conf.env.AppendUnique(CPPDEFINES  = ["MACHINELEARNING_SOURCES", "MACHINELEARNING_SOURCES_TWITTER"])
    localconf["clibraries"].append("xml2")
    localconf["cpplibraries"].append("json")
    localconf["cheaders"].extend([
                            os.path.join("libxml", "parser.h"),
                            os.path.join("libxml", "tree.h"),
                            os.path.join("libxml", "xpath.h"),
                            os.path.join("libxml", "xpathInternals.h")
    ])
    localconf["cppheaders"].extend([
                            "locale",
                            os.path.join("json", "json.h"),
                            os.path.join("boost", "asio.hpp"),
                            os.path.join("boost", "xpressive", "xpressive.hpp"),
                            os.path.join("boost", "date_time", "time_facet.hpp"),
                            os.path.join("boost", "date_time", "gregorian", "gregorian.hpp"),
                            os.path.join("boost", "date_time", "local_time", "local_time.hpp")
    ])
# ==========================================================================

#help.checkConfiguratin( conf, localconf )
