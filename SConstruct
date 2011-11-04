# -*- coding: utf-8 -*-

import os
import glob
import string
import platform
import sys
import urllib2
import re
import shutil
import subprocess


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
    
    vars.Add(EnumVariable("atlaspointerwidth", "pointer width for compiling ATLAS (empty = system default, 32 = 32 Bit, 64 = 64 Bit)", "", allowed_values=("", "32", "64")))
    vars.Add(BoolVariable("atlascputhrottle", "enable / disable detection of CPU throtteling", False))
    vars.Add(ListVariable("skipbuild", "skipping library builds", "", ["atlas", "boost", "hdf", "ginac", "json"]))
    vars.Add(BoolVariable("skipbuilderror", "ignore / skip build errors", False))


#=== function for os configuration ===================================================================================================
# configuration for OSX build
def configuration_macosx(config, vars, version, architecture) :
    # check the OSX build for set the correct architecture
    arch = architecture
    ver  = version.split(".")

    if ver[0] == "10" and ver[1] == "6" :
        arch = "x86_64"
    elif ver[0] == "10" and ver[1] == "7" :
        arch = "x86_64"


    config["shlinkerflags"]     = ""
    config["linkerflags"]       = "-pthread"
    config["include"]           = ""
    config["librarypath"]       = ""
    
    if os.environ.has_key("CPPPATH") :
        config["include"]       = os.environ["CPPPATH"]
    elif os.environ.has_key("CPATH") :
        config["include"]       = os.environ["CPATH"]
        
    if os.environ.has_key("DYLD_LIBRARY_PATH") :
        config["librarypath"]   = os.environ["DYLD_LIBRARY_PATH"]
    elif os.environ.has_key("LD_LIBRARY_PATH") :
        config["librarypath"]   = os.environ["LD_LIBRARY_PATH"]
    elif os.environ.has_key("LIBRARY_PATH") :
        config["librarypath"]   = os.environ["LIBRARY_PATH"]
    
        
    config["compileflags"]      = "-pipe -Wall -Wextra -arch "+arch+" -D BOOST_FILESYSTEM_NO_DEPRECATED -D BOOST_NUMERIC_BINDINGS_BLAS_CBLAS"
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
    config["linkerflags"]       = "-pthread"
    config["include"]           = ""
    config["librarypath"]       = ""
    
    if os.environ.has_key("CPPPATH") :
        config["include"]       = os.environ["CPPPATH"]
    elif os.environ.has_key("CPATH") :
        config["include"]       = os.environ["CPATH"]
    
    if os.environ.has_key("LD_LIBRARY_PATH") :
        config["librarypath"]   = os.environ["LD_LIBRARY_PATH"]
    elif os.environ.has_key("LIBRARY_PATH") :
        config["librarypath"]   = os.environ["LIBRARY_PATH"]
    
    config["compileflags"]      = "-pipe -Wall -Wextra -D BOOST_FILESYSTEM_NO_DEPRECATED -D BOOST_NUMERIC_BINDINGS_BLAS_CBLAS"
    config["linkto"]            = ["boost_system", "boost_thread", "boost_iostreams", "boost_regex", "gfortran"]


    # Java target must not linked again different boost libs
    if not("javac" in COMMAND_LINE_TARGETS) :
        config["linkto"].extend(["boost_program_options", "boost_exception", "boost_filesystem"])
    else :
        config["linkerflags"] = "-Wl,--rpath=\\$$ORIGIN"

    if vars["atlaslink"] == "multi" :
        config["linkto"].extend( ["lapack", "ptcblas", "ptf77blas", "atlas"] )
    else :
        config["linkto"].extend( ["lapack", "cblas", "f77blas", "atlas"] )

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
    config["linkerflags"]       = "-enable-stdcall-fixup -mthread -mconsole"
    config["include"]           = ""
    config["librarypath"]       = ""
    
    if os.environ.has_key("CPPPATH") :
        config["include"]       = os.environ["CPPPATH"]
    elif os.environ.has_key("CPATH") :
        config["include"]       = os.environ["CPATH"]
        
    if os.environ.has_key("PATH") :
        config["librarypath"]   = os.environ["PATH"]
    elif os.environ.has_key("LD_LIBRARY_PATH") :
        config["librarypath"]   = os.environ["LD_LIBRARY_PATH"]
    elif os.environ.has_key("LIBRARY_PATH") :
        config["librarypath"]   = os.environ["LIBRARY_PATH"]
    
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

	# Atlas build creates a static library under Cygwin, so we link directly without the "atlaslink" option
    # Library sequence must be preserved !!
    config["linkto"].extend( ["lapack", "cblas", "f77blas", "atlas", "gfortran"] )

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
    syspath = []
    if os.environ.has_key("PATH") :
        syspath = [env["ENV"]["PATH"], os.environ["PATH"]]

    if env['PLATFORM'].lower() == "darwin" :
        configuration_macosx(config, env, platform.mac_ver()[0], platform.machine())
        env["ENV"]["PATH"] = os.pathsep.join(syspath)

    elif env['PLATFORM'].lower() == "cygwin" :
        configuration_cygwin(config, env, "", platform.machine())
        env["ENV"]["PATH"] = os.pathsep.join(syspath)

    elif env['PLATFORM'].lower() == "posix" :
        configuration_posix(config, env, "", platform.machine())
        env["ENV"]["PATH"] = os.pathsep.join(syspath)

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
def createTarget(env, path, sources, framework) :
    lst = []
    if sources.count == 0 :
        return

    for i in sources :
        compilesource = [os.path.join(path, i)]
        compilesource.extend(framework)

        lst.append( env.Program( target=os.path.join("#build", os.path.splitext(i)[0]), source=compilesource ) )

    return lst



