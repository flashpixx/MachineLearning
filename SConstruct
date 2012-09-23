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
import re
import sys
import urllib2
import platform
import subprocess
import SCons.Action
import SCons.Node



#=== CLI parameters ====================================================================================================================
def createVariables(vars) :
    vars.Add(BoolVariable("withrandomdevice", "installation with random device support", False))
    vars.Add(BoolVariable("withmpi", "installation with MPI support", False))
    vars.Add(BoolVariable("withmultilanguage", "installation with multilanguage support", False))
    vars.Add(BoolVariable("withsources", "installation with source like nntp or something else", False))
    vars.Add(BoolVariable("withfiles", "installation with file reading support for CSV & HDF", True))
    vars.Add(BoolVariable("withlogger", "use the interal logger of the framework", False))
    vars.Add(BoolVariable("withsymbolicmath", "compile for using symbolic math expression (needed by gradient descent)", False))
    
    vars.Add(EnumVariable("buildtype", "value of the buildtype", "release", allowed_values=("debug", "release")))
    vars.Add(BoolVariable("uselocallibrary", "use the library in the local directory only", False))
    vars.Add(ListVariable("skiplibrary", "skipping library builds / downloads", ["xml"], ["atlas", "boost", "hdf", "ginac", "json", "xml"]))
    vars.Add(BoolVariable("zipsupport", "build Bzip2 and ZLib support for Boost", (platform.system().lower()=="cygwin")))
    vars.Add(EnumVariable("atlaslink", "type of the ATLAS link file", "multi", allowed_values=("single", "multi")))
    vars.Add(EnumVariable("atlaspointerwidth", "pointer width for compiling ATLAS (empty = system default, 32 = 32 Bit, 64 = 64 Bit)", "", allowed_values=("", "32", "64")))
    

    
    
    
#=== build environment check ===========================================================================================================
def checkCPPEnv(conf, localconf) :
    if conf.env.GetOption("clean") :
        return
    for i in ["documentation", "librarybuild", "librarydownload", "language"] :
        if i in COMMAND_LINE_TARGETS :
            return
            

    if not conf.CheckCXX() :
        sys.exit(1)
        
    # check data (header & libraries)
    for i in localconf["cheaders"] :
        if not conf.CheckCHeader(i) :
            sys.exit(1)
            
    for i in localconf["cppheaders"] :
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
    if platform.system().lower() == "posix" :
        env["TOOLKIT"]      = "posix"
    elif platform.system().lower() == "darwin" :
        env["TOOLKIT"]      = "darwin"
    elif platform.system().lower() == "cygwin" :
        env["PLATFORM"]     = "cygwin"
    else :
        raise RuntimeError("toolkit not known")

    # adding OS environment data
    if "PATH" in os.environ :
        laPathList = env["ENV"]["PATH"].split(os.pathsep)
        laPathList.extend(os.environ["PATH"].split(os.pathsep))
        env["ENV"]["PATH"] = laPathList
        print("Appending custom path (PATH)")


def GlobRekursiv(startdir, extensions=[], excludedir=[]) :
    lst = []
    if not extensions :
        return lst
    for root, dirs, filenames in os.walk(startdir) :
        if not [i.startswith(".") for i in dirs]  or  any([i in root for i in excludedir]) :
            continue
        for filename in filenames :
            if any([filename.endswith(i) for i in extensions]) :
                lst.append( os.path.abspath(os.path.join(root, filename)) )
    return lst


#===  builder ============================================================================================================
def url_print(s, target, source, env) : 
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



def extract_print(s, target, source, env) : 
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



def swigjava_print(s, target, source, env) : 
    print "=> swig ["+str(source[0])+"] ..."
    
