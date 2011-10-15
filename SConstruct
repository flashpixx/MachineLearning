# -*- coding: utf-8 -*-

import os
import glob
import string
import platform
import sys


#=== CLI parameters ===================================================================================================
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

    vars.Add(EnumVariable("winver", "value of the Windows version", "win7", allowed_values=("win7", "srv2008", "vista", "srv2003sp1", "xpsp2", "srv2003", "xp", "w2000")))

    vars.Add(EnumVariable("atlaslink", "value of the atlas threadding (multi = tatlas, single = satlas)", "multi", allowed_values=("multi", "single")))
    
    vars.Add(EnumVariable("cputype", "value of the cpu type [see: http://gcc.gnu.org/onlinedocs/gcc/i386-and-x86_002d64-Options.html]", "native", allowed_values=("native", "generic", "i386", "i486", "i586", "i686", "pentium-mmx", "pentiumpro", "pentium2", "pentium3", "pentium-m", "pentium4", "prescott", "nocona", "core2", "corei7", "corei7-avx", "core-avx-i", "atom", "k6", "k6-2", "athlon", "athlon-4", "k8", "k8-sse3", "amdfam10", "winchip-c6", "winchip2", "c3", "c3-2", "geode" )))


#=== function for os configuration ===================================================================================================
# configuration for OSX build
def configuration_macosx(config, vars, version, architecture) :
    # check the OSX build for set the correct architecture
    arch = architecture
    ver  = version.split(".")

    if ver[0] == "10" and ver[1] == "6" :
        arch = "x86_64"

    config["shlinkerflags"]     = ""
    config["linkerflags"]       = ""
    config["include"]           = os.environ["CPPPATH"]
    config["librarypath"]       = os.environ["LIBRARY_PATH"]
    config["compileflags"]      = "-pipe -Wall -Wextra -pthread -arch "+arch+" -D BOOST_FILESYSTEM_NO_DEPRECATED -D BOOST_NUMERIC_BINDINGS_BLAS_CBLAS"
    config["linkto"]            = ["boost_system", "boost_thread", "boost_iostreams", "boost_regex"]


    # Java target must not linked again different boost libs
    if not("javac" in COMMAND_LINE_TARGETS) :
        config["linkto"].extend(["boost_program_options", "boost_exception", "boost_filesystem"])
    else :
        # on the java target the installed name of the library must be set
        config["shlinkerflags"] = "-install_name ${TARGET.file} -headerpad_max_install_name"

    if vars["atlaslink"] == "multi" :
        config["linkto"].extend( ["tatlas"] )
    else :
        config["linkto"].extend( ["satlas"] )

    if vars["withdebug"] :
        config["compileflags"]      += " -g"
    else :
        config["compileflags"]      += " -D NDEBUG -D BOOST_UBLAS_NDEBUG"

    if vars["withmpi"] :
        config["compiler"]          = "mpic++"
        config["compileflags"]      += " -D MACHINELEARNING_MPI"
        config["linkto"].extend( ["boost_mpi", "boost_serialization"] )
    else :
        config["compiler"]          =  "g++"

    if vars["withrandomdevice"] :
        config["compileflags"]      += " -D MACHINELEARNING_RANDOMDEVICE"
        config["linkto"].append("boost_random");

    if vars["withmultilanguage"] :
        config["compileflags"]      += " -D MACHINELEARNING_MULTILANGUAGE"
        config["linkto"].append("intl");

    if vars["withsources"] :
        config["compileflags"]      += " -D MACHINELEARNING_SOURCES -D MACHINELEARNING_SOURCES_TWITTER"
        config["linkto"].extend( ["xml2", "json"] )

    if vars["withfiles"] :
        config["compileflags"]      += " -D MACHINELEARNING_FILES -D MACHINELEARNING_FILES_HDF"
        config["linkto"].extend( ["hdf5_cpp", "hdf5"] )

    if vars["withsymbolicmath"] :
        config["compileflags"]      += " -D MACHINELEARNING_SYMBOLICMATH"
        config["linkto"].append("ginac")
        
    if vars["withoptimize"] :
        config["compileflags"]      += " -O2 -Os -s -mfpmath=sse -finline-functions -mtune="+vars["cputype"]
        
    if vars["withlogger"] :
        config["compileflags"]      += " -D MACHINELEARNING_LOGGER"


