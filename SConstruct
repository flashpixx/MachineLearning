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
import sys
import platform
sys.path.append("scons")
import help
import colorama


#=== CLI parameters ====================================================================================================================
def createVariables(vars) :
    vars.Add(BoolVariable("withrandomdevice", "installation with random device support", False))
    vars.Add(BoolVariable("withmpi", "installation with MPI support", False))
    vars.Add(BoolVariable("withmultilanguage", "installation with multilanguage support", False))
    vars.Add(BoolVariable("withsources", "installation with source like nntp or something else", False))
    vars.Add(BoolVariable("withfiles", "installation with file reading support for CSV & HDF", True))
    vars.Add(BoolVariable("withlogger", "use the interal logger of the framework", False))
    vars.Add(BoolVariable("withsymbolicmath", "compile for using symbolic math expression (needed by gradient descent)", False))

    vars.Add(BoolVariable("withdebug", "compile with debug information", False))
    vars.Add(BoolVariable("withoptimize", "compile with CPU optimization code", True))
    vars.Add(EnumVariable("math", "optimization of math structure", "sse3", allowed_values=("sse3", "sse", "387")))

    vars.Add(EnumVariable("atlaslink", "value of the atlas threadding (multi = tatlas, single = satlas)", "multi", allowed_values=("multi", "single")))
    vars.Add(BoolVariable("staticlink", "libraries will linked static", True))

    vars.Add(EnumVariable("cputype", "value of the cpu type [see: http://gcc.gnu.org/onlinedocs/gcc/i386-and-x86_002d64-Options.html]", "native", allowed_values=("native", "generic", "i386", "i486", "i586", "i686", "pentium-mmx", "pentiumpro", "pentium2", "pentium3", "pentium-m", "pentium4", "prescott", "nocona", "core2", "corei7", "corei7-avx", "core-avx-i", "atom", "k6", "k6-2", "athlon", "athlon-4", "k8", "k8-sse3", "amdfam10", "winchip-c6", "winchip2", "c3", "c3-2", "geode" )))
    
    vars.Add(EnumVariable("atlaspointerwidth", "pointer width for compiling ATLAS (empty = system default, 32 = 32 Bit, 64 = 64 Bit)", "", allowed_values=("", "32", "64")))
    vars.Add(BoolVariable("atlascputhrottle", "enable / disable detection of CPU throtteling", False))
    
    libs = ["atlas", "boost", "hdf", "ginac", "json"]
    if "cygwin" in platform.system().lower() :
        vars.Add(EnumVariable("winver", "value of the Windows version", "win7", allowed_values=("win7", "srv2008", "vista", "srv2003sp1", "xpsp2", "srv2003", "xp", "w2000")))
        libs.append("xml")
    vars.Add(ListVariable("skipbuild", "skipping library builds", "", libs))
    vars.Add(BoolVariable("skipbuilderror", "ignore / skip build errors", False))
    

#=== clean targets =====================================================================================================================
def cleantarget(env) :
    files = []
    
    files.extend( help.getRekusivFiles(os.curdir, env["OBJSUFFIX"]) )
    files.extend( help.getRekusivFiles(os.curdir, env["SHOBJSUFFIX"]) )
    files.extend( help.getRekusivFiles(os.curdir, env["SHLIBSUFFIX"]) )
    files.extend( help.getRekusivFiles(os.curdir, ".po~") )
    files.extend( help.getRekusivFiles(os.curdir, ".mo") )
    files.extend( help.getRekusivFiles(os.curdir, ".jar") )
    files.extend( help.getRekusivFiles(os.curdir, ".stackdump") )
    files.extend( help.getRekusivFiles(os.curdir, ".core") )
    files.extend( help.getRekusivFiles(os.curdir, ".pyc") )
    files.extend( help.getRekusivFiles(os.curdir, ".bz2") )
    files.extend( help.getRekusivFiles(os.curdir, ".gz") )
    files.extend( help.getRekusivFiles(os.curdir, ".tgz") )

    # don't add the current path, because scons run the directories until "/"
    files.extend( ["build", os.path.join("library", "build")] )

    env.Clean("clean", files)

    
