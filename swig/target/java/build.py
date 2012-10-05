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


# build script for the Java port of the framework
# $LastChangedDate$

import os
import re
import platform
import subprocess
Import("*")



# library change for Mac OS X
def libchange_osx(source) :
    cmd = subprocess.Popen( "otool -L "+source, shell=True, stdout=subprocess.PIPE)
    output = cmd.stdout.readlines()
    cmd.communicate()
    if cmd.returncode <> 0 :
        raise SCons.Errors.UserError("error reading output")

    # get linked libs and change the path
    os.system("install_name_tool -id " + os.path.basename(source) + " " + source )
    for n in [i.strip().split(" ")[0] for i in output[2:]] :
        fname  = os.path.splitext(os.path.basename(n))
        fname  = re.sub(r"\.[0-9]+", "", fname[0])+fname[1]

        if os.path.isfile( os.path.join( os.path.dirname(source), fname) ) :
            os.system("install_name_tool -change " + n + " @loader_path" + os.path.sep + fname + " " + source)
            libchange_osx( os.path.join( os.path.dirname(source), fname) )


# linux change: os.system( "objdump -p " + pathfile + " | grep -i soname | awk '{system(\"mv " + pathfile + " "+nativepath+os.path.sep+"\"$2)}'" )


# builder command function that is run on the build JNI library for setup relative links
def libchange(target, source, env) :
    if platform.system().lower() == "posix" :
        pass
    elif platform.system().lower() == "linux" :
        pass
    elif platform.system().lower() == "darwin" :
        libchange_osx(str(source[0]))
    else :
        raise RuntimeError("platform not known")




cpp  = []

# glob all Swig files and call the builder
for i in GlobRekursiv( os.path.join("..", ".."), [".i"], ["target"]) :

    # split all result files into Java and C++ sources
    for n in env.SwigJava( os.path.join("#build", env["buildtype"], "jar", "source"), i ) :
        if os.path.splitext(str(n))[1] <> env["JAVASUFFIX"] :
            cpp.append(n)
        
# call Java & C++ builder
dll    = env.SharedLibrary( os.path.join("#build", env["buildtype"], "jar", "build", "native", "machinelearning"), defaultcpp + cpp )
java   = env.Java(  os.path.join("#build", env["buildtype"], "jar", "build"), os.path.join("#build", env["buildtype"], "jar", "source", "java")  )
libcp  = env.LibraryCopy( os.path.join("#build", env["buildtype"], "jar", "build", "native"), [] )
rellib = env.Command("rellib_dll", dll, libchange )
Depends(rellib, libcp)
    
# set Jar flags (we need a own command for the -C option)   
env["JARCOM"] = "$JAR $_JARFLAGS $TARGET $_JARMANIFEST $_JARCHDIR $_JARSOURCES -C " + os.path.join("build", env["buildtype"], "jar", "build") + " ."
jar = env.Jar(os.path.join("#build", env["buildtype"], "machinelearning.jar"), [])
Depends(jar, [rellib, java])

# set Alias with Jar build
env.Clean(
    env.Alias( "java", jar ),
    os.path.join("#build", env["buildtype"], "jar")
)