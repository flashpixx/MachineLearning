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


# main build script, that sets the script parameters, detects the build toolkit,
# sets the builders / emitters and collect all sub build scripts
# $LastChangedDate$

import os
import re
import sys
import shutil
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
    vars.Add(BoolVariable("usedistcc", "use distributed compiling with DistCC", False))
    vars.Add(ListVariable("skiplibrary", "skipping library builds / downloads", "", ["atlas", "boost", "hdf", "ginac", "json", "xml"]))
    vars.Add(BoolVariable("copylibrary", "copy the dynamic link libraries into the build dir", False))
    
    if platform.system().lower().split("_")[0] == "cygwin" :
        vars.Add(BoolVariable("zipsupport", "build Bzip2 and ZLib support for Boost", True))
        vars.Add(EnumVariable("atlaslink", "type of the ATLAS link file", "single", allowed_values=("single", "multi")))
    else :
        vars.Add(BoolVariable("zipsupport", "build Bzip2 and ZLib support for Boost", False))
        vars.Add(EnumVariable("atlaslink", "type of the ATLAS link file", "multi", allowed_values=("single", "multi")))
    vars.Add(EnumVariable("atlaspointerwidth", "pointer width for compiling ATLAS (empty = system default, 32 = 32 Bit, 64 = 64 Bit)", "", allowed_values=("", "32", "64")))
    

    
    
#=== build environment check ===========================================================================================================

# function for checking C/C++ configuration data, function is called from the environment scripts
# @param conf configuration object
# @localconf configuration directionary
def checkCPPEnv(conf, localconf) :
    if conf.env.GetOption("clean")  or  any([i in COMMAND_LINE_TARGETS for i in ["documentation", "librarybuild", "librarydownload", "language", "javareduce", "javaclustering", "javatools"]]) :
        conf.env["LIBS"] = []
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
        
    # setup DistCC or on local color-compiler
    if "DISTCC_HOSTS" in os.environ  or  conf.env["usedistcc"]:
        if "DISTCC_VERBOSE" in os.environ :
            conf.env.Replace(DISTCC_VERBOSE = os.environ["DISTCC_VERBOSE"])
        if "DISTCC_HOSTS" in os.environ :
            conf.env.AppendUnique(DISTCC_HOSTS = os.environ["DISTCC_HOSTS"].split(os.pathsep))
        conf.env.Replace(CXX = "distcc --randomize " + conf.env["CXX"])
        print("use distributed compiling (DistCC)")
    
    else :
        lxPath = conf.env.FindFile("color-"+conf.env["CXX"], conf.env["ENV"]["PATH"])
        if lxPath <> None :
            conf.env.Replace(CXX = "color-"+conf.env["CXX"])
            print("use colored compiling")

    
    
# checks the environment path for executables
# @param conf configuration object
# @param commands list or string with executable
def checkExecutables(conf, commands) :
    cmd = commands
    if type(cmd) <> type([]) :
        cmd = [cmd]
    for i in cmd :
        if env.FindFile(i, conf.env["ENV"]["PATH"]) == None :
            raise RuntimeError("build tool ["+i+"] not found")



# function for checking the environment structur and setup toolkit values
# @param env environment object
def setupToolkitEnv(env) :
    # check the toolkit option
    env["TOOLKIT_ARCH"] = (platform.architecture()[0]).replace("bit", "")
    if platform.system().lower() == "posix" :
        env["TOOLKIT"]      = "posix"
    elif platform.system().lower() == "linux" :
        env["TOOLKIT"]      = "posix"
    elif platform.system().lower() == "darwin" :
        env["TOOLKIT"]      = "darwin"
    elif platform.system().lower().split("_")[0] == "cygwin" :
        env["TOOLKIT"]      = "cygwin"
    else :
        raise RuntimeError("toolkit not known")

    # adding OS environment data
    if "PATH" in os.environ :
        laPathList = env["ENV"]["PATH"].split(os.pathsep)
        laPathList.extend(os.environ["PATH"].split(os.pathsep))
        env["ENV"]["PATH"] = laPathList
        print("Appending custom path (PATH)")



# glob recursiv file / directories
# @param startdir root directory
# @param extension extension list of the files
# @param excludedir directory parts for excluding
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

