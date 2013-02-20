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
    [platform, arch] = detectPlatform()
    
    atlaslink     = "multi"
    zipsupport    = False
    colorcompiler = True
    if (platform == "cygwin")  or  (platform == "msys") :
        atlaslink     = "single"
        zipsupport    = True
        colorcompiler = False
    

    vars.Add(BoolVariable("withrandomdevice", "installation with random device support", False))
    vars.Add(BoolVariable("withmpi", "installation with MPI support", False))
    vars.Add(BoolVariable("withmultilanguage", "installation with multilanguage support", False))
    vars.Add(BoolVariable("withsources", "installation with source like nntp or something else", False))
    vars.Add(BoolVariable("withfiles", "installation with file reading support for CSV & HDF", True))
    vars.Add(BoolVariable("withlogger", "use the interal logger of the framework", False))
    vars.Add(BoolVariable("withsymbolicmath", "compile for using symbolic math expression (needed by gradient descent)", False))
    
    vars.Add(EnumVariable("buildtype", "value of the buildtype", "release", allowed_values=("debug", "release")))
    vars.Add(BoolVariable("uselocallibrary", "use the library in the local directory only", False))
    vars.Add(PathVariable("jnipath",  "path to jni.h",  None))
    vars.Add(BoolVariable("usedistcc", "use distributed compiling with DistCC", False))
    vars.Add(BoolVariable("usecolorcompiler", "enable / disable searching for color compiler", colorcompiler))
    vars.Add(BoolVariable("copylibrary", "copy the dynamic link libraries into the build dir", False))
    vars.Add(EnumVariable("atlaslink", "type of the ATLAS link file", atlaslink, allowed_values=("single", "multi")))
    
    if platform == "msys" :
        vars.Add(EnumVariable("winversion", "value of the Windows version", "win7", allowed_values=("win7", "srv2008", "vista", "srv2003sp1", "xpsp2", "srv2003", "xp", "w2000")))
    
    vars.Add(ListVariable("skiplibrary", "skipping library builds / downloads", "", ["lapack", "boost", "hdf", "ginac", "json", "xml"]))
    vars.Add(BoolVariable("zipsupport", "build Bzip2 and ZLib support for Boost", zipsupport))
    vars.Add(EnumVariable("boostbuild", "Boost build option: required builds all libraries, requiredoptional build the required libraries with optional libraries, full build all libraries", "requiredoptional", allowed_values=("required", "requiredoptional", "full")))
    vars.Add(EnumVariable("atlaspointerwidth", "pointer width for compiling ATLAS (empty = system default, 32 = 32 Bit, 64 = 64 Bit)", "", allowed_values=("", "32", "64")))
    vars.Add(EnumVariable("atlasversion", "version of the ATLAS library (developer or stable version)", "devel", allowed_values=("stable", "devel")))
    vars.Add(EnumVariable("jsoncppversion", "version of the Json-Cpp library (developer or stable version)", "devel", allowed_values=("stable", "devel")))

    
    
#=== build environment check ===========================================================================================================

# function for checking the environment structur and setup toolkit values
# @param vars script variables
# @return environment object
def setupToolkitEnv(vars) :
    # we disable all tools and set it manually on the platform
    env = Environment( tools = [], variables=vars,
        BUILDERS = { "LibraryCopy" : LibraryCopyBuilder, "SwigJava" : SwigJavaBuilder, "Download" : DownloadBuilder, "Extract" : ExtractBuilder },
        LIBRARYCOPY = librarycopy_action, SwigJavaPackage = swigjava_packageaction, SwigJavaOutDir  = swigjava_outdiraction, SwigJavaCppDir  = swigjava_cppdiraction
    )
    
    # adding OS environment data
    if "PATH" in os.environ :
        laPathList = env["ENV"]["PATH"].split(os.pathsep)
        laPathList.extend(os.environ["PATH"].split(os.pathsep))
        env["ENV"]["PATH"] = laPathList
        print("Appending custom path (PATH)")

    # setup toolkit values (that are called by eg librarybuild) 
    [env["TOOLKIT"], env["TOOLKIT_ARCH"]] = detectPlatform()
    if env["TOOLKIT"] in ["posix", "darwin", "cygwin"] :
        env.Tool("default")
    elif env["TOOLKIT"] == "msys":
        env.Tool("mingw")
        if any([i in COMMAND_LINE_TARGETS for i in ["java", "javareduce", "javaclustering", "javatools"]]) :
            env.Tool("javac")
            env.Tool("jar")
        
        # set unix environment only on librarybuild, otherwise we get problems on compiler calls
        if "librarybuild" in COMMAND_LINE_TARGETS or "librarydownload" in COMMAND_LINE_TARGETS :        
            if not( "msyscon" in os.environ ) :
                raise RuntimeError("MSYS environment shell not found")
            shell = findfile( os.environ["msyscon"], env["ENV"]["PATH"] )
            if shell == None :
                raise RuntimeError("MSYS shell not found")
        
            from SCons.Platform.win32 import exec_spawn
            from SCons.Platform.posix import escape
            env["MSYSSPAWN"] = env["SPAWN"]
            env["MSYSSHELL"] = env["SHELL"]
            env["SHELL"]     = shell
            env["SPAWN"]     = (lambda sh, esc, cmd, args, envparam : exec_spawn([sh, "-c", escape(" ".join(args))], envparam))
    
    print "Build environment is ["+env["TOOLKIT"]+"] and architecture ["+env["TOOLKIT_ARCH"]+"]"
    return env
    
    
