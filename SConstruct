# -*- coding: utf-8 -*-

import os
import glob

#=== function for os configuration ===================================================================================================
print "==>> real <<==\n\n"
# using CPUs for compiling
COMPILECPU = 6
# constante for creating language files
CREATELANGUAGE = False

def configuration_macosx(config) :
    config["compiler"]          = "mpic++"
    config["compileflags"]      = "-O2 -pipe -Wall -pthread -finline-functions -D CLUSTER -D MULTILANGUAGE -D RANDOMDEVICE -D NDEBUG -D BOOST_UBLAS_NDEBUG -D BOOST_NUMERIC_BINDINGS_BLAS_CBLAS"
    config["linkerflags"]       = ""
    config["include"]           = os.environ["CPPPATH"]
    config["librarypath"]       = os.environ["LIBRARY_PATH"]
    config["linkto"]            = ["intl", "boost_mpi", "boost_serialization", "boost_random", "boost_thread", "hdf5_cpp", "hdf5", "ginac", "atlas", "lapack", "ptcblas"]

    
    
def configuration_posix(config) :
    config["compiler"]          = "mpic++"
    config["compileflags"]      = "-O2 -pipe -Wall -pthread -finline-functions -D CLUSTER -D MULTILANGUAGE -D RANDOMDEVICE -D NDEBUG -D BOOST_UBLAS_NDEBUG -D BOOST_NUMERIC_BINDINGS_BLAS_CBLAS"
    config["linkerflags"]       = ""
    config["include"]           = os.environ["CPPPATH"]
    config["librarypath"]       = os.environ["LIBRARY_PATH"]
    config["linkto"]            = ["boost_mpi", "boost_serialization", "boost_random", "boost_thread", "hdf5_cpp", "hdf5", "ginac", "atlas", "lapack", "ptcblas"]

    
def configuration_win32(config) :
    config = []
#=======================================================================================================================================



#=== function for configuration creation================================================================================================

# function for reading os configuration
# and setting environment
def getConfig():
    env = Environment()
    config = {}
    
    if env['PLATFORM'].lower() == "darwin" :
        configuration_macosx(config)
    elif env['PLATFORM'].lower() == "win32" :
        configuration_win32(config)
    elif env['PLATFORM'].lower() == "posix" :
        configuration_posix(config)
    else :
        print "configuration for ["+env['PLATFORM']+"] not exists"
        exit(1) 
    
    if not(config) :
        print "Configuration is empty"
        exit(1)

    for i in [ "compiler", "compileflags", "linkerflags", "include", "librarypath", "linkto" ] :
        if not(config.has_key(i)) :
            print "field ["+i+"] is not set in the configuration"
            exit(1)

    for i in [ "compiler", "compileflags", "linkerflags", "include", "librarypath" ] :
        if type(config[i]) <> type("str") :
            print "field ["+i+"] must be a string type"
            exit(1)

    for i in [ "linkto" ] :
        if type(config[i]) <> type([]) :
            print "field ["+i+"] must be a list type"
            exit(1)

    if not(config["compiler"]) :
        print "no compiler is set, using default"
        if not(env["CXX"]) :
            print "no compiler set"
            exit(1)
    else :
        env.Replace(CXX         = config["compiler"])
    
    env.Replace(CXXFLAGS    = config["compileflags"])
    env.Replace(CPPPATH     = config["include"])
    env.Replace(LINKFLAGS   = config["linkerflags"])
    env.Replace(LIBS        = config["linkto"])
    env.Replace(LIBPATH     = config["librarypath"])
    
    #dict = env.Dictionary()
    #for i,j in dict.iteritems():
    #    print i, j		   
    
    return env
    
    
       
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
        
       
# build languagefiles
def createLanguage() :
    if not(CREATELANGUAGE) :
        return

    # compiling with: msgfmt -v -o target.mo source.po
    # add new data: msgmerge --no-wrap --update old_file.po newer_file.pot
    sources = []
    sources.extend( getRekusivFiles(os.curdir, ".h") )
    sources.extend( getRekusivFiles(os.curdir, ".hpp") )
    sources.extend( getRekusivFiles(os.curdir, ".cpp") )

    cmd = "xgettext --output=tools/language/language.pot --keyword=_ --language=c++ ";
    for i in sources :
        cmd = cmd + i + " "
    os.system(cmd);

#=======================================================================================================================================


#=== create environment and compiling ==================================================================================================
env = getConfig()

SetOption('num_jobs',   int(os.environ.get('NUM_CPU', COMPILECPU)))

# get all cpp-files and compile
env.Program( getRekusivFiles(os.curdir, ".cpp") )

createLanguage()