# --- Library Copy Builder --------------------------------------------------
def librarycopy_print(s, target, source, env): 
    for j,i in zip(target,source) :
        print "copy dynamic library ["+str(i)+"] to ["+str(j)+"]..."

def librarycopy_emitter(target, source, env) :
    listsources = []
    listtargets = []
    lst         = []

    lst.extend( env["LIBS"] )
    if source :
        if SCons.Util.is_List(source) :
            lst.extend( source )
        else : 
            lst.append( source ) 
        
    if "COPYLIBRARY" in env :
        if SCons.Util.is_List(env["COPYLIBRARY"]) :
            lst.extend( env["COPYLIBRARY"] )
        else :
            lst.append( env["COPYLIBRARY"] )
    lst = set(lst)
    
    removelib = []
    if "NOTCOPYLIBRARY" in env :
        if SCons.Util.is_List(env["NOTCOPYLIBRARY"]) :
            removelib.extend(env["NOTCOPYLIBRARY"])
        else :
            removelib.append(env["NOTCOPYLIBRARY"])
    removelib = set(removelib)
        
        
    for i in lst :
        if i in removelib :
            continue
            
        libnames = [ env["LIBPREFIX"]+str(i)+env["SHLIBSUFFIX"] ]
        if env["TOOLKIT"] == "cygwin" :
            libnames.extend([ str(i)+env["SHLIBSUFFIX"], "cyg"+str(i)+env["SHLIBSUFFIX"] ])
        
        for i in  libnames :
            lib  = env.FindFile(i, env["LIBPATH"])
            if lib <> None :
                listsources.append(lib)
                listtargets.append( os.path.join(str(target[0]), i) )
                break
            
    return listtargets, listsources
    
def librarycopy_action(target, source, env) :
    for i,j in zip(source, target) :
        shutil.copy2( str(i), str(j) )
    
LibraryCopyBuilder = Builder( action = SCons.Action.Action("${LIBRARYCOPY}"), emitter = librarycopy_emitter, single_source = False, target_factory=SCons.Node.FS.default_fs.Entry, source_factory=File, PRINT_CMD_LINE_FUNC=librarycopy_print )
# ---------------------------------------------------------------------------



# --- URL Download builder --------------------------------------------------
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
# ---------------------------------------------------------------------------



# --- GZip Extract Builder --------------------------------------------------
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
# ---------------------------------------------------------------------------


# --- Java Swig Builder -----------------------------------------------------
def swigjava_emitter(target, source, env) :
    # create build dir path
    jbuilddir = os.path.join(str(target[0]), "java")
    nbuilddir = os.path.join(str(target[0]), "native")

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

    target = []
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
        
        
        # adding target filenames with path (first the cpp name, second the java names)
        target.append( os.path.normpath(os.path.join(nbuilddir, data["sourcename"]+".cpp" )) )
        
        # we read the cpp classname, get the template parameter which matchs the cpp class name in the value
        # if the rename option is not empty and matches the cpp class name, we use this result for the java class name
        # because the template parameter points to a static function, otherwise we use the template name
        for n in data["cppclass"] :
            if data["rename"].has_key(n) :
                target.append( os.path.normpath(os.path.join(jbuilddir, os.sep.join(data["cppnamespace"][n]), data["rename"][n]+".java")) )
            else :
                for l in data["template"][n] :
                    target.append( os.path.normpath(os.path.join(jbuilddir, os.sep.join(data["cppnamespace"][n]), l+".java")) )
                    
            #adding generated file, that should be deleted later to the targets (with the extension .del)
            #target.append( os.path.join(jbuilddir, os.sep.join(data["cppnamespace"][n]), str(data["module"]).replace("'","").replace("]","").replace("[","")+".java.del") )
                    
    return target, source
    
def swigjava_packageaction(dirname) :
    return ".".join( str(dirname).split(os.path.sep)[1:] )
    
def swigjava_outdiraction(sourcedir, targets) :
    path = os.path.join( os.path.commonprefix([str(i) for i in targets]), "java", os.path.sep.join(str(sourcedir).split(os.path.sep)[1:]) )

    try :
        if "java" in COMMAND_LINE_TARGETS :
            os.makedirs(path)
    except :
        pass
    return path
    