# target for building CPP examples
def target_cpp(env, framework) :
    listGA = createTarget(env, os.path.join(".", "examples", "geneticalgorithm"), ["knapsack.cpp"], framework)

    srcOther = []
    if env["withfiles"] and env["withsources"] :
        srcOther.extend( ["mds_nntp.cpp", "mds_wikipedia.cpp", "mds_twitter.cpp"] )
    if env["withfiles"] :
        srcOther.append("mds_file.cpp")
    listOther = createTarget(env, os.path.join(".", "examples", "other"), srcOther, framework)

    srcClassifier = []
    if env["withfiles"] :
        srcClassifier.extend( ["lazy.cpp"] )
    listClassifier = createTarget(env, os.path.join(".", "examples", "classifier"), srcClassifier, framework)

    srcDistance = []
    if env["withfiles"] :
        srcDistance.extend( ["ncd.cpp"] )
    listDistance = createTarget(env, os.path.join(".", "examples", "distance"), srcDistance, framework)

    srcReduce = []
    if env["withfiles"] :
        srcReduce.extend( ["lda.cpp", "mds.cpp", "pca.cpp"] )
    listReduce = createTarget(env, os.path.join(".", "examples", "reducing"), srcReduce, framework)

    srcCluster = []
    if env["withfiles"] :
        srcCluster.extend( ["rlvq.cpp", "kmeans.cpp", "neuralgas.cpp", "patch_neuralgas.cpp", "relational_neuralgas.cpp", "spectral.cpp"] )
    listCluster = createTarget(env, os.path.join(".", "examples", "clustering"), srcCluster, framework)

    srcSources = []
    if env["withsources"] :
        srcSources.extend( ["twitter.cpp", "newsgroup.cpp", "wikipedia.cpp"] )
    if env["withfiles"] :
        srcSources.append( "cloud.cpp" )
    listSources = createTarget(env, os.path.join(".", "examples", "sources"), srcSources, framework)
    
    # create language files if multilanguage is used
    if env["withmultilanguage"] :
        lst = []
        lang = getRekusivFiles( os.path.join(os.curdir, "tools", "language"), ".po")
        for i in lang :
            lst.append( env.Command("msgfmt", "", "msgfmt -v -o " + os.path.join(os.path.dirname(i),"machinelearning.mo") +" "+ i ) )
            src      = i.split(os.path.sep)[1:]
            src[-1]  = "machinelearning.mo"

            langfiles     = i.split(os.path.sep)[3:]
            langfiles[-1] = "machinelearning.mo"            
            target = ["build", "language"]
            target.extend(langfiles)
            
            lst.append( env.Command("mkdirlang", "", Mkdir(os.path.dirname(os.path.sep.join(target)))) )
            lst.append( env.Command("copylang", "", Copy(os.path.sep.join(target), os.path.sep.join(src))) )
            
        listGA.extend( lst )
        listOther.extend( lst )
        listClassifier.extend( lst )
        listDistance.extend( lst )
        listReduce.extend( lst )
        listCluster.extend( lst )
        listSources.extend( lst )
    
    # adding targets
    env.Alias("ga", listGA)
    env.Alias("other", listOther)
    env.Alias("classifier", listClassifier)
    env.Alias("distance", listDistance)
    env.Alias("reducing", listReduce)
    env.Alias("clustering", listCluster)
    env.Alias("sources", listSources)