# platform detection
# @return returns name and architectur of the platform
def detectPlatform() :
    arch = (platform.architecture()[0]).replace("bit", "")

    if platform.system().lower() == "posix" or platform.system().lower() == "linux" :
        return "posix", arch 
    elif platform.system().lower() == "darwin" :
        return "darwin", arch
    elif platform.system().lower().split("_")[0] == "cygwin" :
        return "cygwin", arch
    elif "msystem" in os.environ  and  os.environ["msystem"].lower() == "mingw32":
        return "msys", arch
   
    raise RuntimeError("toolkit ["+platform.system()+"] not known")
    


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
        
    # boost_mpi breaks CheckLib, so we does not check it
    for i in localconf["cpplibraries"] :
        if i == "boost_mpi" :                                                                                                                                                                                                                                                 
            conf.env.Append(LIBS = i)
            continue                                                                                                                                                                                                                                       
        if not conf.CheckLib(i, language="C++") :                                                                                                                                                                                                                         
            sys.exit(1) 

    conf.env.AppendUnique(LINKFLAGS = localconf["linkflags"])

    # setup DistCC or on local color-compiler
    if "DISTCC_HOSTS" in os.environ  or  conf.env["usedistcc"] :
        if "DISTCC_VERBOSE" in os.environ :
            conf.env.Replace(DISTCC_VERBOSE = os.environ["DISTCC_VERBOSE"])
        if "DISTCC_HOSTS" in os.environ :
            conf.env.AppendUnique(DISTCC_HOSTS = os.environ["DISTCC_HOSTS"].split(os.pathsep))
        conf.env.Replace(CXX = "distcc --randomize " + conf.env["CXX"])
        print("use distributed compiling (DistCC)")
    
    else :
        if conf.env["usecolorcompiler"] :
            for i in [ "color-"+conf.env["CXX"], "color"+conf.env["CXX"] ] :
                lxPath = findfile(i, conf.env["ENV"]["PATH"])
                if lxPath <> None :
                    conf.env.Replace(CXX = "color-"+conf.env["CXX"])
                    print("use colored compiling")
                    break
    
# checks the environment path for executables
# @param conf configuration object
# @param commands list or string with executable
def checkExecutables(conf, commands) :
    cmd = commands
    if type(cmd) <> type([]) :
        cmd = [cmd]
    for i in cmd :
        if findfile(i, conf.env["ENV"]["PATH"]) == None :
            raise RuntimeError("build tool ["+i+"] not found")


# glob recursiv file / directories
# @param startdir root directory
# @param extension extension list of the files
# @param excludedir directory parts for excluding
def GlobRekursiv(startdir, extensions=[], excludedir=[]) :
    lst = []
    if not extensions :
        return lst
    for root, dirs, filenames in os.walk(startdir) :
        if any([i in root for i in excludedir]) :
            continue
        for filename in filenames :
            if filename.startswith(".") :
                continue
            if any([filename.endswith(i) for i in extensions]) :
                lst.append( os.path.abspath(os.path.join(root, filename)) )
    return lst


