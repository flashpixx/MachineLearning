# -*- coding: utf-8 -*-

import os
import glob
import platform


AddOption("--with-randomdevice", dest="withrandom", type="string", nargs=0, action="store", help="installation with random device support")
AddOption("--with-mpi", dest="withmpi", type="string", nargs=0, action="store", help="installation with MPI support")
AddOption("--with-multilanguage", dest="withmultilanguage", type="string", nargs=0, action="store", help="installation with multilanguage support")
AddOption("--with-sources", dest="withsources", type="string", nargs=0, action="store", help="installation with source like nntp or something else")
AddOption("--with-files", dest="withfiles", type="string", nargs=0, action="store", help="installation with file reading support for CSV & HDF")
AddOption("--create-language", dest="createlang", type="string", nargs=0, action="store", help="reads the data for translation and add them to the different language files")
AddOption("--compile-language", dest="compilelang", type="string", nargs=0, action="store", help="compiles the language files")
AddOption("--create-documentation", dest="createdocu", type="string", nargs=0, action="store", help="creates the doxygen documentation (doxygen must be within the path)")


#=== function for os configuration ===================================================================================================
# using CPUs for compiling (default)
COMPILECPU = 2


# configuration for OSX build
def configuration_macosx(config, version, architecture) :
    # check the OSX build for set the correct architecture
    arch = architecture
    ver  = version.split(".")
    
    if ver[0] == "10" and ver[1] == "6" :
        arch = "x86_64"
    
    
    config["linkerflags"]       = ""
    config["include"]           = os.environ["CPPPATH"]
    config["librarypath"]       = os.environ["LIBRARY_PATH"]
    config["compileflags"]      = "-O2 -pipe -Wall -pthread -finline-functions -arch "+arch+" -D NDEBUG -D BOOST_UBLAS_NDEBUG -D BOOST_NUMERIC_BINDINGS_BLAS_CBLAS"
    config["linkto"]            = ["boost_system", "boost_thread", "boost_iostreams", "ginac", "atlas", "lapack", "ptcblas"]
    
    
    if GetOption("withmpi") == None :
        config["compiler"]          =  "g++"
    else :
        config["compiler"]          = "mpic++"
        config["compileflags"]      += " -D CLUSTER"
        config["linkto"].extend( ["boost_mpi", "boost_serialization"] )
                
    if GetOption("withrandom") != None :   
        config["compileflags"]      += " -D RANDOMDEVICE"
        config["linkto"].append("boost_random");
            
    if GetOption("withmultilanguage") != None :
        config["compileflags"]      += " -D MULTILANGUAGE"
        config["linkto"].append("intl");
        
    if GetOption("withsources") != None :
        config["compileflags"]      += " -D SOURCES"
        
    if GetOption("withfiles") != None :
        config["compileflags"]      += " -D FILES"
        config["linkto"].extend( ["hdf5_cpp", "hdf5"] )
    
    

# configuration for Posix (Linux) build
def configuration_posix(config, version, architecture) :
    config["linkerflags"]       = ""
    config["include"]           = os.environ["CPPPATH"]
    config["librarypath"]       = os.environ["LIBRARY_PATH"]
    config["compileflags"]      = "-O2 -pipe -Wall -pthread -finline-functions -D NDEBUG -D BOOST_UBLAS_NDEBUG -D BOOST_NUMERIC_BINDINGS_BLAS_CBLAS"
    config["linkto"]            = ["boost_system", "boost_thread", "boost_iostreams", "ginac", "atlas", "lapack", "ptcblas", "ptf77blas"]
    
    
    if GetOption("withmpi") == None :
        config["compiler"]          =  "g++"
    else :
        config["compiler"]          = "mpic++"
        config["compileflags"]      += " -D CLUSTER"
        config["linkto"].extend( ["boost_mpi", "boost_serialization"] )
                
    if GetOption("withrandom") != None :   
        config["compileflags"]      += " -D RANDOMDEVICE"
        config["linkto"].append("boost_random");
            
    if GetOption("withmultilanguage") != None :
        config["compileflags"]      += " -D MULTILANGUAGE"

    if GetOption("withsources") != None :
        config["compileflags"]      += " -D SOURCES"

    if GetOption("withfiles") != None :
        config["compileflags"]      += " -D FILES"
        config["linkto"].extend( ["hdf5_cpp", "hdf5"] )
        
        

# configuration for Windows build
def configuration_win32(config, version, architecture) :
    config = []
#=======================================================================================================================================



#=== function for configuration creation================================================================================================

# function for reading os configuration
# and setting environment
def getConfig():
    env = Environment()
    config = {}
    
    if env['PLATFORM'].lower() == "darwin" :
        configuration_macosx(config, platform.mac_ver()[0], platform.machine())
    elif env['PLATFORM'].lower() == "win32" :
        configuration_win32(config, "", platform.machine())
    elif env['PLATFORM'].lower() == "posix" :
        configuration_posix(config, "", platform.machine())
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
    env.Replace(CPPSUFFIXES = [".hpp", ".h", ".cpp"])
    
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
def createLanguage(env, onlycompile=False) :
    # compiling with: msgfmt -v -o target.mo source.po
    # add new data: msgmerge --no-wrap --update old_file.po newer_file.pot

    po = getRekusivFiles(os.curdir, ".po")
    
    if onlycompile :
        # compiling all files
        for i in po :
            os.system( "msgfmt -v -o " + os.path.join(os.path.dirname(i),"ml.mo") +" "+ i )
        
    else :
        sources = []
        for i in env["CPPSUFFIXES"] :
            sources.extend( getRekusivFiles(os.curdir, i) )

        cmd = "xgettext --output=tools/language/language.po --keyword=_ --language=c++ ";
        for i in sources :
            cmd = cmd + i + " "
        os.system(cmd);
        
        
        # get all language files in the subdirs and add the new texts
        for i in po :
            os.system( "msgmerge --no-wrap --update " + i + " tools/language/language.po" )
            
        os.remove("tools/language/language.po")
        
        # compiling all files
        for i in po :
            os.system( "msgfmt -v -o " + os.path.join(os.path.dirname(i),"ml.mo") +" "+ i )
    

#=======================================================================================================================================


#=== create environment and compiling ==================================================================================================
env = getConfig()

SetOption('num_jobs',   int(os.environ.get('NUM_CPU', COMPILECPU)))

# get all cpp-files and compile and create language file
if GetOption("createlang") != None :
    createLanguage(env)
elif GetOption("compilelang") != None :
        createLanguage(env, True)
elif GetOption("createdocu") != None :
    os.system("doxygen documentation.doxyfile")
else :
    env.Program( getRekusivFiles(os.curdir, ".cpp") )