# change the library calls under OSX
def java_osxlinkedlibs(target, source, env) :
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
    for root, dirs, filenames in os.walk(os.path.join("build", "javalib", "native")) :
        for filename in filenames :
    	    os.system( "objdump -p build/javalib/native/" + filename + " | grep -i soname | awk '{system(\"mv build/javalib/native/" + filename + " build/javalib/native/\"$2)}'" )

    return []



# target for building java package
def target_javac(env, framework) :
    if env["withmpi"] and ("javac" in COMMAND_LINE_TARGETS) :
        print "MPI build does not work with Java"
        sys.exit(1)

    targets = []

    # compile Java classes
    targets.append( targets.extend( env.Java(target=os.path.join("#build", "javalib"), source=os.path.join(os.curdir, "java")) ) )

    # list with Java classes that are used for the JavaP command
    javaplist = []
    javaplist.extend(targets)

    # create JNI stubs and JavaP command (so we add the classes manually)
    stubs = ["machinelearning.dimensionreduce.nonsupervised.PCA", "machinelearning.dimensionreduce.nonsupervised.MDS", "machinelearning.util.Math", "machinelearning.util.Random"]
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
    libs      = []
    copyfiles = []
    
    libs.extend(env["LIBS"])
    if env['PLATFORM'].lower() == "cygwin" :
        libs.extend(["cygwin1", "cyggcc_s-1", "cyggfortran-3", "cygstdc++-6"])
    
    for n in libs :
        libname = ""
        if env['PLATFORM'].lower() == "darwin" or env['PLATFORM'].lower() == "postfix" :
            libname = env["LIBPREFIX"] + n + env["SHLIBSUFFIX"]
        elif env['PLATFORM'].lower() == "cygwin" :
            libname = n + env["SHLIBSUFFIX"]

        libfiles = env.FindFile(libname, dirs)
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
    env.Alias("javautil", env.Java(target=os.path.join("#build", "java", "util"), source=os.path.join(os.curdir, "examples", "java", "util"), JAVACLASSPATH = [os.path.join(os.curdir, "build", "machinelearning.jar")]) )



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
        updatetargets.append( env.Command("msgfmt", "", "msgfmt -v -o " + os.path.join(os.path.dirname(i),"machinelearning.mo") +" "+ i ) )

    env.Alias("updatelanguage", updatetargets)
    env.Alias("createlanguage", createtargets)




# target for creating documentation
def target_documentation(env) :
    env.Alias("documentation", env.Command("doxygen", "", "doxygen documentation.doxyfile"))
    
    

    
#=== building depend libraries =======================================================================================================
def runsyscmd(cmd, env) :
    ret = subprocess.call( cmd, shell=True )
    if ret <> 0 and not(env["skipbuilderror"]) :
        print "\nan error occurred during building"
        res = ""
        while res != "a" and res != "c" :
            res = raw_input("(a)bort or (c)ontinue: ")
            if res == "a" :
                sys.exit(1)
                