# find file implementation, because on some systems
# there is a problem with the env-FindFile
# @param name filename
# @param pathlist a list of directories
# @return None or the first found item with full path
def findfile(name, pathlist=[]):
    for l in pathlist :
        if not os.path.exists(l) :
            continue
        for root, dirs, files in os.walk(l) :
            if name in files :
                return os.path.join(root, name)
    return None



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
        
    pathlist = []
    if "LIBPATH" in env :
        pathlist.extend(env["LIBPATH"])
    if ("COPYLIBRARYPATH" in env) and SCons.Util.is_List(env["COPYLIBRARYPATH"]) :
        pathlist.extend(env["COPYLIBRARYPATH"])
    pathlist = set(pathlist)
        
    for i in lst :
        if i in removelib :
            continue
            
        libnames = [ env["LIBPREFIX"]+str(i)+env["SHLIBSUFFIX"] ]
        if env["TOOLKIT"] == "cygwin" :
            libnames.extend([ str(i)+env["SHLIBSUFFIX"], "cyg"+str(i)+env["SHLIBSUFFIX"] ])
        elif env["TOOLKIT"] == "msys" :
            libnames.append( str(i)+env["SHLIBSUFFIX"] )
            
        for n in libnames :
            targetfile = n
            found      = findfile(targetfile, pathlist)
            if found <> None :
                if env["TOOLKIT"] == "posix" and os.path.islink(found) :
                    targetfile = os.path.basename(os.readlink(found))
                listsources.append(found)
                listtargets.append( os.path.join(str(target[0]), targetfile) )
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

    [url, filename] = parsefunction(env)
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
    if env["withmpi"] :
        raise SCons.Errors.UserError("Java Swig Builder does not work with MPI")

    # create build dir path
    jbuilddir = os.path.join(str(target[0]), "java")
    nbuilddir = os.path.join(str(target[0]), "native")

    regex = {
          # remove expression of the interface file (store a list with expressions)
          "remove"              : [ re.compile( r"#ifdef SWIGPYTHON(.*)#endif", re.DOTALL ),
                                    re.compile( r"#ifndef SWIG(.*?)#endif", re.DOTALL ),
                                    re.compile( r"#ifdef MACHINELEARNING_MPI(.*?)#endif", re.DOTALL ), 
                                    re.compile( r"%pragma(.*?)%}", re.DOTALL ) 
                                  ],
          
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
          "cppnamespace"        : re.compile( r"namespace(.*?)class", re.DOTALL ),
          "cppstaticfunction"   : re.compile( r"static (.*) (.*)\(" ), 
          
          # regex helpers 
          "cppremove"           : re.compile( r"(\(|\)|<(.*)>)" )
    }
    
    cppdefines = [q[0] for q in env["CPPDEFINES"]]
    if not "MACHINELEARNING_SOURCES" in cppdefines :
        regex["remove"].append( re.compile( r"#(.*?)MACHINELEARNING_SOURCES(.*?)#endif", re.DOTALL ) )
    if not "MACHINELEARNING_SOURCES_TWITTER" in cppdefines :
        regex["remove"].append( re.compile( r"#(.*?)MACHINELEARNING_SOURCES_TWITTER(.*?)#endif", re.DOTALL ) )
    if not "MACHINELEARNING_LOGGER" in cppdefines :
        regex["remove"].append( re.compile( r"#(.*?)MACHINELEARNING_LOGGER(.*?)#endif", re.DOTALL ) )
    if not "MACHINELEARNING_FILES" in cppdefines :
        regex["remove"].append( re.compile( r"#(.*?)MACHINELEARNING_FILES(.*?)#endif", re.DOTALL ) )
    if not "MACHINELEARNING_FILES_HDF" in cppdefines :
        regex["remove"].append( re.compile( r"#(.*?)MACHINELEARNING_FILES_HDF(.*?)#endif", re.DOTALL ) )
    if not "MACHINELEARNING_SYMBOLICMATH" in cppdefines :
        regex["remove"].append( re.compile( r"#(.*?)MACHINELEARNING_SYMBOLICMATH(.*?)#endif", re.DOTALL ) )
    if not "MACHINELEARNING_MULTILANGUAGE" in cppdefines :
        regex["remove"].append( re.compile( r"#(.*?)MACHINELEARNING_MULTILANGUAGE(.*?)#endif", re.DOTALL ) )

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
        ifacetext = re.sub(regex["cppcomment"], "", ifacetext)
        ifacetext = ifacetext.replace("#endif", "").strip()
        if not ifacetext :
            continue
        
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
                 "cppstaticfunction" : [],
                 # list with class names that are abstract
                 "abstract"          : []
        }
        
        # getting C++ class name (read the %include file name)
        # [bug: if a class with the same name exists in different namespaces, the dict stores only the last namespace entry]
        # [bug: namespace and target directory that is extracted by the builder can be different]
        cpptext = ""
        for n in data["include"] :

            # read cpp data
            oFile   = open( os.path.normpath(os.path.join(ifacepath, n)), "r" )
            cpptext = re.sub(regex["cppcomment"], "", oFile.read()) 
            oFile.close()
            for rx in regex["remove"] :
                cpptext = re.sub(rx, "", cpptext)
            cpptext = cpptext.replace("#endif", "").strip()
        
        # get class names and static function
        classnames = re.findall(regex["cppclass"], cpptext)
        classnames = [ re.sub(regex["cppbaseclass"], "", k).strip() for k in classnames ]
        data["cppstaticfunction"].extend( [ k[1] for k in re.findall(regex["cppstaticfunction"], cpptext) ] ) 

        # determine class and namespace connection (which class exists in which namespace)
        namespaces = re.findall(regex["cppnamespace"], cpptext)
 
        # remove all chars from the end till the latest {
        namespaces = [ n.replace("\n", " ").replace("#", "").replace("endif", "") for n in namespaces ]
        namespaces = [ n[0:n.rfind("{")] for n in namespaces ]
        
        nslist     = [ n.replace("{", "").split("namespace") for n in namespaces ]
        nslist     = [ [ k.strip() for k in i ] for i in nslist ]
        
        for k in classnames :
            for ns in nslist :
                if len(ns) < 2 :
                    continue
            
                # create the regex for searching the class in the namespace
                nc = [ "namespace(.*)"+s+"(.*){" for s in ns ]
                nc = "(.*)".join(nc) + "(.*)class(.*)" + k
                if re.search( re.compile(nc, re.DOTALL ), cpptext ) :
                    data["cppnamespace"][k] = ns
                    
        
        # add classnames to the dict
        data["cppclass"].extend( classnames  )
        data["cppclass"] = list(set(data["cppclass"]))

        # create the remap dict { cpp class name : swig rename}
        help = {}
        for targetname,sourcename in data["rename"] :
            help[sourcename.replace(";","").strip()] = targetname.strip()
        data["rename"] = help
        
        # remove template parameters <> or brackets
        data["template"] = [ re.sub(regex["cppremove"], "", i) for i in data["template"] ]
        help = {}
        for n in data["template"] :
            k = n.split()
            help[k[0]] = k[1].split("::")[-2:]
        data["template"] = help
        
        # create a unique list of the static functions
        data["cppstaticfunction"] = list(set(data["cppstaticfunction"]))

        # determine classname of each template parameter and
        # change the template parameter in this way, that we get a dict with { cpp class name : [target names] }
        help = {}
        for k,v in data["template"].items() :
            newval = list(set(data["cppclass"]) & set(v))
            if newval : 
                if help.has_key(newval[0]) :
                    help[newval[0]].append(k)
                else :
                    help[newval[0]] = [k]
        data["template"] = help
        
        # adding target filenames with path (first the cpp name, second the java names),
        # on abstract classes wie do not create any cpp file
        for i in data["cppclass"] :
            if data["cppnamespace"].has_key(i) :
                lc = "::".join(data["cppnamespace"][i]) + "::" + i
                if re.search( re.compile("javaclassmodifiers(.+?)" +lc+ "(.*?)public abstract class"), ifacetext) or re.search( re.compile("javaclassmodifiers(.+?)" +lc+ "(.*?)public interface"), ifacetext) :
                    data["abstract"].append(i)
                else :
                    target.append( os.path.normpath(os.path.join(nbuilddir, data["sourcename"]+".cpp" )) )
        
        # we read the cpp classname, get the template parameter which matchs the cpp class name in the value
        # if the rename option is not empty and matches the cpp class name, we use this result for the java class name
        # because the template parameter points to a static function, otherwise we use the template name
        for n in data["cppclass"] :
            
            # add module java file
            if not n in data["abstract"] :
                for i in data["module"] :
                    target.append(  os.path.normpath(os.path.join(jbuilddir, os.sep.join(data["cppnamespace"][n]), i + "JNI" + env["JAVASUFFIX"]))  )

            # class file
            if data["rename"].has_key(n) :
                target.append( os.path.normpath(os.path.join(jbuilddir, os.sep.join(data["cppnamespace"][n]), data["rename"][n] + env["JAVASUFFIX"])) )
            elif data["template"] :
                for l in data["template"][n] :
                    target.append( os.path.normpath(os.path.join(jbuilddir, os.sep.join(data["cppnamespace"][n]), l + env["JAVASUFFIX"])) )
    target = list(set(target))
    return target, source
    
