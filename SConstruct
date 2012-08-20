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
import sys
import urllib2
import platform
import subprocess
import SCons.Action
import SCons.Node
sys.path.append("scons")
import help



#=== CLI parameters ====================================================================================================================
def createVariables(vars) :
    vars.Add(BoolVariable("withrandomdevice", "installation with random device support", False))
    vars.Add(BoolVariable("withmpi", "installation with MPI support", False))
    vars.Add(BoolVariable("withmultilanguage", "installation with multilanguage support", False))
    vars.Add(BoolVariable("withsources", "installation with source like nntp or something else", False))
    vars.Add(BoolVariable("withfiles", "installation with file reading support for CSV & HDF", True))
    vars.Add(BoolVariable("withlogger", "use the interal logger of the framework", False))
    vars.Add(BoolVariable("withsymbolicmath", "compile for using symbolic math expression (needed by gradient descent)", False))

    #vars.Add(BoolVariable("withdebug", "compile with debug information", False))
    #vars.Add(BoolVariable("withframeworkdebug", "compile with frameworks debug option", True)) #-- move to debug
    vars.Add(BoolVariable("withoptimize", "compile with CPU optimization code", True))
    vars.Add(EnumVariable("math", "optimization of math structure", "sse3", allowed_values=("sse3", "sse", "387"))) # ------
    
    vars.Add(EnumVariable("buildtype", "value of the buildtype", "release", allowed_values=("debug", "release")))

    vars.Add(EnumVariable("atlaslink", "value of the atlas threadding (multi = tatlas, single = satlas)", "multi", allowed_values=("multi", "single"))) #----??
    vars.Add(BoolVariable("staticlink", "libraries will linked static", False)) #-----??
    #vars.Add(BoolVariable("showconfig", "shows the environment configuration", False)) #------

    vars.Add(EnumVariable("cputype", "value of the cpu type [see: http://gcc.gnu.org/onlinedocs/gcc/i386-and-x86_002d64-Options.html]", "native", allowed_values=("native", "generic", "i386", "i486", "i586", "i686", "pentium-mmx", "pentiumpro", "pentium2", "pentium3", "pentium-m", "pentium4", "prescott", "nocona", "core2", "corei7", "corei7-avx", "core-avx-i", "atom", "k6", "k6-2", "athlon", "athlon-4", "k8", "k8-sse3", "amdfam10", "winchip-c6", "winchip2", "c3", "c3-2", "geode" ))) #------
    
    vars.Add(EnumVariable("atlaspointerwidth", "pointer width for compiling ATLAS (empty = system default, 32 = 32 Bit, 64 = 64 Bit)", "", allowed_values=("", "32", "64")))
    
    vars.Add(BoolVariable("uselocallibrary", "use the library in the local directory only", False))
    vars.Add(ListVariable("skiplibrary", "skipping library builds / downloads", "", ["atlas", "boost", "hdf", "ginac", "json", "xml"]))
    vars.Add(BoolVariable("uselocallibrary", "use the library in the local directory only", False))
    
    
    
#=== build environment check ===========================================================================================================
def checkCPPEnv(conf, localconf) :
    if conf.env.GetOption("clean") :
        return
    for i in ["documentation", "librarybuild", "librarydownload", "createlanguage", "updatelanguage"] :
        if i in COMMAND_LINE_TARGETS :
            return
            

    if not conf.CheckCXX() :
        sys.exit(1)
        
    # check data (header & libraries)
    for i in localconf["cheader"] :
        if not conf.CheckCHeader(i) :
            sys.exit(1)
            
    for i in localconf["cppheader"] :
        if not conf.CheckCXXHeader(i) :
            sys.exit(1)

    for i in localconf["clibraries"] :
        if not conf.CheckLib(i, language="C") :
            sys.exit(1)
        
    for i in localconf["cpplibraries"] :
        if not conf.CheckLib(i, language="C++") :
            sys.exit(1)
            
    # set the colorgcc prefix after checkig, because otherwise it can creates errors on checklibrary
    lxPath = conf.env.FindFile("color-"+conf.env["CXX"], conf.env["ENV"]["PATH"])
    if lxPath <> None :
        conf.env.Replace(CXX = "color-"+conf.env["CXX"])
    lxPath = conf.env.FindFile("color-"+conf.env["CC"], conf.env["ENV"]["PATH"])
    if lxPath <> None :
        conf.env.Replace(CC = "color-"+conf.env["CC"])

    
def checkExecutables(conf, commands) :
    cmd = commands
    if type(cmd) <> type([]) :
        cmd = [cmd]
    for i in cmd :
        if env.FindFile(i, conf.env["ENV"]["PATH"]) == None :
            raise RuntimeError("build tool ["+i+"] not found")

