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


# environmental script for Msys system. The libraries must be stored within
# the global system paths or must be set within the environment variables. The env
# variables are read here and are added to the environment object

import os
import re
import distutils.version as dv
Import("*")


if conf.env["withmpi"] :
    raise RuntimeError("MPI build does not work under Msys")


# === default configuration ================================================
localconf = {
    "linkflags"             : [ ],

    "cpplibraries"          : [  "stdc++",
                                 "boost_iostreams-mt"
    ],
    
    # gfortran must be linked after Lapack, otherwise a linker error is shown
    "clibraries"            : [ "lapack",
                                "lapacke",
                                "blas",
                                "gfortran"
    ],
    
    "cheaders"              : [ "omp.h"
    ],
    
    "cppheaders"            : [ "map",
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
                                os.path.join("boost", "iostreams", "copy.hpp")
    ]
}
# ==========================================================================



# === posix environment configuration ======================================
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
    
if "CXXFLAGS" in os.environ:
    conf.env.Append(CXXFLAGS = os.environ["CXXFLAGS"].split(" "))
    print("Appending custom build flags (CXXFLAGS)")
 
if "LDFLAGS" in os.environ:
    conf.env.Append(LINKFLAGS = os.environ["LDFLAGS"].split(" "))
    print("Appending custom link flags (LDFLAG)")

if "HOME" in os.environ :
    conf.env["ENV"]["HOME"] = os.environ["HOME"]
    print("Using home environment variable (HOME)")
    
if conf.env["uselocallibrary"] :
    libdir      = []
    includedir  = []
    
    path   = os.path.abspath(os.path.join(os.path.abspath(os.curdir), "..", "library", "build"))
    reg    = re.compile("\d(.*)")
    
    for i in ["lapack", "boost", "cln", "ginac", "hdf", "jsoncpp", "xml"] :
        dir = os.path.join(path, i)
        if not(os.path.isdir(dir)) :
            continue
            
        version = [dv.LooseVersion(j) for j in filter(lambda x: reg.match(x) <> None, os.listdir( dir ))]
        version.sort(reverse=True)
        if version :
            libdir.append( os.path.join(dir, str(version[0]), "lib" ) )
            libdir.append( os.path.join(dir, str(version[0]), "bin" ) )
            
            if i == "xml" :
                includedir.append( os.path.join(dir, str(version[0]), "include", "libxml2" ) )
            else :
                includedir.append( os.path.join(dir, str(version[0]), "include" ) )
                
            if i == "hdf" :
                includedir.append( os.path.join(dir, str(version[0]), "include", "cpp" ) )
            
            if i == "boost" :
                includedir.append( os.path.join(dir, "numeric_bindings" ) )
        
    conf.env.Replace(LIBPATH = libdir)
    conf.env.Replace(CPPPATH = includedir)
    print("Using local C++ path")
    print("Using local dynamic library path")
    
else :
    if "CPPPATH" in os.environ :
        conf.env.AppendUnique(CPPPATH = os.environ["CPPPATH"].split(os.pathsep))
        print("Appending custom C++ path (CPPPATH)")

    if "LD_LIBRARY_PATH" in os.environ :
        conf.env.AppendUnique(LIBPATH = os.environ["LD_LIBRARY_PATH"].split(os.pathsep)) 
        print("Appending custom posix library path (LD_LIBRARY_PATH)")
    elif "LIBRARY_PATH" in os.environ :
        conf.env.AppendUnique(LIBPATH = os.environ["LIBRARY_PATH"].split(os.pathsep)) 
        print("Appending custom posix library path (LIBRARY_PATH)")
        
#manual pathes
if "jnipath" in conf.env and conf.env["jnipath"] :
    conf.env.AppendUnique(CPPPATH = [conf.env["jnipath"], os.path.join(conf.env["jnipath"], "win32")])
    
# append main framework directory directory (read mingw/bin directory from path)
conf.env.AppendUnique(CPPPATH = [Dir("#")])
conf.env["COPYLIBRARYPATH"]   = []
for i in os.environ["PATH"].split(os.pathsep) :
    if i.endswith("\\mingw\\bin") :
        conf.env["COPYLIBRARYPATH"].append(i)
        break
# set additional dynamic link libraries which should be copied into the build dir    
conf.env["COPYLIBRARY"] = ["gcc_s_dw2-1", "gfortran-3", "quadmath-0", "gomp-1", "pthreadGC2", "stdc++-6"]