def swigjava_packageaction(iface) :
    return "-package " +  ".".join(  ["machinelearning"] + os.path.dirname(str(iface)).split(os.path.sep) )

    
def swigjava_outdiraction(sourcedir, targets) :
    if not "java" in COMMAND_LINE_TARGETS :
        return ""

    for i in targets :
        if str(i).endswith(env["JAVASUFFIX"]) :
            path = os.path.dirname(str(i))
            try :
                os.makedirs(path)
            except :
                pass
            return "-outdir " + path
    return ""
    
def swigjava_cppdiraction(source, interface, targets) :
    if not "java" in COMMAND_LINE_TARGETS :
        return ""
        
    # read source file
    oFile = open( str(interface), "r" )
    ifacetext = oFile.read()
    oFile.close()
    
    path = None
    for i in targets :
        if str(i).endswith(".cpp") :
            path = os.path.dirname(str(i))
            break
    
    if not path :
        return "-o " + os.path.join( "build",  str(source) + ".tmp") 
    try :
        os.makedirs(path)
    except :
        pass
    return "-o " + os.path.join(path, str(source) + ".cpp")
    
SwigJavaBuilder = Builder( action = SCons.Action.Action("swig $_CPPDEFFLAGS -O -templatereduce -c++ -java ${SwigJavaPackage(SOURCE)} ${SwigJavaOutDir(SOURCE.dir, TARGETS)} ${SwigJavaCppDir(SOURCE.filebase, SOURCE, TARGETS)} $SOURCE"), emitter=swigjava_emitter, single_source = True, src_suffix=".i", target_factory=Entry, source_factory=File )
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
env = setupToolkitEnv(vars)
env.VariantDir("build", ".", duplicate=0)
Help(vars.GenerateHelpText(env))
conf = Configure(env)


