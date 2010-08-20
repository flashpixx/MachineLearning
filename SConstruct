# -*- coding: utf-8 -*-

import os
import glob

#=== function for os configuration ===================================================================================================

# constante for showing messages during compiling
SHOWCONFIG = False
# using CPUs for compiling
COMPILECPU = 6

def configuration_macosx(config) :
    config["seperator"]         = ":"
    config["compiler"]          = "g++"
    config["compileflags"]      = "-D MULTILANGUAGE -D NDEBUG -D BOOST_UBLAS_NDEBUG -D BOOST_NUMERIC_BINDINGS_BLAS_CBLAS -O2 -Wall -ftree-vectorize"
    config["include"]           = os.environ["CPPPATH"]
    config["libpath"]           = os.environ["LIBRARY_PATH"]
    config["libs"]              = "intl"
    config["libsuffix"]         = [".dylib", ".a"]
    config["libremovestring"]   = "lib"
    config["libremovesuffix"]   = True  
    config["ldremove"]          = ["boost_python", "hdf5.6", "hdf5_cpp.6", "hdf5_hl.6", "hdf5_hl_cpp.6", "ptcblas", "ptf77blas", "f77blas", "ginac", "boost_wave", "boost_signals", "boost_regex", "boost_date_time", "boost_graph", "boost_mpi", "boost_graph", "boost_graph_parallel", "boost_serialization", "boost_system", "boost_filesystem", "boost_math_c99", "boost_math_tr1", "boost_math_tr1l", "boost_math_tr1f",  "boost_math_c99f", "boost_math_c99l"]
    
    
def configuration_posix(config) :
    config["seperator"]         = ":"
    config["compiler"]          = "g++"
    config["compileflags"]      = "-D MULTILANGUAGE -D RANDOMDEVICE -D NDEBUG -D BOOST_UBLAS_NDEBUG -D BOOST_NUMERIC_BINDINGS_BLAS_CBLAS -O2 -Wall -ftree-vectorize"
    config["include"]           = os.environ["CPPPATH"]
    config["libpath"]           = os.environ["LIBRARY_PATH"]
    config["libs"]              = "intl"
    config["libsuffix"]         = [".so", ".a"]
    config["libremovestring"]   = "lib"
    config["libremovesuffix"]   = True  
    config["ldremove"]          = ["boost_python", "hdf5.6", "hdf5_cpp.6", "hdf5_hl.6", "hdf5_hl_cpp.6", "ptcblas", "ptf77blas", "f77blas", "ginac"]

    
def configuration_win32(config) :
    config["seperator"]         = ";"
    config["compiler"]          = ""
    config["compileflags"]      = ""
    config["include"]           = os.environ["INCLUDE"]
    config["libpath"]           = os.environ["LIBS"]
    config["libs"]              = ""
    config["libsuffix"]         = ""
    config["libremovestring"]   = ""
    config["libremovesuffix"]   = False 
#=======================================================================================================================================
    
    
    
    
    
    
    
    
    
    
    
    
    
#=== function for configuration creation================================================================================================

# function for reading os configuration
# and setting environment
def checkSysConfig():
    env = Environment()
    config = {}
    
    if env['PLATFORM'].lower() == "darwin" :
        configuration_macosx(config)
    elif env['PLATFORM'].lower() == "win32" :
        configuration_win32(config)
    elif env['PLATFORM'].lower() == "posix" :
        configuration_posix(config)
    else :
        msg("configuration for ["+env['PLATFORM']+"] not exists")
        exit(1) 
    
    if not(config) :
        msg("Configuration is empty")
        exit(1)

    if type(config["libsuffix"]) == type([]) :
        config["libsuffix"] = config["seperator"].join(config["libsuffix"])


    for i in ["compiler", "compileflags", "include", "libpath", "libs", "seperator", "libsuffix", "libremovestring", "libremovesuffix" ] :
        if not(config.has_key(i)) :
            msg("field ["+i+"] is not set in the configuration")
            exit(1)

    for i in ["compiler", "compileflags", "include", "libpath", "libs", "seperator", "libremovestring" ] :
        if type(config[i]) <> type("str") :
            msg("field ["+i+"] must be a string type")
            exit(1)
            
        if not(config[i]) :
            msg("field ["+i+"] is empty")
            
            
    if type(config["libremovesuffix"]) <> type(False) :
        msg("field [libremovesuffix] must be a boolean type")
        exit(1)

    if not(env["CXX"]) :
        msg("no compiler set")
        exit(1)

    if not(config["compiler"]) :
        msg("no compiler is set, using default ["+env["CXX"]+"]")
    
    env.Replace(CXX         = config["compiler"])
    env.Replace(CXXFLAGS    = config["compileflags"])
    env.Replace(CCFLAGS     = config["compileflags"])
    env.Replace(CPPPATH     = config["include"])
    env["seperator"]        = config["seperator"]
    env["libs"]             = config["libs"]
    env["libpath"]          = config["libpath"]
    env["libsuffix"]        = config["libsuffix"].split(config["seperator"])
    env["libremovesuffix"]  = config["libremovesuffix"]
    env["libremovestring"]  = config["libremovestring"]
    env["ldremove"]         = config["ldremove"]

    # only *.o files will be clean ??
    #env.NoClean( getRekusivFiles(os.curdir, ".o") )

    # add the doxyfile
    # doxyfile = glob.glob("*.doxyfile")
    # if doxyfile :
    #    env.Doxygen( doxyfile )
    
    msg("\n")
    msg("------->>> compiling now with these properties:\n")
    msg("plattform            : " + env['PLATFORM'] + "\n")
    msg("compiler             : " + env["CXX"] + "\n")
    msg("linker flags         : " + env["CXXFLAGS"])
    msg("\n\n")
    return env


