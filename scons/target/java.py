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

import sys
sys.path.append("..")
import help
import os

Import("*")


# change the library calls under OSX
def java_osxlinkedlibs(target, source, env) :
    oFile = open( os.path.join("build", "linkedlibs.txt"), "r" )
    libs = oFile.read()
    oFile.close()

    # split string on each space
    names = []
    lines = libs.split()

    # test each linked library to the otool results
    for n in env["LIBS"] :
        for i in lines :
            if n in i :
                names.append(i)

    # build change list
    change = []
    for i in names :
        name = i.split(os.sep)[-1]
        change.append("-change " + i + " @loader_path" + os.path.sep + name)
    change = help.unique(change)

    # run the build command on a system shell
    os.system( "install_name_tool " + " ".join(change) + " " + os.path.join("build", "javalib", "native", env["LIBPREFIX"]+"machinelearning"+env["SHLIBSUFFIX"]) )
    return []



# changes the library calls under Linux (sonames)
def java_linuxsonames(target, source, env) :

    # read all files within the native directory and get the SONAME of each library and change the filename
    nativepath = os.path.join("build", "javalib", "native")
    for root, dirs, filenames in os.walk(nativepath) :
        for filename in filenames :
            pathfile = os.path.join(nativepath, filename)
    	    os.system( "objdump -p " + pathfile + " | grep -i soname | awk '{system(\"mv " + pathfile + " "+nativepath+os.path.sep+"\"$2)}'" )

    return []



# compile Java classes
targets = []
targets.append( targets.extend( env.Java(target=os.path.join("#build", "javalib"), source=os.path.join("..", "..", "java")) ) )

# list with Java classes that are used for the JavaP command
javaplist = []
javaplist.extend(targets)

# create JNI stubs and JavaP command (so we add the classes manually)
stubs = ["machinelearning.dimensionreduce.nonsupervised.PCA", "machinelearning.dimensionreduce.nonsupervised.MDS", "machinelearning.util.Math", "machinelearning.util.Random"]
for i in stubs :
    # split file and directory parts and substitute $ to _ and create the headerfile
    parts = i.replace("$", "_").split(".")
    headerfile = (os.sep.join(parts) + ".h").lower()

    targets.append( env.Command( headerfile, "", "javah -classpath " + os.path.join("build", "javalib") + " -o " + os.path.join("java", headerfile) + " " + i  ) )

# build SharedLibrary
# default cpps that must be compiled on each run
framework = []
if env["withlogger"] or env["withrandomdevice"] :
    framework.append( "machinelearning.cpp" )
    
sources = help.getRekusivFiles( os.path.join("..", "..", "java"), ".cpp")
sources.extend(framework)
targets.append( env.SharedLibrary( target=os.path.join("#build", "javalib", "native", "machinelearning"), source=sources ) )

# on OSX the path of the linked libraries within the libmachinelearning.dylib must be changed to @loader_path/<library>
if env["PLATFORM"].lower() == "darwin" :
    targets.append( env.Command("createlibrarynames", "", "otool -L " + os.path.join("build", "javalib", "native", env["LIBPREFIX"]+"machinelearning"+env["SHLIBSUFFIX"]) + " > " + os.path.join("build", "linkedlibs.txt") ) )
    targets.append( env.Command("linkedlibs", "", java_osxlinkedlibs) )
    targets.append( env.Command("libnames.txt", "", Delete(os.path.join("build", "linkedlibs.txt")) ) )


# copy external libraries in the native directory for Jar adding (copy works only if target directories exists)
dirs      = env["LIBPATH"]
libs      = []
copyfiles = []

libs.extend(env["LIBS"])
if env["PLATFORM"].lower() == "cygwin" :
    libs.extend(["cygwin1", "cyggcc_s-1", "cyggfortran-3", "cygstdc++-6"])

for n in libs :
    libname = ""
    if env["PLATFORM"].lower() == "darwin" or env["PLATFORM"].lower() == "posix" :
        libname = str(env["LIBPREFIX"]) + str(n) + str(env["SHLIBSUFFIX"])
    elif env["PLATFORM"].lower() == "cygwin" :
        libname = n + env["SHLIBSUFFIX"]

    libfiles = env.FindFile(libname, dirs)
    if libfiles <> None :
        # remove any symbolic names, so the file in the native directory becomes the original filename
        name = os.path.realpath(libfiles.path).split(os.path.sep)[-1]

        copyfiles.append( Copy(os.path.join("build", "javalib", "native", name), libfiles.path) )
targets.append( env.Command("copyexternallib", "", copyfiles) )

# on Linux all libraries must use the filename, that is set with the "soname" within the library or filename must be changed to "soname"
if env["PLATFORM"].lower() == "posix" :
    targets.append( env.Command("sonames", "", java_linuxsonames) )



# build Jar and create Jar Index
targets.append( env.Command("buildjar", "", "jar cf " + os.path.join("build", "machinelearning.jar") + " -C " + os.path.join("build", "javalib" ) + " .") )
targets.append( env.Command("buildjarindex", "", "jar i " + os.path.join("build", "machinelearning.jar") ) )

env.Alias("javac", targets)






# set classpath only for example compiling (jar file must be set within the build directory)
env.Alias("javareduce", env.Java(target=os.path.join("#build", "java", "reduce"), source=os.path.join("..", "..", "examples", "java", "reducing"), JAVACLASSPATH = [os.path.join("build", "machinelearning.jar")]) )
env.Alias("javautil", env.Java(target=os.path.join("#build", "java", "util"), source=os.path.join("..", "..", "examples", "java", "util"), JAVACLASSPATH = [os.path.join("build", "machinelearning.jar")]) )