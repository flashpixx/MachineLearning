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

import SCons.Action
import SCons.Builder
import SCons.Util




def exists(env):
    """ Try to detect the Swig """
    
    try: 
        return env["SwigJava"]
    except KeyError: 
        pass

    swig = env.WhereIs("swig")
    print swig
    if swig :
        return swig

    return None
    
    
    
def emitter(target, source, env) :

        swig_suffix = env.subst("$SWIGJAVA_SUFFIX")
        cpp_suffix  = env.subst("$SWIGJAVA_CPPSUFFIX")
        java_suffix = env.subst("$JAVASUFFIX")
        
        
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
            if not input.endswith(swig_suffix) :
                continue
        
        
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
                     "sourcename"        : (str(input).split(os.sep)[-1]).replace(swig_suffix, ""),
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
            target.append( data["sourcename"]+cpp_suffix )
            
            # we read the cpp classname, get the template parameter which matchs the cpp class name in the value
            # if the rename option is not empty and matches the cpp class name, we use this result for the java class name
            # because the template parameter points to a static function, otherwise we use the template name
            for n in data["cppclass"] :
                if data["rename"].has_key(n) :
                    target.append( os.path.normpath(os.path.join(Dir("#").abspath, jbuilddir, os.sep.join(data["cppnamespace"][n]), data["rename"][n]+java_suffix)) )
                else :
                    for l in data["template"][n] :
                        target.append( os.path.normpath(os.path.join(Dir("#").abspath, jbuilddir, os.sep.join(data["cppnamespace"][n]), l+java_suffix)) )
            
        return target, source





def generate(env) :
    env.AddMethod(SwigJava)
   
    env["SWIGJAVA_FLAGS"]        = SCons.Util.CLVar("-Wall -O -templatereduce -c++ -java"),
    env["SWIGJAVA_SUFFIX"]       = ".i",
    env["SWIGJAVA_CPPSUFFIX"]    = ".cpp",
    env["SWIGJAVA_CPPDIR"]       = "cppsource",
    env["SWIGJAVA_JAVADIR"]      = "javasource",
    env["SWIGJAVA_COM"]          = "$SwigJava $SWIGJAVA_FLAGS -package ??? -outdir ??? -o $SWIGJAVA_CPPDIR/$SOURCES.$SWIGJAVA_CPPSUFFIX $SOURCES"
    
    
    
    
SwigJavaAction  = SCons.Action.Action("$SWIGJAVA_COM")
SwigJavaBuilder = SCons.Builder.Builder(action = SwigJavaAction,
                    emitter = emitter,
                    target_factory = SCons.Node.FS.Entry,
                    source_factory = SCons.Node.FS.Entry)
    
    
def SwigJava(env, target, source=None, *args, **kw):

    if not SCons.Util.is_List(target):
        target = [target]
    if not source:
        source = target[:]
    if not SCons.Util.is_List(source):
        source = [source]
        
    result = []

    for t, s in zip(target, source):
        pass
    
    return result