def download_boost(target, source, env)  :
    if os.path.exists(os.path.join(os.curdir, "install", "boost.tar.bz2")) :
        return []

    # read download path of the Boost (latest version)
    f = urllib2.urlopen("http://www.boost.org/users/download/")
    html = f.read()
    f.close()
    
    found = re.search("<a href=\"http://sourceforge.net/projects/boost/files/(.*)\">Download</a>", html)
    if found == None :
        print "Boost Download URL not found"
        sys.exit(1)
    downloadurl = found.group(0)
    downloadurl = downloadurl.replace("<a href=\"", "")
    downloadurl = downloadurl.replace("\">Download</a>", "")
    
    # read url of the tar.bz2
    f = urllib2.urlopen(downloadurl)
    html = f.read()
    f.close()

    found = re.search("<a href=\"http://sourceforge.net/projects/boost/files/boost(.*).tar.bz2/download", html)
    if found == None :
        print "Boost Download URL not found"
        sys.exit(1)
    downloadurl = found.group(0)
    downloadurl = downloadurl.replace("<a href=\"", "")
    
    # saves the boost into "boost.tar.bz2"
    target = open( os.path.join(os.curdir, "install", "boost.tar.bz2"), "w" )
    f = urllib2.urlopen(downloadurl)
    target.write(f.read())
    target.close()
    f.close()
    
    return []
    
    
def build_boost(target, source, env)  :
    boostpath = glob.glob(os.path.join("install", "boost_*"))
    if boostpath == None or not(boostpath) :
        print "Boost Build Directory not found"
        sys.exit(1)
    boostpath     = boostpath[0]
    
    # extract the version part
    boostversion  = boostpath.replace(os.path.join("install", "boost_"), "")
    boostversion  = boostversion.replace("_", ".")

    # for calling bootstrap.sh change the current work directory
    runsyscmd("cd "+boostpath+"; ./bootstrap.sh", env)
    
    # call the bjam command
    toolset = "gcc"
    if env['PLATFORM'].lower() == "darwin" :
        toolset = "darwin"
        
    # if MPI is set, compile Boost with MPI support
    mpi = ""
    if env["withmpi"] :
        oFile = open(os.path.join(boostpath, "tools", "build", "v2", "user-config.jam"), "a+")
        oFile.write("\n using mpi ;\n")
        oFile.close()
        mpi = "--with-mpi"
            
    # build the Boost
    runsyscmd("cd "+boostpath+"; ./b2 "+mpi+" --with-exception --with-filesystem --with-math --with-random --with-regex --with-date_time --with-thread --with-system --with-program_options --with-serialization --with-iostreams --disable-filesystem2 threading=multi runtime-link=shared variant=release toolset="+toolset+" install --prefix="+os.path.abspath(os.path.join(os.curdir, "install", "build", "boost", boostversion)), env)

    # checkout the numerical binding
    runsyscmd("svn checkout http://svn.boost.org/svn/boost/sandbox/numeric_bindings/ "+os.path.join(os.curdir, "install", "build", "boost", "sandbox", "numeric_bindings"), env )

    return []
    
    
def download_hdf(target, source, env) :
    if os.path.exists(os.path.join(os.curdir, "install", "hdf.tar.bz2")) :
        return []

    # read download path of the HDF
    f = urllib2.urlopen("http://www.hdfgroup.org/ftp/HDF5/current/src/")
    html = f.read()
    f.close()
    
    found = re.search("<a href=\"(.*)tar.bz2\">", html)
    if found == None :
        print "HDF Download URL not found"
        sys.exit(1)
    downloadurl = found.group(0)

    downloadurl = downloadurl.replace("<a href=\"", "")
    downloadurl = downloadurl.replace("\">", "")

    # download the package
    target = open( os.path.join(os.curdir, "install", "hdf.tar.bz2"), "w" )
    f = urllib2.urlopen("http://www.hdfgroup.org/ftp/HDF5/current/src/"+downloadurl)
    target.write(f.read())
    target.close()
    f.close()

    return []
    
    
def build_hdf(target, source, env) :
    hdfpath = glob.glob(os.path.join("install", "hdf?-*"))
    if hdfpath == None or not(hdfpath) :
        print "HDF Build Directory not found"
        sys.exit(1)
    hdfpath     = hdfpath[0]
    hdfversion  = hdfpath.replace(os.path.join("install", "hdf"), "")

    runsyscmd( "cd "+hdfpath+"; ./configure --enable-cxx --prefix="+os.path.abspath(os.path.join("install", "build", "hdf", hdfversion))+ "; make; make install", env )
    return []
    
    