# configuration for Posix (Linux) build
def configuration_posix(config, vars, version, architecture) :
    config["shlinkerflags"]     = ""
    config["linkerflags"]       = ""
    config["include"]           = os.environ["CPPPATH"]
    config["librarypath"]       = os.environ["LIBRARY_PATH"]
    config["compileflags"]      = "-pipe -Wall -Wextra -pthread -D BOOST_FILESYSTEM_NO_DEPRECATED -D BOOST_NUMERIC_BINDINGS_BLAS_CBLAS"
    config["linkto"]            = ["boost_system", "boost_thread", "boost_iostreams", "boost_regex"]


    # Java target must not linked again different boost libs
    if not("javac" in COMMAND_LINE_TARGETS) :
        config["linkto"].extend(["boost_program_options", "boost_exception", "boost_filesystem"])
    else :
        config["linkerflags"] = "-Wl,--rpath=\\$$ORIGIN"

    if vars["atlaslink"] == "multi" :
        config["linkto"].extend( ["tatlas"] )
    else :
        config["linkto"].extend( ["satlas"] )

    if vars["withdebug"] :
        config["compileflags"]      += " -g"
    else :
        config["compileflags"]      += " -D NDEBUG -D BOOST_UBLAS_NDEBUG"

    if vars["withmpi"] :
        config["compiler"]          = "mpic++"
        config["compileflags"]      += " -D MACHINELEARNING_MPI"
        config["linkto"].extend( ["boost_mpi", "boost_serialization"] )
    else :
        config["compiler"]          =  "g++"

    if vars["withrandomdevice"] :
        config["compileflags"]      += " -D MACHINELEARNING_RANDOMDEVICE"
        config["linkto"].append("boost_random");

    if vars["withmultilanguage"] :
        config["compileflags"]      += " -D MACHINELEARNING_MULTILANGUAGE"

    if vars["withsources"] :
        config["compileflags"]      += " -D MACHINELEARNING_SOURCES -D MACHINELEARNING_SOURCES_TWITTER"
        config["linkto"].extend( ["xml2", "json"] )

    if vars["withfiles"] :
        config["compileflags"]      += " -D MACHINELEARNING_FILES -D MACHINELEARNING_FILES_HDF"
        config["linkto"].extend( ["hdf5_cpp", "hdf5"] )

    if vars["withsymbolicmath"] :
        config["compileflags"]      += " -D MACHINELEARNING_SYMBOLICMATH"
        config["linkto"].append("ginac")
        
    if vars["withoptimize"] :
        config["compileflags"]      += " -O2 -Os -s -mfpmath=sse -finline-functions -mtune="+vars["cputype"]
        
    if vars["withlogger"] :
        config["compileflags"]      += " -D MACHINELEARNING_LOGGER"