def swigjava_cppdiraction(source, targets) :
    path = os.path.join( os.path.commonprefix([str(i) for i in targets]), "native" )
    try :
        if "java" in COMMAND_LINE_TARGETS :
            os.makedirs(path)
    except :
        pass
    return os.path.join(path, str(source)+".cpp")
    
def swigjava_remove(sources, target) :
    jbuilddir = os.path.sep.join(  str(target).split(os.path.sep)[0:-2]  )

    remove = re.compile( r"#ifdef SWIGPYTHON(.*)#endif", re.DOTALL )
    module = re.compile( r"%module \"(.*)\"" )

    for input in sources :
        delpath = os.path.join( jbuilddir, "java", os.path.sep.join(str(input).split(os.path.sep)[1:-1]) )
    
        # read source file
        oFile = open( str(input), "r" )
        ifacetext = oFile.read()
        oFile.close()

        ifacetext = re.sub(remove, "", ifacetext)
        for i in re.findall(module , ifacetext) :
            try :
                os.remove(os.path.join(delpath, i+".java"))
            except :
                pass
    
SwigJavaBuilder = Builder( action = SCons.Action.Action(["swig -Wall -O -templatereduce -c++ -java -package ${SwigJavaPackage(SOURCE.dir)} -outdir ${SwigJavaOutDir(SOURCE.dir, TARGETS)} -o ${SwigJavaCppDir(SOURCE.filebase, TARGETS)} $SOURCE", "${SwigJavaRemove(SOURCES, TARGET)}"]), emitter=swigjava_emitter, single_source = True, src_suffix=".i", target_factory=Entry, source_factory=File )
# ---------------------------------------------------------------------------



#=== licence ===========================================================================================================================

# licence text
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

env = Environment( 
        LIBRARYCOPY     = librarycopy_action,
        SwigJavaPackage = swigjava_packageaction, 
        SwigJavaOutDir  = swigjava_outdiraction,
        SwigJavaCppDir  = swigjava_cppdiraction,
        SwigJavaRemove  = swigjava_remove,
        
        variables=vars,
        
        BUILDERS = { 
            "LibraryCopy"   : LibraryCopyBuilder,
            "SwigJava"      : SwigJavaBuilder, 
            "Download"      : DownloadBuilder, 
            "Extract"       : ExtractBuilder
        }
)
env.Tool("gettext")
env.VariantDir("build", ".", duplicate=0)
Help(vars.GenerateHelpText(env))
setupToolkitEnv(env)
conf = Configure(env)


# changing flags if needed
if "sources" in COMMAND_LINE_TARGETS : 
    conf.env["withsources"] = True;

# read platform configuration (only if not clean target is used)
platformconfig = env["TOOLKIT"]
if not(os.path.isfile(os.path.join("buildenvironment", platformconfig+".py"))) :
    raise ImportError("toolkit configuration script ["+platformconfig+"] not found")

env.SConscript( os.path.join("buildenvironment", platformconfig+".py"), exports="conf checkCPPEnv checkExecutables" )
env = conf.Finish()



# setup default builds and default clean
defaultcpp = [os.path.join(os.path.abspath(os.curdir), "machinelearning.cpp")]
env.Clean(defaultcpp, [
    "build",
    "config.log",
    ".sconf_temp",
    ".sconsign.dblite"
])


# setup all different sub build script
env.SConscript( os.path.join("tools", "language", "build.py"), exports="env defaultcpp GlobRekursiv" )
env.SConscript( os.path.join("documentation", "build.py"), exports="env defaultcpp" )
env.SConscript( os.path.join("library", "build.py"), exports="env defaultcpp" )

env.SConscript( os.path.join("swig", "target", "java", "build.py"), exports="env defaultcpp GlobRekursiv" )
for i in ["clustering", "tools", "reducing"] :
    env.SConscript( os.path.join("examples", "java", i, "build.py"), exports="env defaultcpp" )

for i in ["geneticalgorithm", "classifier", "clustering", "distance", "other", "reducing", "sources"] :
    env.SConscript( os.path.join("examples", i, "build.py"), exports="env defaultcpp" )