#=== licence ===========================================================================================================================
def showlicence() :
    print "############################################################################"
    print "# LGPL License                                                             #"
    print "#                                                                          #"
    print "# This file is part of the Machine Learning Framework.                     #"
    print "# Copyright (c) 2010, Philipp Kraus, <philipp.kraus@flashpixx.de>          #"
    print "# This program is free software: you can redistribute it and/or modify     #"
    print "# it under the terms of the GNU Lesser General Public License as           #"
    print "# published by the Free Software Foundation, either version 3 of the       #"
    print "# License, or (at your option) any later version.                          #"
    print "#                                                                          #"
    print "# This program is distributed in the hope that it will be useful,          #"
    print "# but WITHOUT ANY WARRANTY; without even the implied warranty of           #"
    print "# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #"
    print "# GNU Lesser General Public License for more details.                      #"
    print "#                                                                          #"
    print "# You should have received a copy of the GNU Lesser General Public License #"
    print "# along with this program. If not, see <http://www.gnu.org/licenses/>.     #"
    print "############################################################################"
    print "\n\n"


#=== create environment and compiling ==================================================================================================
colorama.init()
showlicence()

#detect if a space is within the current path
if " " in os.path.abspath(os.curdir) : 
    print colorama.Style.BRIGHT + "Warning: The path contains spaces, it is recommand to use a path without spaces\n" + colorama.Style.RESET_ALL

vars = Variables()
createVariables(vars)

env = Environment(variables=vars)
Help(vars.GenerateHelpText(env))
# Scons < 2: env.BuildDir("build", ".", duplicate=0)
env.VariantDir("build", ".", duplicate=0)
if os.environ.has_key("PATH") :
    env["ENV"]["PATH"] = [env["ENV"]["PATH"], os.environ["PATH"]]
cleantarget(env)

# adding platform scripts
platformconfig = env["PLATFORM"].lower()
if not(os.path.isfile(os.path.join("scons", "platform", platformconfig+".py"))) :
    raise ImportError("platform configuration script ["+platformconfig+"] not found")
    
env.SConscript( os.path.join("scons", "platform", platformconfig+".py"), exports="env" )

# adding the main path (parent of the current directory) to the CPPPATH and uniquify each option, 
# that is setup with data of the system environment
if env.has_key("CPPPATH") :
    env.AppendUnique(CPPPATH = [os.path.abspath(os.curdir)])
if env.has_key("CXXFLAGS") :
    env.AppendUnique(CXXFLAGS = [env["CXXFLAGS"]])
if env.has_key("LINKFLAGS") :    
    env.AppendUnique(LINKFLAGS = [env["LINKFLAGS"]])
if env.has_key("LIBS") :
    env.AppendUnique(LIBS = [env["LIBS"]])
if env.has_key("LIBPATH") :
    env.AppendUnique(LIBPATH = [env["LIBPATH"]])
    
# set manually if needed the MPI C++ compiler
if env["withmpi"] :
    env.Replace(CXX = "mpic++")
    
# main cpp must compiled in
defaultcpp = []
if env["withlogger"] or env["withrandomdevice"] :
    defaultcpp.append( "machinelearning.cpp" )
    
#adding configuration checks http://www.scons.org/wiki/SconsAutoconf


# setup all different sub build script
env.SConscript( os.path.join("tools", "language", "SConscript"), exports="env colorama defaultcpp help" )
env.SConscript( os.path.join("documentation", "SConscript"), exports="env colorama defaultcpp help" )
env.SConscript( os.path.join("library", "SConscript"), exports="env colorama defaultcpp help" )

env.SConscript( os.path.join("swig", "target", "java", "SConscript"), exports="env colorama defaultcpp help" )
env.SConscript( os.path.join("swig", "target", "python", "SConscript"), exports="env colorama defaultcpp help" )
#env.SConscript( os.path.join("swig", "target", "php", "SConscript"), exports="env colorama defaultcpp help" )

for i in ["geneticalgorithm", "classifier", "clustering", "distance", "other", "reducing", "sources"] :
    env.SConscript( os.path.join("examples", i, "SConscript"), exports="env colorama defaultcpp help" )