# configuration for Windows Cygwin build
def configuration_cygwin(config, vars, version, architecture) :
    config["shlinkerflags"]     = ""
    config["linkerflags"]       = "-enable-stdcall-fixup"
    config["include"]           = os.environ["CPPPATH"]
    config["librarypath"]       = os.environ["PATH"]
    config["compileflags"]      = "-pipe -Wall -Wextra -D BOOST_FILESYSTEM_NO_DEPRECATED -D BOOST_NUMERIC_BINDINGS_BLAS_CBLAS"
    config["linkto"]            = ["cygboost_system", "cygboost_thread", "cygboost_iostreams", "cygboost_regex"]


    # Java target must not linked again different boost libs
    if not("javac" in COMMAND_LINE_TARGETS) :
        config["linkto"].extend(["cygboost_program_options", "boost_exception", "cygboost_filesystem"])

    #Windows Version options see http://msdn.microsoft.com/en-us/library/aa383745%28v=vs.85%29.aspx
    if vars["winver"] == "win7" :
        config["compileflags"] += " -D _WIN32_WINNT=0x0601"
    elif vars["winver"] == "srv2008" :
        config["compileflags"] += " -D _WIN32_WINNT=0x0600"
    elif vars["winver"] == "vista" :
        config["compileflags"] += " -D _WIN32_WINNT=0x0600"
    elif vars["winver"] == "srv2003sp1" :
        config["compileflags"] += " -D _WIN32_WINNT=0x0502"
    elif vars["winver"] == "xpsp2" :
        config["compileflags"] += " -D _WIN32_WINNT=0x0502"
    elif vars["winver"] == "srv2003" :
        config["compileflags"] += " -D _WIN32_WINNT=0x0501"
    elif vars["winver"] == "xp" :
        config["compileflags"] += " -D _WIN32_WINNT=0x0501"
    elif vars["winver"] == "w2000" :
        config["compileflags"] += " -D _WIN32_WINNT=0x0500"

    if vars["atlaslink"] == "multi" :
        config["linkto"].extend( ["tatlas", "lapack", "cblas", "f77blas", "atlas", "gfortran"] )
    else :
        config["linkto"].extend( ["satlas", "lapack", "cblas", "f77blas", "atlas", "gfortran"] )

    if vars["withdebug"] :
        config["compileflags"]      += " -g"
    else :
        config["compileflags"]      += " -D NDEBUG -D BOOST_UBLAS_NDEBUG"

    if vars["withmpi"] :
        print "MPI builds are not existing under Cygwin"
        sys.exit(1)
    else :
        config["compiler"]          =  "g++"

    if vars["withrandomdevice"] :
        config["compileflags"]      += " -D MACHINELEARNING_RANDOMDEVICE"
        config["linkto"].append("cygboost_random");

    if vars["withmultilanguage"] :
        print "Multilanguage support builds are not existing under Cygwin"
        sys.exit(1)

    if vars["withsources"] :
        config["compileflags"]      += " -D MACHINELEARNING_SOURCES -D MACHINELEARNING_SOURCES_TWITTER -D __USE_W32_SOCKETS"
        config["linkto"].extend( ["cygxml2-2", "ws2_32", "json"] )

    if vars["withfiles"] :
        config["compileflags"]      += " -D MACHINELEARNING_FILES -D MACHINELEARNING_FILES_HDF"
        config["linkto"].extend( ["hdf5_cpp", "hdf5"] )

    if vars["withsymbolicmath"] :
        config["compileflags"]      += " -D MACHINELEARNING_SYMBOLICMATH"
        config["linkto"].append("ginac")
        
    if vars["withoptimize"] :
        config["compileflags"]      += " -O2 -Os -s -mfpmath=sse -finline-functions -mtune="+vars["cputype"]
        
    if vars["withlogger"] :
        config["compileflags"]      += " -D MACHINELEARNING_LOGGER"
#=======================================================================================================================================



#=== function for configuration creation================================================================================================
# function for reading os configuration
# and setting environment
def getConfig(vars):
    env = Environment(variables=vars)
    config = {}
    
    # read path and add environment path values
    syspath = [env["ENV"]["PATH"], os.environ["PATH"]]

    if env['PLATFORM'].lower() == "darwin" :
        configuration_macosx(config, env, platform.mac_ver()[0], platform.machine())
        env["ENV"]["PATH"] = ":".join(syspath)
        
    elif env['PLATFORM'].lower() == "cygwin" :
        configuration_cygwin(config, env, "", platform.machine())
        env["ENV"]["PATH"] = ":".join(syspath)
        
    elif env['PLATFORM'].lower() == "posix" :
        configuration_posix(config, env, "", platform.machine())
        env["ENV"]["PATH"] = ":".join(syspath)

    else :
        print "configuration for ["+env['PLATFORM']+"] not exists"
        exit(1)


    if not(config) :
        print "Configuration is empty"
        exit(1)

    for i in [ "compiler", "compileflags", "linkerflags", "shlinkerflags", "include", "librarypath", "linkto" ] :
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
    env.Replace(CPPPATH     = config["include"].split(os.pathsep))
    env.Replace(LINKFLAGS   = config["linkerflags"])
    env.Replace(LIBS        = config["linkto"])
    env.Replace(LIBPATH     = config["librarypath"])
    env.Replace(CPPSUFFIXES = [".hpp", ".h", ".cpp"])
    env.Append(SHLINKFLAGS  = config["shlinkerflags"])
    
    # Scons < 2: env.BuildDir("build", ".", duplicate=0)
    env.VariantDir("build", ".", duplicate=0)
    env.Append(CPPPATH=["."])

    #dict = env.Dictionary()
    #for i,j in dict.iteritems():
    #    print i, j

    return env