# print the list on console output
def showList(pName, pLST) :
    lc = pName
    for i in range(20-len(pName)) :
        lc += " "
    
    lc += " : " 
    for i in pLST :
        msg(lc + i)
        lc = "                       "

#remove item from list if exists
def rmListItem( pList, pItem ) :
    la = pList

    if type(pItem) == type([]) :
        for i in pItem :
            if i in la :
                la.remove(i)
    else :
        if pItem in la :
            la.remove(pItem)
    
    return la


# seperated the string with the environmental
# seperation which is set in the environmental
def splitter(pEnv, pStr) :

    lst = pStr.split( pEnv["seperator"] );
    lst = [i.strip() for i in lst if i]
    #für Python < 2.4: lst = dict(map(lambda i: (i,1),lst)).keys()
    lst = list(set(lst))
    lst.sort();
    
    return lst


# read all libs on the path and clear the
# filenames on the configuration parameter patterns
def readLibs(pEnv, pPath):
    if not(os.path.exists(pPath)) :
        return null
        
    lstdir      = os.listdir( pPath )
    lst         = []
    libremove   = pEnv["libremovestring"].split(pEnv["seperator"])
    
    for i in lstdir :
        if not(os.path.isfile( os.path.join(pPath, i) )) :
            continue
            
        for n in pEnv["libsuffix"] :
            if i.endswith(n) :
                
                c = i
                if env["libremovesuffix"] and c.endswith(n) :
                    c = c.replace( n, "")

                for x in libremove :
                    if c.startswith(x) :
                        c = c.replace( x, "")
                        
                lst.append(c)

    #für Python < 2.4: lst = dict(map(lambda i: (i,1),lst)).keys()
    lst = list(set(lst))
    lst.sort()
    return lst
    
# get the first entry of the list with
# the string pattern 
def getListPattern(pStr, pLst, pLower=False):
    for i in pLst :
        if pLower and pStr.lower() in i.lower() :
            return i
        elif not(pLower) and pStr in i :
            return i
    return ""
       
# get rekursiv all files
def getRekusivFiles(startdir, ending, pdontuse=[], pShowPath=True) :
    lst = []
    
    ldontuse = [os.path.join(startdir, i) for i in pdontuse]
    
    for root, dirs, filenames in os.walk(startdir):
        for filename in filenames:
            if not(root in ldontuse) :
                if filename.endswith(ending):
                    if pShowPath :
                        lst.append( os.path.join(root, filename) )
                    else :
                        lst.append(filename)
    return lst
        
# print messages
def msg(pStr="") :
    if SHOWCONFIG :
        print pStr
#=======================================================================================================================================


#=== create environment and compiling ==================================================================================================
msg("=== configuration =====================================================================================")
env = checkSysConfig()
    
include_path = splitter(env, env["CPPPATH"])
showList("include path(s)", include_path)
msg()

library_path = splitter(env, env["libpath"])
showList("library path(s)", library_path)
msg()


boostlibs = []
boostpath = getListPattern("boost", library_path, True)
if boostpath :
    boostlibs = readLibs(env, boostpath)
    msg("boost is located in ["+boostpath+"]\n")
    showList("boost libs", boostlibs)
else :
    msg("boost library path is not found\n")

msg()

atlaslibs = []
atlaspath = getListPattern("atlas", library_path, True)
if atlaspath :
    atlaslibs = readLibs(env, atlaspath)
    msg("atlas is located in ["+atlaspath+"]\n")
    showList("atlas libs", atlaslibs)
else :
    msg("atlas library path is not found\n")

msg()

hdflibs = []
hdfpath = getListPattern("hdf", library_path, True)
if hdfpath :
    hdflibs = readLibs(env, hdfpath)
    msg("hdf is located in ["+hdfpath+"]\n")
    showList("hdf libs", hdflibs)
else :
    msg("hdf library path is not found\n")

msg()


ginaclibs = []
ginacpath = getListPattern("ginac", library_path, True)
if ginacpath :
    ginaclibs = readLibs(env, ginacpath)
    msg("ginac is located in ["+ginacpath+"]\n")
    showList("ginac libs", ginaclibs)
else :
    msg("ginac library path is not found\n")

msg()



#get defaultlibs
general_libs = splitter(env, env["libs"])
showList("general libs", general_libs)

#remove duplicates
alllibs = []
alllibs.extend(general_libs)
alllibs.extend(boostlibs)
alllibs.extend(atlaslibs)
alllibs.extend(hdflibs)
alllibs.extend(ginaclibs)

alllibs = rmListItem(alllibs, env["ldremove"])

#für Python < 2.4: alllibs = dict(map(lambda i: (i,1),alllibs)).keys()
alllibs = list(set(alllibs))


msg("=== compiling =========================================================================================")
SetOption('num_jobs',   int(os.environ.get('NUM_CPU', COMPILECPU)))

# get all cpp-files and compile
env.Program( getRekusivFiles(os.curdir, ".cpp"), LIBS=alllibs, LIBPATH=library_path )


# build languagefiles
# compiling with: msgfmt -v -o target.mo source.po
# add new data: msgmerge --no-wrap --update old_file.po newer_file.po
sources = []
sources.extend( getRekusivFiles(os.curdir, ".h") )
sources.extend( getRekusivFiles(os.curdir, ".hpp") )
sources.extend( getRekusivFiles(os.curdir, ".cpp") )

cmd = "xgettext --output=tools/language/language.po --keyword=_ --language=c++ ";
for i in sources :
    cmd = cmd + i + " "
os.system(cmd);
