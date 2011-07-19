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
    vars.Add(BoolVariable("withsymbolicmath", "compile for using symbolic math expression (needed by gradient descent)", False))

    vars.Add(BoolVariable("withdebug", "compile with debug information", False))

    vars.Add(EnumVariable("winver", "value of the Windows version", "win7", allowed_values=("win7", "srv2008", "vista", "srv2003sp1", "xpsp2", "srv2003", "xp", "w2000")))


#=== function for os configuration ===================================================================================================
# configuration for OSX build
def configuration_macosx(config, vars, version, architecture) :
    # check the OSX build for set the correct architecture
    arch = architecture
    ver  = version.split(".")

    if ver[0] == "10" and ver[1] == "6" :
        arch = "x86_64"

    config["linkerflags"]       = ""
    config["include"]           = os.environ["CPPPATH"]
    config["librarypath"]       = os.environ["LIBRARY_PATH"]
    config["compileflags"]      = "-O2 -Os -s -pipe -Wall -pthread -finline-functions -arch "+arch+" -D BOOST_FILESYSTEM_NO_DEPRECATED -D BOOST_NUMERIC_BINDINGS_BLAS_CBLAS"
    config["linkto"]            = ["boost_exception", "boost_system", "boost_thread", "boost_iostreams", "boost_filesystem", "boost_regex"]

    if ver[0] == "10" and ver[1] == "6" :
        config["linkto"].append("tatlas")
    else :
        config["linkto"].extend( ["atlas", "lapack"] )


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
        config["compileflags"]      += " -D MACHINELEARNING_SOURCES"
        config["linkto"].extend( ["xml2"] )

    if vars["withfiles"] :
        config["compileflags"]      += " -D MACHINELEARNING_FILES -D MACHINELEARNING_FILES_HDF"
        config["linkto"].extend( ["hdf5_cpp", "hdf5"] )

    if vars["withsymbolicmath"] :
        config["compileflags"]      += " -D MACHINELEARNING_SYMBOLICMATH"
        config["linkto"].append("ginac")


# configuration for Posix (Linux) build
def configuration_posix(config, vars, version, architecture) :
    config["linkerflags"]       = ""
    config["include"]           = os.environ["CPPPATH"]
    config["librarypath"]       = os.environ["LIBRARY_PATH"]
    config["compileflags"]      = "-O2 -Os -s -pipe -Wall -pthread -finline-functions -D BOOST_FILESYSTEM_NO_DEPRECATED -D BOOST_NUMERIC_BINDINGS_BLAS_CBLAS"
    config["linkto"]            = ["boost_exception", "boost_system", "boost_thread", "boost_iostreams", "boost_filesystem", "boost_regex", "tatlas"]

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
        config["compileflags"]      += " -D MACHINELEARNING_SOURCES"
        config["linkto"].extend( ["xml2"] )

    if vars["withfiles"] :
        config["compileflags"]      += " -D MACHINELEARNING_FILES -D MACHINELEARNING_FILES_HDF"
        config["linkto"].extend( ["hdf5_cpp", "hdf5"] )

    if vars["withsymbolicmath"] :
        config["compileflags"]      += " -D MACHINELEARNING_SYMBOLICMATH"
        config["linkto"].append("ginac")


# configuration for Windows Cygwin build
def configuration_cygwin(config, vars, version, architecture) :
    config["linkerflags"]       = "-enable-stdcall-fixup"
    config["include"]           = os.environ["CPPPATH"]
    config["librarypath"]       = os.environ["PATH"]
    config["compileflags"]      = "-O2 -Os -s -pipe -Wall -finline-functions -D BOOST_FILESYSTEM_NO_DEPRECATED -D BOOST_NUMERIC_BINDINGS_BLAS_CBLAS"
    config["linkto"]            = ["boost_exception", "cygboost_system", "cygboost_thread", "cygboost_iostreams", "cygboost_filesystem", "cygboost_regex", "lapack", "cblas", "f77blas", "atlas", "gfortran"]

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
        config["compileflags"]      += " -D MACHINELEARNING_SOURCES -D __USE_W32_SOCKETS"
        config["linkto"].extend( ["cygxml2-2", "ws2_32"] )

    if vars["withfiles"] :
        config["compileflags"]      += " -D MACHINELEARNING_FILES -D MACHINELEARNING_FILES_HDF"
        config["linkto"].extend( ["hdf5_cpp", "hdf5"] )

    if vars["withsymbolicmath"] :
        config["compileflags"]      += " -D MACHINELEARNING_SYMBOLICMATH"
        config["linkto"].append("ginac")
#=======================================================================================================================================



