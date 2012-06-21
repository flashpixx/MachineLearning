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

import sys
sys.path.append("..")
import help
import os
import re

Import("*")



# ----------------------------------------------------------------------------------------------------------------------------------------------------------
targets = []

#read all *.i files and call swig for generating Python and Cpp files
cppsources = []
interfaces = help.getRekusivFiles( os.path.join("..", "..", "swig", "machinelearning"), ".i")
targets.append( env.Command( "pylibdir", "", Mkdir(os.path.join("build", "python", "module", "lib")) ) )
targets.append( env.Command( "pydir", "", Mkdir(os.path.join("build", "python", "module", "nativesrc")) ) )

for i in interfaces :
    package    = ".".join(i.split(os.sep)[3:-1])
    cppname    = os.path.join("build", "python", "module", "nativesrc", os.path.splitext(i.split(os.sep)[-1])[0] + ".cpp")
    pytarget = os.path.join("build", "python", "module", "lib", package.replace(".", os.sep))
    ifacename  = os.sep.join(i.split(os.sep)[2:])
    
    cppsources.append( os.path.join("..", "..", cppname) )
    
    #create target directories for Jar and call Swig
    targets.append( env.Command( "swig"+package, "", Mkdir(pytarget) ) )
    targets.append( env.Command( "swigpy"+ifacename, "", "swig -Wall -O -c++ -python -outdir " + pytarget + " -o " + cppname + " " + ifacename ) )
    
# add to each directory in the package directory a file with name __init__.py
for dirname, dirnames, filenames in os.walk(os.path.join("build", "python", "module", "lib")):
    for subdirname in dirnames:
        print os.path.join(dirname, subdirname)    

#get CPPs and build native library
if env["withlogger"] or env["withrandomdevice"] :
    cppsources.append( "machinelearning.cpp" )
#targets.append( env.SharedLibrary( target=os.path.join("#build", "python", "module", "lib", "native", "machinelearning"), source=cppsources ) )


env.Alias("python", targets)