def swigjava_emitter(target, source, env) :
    listtargets = []

    regex = {
          # remove expression of the interface file (store a list with expressions)
          "remove"              : [ re.compile( r"#ifdef SWIGPYTHON(.*)#endif", re.DOTALL ) ],
          
          # regex for extracting data of the interface file
          "template"            : re.compile( r"%template(.*);" ),
          "include"             : re.compile( r"%include \"(.*\.h.?.?)\"" ),
          "rename"              : re.compile( r"%rename\((.*)\)(.*)" ),
          "module"              : re.compile( r"%module \"(.*)\"" ),
          
          # regex for C++ comments
          "cppcomment"          : re.compile( r"//.*?\n|/\*.*?\*/", re.DOTALL ),
          
          # regex for the C++ class name
          "cppclass"            : re.compile( r"class (.*)" ),
          "cppbaseclass"        : re.compile( r":(.*)" ),
          "cppnamespace"        : re.compile( r"namespace(.*){" ),
          "cppstaticfunction"   : re.compile( r"static (.*) (.*)\(" ), 
          
          # regex helpers
          "cppremove"           : re.compile( r"(\(|\)|<(.*)>)" )
    }

    for input in source :
        # read source file
        oFile = open( str(input), "r" )
        ifacetext = oFile.read()
        oFile.close()
        
        #path of the source file and remove reduce the interface file for parsing
        ifacepath = os.sep.join( str(input).split(os.sep)[0:-1] )
        for n in regex["remove"] :
            ifacetext = re.sub(n, "", ifacetext)
        
        #getting all needed informations if the interface file
        data = {
                 # stores the fill source filepath
                 "source"            : str(input),
                 # stores the source file name
                 "sourcename"        : (str(input).split(os.sep)[-1]).replace(".i", ""),
                 # stores the template parameter with a dict like { templatename : [cpp classname, namespace of the cpp class] or [ static function name, cpp class name]              
                 "template"          : re.findall(regex["template"], ifacetext),
                 # stores a rename structure { cpp classname : target name }
                 "rename"            : re.findall(regex["rename"], ifacetext),
                 # stores the module name
                 "module"            : re.findall(regex["module"], ifacetext),
                 # stores all %include files
                 "include"           : re.findall(regex["include"], ifacetext),
                 # stores a dict of cpp namespaces
                 "cppnamespace"      : {},
                 # stores a list of cpp classnames
                 "cppclass"          : [],
                 # stores a list of static function names
                 "cppstaticfunction" : []
        }
        
        # getting C++ class name (read the %include file name)
        # [bug: if a class with the same name exists in different namespaces, the dict stores only the last namespace entry]
        # [bug: namespace and target directory that is extracted by the builder can be different]
        for n in data["include"] :
        
            # read cpp data
            oFile   = open( os.path.normpath(os.path.join(ifacepath, n)), "r" )
            cpptext = re.sub(regex["cppcomment"], "", oFile.read()) 
            oFile.close()
            
            # get class names and static function
            classnames = re.findall(regex["cppclass"], cpptext)
            classnames = [ re.sub(regex["cppbaseclass"], "", k).strip() for k in classnames ]
            data["cppstaticfunction"].extend( [ k[1] for k in re.findall(regex["cppstaticfunction"], cpptext) ] ) 

            # determine class and namespace connection (which class exists in which namespace)
            namespaces = re.findall(regex["cppnamespace"], cpptext)
            nslist     = [ n.replace("{", "").split("namespace") for n in namespaces ]
            nslist     = [ [ k.strip() for k in i ] for i in nslist ]
            
            for k in classnames :
                for l,j in enumerate(namespaces) :
                    if re.search( re.compile( r"namespace" + j + "{(.*)class " + k, re.DOTALL ), cpptext ) :
                        data["cppnamespace"][k] = nslist[l]
            
            # add classnames ti the dict
            data["cppclass"].extend( classnames  )



        # create the remap dict { cpp class name : swig rename}
        help = {}
        for targetname,sourcename in data["rename"] :
            help[sourcename.replace(";","").strip()] = targetname.strip()
        data["rename"]            = help
        
        # remove template parameters <> or brackets
        data["template"]          = [ re.sub(regex["cppremove"], "", i) for i in data["template"] ]
        help = {}
        for n in data["template"] :
            k = n.split()
            help[k[0]] = k[1].split("::")[-2:]
        data["template"]          = help
        
        # create a unique list of the static functions
        noDupes = []
        [noDupes.append(i) for i in data["cppstaticfunction"] if not noDupes.count(i)]
        data["cppstaticfunction"] = noDupes



        # determine classname of each template parameter and
        # change the template parameter in this way, that we get a dict with { cpp class name : [target names] }
        help = {}
        for k,v in data["template"].items() :
            newval = list(set(data["cppclass"]) & set(v))[0]
            if help.has_key(newval) :
                help[newval].append(k)
            else :
                help[newval] = [k]
        data["template"] = help
        
        print data

        
    return listtargets, source
    
SwigJavaBuilder = Builder( action = SCons.Action.Action("swig -Wall -O -templatereduce -c++ -java -package ??? -outdir ??? -o ${SOURCE.filebase}.cpp $SOURCE"), emitter=swigjava_emitter, single_source = True, src_suffix=".i", target_factory=Dir, source_factory=File, PRINT_CMD_LINE_FUNC=swigjava_print )

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

env = Environment( variables=vars, tools = ["default", "gettext"], BUILDERS = { "SwigJava" : SwigJavaBuilder, "Download" : DownloadBuilder, "Extract" : ExtractBuilder } )
env.VariantDir("build", ".", duplicate=0)
Help(vars.GenerateHelpText(env))
setupToolkitEnv(env)
conf = Configure(env)


# changing flags if needed
if "sources" in COMMAND_LINE_TARGETS : 
    conf.env["withsources"] = True;

# read platform configuration (only if not clean target is used)
platformconfig = env["TOOLKIT"]
if not(os.path.isfile(os.path.join("scons", "platform", platformconfig+".py"))) :
    raise ImportError("toolkit configuration script ["+platformconfig+"] not found")

env.SConscript( os.path.join("scons", "platform", platformconfig+".py"), exports="conf checkCPPEnv checkExecutables" )
env = conf.Finish()


# main cpp must compiled in
defaultcpp = [os.path.join(os.path.abspath(os.curdir), "machinelearning.cpp")]

# setup all different sub build script
env.SConscript( os.path.join("tools", "language", "build.py"), exports="env defaultcpp GlobRekursiv" )
env.SConscript( os.path.join("documentation", "build.py"), exports="env defaultcpp" )
env.SConscript( os.path.join("library", "build.py"), exports="env defaultcpp" )

env.SConscript( os.path.join("swig", "target", "java", "build.py"), exports="env defaultcpp GlobRekursiv" )
#env.SConscript( os.path.join("swig", "target", "python", "SConscript"), exports="env defaultcpp" )
#env.SConscript( os.path.join("swig", "target", "php", "SConscript"), exports="env defaultcpp" )

for i in ["geneticalgorithm", "classifier", "clustering", "distance", "other", "reducing", "sources"] :
    env.SConscript( os.path.join("examples", i, "build.py"), exports="env defaultcpp" )