def download_atlaslapack(target, source, env) :
    
    if not(os.path.exists(os.path.join(os.curdir, "install", "lapack.tgz")) ) :
        # read download path of the LAPack (latest version)
        f = urllib2.urlopen("http://www.netlib.org/lapack/")
        html = f.read()
        f.close()
        
        found = re.search("<a href=\"http://www.netlib.org/lapack/(.*)tgz\">", html)
        if found == None :
            print "LAPack Download URL not found"
            sys.exit(1)
        downloadurl = found.group(0)
        downloadurl = downloadurl.replace("<a href=\"", "")
        downloadurl = downloadurl.replace("\">", "")
        
        target = open( os.path.join(os.curdir, "install", "lapack.tgz"), "w" )
        f = urllib2.urlopen(downloadurl)
        target.write(f.read())
        target.close()
        f.close()
    
    
    if not(os.path.exists(os.path.join(os.curdir, "install", "atlas.tar.bz2")) ) :
        target = open( os.path.join(os.curdir, "install", "atlas.tar.bz2"), "w" )
        f = urllib2.urlopen("http://sourceforge.net/projects/math-atlas/files/latest/download?source=files")
        target.write(f.read())
        target.close()
        f.close()
    
    # extract ATLAS tar here, because errors are ignored
    os.system("tar xfvj "+os.path.join("install", "atlas.tar.bz2")+" -C install")
    
    return []
    
    
def build_atlaslapack(target, source, env) :
    f = urllib2.urlopen("http://sourceforge.net/projects/math-atlas/files/")
    html = f.read()
    f.close()

    found = re.search("<small title=\"(.*)tar.bz2\">(.*)</small>", html)
    if found == None :
        print "ATLAS Download URL not found"
        sys.exit(1)
    atlasversion = found.group(2)
    atlasversion = atlasversion.replace("atlas", "")
    atlasversion = atlasversion.replace(".tar.bz2", "")

    ptrwidth = ""
    if env["atlaspointerwidth"] == "32" :
        ptrwidth = "-b 32"
    elif env["atlaspointerwidth"] == "64" :
        ptrwidth = "-b 64"

    cputhrottle = ""
    if not(env["atlascputhrottle"]) :
        cputhrottle = "-Si cputhrchk 0"
    
    runsyscmd( "cd "+os.path.join("install", "atlasbuild")+"; ../ATLAS/configure --dylibs "+ptrwidth+" "+cputhrottle+" --with-netlib-lapack-tarfile=../lapack.tgz --prefix="+os.path.abspath(os.path.join("install", "build", "atlas", atlasversion))+ "; make", env )
    return []
    
    
def soname_atlaslapack(target, source, env) :
    oFile = open( os.path.join(os.curdir, "install", "atlasbuild", "lib", "Makefile"), "r" )
    makefile = oFile.read()
    oFile.close()
    
    makefile = makefile.replace("(LD) $(LDFLAGS) -shared -soname $(LIBINSTdir)/$(outso) -o $(outso)", "(LD) $(LDFLAGS) -shared -soname $(outso) -o $(outso)")

    oFile = open( os.path.join(os.curdir, "install", "atlasbuild", "lib", "Makefile"), "w" )
    oFile.write(makefile)
    oFile.close()

    return []


def install_atlaslapack(target, source, env) :
    runsyscmd( "cd "+os.path.join("install", "atlasbuild")+"; make shared; make install", env )
    return []

        
def download_ginaccln(target, source, env) :

    if not(os.path.exists(os.path.join(os.curdir, "install", "ginac.tar.bz2"))) :
        # read download path of the GiNaC (latest version)
        f = urllib2.urlopen("http://www.ginac.de/Download.html")
        html = f.read()
        f.close()
        
        found = re.search("<a href=\"http://www.ginac.de/(.*).tar.bz2\">this link</a>", html)
        if found == None :
            print "GiNaC Download URL not found"
            sys.exit(1)
        downloadurl = found.group(0)
        downloadurl = downloadurl.replace("<a href=\"", "")
        downloadurl = downloadurl.replace("\">this link</a>", "")
        
        target = open( os.path.join(os.curdir, "install", "ginac.tar.bz2"), "w" )
        f = urllib2.urlopen(downloadurl)
        target.write(f.read())
        target.close()
        f.close()
    
    
    if not(os.path.exists(os.path.join(os.curdir, "install", "cln.tar.bz2"))) :
        # read download path of the CLN (latest version)
        f = urllib2.urlopen("http://www.ginac.de/CLN/")
        html = f.read()
        f.close()
        
        found = re.search("<a href=\"(.*).tar.bz2\">from here</a>", html)
        if found == None :
            print "CLN Download URL not found"
            sys.exit(1)
        downloadurl = found.group(0)
        downloadurl = downloadurl.replace("<a href=\"", "")
        downloadurl = "http://www.ginac.de/CLN/" + downloadurl.replace("\">from here</a>", "")
        
        target = open( os.path.join(os.curdir, "install", "cln.tar.bz2"), "w" )
        f = urllib2.urlopen(downloadurl)
        target.write(f.read())
        target.close()
        f.close()
    
    
    return []