def setupToolkitEnv(env) :
    # check the toolkit option
    env["TOOLKIT_ARCH"] = (platform.architecture()[0]).replace("bit", "")
    if env["PLATFORM"].lower() == "posix" :
        env["TOOLKIT"]      = "posix"
    elif env["PLATFORM"].lower() == "darwin" :
        env["TOOLKIT"]      = "darwin"
    elif env["PLATFORM"].lower() == "cygwin" :
        env["PLATFORM"]     = "cygwin"
    else :
        raise RuntimeError("toolkit not known")

    # adding OS environment data
    if "PATH" in os.environ :
        laPathList = env["ENV"]["PATH"].split(os.pathsep)
        laPathList.extend(os.environ["PATH"].split(os.pathsep))
        env["ENV"]["PATH"] = laPathList
        print("Appending custom path (PATH)")

    if "TERM" in os.environ :
        env["ENV"]["TERM"] = os.environ["TERM"]
        print("Using term environment variable (TERM)")

    if "HOME" in os.environ :
        env["ENV"]["HOME"] = os.environ["HOME"]
        print("Using home environment variable (HOME)")



#===  builder ============================================================================================================
def url_print(s, target, source, env): 
    print "downloading ["+str(source[0])+"] to ["+str(target[0])+"] ..."

def url_downloadfile(target, source, env) :
    stream = urllib2.urlopen( str(source[0]) )
    file   = open( str(target[0]), "wb" )
    file.write(stream.read())
    file.close()
    stream.close()
    return None
    
def ParseAndDownload(env, parsefunction=None):
    if not(callable(parsefunction)) :
        raise SCons.Errors.UserError("parameter is a ['%s'] not a function " % parsefunction.__class__)

    [url, filename] = parsefunction()
    if (type(url) <> type("")) or (type(filename) <> type("")) or not(url) or not(filename):
        raise SCons.Errors.UserError("return parameter of the function must be string and need not be empty")
    
    return env.Download( filename, url )

AddMethod(Environment, ParseAndDownload)
DownloadBuilder = Builder( action = url_downloadfile, single_source = True, target_factory=File, source_factory=Value, PRINT_CMD_LINE_FUNC=url_print )



def extract_print(s, target, source, env): 
    print "extracting ["+str(source[0])+"] ..."
    
def extract_emitter(target, source, env) :
    listtargets = []
    if not os.path.isfile( str(source[0]) ) :
        return listtargets, source
    
    # running subprocess and check the return
    cmd         = subprocess.Popen( env["EXTRACT_LIST"].replace("$SOURCE",str(source[0])), shell=True, stdout=subprocess.PIPE)
    output      = cmd.stdout.readlines()
    cmd.communicate()
    if cmd.returncode <> 0 :
        raise SCons.Errors.UserError("error on running list archive")
    
    parsefunction = env["EXTRACT_PARSE"]
    if not(callable(parsefunction)) :
        raise SCons.Errors.UserError("parse call is a ['%s'] not a function " % parsefunction.__class__)
    
    # parse data andd add the filenames to the target list
    for i in output :
        item = parsefunction(i)
        if item <> None :
            listtargets.append( item )
        
    return listtargets, source
       
ExtractBuilder = Builder( action = SCons.Action.Action("$EXTRACT_CMD$extractsuffix"), emitter=extract_emitter, single_source = True, src_suffix=".tar.gz", target_factory=Entry, source_factory=File, PRINT_CMD_LINE_FUNC=extract_print )



#=== licence ===========================================================================================================================
def showlicence() :
    print "############################################################################"
    print "# LGPL License                                                             #"
    print "#                                                                          #"
    print "# This file is part of the Machine Learning Framework.                     #"
    print "# Copyright (c) 2010-2012, Philipp Kraus, <philipp.kraus@flashpixx.de>     #"
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
showlicence()





# create configuration option
vars = Variables()
createVariables(vars)

env = Environment( variables=vars, BUILDERS = { "Download" : DownloadBuilder, "Extract" : ExtractBuilder } )
env.VariantDir("build", ".", duplicate=0)
Help(vars.GenerateHelpText(env))
setupToolkitEnv(env)
conf = Configure(env)

# read platform configuration (only if not clean target is used)
platformconfig = env["TOOLKIT"]
if not(os.path.isfile(os.path.join("scons", "platform", platformconfig+".py"))) :
    raise ImportError("toolkit configuration script ["+platformconfig+"] not found")

env.SConscript( os.path.join("scons", "platform", platformconfig+".py"), exports="conf checkCPPEnv checkExecutables" )

# changing falgs if needed
if "sources" in COMMAND_LINE_TARGETS : 
    conf.env["withsources"] = True;

env = conf.Finish()


# main cpp must compiled in
defaultcpp = []
if env["withlogger"] or env["withrandomdevice"] :
    defaultcpp.append( os.path.join(os.path.abspath(os.curdir), "machinelearning.cpp") )

# setup all different sub build script
env.SConscript( os.path.join("tools", "language", "SConscript"), exports="env defaultcpp help" )
env.SConscript( os.path.join("documentation", "SConscript"), exports="env defaultcpp help" )
env.SConscript( os.path.join("library", "SConscript"), exports="env defaultcpp help" )

env.SConscript( os.path.join("swig", "target", "java", "SConscript"), exports="env defaultcpp help" )
#env.SConscript( os.path.join("swig", "target", "python", "SConscript"), exports="env defaultcpp help" )
#env.SConscript( os.path.join("swig", "target", "php", "SConscript"), exports="env defaultcpp help" )

for i in ["geneticalgorithm", "classifier", "clustering", "distance", "other", "reducing", "sources"] :
    env.SConscript( os.path.join("examples", i, "SConscript"), exports="env defaultcpp help" )
