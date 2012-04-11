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
import glob
import os
import re

Import("*")


# change the library calls under OSX
def java_osxlinkedlibs(target, source, env) :
    oFile = open( os.path.join("build", "java", "jar", "linkedlibs.txt"), "r" )
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
    os.system( "install_name_tool " + " ".join(change) + " " + os.path.join("build", "java", "jar", "lib", "native", env["LIBPREFIX"]+"machinelearning"+env["SHLIBSUFFIX"]) )
    return []


# changes the library calls under Linux (sonames)
def java_linuxsonames(target, source, env) :

    # read all files within the native directory and get the SONAME of each library and change the filename
    nativepath = os.path.join("build", "java", "jar", "lib", "native")
    for root, dirs, filenames in os.walk(nativepath) :
        for filename in filenames :
            pathfile = os.path.join(nativepath, filename)
    	    os.system( "objdump -p " + pathfile + " | grep -i soname | awk '{system(\"mv " + pathfile + " "+nativepath+os.path.sep+"\"$2)}'" )

    return []




# ----------------------------------------------------------------------------------------------------------------------------------------------------------
targets = []
"""
#read all *.i files and call swig for generating Java and Cpp files
cppsources = []
interfaces = help.getRekusivFiles( os.path.join("..", "..", "swig", "machinelearning"), ".i")
targets.append( env.Command( "javalibdir", "", Mkdir(os.path.join("build", "java", "jar", "lib")) ) )
targets.append( env.Command( "javalibsrcdir", "", Mkdir(os.path.join("build", "java", "jar", "javasrc")) ) )
targets.append( env.Command( "libdir", "", Mkdir(os.path.join("build", "java", "jar", "nativesrc")) ) )

for i in interfaces :
    package    = ".".join(i.split(os.sep)[3:-1])
    cppname    = os.path.join("build", "java", "jar", "nativesrc", os.path.splitext(i.split(os.sep)[-1])[0] + ".cpp")
    javatarget = os.path.join("build", "java", "jar", "javasrc", package.replace(".", os.sep))
    ifacename  = os.sep.join(i.split(os.sep)[2:])
    
    cppsources.append( os.path.join("..", "..", cppname) )
    
    #create target directories for Jar and call Swig
    targets.append( env.Command( "java"+package, "", Mkdir(javatarget) ) )
    targets.append( env.Command( "swigjava"+ifacename, "", "swig -Wall -O -c++ -java -package " + package + " -outdir " + javatarget + " -o " + cppname + " " + ifacename ) )
    
    # read on each interface file the %module part and remove this Java class, because it is an empty class
    oFile     = open( os.path.join("..", "..", ifacename), "r" )
    ifacetext = oFile.read()
    oFile.close()

    found = re.search("%module(.*)\"", ifacetext)
    if found <> None :
        targets.append( env.Command("delmodul"+ifacename, "", Delete(os.path.join(javatarget, found.group(1).replace("\"", "").strip() + ".java"))) )
    


#get CPPs and build native library and Java classes
if env["withlogger"] or env["withrandomdevice"] :
    cppsources.append( "machinelearning.cpp" )
targets.append( env.SharedLibrary( target=os.path.join("#build", "java", "jar", "lib", "native", "machinelearning"), source=cppsources ) )
targets.append( env.Java( target=os.path.join("#build", "java", "jar", "lib"), source=os.path.join("#build", "java", "jar", "javasrc") ) )



# on OSX the path of the linked libraries within the libmachinelearning.dylib must be changed to @loader_path/<library>
if env["PLATFORM"].lower() == "darwin" :
    targets.append( env.Command("createlibrarynames", "", "otool -L " + os.path.join("build", "java", "jar", "lib", "native", env["LIBPREFIX"]+"machinelearning"+env["SHLIBSUFFIX"]) + " > " + os.path.join("build", "java", "jar", "linkedlibs.txt") ) )
    targets.append( env.Command("linkedlibs", "", java_osxlinkedlibs) )
  

# copy external libraries in the native directory for Jar adding (copy works only if target directories exists)
dirs      = []
if env.has_key("LIBPATH") :
    dirs.extend(env["LIBPATH"])

libs      = []
if env.has_key("LIBS") :
    libs.extend(env["LIBS"])

if env["PLATFORM"].lower() == "cygwin" :
    libs.extend(["cygwin1", "cyggcc_s-1", "cyggfortran-3", "cygstdc++-6"])

copyfiles = []
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

        copyfiles.append( Copy(os.path.join("build", "java", "jar", "lib", "native", name), libfiles.path) )
targets.append( env.Command("copyexternallib", "", copyfiles) )

# on Linux all libraries must use the filename, that is set with the "soname" within the library or filename must be changed to "soname"
if env["PLATFORM"].lower() == "posix" :
    targets.append( env.Command("sonames", "", java_linuxsonames) )

# copy licence
targets.append( env.Command("license", "", Copy(os.path.join("build", "java", "jar", "lib", "license.txt"), "license.txt")) )


#build Jar
targets.append( env.Command("buildjar", "", "jar cf " + os.path.join("build", "machinelearning.jar") + " -C " + os.path.join("build", "java", "jar", "lib" ) + " .") )
targets.append( env.Command("buildjarindex", "", "jar i " + os.path.join("build", "machinelearning.jar") ) )
"""