def build_ginaccln(target, source, env) :
    clnpath = glob.glob(os.path.join("install", "cln-*"))
    if clnpath == None or not(clnpath) :
        print "CLN Build Directory not found"
        sys.exit(1)
    clnpath     = clnpath[0]
    clnversion  = clnpath.replace(os.path.join("install", "cln-"), "")
    
    ginacpath = glob.glob(os.path.join("install", "ginac-*"))
    if ginacpath == None or not(ginacpath) :
        print "GiNaC Build Directory not found"
        sys.exit(1)
    ginacpath     = ginacpath[0]
    ginacversion  = ginacpath.replace(os.path.join("install", "ginac-"), "")

    runsyscmd( "cd "+clnpath+"; ./configure --prefix="+os.path.abspath(os.path.join("install", "build", "cln", clnversion))+ "; make; make install", env )
    runsyscmd( "cd "+ginacpath+"; export CLN_CFLAGS=-I"+os.path.abspath(os.path.join("install", "build", "cln", clnversion, "include"))+"; export CLN_LIBS=\"-L"+os.path.abspath(os.path.join("install", "build", "cln", clnversion, "lib"))+" -lcln\"; ./configure --prefix="+os.path.abspath(os.path.join("install", "build", "ginac", ginacversion))+ "; make; make install", env )
    return []


def download_jsoncpp(target, source, env) :
    if os.path.exists(os.path.join(os.curdir, "install", "json.tar.gz")) :
        return []

    target = open( os.path.join(os.curdir, "install", "jsoncpp.tar.gz"), "w" )
    f = urllib2.urlopen("http://sourceforge.net/projects/jsoncpp/files/latest/download?source=files")
    target.write(f.read())
    target.close()
    f.close()

    return []
    
    
def build_jsoncpp(target, source, env) :
    jsonpath = glob.glob(os.path.join("install", "jsoncpp-src-*"))
    if jsonpath == None or not(jsonpath) :
        print "JSON CPP Build Directory not found"
        sys.exit(1)
    jsonpath     = jsonpath[0]
    jsonversion  = jsonpath.replace(os.path.join("install", "jsoncpp-src-"), "")
    
    runsyscmd("cd "+jsonpath+"; scons platform=linux-gcc", env)
    
    # manual copy of the data
    try :
        os.makedirs(os.path.join("install", "build", "jsoncpp", jsonversion))
    except :
        pass
    try :
        os.makedirs(os.path.join("install", "build", "jsoncpp", jsonversion, "lib"))
    except :
        pass
    try :
        shutil.copytree(os.path.join(jsonpath, "include"), os.path.join("install", "build", "jsoncpp", jsonversion, "include"))
    except :
        pass

    files = []
    files.extend( getRekusivFiles(os.path.join(jsonpath, "libs"), env["SHLIBSUFFIX"]) )
    files.extend( getRekusivFiles(os.path.join(jsonpath, "libs"), env["LIBSUFFIX"]) )
    for i in files :
        shutil.copy(i, os.path.join("install", "build", "jsoncpp", jsonversion, "lib", os.path.split(i)[-1]))

    return []
    
    
def clearbuilddir(target, source, env) :
    clearlist = []
    for i in os.listdir("install") :
        if os.path.isfile(os.path.join("install", i)) :
            continue
        if i <> "build" :
            clearlist.append(i)
    
    for i in clearlist :
        for pathentry in os.walk(os.path.join("install", i), False):
            for dir in pathentry[1]:
                path = os.path.join(pathentry[0], dir)
                if os.path.islink(path):
                    os.unlink(path)
                else:
                    os.rmdir(path)

            for file in pathentry[2]:
                path = os.path.join(pathentry[0], file)
                os.unlink(path)
       
        os.removedirs(os.path.join("install", i))
    
    return []