# get rekursiv all files
def getRekusivFiles(startdir, ending, pdontuse=[], pShowPath=True, pAbsPath=False) :
    lst = []

    for root, dirs, filenames in os.walk(startdir) :
        for filename in filenames :
            if filename.endswith(ending) :
                if pShowPath :

                    if pAbsPath :
                        lst.append( os.path.abspath(os.path.join(root, filename)) )
                    else :
                        lst.append( os.path.join(root, filename) )
                else :

                    if pAbsPath :
                        lst.append(os.path.abspath(filename))
                    else :
                        lst.append(filename)

    clst = []
    if not pdontuse :
        clst.extend(lst)
    else :
        ldontuse = [os.path.join(startdir, i) for i in pdontuse]
        for i in lst :
            lladd = True;

            for n in ldontuse :
                lladd = lladd and not(i.startswith(n));
                
            if lladd :
                clst.append(i)

    return clst

# creates a list with unique entries    
def unique(seq):
    keys = {}
    for e in seq:
        keys[e] = 1
    return keys.keys()
#=======================================================================================================================================



#=== build targets =====================================================================================================================

# create target
def createTarget(env, alias, path, sources, framework) :
    lst = []
    if sources.count == 0 :
        return

    for i in sources :
        compilesource = [os.path.join(path, i)]
        compilesource.extend(framework)

        lst.append( env.Program( target=os.path.join("#build", os.path.splitext(i)[0]), source=compilesource ) )

    env.Alias(alias, lst)



# target for building CPP examples
def target_cpp(env, framework) :
    createTarget(env, "ga", os.path.join(".", "examples", "geneticalgorithm"), ["knapsack.cpp"], framework)
    
    srcOther = []
    if env["withfiles"] and env["withsources"] :
        srcOther.extend( ["mds_nntp.cpp", "mds_wikipedia.cpp", "mds_twitter.cpp"] )
    if env["withfiles"] :
        srcOther.append("mds_file.cpp")
    createTarget(env, "other", os.path.join(".", "examples", "other"), srcOther, framework)
    
    srcClassifier = []
    if env["withfiles"] :
        srcClassifier.extend( ["lazy.cpp"] )
    createTarget(env, "classifier", os.path.join(".", "examples", "classifier"), srcClassifier, framework)
    
    srcDistance = []
    if env["withfiles"] :
        srcDistance.extend( ["ncd.cpp"] )
    createTarget(env, "distance", os.path.join(".", "examples", "distance"), srcDistance, framework)
    
    srcReduce = []
    if env["withfiles"] :
        srcReduce.extend( ["lda.cpp", "mds.cpp", "pca.cpp"] )
    createTarget(env, "reducing", os.path.join(".", "examples", "reducing"), srcReduce, framework)
    
    srcCluster = []
    if env["withfiles"] :
        srcCluster.extend( ["rlvq.cpp", "kmeans.cpp", "neuralgas.cpp", "patch_neuralgas.cpp", "relational_neuralgas.cpp", "spectral.cpp"] )
    createTarget(env, "clustering", os.path.join(".", "examples", "clustering"), srcCluster, framework)
    
    srcSources = []
    if env["withsources"] :
        srcSources.extend( ["twitter.cpp", "newsgroup.cpp", "wikipedia.cpp"] )
    if env["withfiles"] :
        srcSources.append( "cloud.cpp" )
    createTarget(env, "sources", os.path.join(".", "examples", "sources"), srcSources, framework)
    
    