# main configuration (the C++ flag BOOST_NUMERIC_BINDINGS_BLAS_CBLAS need not be
# set, because we build without ATLAS support)
conf.env.AppendUnique(LINKFLAGS   = ["-fopenmp"])
conf.env.AppendUnique(CXXFLAGS    = ["-fopenmp", "-pipe"])
conf.env.AppendUnique(CPPDEFINES  = ["BOOST_FILESYSTEM_NO_DEPRECATED"])

if conf.env["buildtype"] == "release" :
    conf.env.AppendUnique(CPPDEFINES  = ["NDEBUG", "BOOST_UBLAS_NDEBUG", "MACHINELEARNING_NDEBUG"])
    conf.env.AppendUnique(CXXFLAGS    = ["-O2", "-fomit-frame-pointer", "-finline-functions"])
elif conf.env["buildtype"] == "debug" :
    conf.env.AppendUnique(LINKFLAGS   = ["-g"])
    conf.env.AppendUnique(CXXFLAGS    = ["-g"])

#Windows Version options see http://msdn.microsoft.com/en-us/library/aa383745%28v=vs.85%29.aspx
if conf.env["winversion"] == "win7" :
    conf.env.AppendUnique(CPPDEFINES = ["_WIN32_WINNT=0x0601"])
elif conf.env["winversion"] == "srv2008" :
    conf.env.AppendUnique(CPPDEFINES = ["_WIN32_WINNT=0x0600"])
elif conf.env["winversion"] == "vista" :
    conf.env.AppendUnique(CPPDEFINES = ["_WIN32_WINNT=0x0600"])
elif conf.env["winversion"] == "srv2003sp1" :
    conf.env.AppendUnique(CPPDEFINES = ["_WIN32_WINNT=0x0502"])
elif conf.env["winversion"] == "xpsp2" :
    conf.env.AppendUnique(CPPDEFINES = ["_WIN32_WINNT=0x0502"])
elif conf.env["winversion"] == "srv2003" :
    conf.env.AppendUnique(CPPDEFINES = ["_WIN32_WINNT=0x0501"])
elif conf.env["winversion"] == "xp" :
    conf.env.AppendUnique(CPPDEFINES = ["_WIN32_WINNT=0x0501"])
elif conf.env["winversion"] == "w2000" :
    conf.env.AppendUnique(CPPDEFINES = ["_WIN32_WINNT=0x0500"])
    
    
if not("java" in COMMAND_LINE_TARGETS) :
    localconf["cpplibraries"].extend([
                            "boost_program_options-mt", "boost_filesystem-mt"
    ])
    localconf["cppheaders"].extend([
                            "cstdlib",
                            os.path.join("boost", "program_options", "parsers.hpp"),
                            os.path.join("boost", "program_options", "variables_map.hpp"),
                            os.path.join("boost", "filesystem.hpp"),
                            os.path.join("boost", "program_options", "options_description.hpp")
    ])
    
if "librarybuild" in COMMAND_LINE_TARGETS :
    checkExecutables(conf, [ "g++.exe", "gcc.exe", "gfortran.exe", "tar.exe", "make.exe", "svn.exe", "cmake.exe" ])
elif "documentation" in COMMAND_LINE_TARGETS :
    checkExecutables(conf, "doxygen.exe")
# ==========================================================================



# === build configuration ==================================================
if "java" in COMMAND_LINE_TARGETS :
    localconf["cheaders"].append("jni.h")
    conf.env["COPYLIBRARY"].append("quserex")
    # we need the kill-at flag, because the JRE & g++ name mangeling are different, so we disable it under g++ http://en.wikipedia.org/wiki/Name_mangling
    conf.env.AppendUnique(LINKFLAGS    = ["-Wl,--kill-at"])

     
if conf.env["withrandomdevice"] :
    conf.env.AppendUnique(CPPDEFINES  = ["MACHINELEARNING_RANDOMDEVICE"])
    localconf["cpplibraries"].append(
                            "boost_random-mt"
    )
    localconf["cppheaders"].append(
                            os.path.join("boost", "nondet_random.hpp")
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
    localconf["cpplibraries"].extend(["boost_thread-mt", "boost_system-mt"])     


if conf.env["withsources"] :
    conf.env.AppendUnique(CPPDEFINES  = ["MACHINELEARNING_SOURCES", "MACHINELEARNING_SOURCES_TWITTER"])
    conf.env["COPYLIBRARY"].extend(["xml2-2", "libiconv-2"])
    localconf["clibraries"].extend(["xml2", "ws2_32"])
    localconf["cpplibraries"].extend(["boost_regex-mt", "json", "boost_system-mt"])
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

checkCPPEnv(conf, localconf)