# create directory structures and targets
#targets.append( env.Command( "jarnativesources", "", Mkdir(os.path.join("build","jar","nativesource")) ) ) 

#targetlists = []
#for i in help.getRekusivFiles( os.path.join("..", "..", "swig", "machinelearning"), ".i") :

"""
    dir = os.path.sep.join( i.split(os.path.sep)[3:-1] )
    if not(dir in targetlists) :
        targetlists.append(dir)
        #targets.append( env.Command( "jarsources"+dir, "", Mkdir(os.path.join("build","jar","javasource", dir)) ) ) 
        jsources = env.Java(os.path.join("..", "..", "build","jar","lib"), Glob(os.path.join("..", "..", "swig", dir, "*.i")), SWIGCOM='$SWIG -o '+os.path.join("build","jar","nativesource")+os.path.sep+'${TARGET.file} -outdir "'+os.path.join("build", "jar", "javasource", dir)+'" ${_SWIGINCFLAGS} $SWIGFLAGS $SOURCES', SWIGOUTDIR=os.path.join("..","..","build", "jar", "javasource", dir), SWIGCXXFILESUFFIX=".cpp", SWIGFLAGS=["-O", "-templatereduce", "-c++", "-java","-package",dir.replace(os.path.sep, ".")] )
        
        for n in jsources :
            n._createDir()
            targets.append(n)
"""
    
def SwigJava(target, source, env) :
    # create build dir path
    builddir = os.path.join("build", "jar")
    
    # create all needed directories
    try :
        os.makedirs( os.path.join(builddir, "nativesource") )
    except :
        pass

    # iterate over all source files
    for i in source :
    
        # get target directory (Java package directories) and cpp file name
        dir = os.path.sep.join( str(i).split(os.path.sep)[1:-1] )
        cpp = ( str(i).split(os.path.sep)[-1] ).replace(".i", ".cpp")
        
        # create Java package directory
        try :
            os.makedirs( os.path.join(builddir, "javasource", dir) )
        except :
            pass
            
        # call swig command
        os.system("swig -Wall -O -templatereduce -c++ -java -package " + dir.replace(os.path.sep, ".") + " -outdir " + os.path.join(builddir, "javasource", dir) + " -o " + os.path.join(builddir, "nativesource", cpp) + " " + str(i))
        
        # delete empty java classes
        delfiles = glob.glob(os.path.join(builddir, "javasource", dir, "*wrap.java"));
        delfiles.extend(glob.glob(os.path.join(builddir, "javasource", dir, "*module.java")))
        for n in delfiles :
            os.unlink(n)



def SwigJar(target, source, env) :
    os.system( "jar cf " + os.path.join("build", "machinelearning.jar") + " -C " + os.path.join("build", "jar", "lib" ) + " .")
    os.system( "jar i " + os.path.join("build", "machinelearning.jar") )






env.Append( BUILDERS = {"SwigJava" : Builder(action = SwigJava) } )
env.Append( BUILDERS = {"SwigJar" : Builder(action = SwigJar) } )


swigbuild = env.SwigJava( help.getRekusivFiles( os.path.join("..", "..", "swig", "machinelearning"), ".i") )
Depends(env.Java( os.path.join("..", "..", "build", "jar", "lib"), os.path.join("..", "..", "build", "jar", "javasource") ), swigbuild) 

cppsources = Glob(os.path.join("build", "jar", "nativesource", "*.cpp"))
if env["withlogger"] or env["withrandomdevice"] :
    cppsources.append( "machinelearning.cpp" )

cppbuild = env.SharedLibrary( target=os.path.join("#build", "jar", "lib", "native", "machinelearning"), source=cppsources )
Depends(cppbuild, swigbuild)

env.Alias("javac", jarbuild)






# set classpath only for example compiling (jar file must be set within the build directory)
#env.Alias("javareduce", env.Java(target=os.path.join("#build", "java", "reduce"), source=os.path.join("..", "..", "examples", "java", "reducing"), JAVACLASSPATH = [os.path.join("build", "machinelearning.jar")]) )
#env.Alias("javaclustering", env.Java(target=os.path.join("#build", "java", "clustering"), source=os.path.join("..", "..", "examples", "java", "clustering"), JAVACLASSPATH = [os.path.join("build", "machinelearning.jar")]) )
#env.Alias("javautil", env.Java(target=os.path.join("#build", "java", "util"), source=os.path.join("..", "..", "examples", "java", "util"), JAVACLASSPATH = [os.path.join("build", "machinelearning.jar")]) )