# change the library calls under OSX    
def java_osxlinkedlibs(target, source, env) :
    global otoolChange

    oFile = open( os.path.join(os.curdir, "build", "linkedlibs.txt"), "r" )
    libs = oFile.read()
    oFile.close()

    # split string on each space
    names = []
    lines = libs.split()
    
    # test each linked library to the otool results
    for n in env["LIBS"] :
        for i in lines :
            if n in i :
                names.append(i)
    
    # build change list
    change = []
    for i in names :
        name = i.split(os.sep)[-1]
        change.append("-change " + i + " @loader_path" + os.path.sep + name)
    change = unique(change)
    
    # run the build command on a system shell
    os.system( "install_name_tool " + " ".join(change) + " " + os.path.join("build", "javalib", "native", env["LIBPREFIX"]+"machinelearning"+env["SHLIBSUFFIX"]) )
    return []



# changes the library calls under Linux (sonames)
def java_linuxsonames(target, source, env) :

    # read all files within the native directory and get the SONAME of each library and change the filename
    for root, dirs, filenames in os.walk( os.path.join(os.curdir, "build", "javalib", "native") ) :
        os.system( "objdump -p " + filenames + " | grep -i soname | awk '{print \"mv " + filenames + " \"$2}'" )

    return []



# target for building java package    
def target_javac(env, framework) :
    if env["withmpi"] :
        print "MPI build does not work with Java"
        sys.exit(1)

    targets = []

    # compile Java classes
    targets.append( targets.extend( env.Java(target=os.path.join("#build", "javalib"), source=os.path.join(os.curdir, "java")) ) )

    # list with Java classes that are used for the JavaP command
    javaplist = []
    javaplist.extend(targets)

    # create JNI stubs and JavaP command (so we add the classes manually)
    stubs = ["machinelearning.dimensionreduce.nonsupervised.PCA", "machinelearning.dimensionreduce.nonsupervised.MDS"]
    for i in stubs :
        # split file and directory parts and substitute $ to _ and create the headerfile
        parts = i.replace("$", "_").split(".") 
        headerfile = (os.sep.join(parts) + ".h").lower()
        
        targets.append( env.Command( headerfile, "", "javah -classpath " + os.path.join(os.curdir, "build", "javalib") + " -o " + os.path.join(os.curdir, "java", headerfile) + " " + i  ) )
        
    # build SharedLibrary
    sources = getRekusivFiles( os.path.join(os.curdir, "java"), ".cpp")
    sources.extend(framework)
    targets.append( env.SharedLibrary( target=os.path.join("#build", "javalib", "native", "machinelearning"), source=sources ) )
    
    # on OSX the path of the linked libraries within the libmachinelearning.dylib must be changed to @loader_path/<library>
    if env['PLATFORM'].lower() == "darwin" :
        targets.append( env.Command("createlibrarynames", "", "otool -L " + os.path.join("build", "javalib", "native", env["LIBPREFIX"]+"machinelearning"+env["SHLIBSUFFIX"]) + " > " + os.path.join("build", "linkedlibs.txt") ) )
        targets.append( env.Command("linkedlibs", "", java_osxlinkedlibs) ) 
        targets.append( env.Command("libnames.txt", "", Delete(os.path.join(os.curdir, "build", "linkedlibs.txt")) ) )
        

    # copy external libraries in the native directory for Jar adding (copy works only if target directories exists)
    dirs      = env["LIBPATH"].split(os.pathsep)
    copyfiles = []
    for n in env["LIBS"] :
        libfiles = env.FindFile(env["LIBPREFIX"] + n + env["SHLIBSUFFIX"], dirs)
        if libfiles <> None :
            # remove any symbolic names, so the file in the native directory becomes the original filename
            name = os.path.realpath(libfiles.path).split(os.path.sep)[-1]

            copyfiles.append( Copy(os.path.join("build", "javalib", "native", name), libfiles.path) )
    targets.append( env.Command("copyexternallib", "", copyfiles) )
    
    # on Linux all libraries must use the filename, that is set with the "soname" within the library or filename must be changed to "soname"
    if env['PLATFORM'].lower() == "posix" :
        targets.append( env.Command("sonames", "", java_linuxsonames) ) 
    
    # build Jar and create Jar Index
    targets.append( env.Command("buildjar", "", "jar cf " + os.path.join(os.curdir, "build", "machinelearning.jar") + " -C " + os.path.join("build", "javalib" ) + " .") )
    targets.append( env.Command("buildjarindex", "", "jar i " + os.path.join(os.curdir, "build", "machinelearning.jar") ) )
    
    env.Alias("javac", targets)
    
    
    