def target_libraryinstall(env) :
    skiplist = str(env["skipbuild"]).split(",")
    if ("librarybuild" in COMMAND_LINE_TARGETS) and ("all" in skiplist) :
        print "nothing to build"
        sys.exit(1)

    #build into a temp dir
    lst = []
    lst.append( env.Command("mkinstalldir", "", Mkdir("install")) )
    lst.append( env.Command("mkbuilddir", "", Mkdir(os.path.join("install", "build"))) )
    
    #clear install directories before compiling
    lst.append( env.Command("cleanbeforebuilddir", "", clearbuilddir) )

    #download LAPack & ATLAS, extract & install
    if not("atlas" in skiplist) :
        lst.append( env.Command("downloadlapackatlas", "", download_atlaslapack) )
        lst.append( env.Command("mkatlasbuilddir", "", Mkdir(os.path.join("install", "atlasbuild"))) )
        lst.append( env.Command("buildatlaslapack", "", build_atlaslapack) )
        if env['PLATFORM'].lower() == "posix" or env['PLATFORM'].lower() == "cygwin" :
            lst.append( env.Command("sonameatlaslapack", "", soname_atlaslapack) )
        lst.append( env.Command("installatlaslapack", "", install_atlaslapack) )
    
    # download Boost, extract & install
    if not("boost" in skiplist) :
        lst.append( env.Command("downloadboost", "", download_boost) )
        lst.append( env.Command("extractboost", "", "tar xfvj "+os.path.join("install", "boost.tar.bz2")+" -C install") )
        lst.append( env.Command("buildboost", "", build_boost) )
    
    # download HDF, extract & install
    if not("hdf" in skiplist) :
        lst.append( env.Command("downloadhdf", "", download_hdf) )
        lst.append( env.Command("extracthdf", "", "tar xfvj "+os.path.join("install", "hdf.tar.bz2")+" -C install") )
        lst.append( env.Command("buildhdf", "", build_hdf) )

    #download GiNaC & CLN, extract & install
    if not("ginac" in skiplist) :
        lst.append( env.Command("downloadginaccln", "", download_ginaccln) )
        lst.append( env.Command("extractginac", "", "tar xfvj "+os.path.join("install", "ginac.tar.bz2")+" -C install") )
        lst.append( env.Command("extractcln", "", "tar xfvj "+os.path.join("install", "cln.tar.bz2")+" -C install") )
        lst.append( env.Command("buildginaccln", "", build_ginaccln) )
    
    #download JSON library, extract & install
    if not("json" in skiplist) :
        lst.append( env.Command("downloadjsoncpp", "", download_jsoncpp) )
        lst.append( env.Command("extractjsoncpp", "", "tar xfvz "+os.path.join("install", "jsoncpp.tar.gz")+" -C install") )
        lst.append( env.Command("buildjsoncpp", "", build_jsoncpp) )
    
    #clear install directories after compiling
    lst.append( env.Command("cleanafterbuilddir", "", clearbuilddir) )
    
    env.Alias("librarybuild", lst)
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
files.extend( getRekusivFiles(os.curdir, ".po~") )
files.extend( getRekusivFiles(os.curdir, ".mo") )
files.extend( getRekusivFiles(os.curdir, ".jar") )
files.extend( getRekusivFiles(os.curdir, ".stackdump") )
files.extend( getRekusivFiles(os.curdir, ".core") )

# don't ad the current path, because scons run the directories until "/"
files.extend( ["build", "documentation", "install"] )

env.Clean("clean", files)


# catch all cpps within the framework directories and compile them to objectfiles into the builddir (manual set for static initializations)
#framework = getRekusivFiles(os.curdir, ".cpp", ["examples"])
framework = []
if env["withlogger"] or env["withrandomdevice"] :
    framework.append( os.path.join( os.curdir, "machinelearning.cpp" ) )

# create building targets
target_libraryinstall( env )
target_language( env )
target_documentation( env )

target_javac( env, framework )
target_javaexamples( env )

target_cpp(env, framework )