#=== function for configuration creation================================================================================================
# function for reading os configuration
# and setting environment
def getConfig(vars):
    env = Environment(variables=vars)
    config = {}

    if env['PLATFORM'].lower() == "darwin" :
        configuration_macosx(config, env, platform.mac_ver()[0], platform.machine())
    elif env['PLATFORM'].lower() == "cygwin" :
        configuration_cygwin(config, env, "", platform.machine())
    elif env['PLATFORM'].lower() == "posix" :
        configuration_posix(config, env, "", platform.machine())
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
    env.Replace(CPPPATH     = config["include"].split(os.pathsep))
    env.Replace(LINKFLAGS   = config["linkerflags"])
    env.Replace(LIBS        = config["linkto"])
    env.Replace(LIBPATH     = config["librarypath"])
    env.Replace(CPPSUFFIXES = [".hpp", ".h", ".cpp"])

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
        for n in ldontuse :
            for i in lst :
                if not(i.startswith(n)) :
                    clst.append(i)

    return clst


# build languagefiles
def createLanguage(env, onlycompile=False) :
    # compiling with: msgfmt -v -o target.mo source.po
    # add new data: msgmerge --no-wrap --update old_file.po newer_file.pot

    po = getRekusivFiles(os.curdir, ".po", ["examples"])

    if onlycompile :
        # compiling all files
        for i in po :
            os.system( "msgfmt -v -o " + os.path.join(os.path.dirname(i),"ml.mo") +" "+ i )

    else :
        sources = []
        for i in env["CPPSUFFIXES"] :
            sources.extend( getRekusivFiles(os.curdir, i, ["examples"]) )

        cmd = "xgettext --output="+os.path.join("tools", "language", "language.po")+" --keyword=_ --language=c++ ";
        for i in sources :
            cmd = cmd + i + " "
        os.system(cmd);


        # get all language files in the subdirs and add the new texts
        for i in po :
            os.system( "msgmerge --no-wrap --update " + i + " "+os.path.join("tools", "language", "language.po") )

        os.remove("tools/language/language.po")

        # compiling all files
        for i in po :
            os.system( "msgfmt -v -o " + os.path.join(os.path.dirname(i),"ml.mo") +" "+ i )


#=======================================================================================================================================

#=== build targets =====================================================================================================================

# create compiling options
def createTarget(env, alias, path, sources, framework) :
    lst = []
    if sources.count == 0 :
        return

    for i in sources :
        compilesource = [os.path.join(path, i)]
        compilesource.extend(framework)

        lst.append( env.Program( target=os.path.join("#build", os.path.splitext(i)[0]), source=compilesource ) )

    env.Alias(alias, lst)




def target_sources(env, framework) :
    path = os.path.join(".", "examples", "sources")
    sources = []

    if env["withsources"] :
        sources.extend( ["newsgroup.cpp", "wikipedia.cpp"] )

    if env["withfiles"] :
        sources.append( "cloud.cpp" )

    createTarget(env, "sources", path, sources, framework)


def target_clustering(env, framework) :
    path = os.path.join(".", "examples", "clustering")
    sources = []

    if env["withfiles"] :
        sources.extend( ["rlvq.cpp", "kmeans.cpp", "neuralgas.cpp", "patch_neuralgas.cpp", "relational_neuralgas.cpp", "spectral.cpp"] )

    createTarget(env, "clustering", path, sources, framework)


def target_reducing(env, framework) :
    path = os.path.join(".", "examples", "reducing")
    sources = []

    if env["withfiles"] :
        sources.extend( ["lda.cpp", "mds.cpp", "pca.cpp"] )

    createTarget(env, "reducing", path, sources, framework)


def target_distance(env, framework) :
    path = os.path.join(".", "examples", "distance")
    sources = []

    if env["withfiles"] :
        sources.extend( ["ncd.cpp"] )

    createTarget(env, "distance", path, sources, framework)


def target_classifier(env, framework) :
    path = os.path.join(".", "examples", "classifier")
    sources = []

    if env["withfiles"] :
        sources.extend( ["lazy.cpp"] )

    createTarget(env, "classifier", path, sources, framework)


def target_other(env, framework) :
    path = os.path.join(".", "examples", "other")
    sources = []

    if env["withfiles"] and env["withsources"] :
        sources.extend( ["mds_nntp.cpp", "mds_wikipedia.cpp"] )

    if env["withfiles"] :
        sources.append("mds_file.cpp")

    createTarget(env, "other", path, sources, framework)
#=======================================================================================================================================





#=== create environment and compiling ==================================================================================================
vars = Variables()
createVariables(vars)
env = getConfig(vars)
Help(vars.GenerateHelpText(env))

# add files for deleting
files = []
files.extend( getRekusivFiles(os.curdir, env["OBJSUFFIX"]) )
files.extend( getRekusivFiles(os.curdir, ".po~") )
env.Clean("clean", files)

#default target
Default(None)

if "documentation" in COMMAND_LINE_TARGETS :
    os.system("doxygen documentation.doxyfile")
elif "createlanguage" in COMMAND_LINE_TARGETS :
    createLanguage(env, True)
elif "compilelanguage" in COMMAND_LINE_TARGETS :
    createLanguage(env)
else :

    # catch all cpps within the framework directories and compile them to objectfiles into the builddir
    #framework = getRekusivFiles(os.curdir, ".cpp", ["examples"])
    framework = []

    # create building targets
    target_sources( env, framework )
    target_clustering( env, framework )
    target_reducing( env, framework )
    target_distance(env, framework )
    target_classifier(env, framework )
    target_other(env, framework )