# targets for building java examples
def target_javaexamples(env) :
    # set classpath only for example compiling (jar file must be set within the build directory)
    env.Alias("javareduce", env.Java(target=os.path.join("#build", "java", "reduce"), source=os.path.join(os.curdir, "examples", "java", "reducing"), JAVACLASSPATH = [os.path.join(os.curdir, "build", "machinelearning.jar")]) )
    
    
    
# target for building language files
def target_language(env) :
    sources = []
    for i in env["CPPSUFFIXES"] :
        sources.extend( getRekusivFiles(os.curdir, i, ["examples"]) )

    # get all strings out of the sources
    updatetargets = []
    createtargets = []
    updatetargets.append( env.Command("xgettext", "", "xgettext --output="+os.path.join("tools", "language", "language.po")+" --keyword=_ --language=c++ " + " ".join(sources)) )
    createtargets.extend( updatetargets )


    # get all language files in the subdirs and add the new texts
    po = getRekusivFiles(os.curdir, ".po", ["examples", "java", "build"])
    for i in po :
        updatetargets.append( env.Command("msmerge", "", "msgmerge --no-wrap --update " + i + " "+os.path.join("tools", "language", "language.po") ) )
        createtargets.append( env.Command("msmerge", "", "msgmerge --no-wrap --update " + i + " "+os.path.join("tools", "language", "language.po") ) )
        
    createtargets.append( env.Command("deletelang", "", [Delete(os.path.join("tools", "language", "language.po"))] ) )
    updatetargets.append( env.Command("deletelang", "", [Delete(os.path.join("tools", "language", "language.po"))] ) )

    # compiling all files
    for i in po :
        updatetargets.append( env.Command("msgfmt", "", "msgfmt -v -o " + os.path.join(os.path.dirname(i),"ml.mo") +" "+ i ) )
    
    env.Alias("updatelanguage", updatetargets)
    env.Alias("createlanguage", createtargets)            
    
    
def target_documentation(env) :
    env.Alias("documentation", env.Command("doxygen", "", "doxygen documentation.doxyfile"))
#=======================================================================================================================================





#=== create environment and compiling ==================================================================================================
vars = Variables()
createVariables(vars)
env = getConfig(vars)
Help(vars.GenerateHelpText(env))

# add files for deleting
files = []
files.extend( getRekusivFiles(os.curdir, env["OBJSUFFIX"]) )
files.extend( getRekusivFiles(os.curdir, env["SHOBJSUFFIX"]) )
files.extend( getRekusivFiles(os.curdir, env["SHLIBSUFFIX"]) )
files.extend( getRekusivFiles(os.curdir, ".jnilib") )
files.extend( getRekusivFiles(os.curdir, ".po~") )
files.extend( getRekusivFiles(os.curdir, ".jar") )

env.Clean("clean", files)


# catch all cpps within the framework directories and compile them to objectfiles into the builddir (manual set for static initializations)
#framework = getRekusivFiles(os.curdir, ".cpp", ["examples"])
framework = []
if env["withlogger"] or env["withrandomdevice"] :
    framework.append( os.path.join( os.curdir, "machinelearning.cpp" ) )

# create building targets
target_language( env )
target_documentation( env )

target_javac( env, framework )
target_javaexamples( env )

target_cpp(env, framework )