# changing flags if needed
if "sources" in COMMAND_LINE_TARGETS : 
    conf.env["withsources"] = True;

# read platform configuration (only if not clean target is used)
if not(os.path.isfile(os.path.join("buildenvironment", env["TOOLKIT"]+".py"))) :
    raise ImportError("toolkit configuration script ["+env["TOOLKIT"]+"] not found")

env.SConscript( os.path.join("buildenvironment", env["TOOLKIT"]+".py"), exports="conf checkCPPEnv checkExecutables" )
env = conf.Finish()

if not env.GetOption("clean") :
    Default(None)

# setup default builds and default clean
defaultcpp = [os.path.join(os.path.abspath(os.curdir), "machinelearning.cpp")]
env.Clean("pseudo", [
    "build",
    "config.log",
    ".sconf_temp",
    ".sconsign.dblite",
    Glob("*.o"),
    Glob("*.os")
])


# setup all different sub build script
if "language" in COMMAND_LINE_TARGETS :
    env.SConscript( os.path.join("tools", "language", "build.py"), exports="env defaultcpp GlobRekursiv" )
if "java" in COMMAND_LINE_TARGETS :
    env.SConscript( os.path.join("swig", "java", "build.py"), exports="env defaultcpp GlobRekursiv" )
if any([i in COMMAND_LINE_TARGETS for i in ["javatools", "javaclustering", "javareduce"]]) :
    for i in ["clustering", "tools", "reducing"] :
        env.SConscript( os.path.join("examples", "java", i, "build.py"), exports="env defaultcpp" )


env.SConscript( os.path.join("documentation", "build.py"), exports="env defaultcpp" )
env.SConscript( os.path.join("library", "build.py"), exports="env defaultcpp" )

for i in ["geneticalgorithm", "classifier", "clustering", "distance", "other", "reducing", "sources"] :
    env.SConscript( os.path.join("examples", i, "build.py"), exports="env defaultcpp" )
