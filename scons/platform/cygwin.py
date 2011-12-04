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

flags = {}


flags["CPPPATH"] = []
if os.environ.has_key("CPPPATH") :
    flags["CPPPATH"].extend( os.environ["CPPPATH"].split(os.pathsep) )
elif os.environ.has_key("CPATH") :
    flags["CPPPATH"].extend( os.environ["CPATH"].split(os.pathsep) )
    
flags["LIBPATH"] = []
if os.environ.has_key("PATH") :
    flags["LIBPATH"].extend(os.environ["PATH"].split(os.pathsep))
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
flags["LINKFLAGS"].extend(["-fopenmp", "-mconsole", "-enable-stdcall-fixup", "-mthread"])
flags["LIBS"]         = ["z", "bz2", "boost_system", "boost_iostreams", "boost_regex"]


# linker command -mdll can be used
if not("javac" in COMMAND_LINE_TARGETS) :
    flags["LIBS"].extend(["boost_program_options", "boost_exception", "boost_filesystem"])
    
#Windows Version options see http://msdn.microsoft.com/en-us/library/aa383745%28v=vs.85%29.aspx
if env["winver"] == "win7" :
    flags["CXXFLAGS"].append("-D _WIN32_WINNT=0x0601")
elif env["winver"] == "srv2008" :
    flags["CXXFLAGS"].append("-D _WIN32_WINNT=0x0600")
elif env["winver"] == "vista" :
    flags["CXXFLAGS"].append("-D _WIN32_WINNT=0x0600")
elif env["winver"] == "srv2003sp1" :
    flags["CXXFLAGS"].append("-D _WIN32_WINNT=0x0502")
elif env["winver"] == "xpsp2" :
    flags["CXXFLAGS"].append("-D _WIN32_WINNT=0x0502")
elif env["winver"] == "srv2003" :
    flags["CXXFLAGS"].append("-D _WIN32_WINNT=0x0501")
elif env["winver"] == "xp" :
    flags["CXXFLAGS"].append("-D _WIN32_WINNT=0x0501")
elif env["winver"] == "w2000" :
    flags["CXXFLAGS"].append("-D _WIN32_WINNT=0x0500")
    
# Atlas build creates a static library under Cygwin, so we link directly without the "atlaslink" option
# Library sequence must be preserved !!
if env["atlaslink"] == "multi" :
    flags["LIBS"].extend( ["lapack", "ptcblas", "ptf77blas", "gfortran"] )
elif env["atlaslink"] == "single" :
    flags["LIBS"].extend( ["lapack", "cblas", "f77blas", "gfortran"] )

if env["staticlink"] :
    flags["LIBS"].append("atlas")
else :
    if env["atlaslink"] == "multi" :
        flags["LIBS"].append("tatlas")
    elif env["atlaslink"] == "single" :
        flags["LIBS"].append("satlas")

    
if env["withdebug"] :
    flags["CXXFLAGS"].append("-g")
else :
    flags["CXXFLAGS"].extend(["-D NDEBUG", "-D BOOST_UBLAS_NDEBUG"])

if env["withrandomdevice"] :
    flags["CXXFLAGS"].append("-D MACHINELEARNING_RANDOMDEVICE")
    flags["LIBS"].append("boost_random");

if env["withsources"] :
    flags["CXXFLAGS"].extend(["-D MACHINELEARNING_SOURCES", "-D MACHINELEARNING_SOURCES_TWITTER", "-D __USE_W32_SOCKETS"])
    flags["LIBS"].extend( ["xml2-2", "ws2_32", "json"] )

if env["withfiles"] :
    flags["CXXFLAGS"].extend(["-D MACHINELEARNING_FILES", "-D MACHINELEARNING_FILES_HDF"])
    flags["LIBS"].extend( ["hdf5_cpp", "hdf5"] )

if env["withsymbolicmath"] :
    flags["CXXFLAGS"].append("-D MACHINELEARNING_SYMBOLICMATH")
    flags["LIBS"].append("ginac")

if env["withoptimize"] :
    flags["CXXFLAGS"].extend(["-O2", "-fomit-frame-pointer", "-finline-functions", "-mtune="+env["cputype"]])
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
    dylink = []
    for i in flags["LIBS"] :
        found = env.FindFile(env["LIBPREFIX"]+i+env["LIBSUFFIX"], flags["LIBPATH"])
        if found <> None :
            flags["LINKFLAGS"].append(found)
        else :
            dylink.append(i);
    flags["LIBS"] = dylink

env.MergeFlags(flags)
