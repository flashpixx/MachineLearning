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


# build script for all libraries

import os
import re
import urllib2
Import("*")



#=== download packages ===============================================================================================================
def Boost_DownloadURL(env)  :
    # read download path of the Boost (latest version)
    f = urllib2.urlopen("http://www.boost.org/users/download/")
    html = f.read()
    f.close()
    
    found = re.search("<a href=\"https://sourceforge.net/projects/boost/files/(.*)\">Download</a>", html)
    if found == None :
        raise RuntimeError("Boost Download URL not found")
        
    downloadurl = found.group(0)
    downloadurl = downloadurl.replace("<a href=\"", "")
    downloadurl = downloadurl.replace("\">Download</a>", "")
    
    version     = found.group(1).replace("boost", "")
    version     = version.replace("/", "")
    
    
    # read url of the tar.gz
    f = urllib2.urlopen(downloadurl)
    html = f.read()
    f.close()

    found = re.search("<a href=\"http://sourceforge.net/projects/boost/files/boost(.*).tar.gz/download", html)
    if found == None :
        raise RuntimeError("Boost Download URL not found")

    downloadurl = found.group(0)
    downloadurl = downloadurl.replace("<a href=\"", "")
   
    return downloadurl, "boost-" + version + ".tar.gz"
    
    
    
def JsonCPP_DownloadURL(env) :
    if env["jsoncppversion"] == "stable" :
        # read download path of the JsonCPP library (latest stable version)
        f = urllib2.urlopen("http://sourceforge.net/projects/jsoncpp/")
        html = f.read()
        f.close()
        
        found = re.search("<a href=\"/projects/jsoncpp/files/latest/download\" title=\"Download /jsoncpp/(.*)/jsoncpp-src-(.*)\.tar\.gz", html)
        if found == None :
            raise RuntimeError("JsonCPP Download URL not found")
        return "http://sourceforge.net/projects/jsoncpp/files/latest/download", "jsoncpp-src-" + found.group(1) + ".tar.gz"
        
    elif env["jsoncppversion"] == "devel" :
        # read download path of the JsonCPP library (latest developer version)
        f = urllib2.urlopen("http://sourceforge.net/projects/jsoncpp/files/jsoncpp/")
        html = f.read()
        f.close()
        
        found = re.search("<a href=\"/projects/jsoncpp/files/jsoncpp/(.+)/\"", html)
        if found == None :
            raise RuntimeError("Json-Cpp Version Download URL not found")
        version  = found.group(0).replace("\"", "").replace("<a href=", "").strip("/").split("/")[-1]     
        filename = "jsoncpp-src-" + version + ".tar.gz"
        return "http://sourceforge.net/projects/jsoncpp/files/jsoncpp/"+version+"/"+filename, filename
    
    raise RuntimeError("Json-Cpp Download unknown")
    
   
     
def LibXML2_DownloadURL(env) :
    # read latest release version of LibXML (latest version under ftp://xmlsoft.org/libxml2/LATEST_LIBXML2 )
    f = urllib2.urlopen("http://xmlsoft.org/news.html")
    html = f.read()
    f.close()
    
    found = re.search("<h3>\d.\d.\d:(.*)</h3>", html)
    if found == None :
        raise RuntimeError("LibXML release version number not found")
    
    version = found.group(0).replace("<", "").replace(">", "").replace("h3", "")     
    version = re.sub(":(.*)", "", version)
    
    return "ftp://xmlsoft.org/libxml2/libxml2-"+version+".tar.gz", "libxml2-"+version+".tar.gz"



def HDF5_DownloadURL(env) :
    # read download path of the HDF library (latest version)
    f = urllib2.urlopen("http://www.hdfgroup.org/ftp/HDF5/current/src/")
    html = f.read()
    f.close()
    
    found = re.search("<a href=\"(.*)tar.gz\">", html)
    if found == None :
        raise RuntimeError("HDF Download URL not found")
    
    filename = found.group(0).replace("<a href=\"./", "").replace("\">", "")

    return "http://www.hdfgroup.org/ftp/HDF5/current/src/"+filename, filename



def LaPack_DownloadURL(env) :
    # read download path of the LAPack (latest version)
    f = urllib2.urlopen("http://www.netlib.org/lapack/")
    html = f.read()
    f.close()
    
    found = re.search("<a href=\"http://www.netlib.org/lapack/(.*)tgz\">", html)
    if found == None :
        raise RuntimeError("LAPack Download URL not found")
        
    downloadurl = found.group(0).replace("<a href=\"", "").replace("\">", "")
    filename    = downloadurl.replace("http://www.netlib.org/lapack/", "")
    
    return downloadurl, filename
    
    
def CLN_DownloadURL(env) :
    # read download path of the CLN (latest version)
    f = urllib2.urlopen("http://www.ginac.de/CLN/")
    html = f.read()
    f.close()
    
    found = re.search("<a href=\"cln-(.*)\.tar\.bz2\">", html)
    if found == None :
        raise RuntimeError("CLN Download URL not found")
        
    filename    = "cln-" + found.group(1) + ".tar.bz2"

    return "http://www.ginac.de/CLN/"+filename, filename
    
    
def Ginac_DownloadURL(env) :
    # read download path of the CLN (latest version)
    f = urllib2.urlopen("http://www.ginac.de/Download.html")
    html = f.read()
    f.close()
    
    found = re.search("<a href=\"http://www.ginac.de/ginac-(.*)\.tar\.bz2\">", html)
    if found == None :
        raise RuntimeError("GiNaC Download URL not found")

    filename = "ginac-" + found.group(1) + ".tar.bz2"

    return "http://www.ginac.de/"+filename, filename
    
    
def Zlib_DownloadURL(env) :
    # read download path of the ZLib (latest version)
    f = urllib2.urlopen("http://www.zlib.net/")
    html = f.read()
    f.close()
    
    found = re.search("http://zlib.net/zlib-(.*)\.tar\.gz", html)
    if found == None :
        raise RuntimeError("ZLib Download URL not found")

    filename = "zlib-" + found.group(1) + ".tar.gz"

    return "http://zlib.net/"+filename, filename

    
def BZip2_DownloadURL(env) :
    # read download path of the BZip2 (latest version)
    f = urllib2.urlopen("http://www.bzip.org/downloads.html")
    html = f.read()
    f.close()
    
    found = re.search("<a href=\"/(.*)/bzip2-(.*)\.tar\.gz\">", html)
    if found == None :
        raise RuntimeError("BZip2 Download URL not found")
        
    version = found.group(1)
        
    return "http://www.bzip.org/"+version+"/bzip2-"+version+".tar.gz", "bzip2-"+version+".tar.gz"



#=== building libraries ==============================================================================================================
def Boost_BuildInstall(env, source, zlibbuild, bzipbuild)  :
    # extract path and version of the source name
    boostdir    = str(source).replace("['", "").replace("']", "").replace(".", "_").replace("-", "_")
    version     = boostdir.replace("boost_", "").replace("_", ".")
    boostpath   = setpath(env, os.path.join("library", boostdir))
    prefix      = setpath(env, os.path.join("..", "build", "boost", version))

    # set the toolset and compile the bjam and build boost (we build the tagged version, so we build on default the release version, because names are different)
    boostlibs = ["exception", "math", "system", "iostreams"]
    boostoptions = [
        "link=shared",
        "runtime-link=shared",
        "threading=multi",
        "variant=release",
        "install",
        "--layout=tagged",
        "--prefix="+prefix
    ]
    
    if env["boostbuild"] == "requiredoptional"  or  env["boostbuild"] == "full" :
        boostlibs.extend(["regex", "random", "thread", "date_time"])
    if env["boostbuild"] == "full" :
        boostlibs.extend(["filesystem", "program_options"])
    if env["withmpi"] :
        boostlibs.extend(["mpi", "serialization"])
   
    cmd = "cd " + boostpath + " && "
    if env["withmpi"] :
        cmd = cmd + " echo \"using mpi ;\" >> " + setpath(env, os.path.join("tools", "build", "v2", "user-config.jam")) + " && "
    cmd = cmd + " ./bootstrap.sh --prefix=" + prefix + " --without-icu --with-libraries=" + ",".join(boostlibs)
    
    if env["TOOLKIT"] == "msys" :
        cmd = cmd + " --with-toolset=mingw && rm project-config.jam && ./b2 " + " ".join(boostoptions) + " --with-"+" --with-".join(boostlibs)
    else :
        cmd = cmd + " && ./b2 " + " ".join(boostoptions) + " --with-"+" --with-".join(boostlibs)
    
    # build Boost with Bzip2 and ZLib support and set the dependency
    dependency = [source]
    if (zlibbuild <> None) and (bzipbuild <> None) :
        zlibpath = os.path.sep.join( str(zlibbuild[0]).split(os.path.sep)[0:-2] )
        bzippath = os.path.sep.join( str(bzipbuild[0]).split(os.path.sep)[0:-2] )
        
        zipcmd   = "export BZIP2_BINARY=bz2 && export ZLIB_BINARY=z && "
        dependency.append(zlibbuild)
        dependency.append(bzipbuild)
        
        zipcmd  += "export ZLIB_INCLUDE="  + setpath(env, os.path.abspath(os.path.join(os.curdir, zlibpath, "include"))) + " && "
        zipcmd  += "export ZLIB_LIBPATH="  + setpath(env, os.path.abspath(os.path.join(os.curdir, zlibpath, "lib"))) + " && "
        zipcmd  += "export BZIP2_INCLUDE=" + setpath(env, os.path.abspath(os.path.join(os.curdir, bzippath, "include"))) + " && "
        zipcmd  += "export BZIP2_LIBPATH=" + setpath(env, os.path.abspath(os.path.join(os.curdir, bzippath, "lib"))) + " && "
        cmd      = zipcmd + cmd
        
    return env.Command("buildboost-"+version, dependency, cmd)
      
      
def Zlib_BuildInstall(env, zlibtargz, extract) :
    sourcepath = str(zlibtargz).replace("['", "").replace("']", "").replace(".tar.gz", "")
    version    = sourcepath.replace("zlib-", "")

    headers = [ "zconf.h", "zlib.h" ]
    
    sources = [ "adler32.c",   "crc32.c",   "deflate.c",   "infback.c",   "inffast.c", "inflate.c",
                "inftrees.c",  "trees.c",   "zutil.c",     "compress.c",  "uncompr.c", "gzclose.c",
                "gzlib.c",     "gzread.c",  "gzwrite.c"
              ]
              
    # extract the needed source from the list
    libsrc      = []
    libheader   = []
    for i in extract :
        name = str(i)
        if any([n == os.path.basename(name) for n in headers]) :
            libheader.append( i ) 
        elif any([n == os.path.basename(name) for n in sources]) :
            libsrc.append( i )
    
    envzlib = Environment()
    envzlib.Replace(CPATH = os.path.join(sourcepath, "include"))
    envzlib.AppendUnique(CFLAGS  = ["-O2", "-DHAVE_HIDDEN"])

    # create lib, install header and libraries
    lib             = envzlib.StaticLibrary(target="z", source=libsrc)   
    libinstall      = env.Install( setpath(env, os.path.join(os.curdir, "build", "zlib", version, "lib")), lib )
    headerdir       = env.Command( setpath(env, os.path.join(os.curdir, "build", "zlib", version, "include")), "", Mkdir("$TARGET"))
    headerinstall   = []
    for i in libheader :
        headerinstall.append( env.Command( os.path.join(str(headerdir).replace("']", "").replace("['", ""), os.path.basename(str(i))), i, Copy("$TARGET", "$SOURCE")) )
    
    # prevent libs and headers during clean
    env.NoClean(libinstall)
    env.NoClean(headerinstall)

    builds = []
    for i in libinstall :
        builds.append(i)
    for i in headerinstall :
        builds.extend( i )
    return builds
   
   
def BZip2_BuildInstall(env, bziptargz, extract) :
    sourcepath = str(bziptargz).replace("['", "").replace("']", "").replace(".tar.gz", "")
    version    = sourcepath.replace("bzip2-", "")
    
    headers = [ "bzlib.h", "bzlib_private.h" ]
    
    sources = [ "blocksort.c",   "huffman.c",   "crctable.c",   "randtable.c",
                "compress.c",  "decompress.c",  "bzlib.c"
              ]
              
    # extract the needed source from the list
    libsrc      = []
    libheader   = []
    for i in extract :
        name = str(i)
        if any([n == os.path.basename(name) for n in headers]) :
            libheader.append( i ) 
        elif any([n == os.path.basename(name) for n in sources]) :
            libsrc.append( i )
            
    envbzip = Environment()
    envbzip.Replace(CPATH = os.path.join(sourcepath, "include"))
    envbzip.AppendUnique(CFLAGS  = ["-Winline", "-O2", "-g", "-D_FILE_OFFSET_BITS=64"])
    
    # create shared lib, install header and libraries
    lib             = envbzip.StaticLibrary(target="bz2", source=libsrc)
    libinstall      = env.Install( setpath(env, os.path.join(os.curdir, "build", "bzip2", version, "lib")), lib )
    headerdir       = env.Command( setpath(env, os.path.join(os.curdir, "build", "bzip2", version, "include")), "", Mkdir("$TARGET"))
    headerinstall   = []
    for i in libheader :
        headerinstall.append( env.Command( os.path.join(str(headerdir).replace("']", "").replace("['", ""), os.path.basename(str(i))), i, Copy("$TARGET", "$SOURCE")) )
    
    # prevent libs and headers during clean
    env.NoClean(libinstall)
    env.NoClean(headerinstall)

    builds = []
    for i in libinstall :
        builds.append(i)
    for i in headerinstall :
        builds.extend( i )
    return builds

      
        
def JsonCPP_BuildInstall(env, targz, extract) :
    # extract version and build regex for getting files
    sourcepath = str(targz).replace(".tar.gz", "").replace("']", "").replace("['", "")
    version    = sourcepath.replace("jsoncpp-src-", "")
    regcpp     = re.compile("jsoncpp-src-(.*)src(.)lib_json(.*)\.cpp")
    regheader  = re.compile("jsoncpp-src-(.*)include(.*)\.h(.*)")
    
    # extract the needed source from the list
    libsrc      = []
    libheader   = []
    for i in extract :
        name = str(i)

        cpp  = regcpp.match( name ) 
        if cpp <> None :
            libsrc.append( i )

        header = regheader.match( name )
        if header <> None :
            libheader.append( i ) 

    envjson = Environment()
    envjson.Replace(CPPPATH = os.path.join(sourcepath, "include"))
    envjson.AppendUnique(CXXFLAGS  = ["-O2"])

    if env["TOOLKIT"] == "msys" :
        envjson["SPAWN"] = env["MSYSSPAWN"]
        envjson["SHELL"] = env["MSYSSHELL"]
        
    elif env["TOOLKIT"] == "darwin" :
        envjson.AppendUnique(LINKFLAGS = ["-Wl,-install_name,libjson.dylib"])
        
    # create shared lib, install header and libraries
    lib             = envjson.SharedLibrary(target="json", source=libsrc)
    libinstall      = env.Install( setpath(env, os.path.join(os.curdir, "build", "jsoncpp", version, "lib")), lib )
    headerdir       = env.Command( setpath(env, os.path.join(os.curdir, "build", "jsoncpp", version, "include", "json")), "", Mkdir("$TARGET"))
    headerinstall   = []
    for i in libheader :
        headerinstall.append( env.Command( os.path.join(str(headerdir).replace("']", "").replace("['", ""), os.path.basename(str(i))), i, Copy("$TARGET", "$SOURCE")) )
    
    # prevent libs and headers during clean
    env.NoClean(libinstall)
    env.NoClean(headerinstall)
    
    builds = []
    for i in libinstall :
        builds.append(i)
    for i in headerinstall :
        builds.extend( i )
    return builds


def Lapack_BuildInstall(env, lapackdir) :
    dir     = str(lapackdir).replace("']", "").replace("['", "")
    version = dir.replace("lapack-", "")
    
    cmd = "cd $SOURCE && cmake . -DCMAKE_BUILD_TYPE:String=release -DLAPACKE:BOOL=\"1\" -DCMAKE_INSTALL_PREFIX:PATH=\"" + setpath(env, os.path.abspath(os.path.join("build", "lapack", version))) + "\" && ";
    
    if env["TOOLKIT"] in ["darwin", "posix", "cygwin", "msys"] :
        return env.Command("buildlapack-"+version, lapackdir, cmd + "make && make install")

    elif env["TOOLKIT"] == "msvc" :
        return env.Command("buildlapack-"+version, lapackdir, cmd + "devenv lapack.sln /build release /Project Install")

    else :
        raise RuntimeError("Lapack can not be build with toolkit ["+env["TOOLKIT"]+"]")


def HDF5_BuildInstall(env, hdfdir) :
    version = str(hdfdir).replace("']", "").replace("['", "").replace("hdf5-", "")
    prefix  = setpath(env, os.path.abspath(os.path.join(os.curdir, "build", "hdf", version)))
    
    cmd = "cd $SOURCE && cmake . -DCMAKE_BUILD_TYPE:String=release -DBUILD_SHARED_LIBS:BOOL=\"1\" -DHDF5_BUILD_HL_LIB:BOOL=\"1\" -DHDF5_BUILD_CPP_LIB:BOOL=\"1\" -DCMAKE_INSTALL_PREFIX:PATH=\"" + prefix + "\" &&"
    
    if env["TOOLKIT"] in ["darwin", "posix", "cygwin", "msys"] :
        return env.Command("buildhdf5-"+version, hdfdir, cmd + "make && make install")
        
    elif env["TOOLKIT"] == "msvc" :
        return env.Command("buildhdf5-"+version, hdfdir, cmd + "devenv hdf5.sln /build release /Project Install")
    
    else :
        raise RuntimeError("HDF5 can not be build with toolkit ["+env["TOOLKIT"]+"]")


def LibXML2_BuildInstall(env, libxml2dir) :
    version   = str(libxml2dir).replace("']", "").replace("['", "").replace("libxml2-", "")
    prefix    = setpath(env, os.path.abspath(os.path.join(os.curdir, "build", "xml", version)))
        
    if env["TOOLKIT"] in ["darwin", "posix", "cygwin", "msys"] :
    
        return env.Command("buildxml2-"+version, libxml2dir, "cd $SOURCE && ./configure --without-python --without-threads --without-debug --prefix=" + prefix + " && make && make install")
    
    else :
        raise RuntimeError("LibXML2 can not be build with toolkit ["+env["TOOLKIT"]+"]")
    
    
def CLNGiNaC_BuildInstall(env, clndir, ginacdir) :
    clnversion   = str(clndir).replace("']", "").replace("['", "").replace("cln-", "")
    clnprefix    = setpath(env, os.path.abspath(os.path.join(os.curdir, "build", "cln", version)))
    
    ginacversion = str(ginacdir).replace("']", "").replace("['", "").replace("ginac-", "")
    ginacprefix  = setpath(env, os.path.abspath(os.path.join(os.curdir, "build", "ginac", version)))


    if env["TOOLKIT"] in ["darwin", "posix", "cygwin", "msys"] :
        
        clnbuild = env.Command("buildcln-"+clnversion, clndir, "cd $SOURCE && ./configure --prefix=" + clnprefix + " && make && make install")
        return env.Command("buildclnginac-"+version, [ginacdir, clnbuild], "cd $SOURCE && export CLN_CFLAGS=-I" + clninclude + " && export CLN_LIBS=\"-L" + clnlib + " -lcln\" && ./configure --prefix=" + ginacprefix + " && make && make install")
    
    else :
        raise RuntimeError("CLN & GiNaC can not be build with toolkit ["+env["TOOLKIT"]+"]")
    
        
def setpath(env, path) :
    if env["TOOLKIT"] == "msys" :
        if path[1] == ":" :
            return "/" + path.replace("\\", "/").replace(":", "")
        else :
            return path.replace("\\", "/")

    return path


#=== target structure ================================================================================================================
def FinishMessage_print(s, target, source, env):
    pass
def FinishMessage(target, source, env) :
    alias = str(target[0])
    if alias == "librarydownload" :
        print "\n==> library sources have been downloaded, run for building the target [librarybuild]\n"
    if alias == "librarybuild" :
        print "\n==> libraries have been built and stored under ["+os.path.join("library", "build")+"]\n"
    
    

skiplist = str(env["skiplibrary"]).split(",")
if (("librarybuild" in COMMAND_LINE_TARGETS) or ("librarydownload" in COMMAND_LINE_TARGETS)) and ("all" in skiplist) :
    raise RuntimeError("nothing to build")

#build into seperate directory, check needed installation tools and get the command line for extracting tar.gz files
lstbuild    = []
lstdownload = []

# build target list
if ("librarybuild" in COMMAND_LINE_TARGETS) or ("librarydownload" in COMMAND_LINE_TARGETS) :

    if " " in os.curdir :
        raise RuntimeError("there are spaces within the path, use a path without spaces")

    # download LaPack, extract & install
    if not("lapack" in skiplist) :
        lapacktargz = env.ParseAndDownload( LaPack_DownloadURL )
        lstdownload.append(lapacktargz)
        lstbuild.append( Lapack_BuildInstall(env, env.Command(str(lapacktargz).replace("[", "").replace("]", "").replace("'", "").replace(".tgz", ""), lapacktargz, env["EXTRACT_CMD"]+env["extractsuffix"]+"library")) )

    # download Boost, extract & install
    if not("boost" in skiplist) :
        bzipbuild = None
        zlibbuild = None
        
        if env["zipsupport"] :
            zlibtargz = env.ParseAndDownload( Zlib_DownloadURL )
            zlibfiles = env.Extract(str(zlibtargz).replace("[", "").replace("]", "").replace("'", "").replace(".tar.gz", ""), zlibtargz, extractsuffix=env["extractsuffix"]+"library")
            zlibbuild = Zlib_BuildInstall(env, zlibtargz, zlibfiles)
            lstbuild.extend( zlibbuild )
            lstdownload.append(zlibtargz)
            
            bziptargz  = env.ParseAndDownload( BZip2_DownloadURL )
            bzlibfiles = env.Extract(str(bziptargz).replace("[", "").replace("]", "").replace("'", "").replace(".tar.gz", ""), bziptargz, extractsuffix=env["extractsuffix"]+"library")
            bzipbuild  = BZip2_BuildInstall(env, bziptargz, bzlibfiles)
            lstbuild.extend( bzipbuild )
            lstdownload.append(bziptargz)
         
        boosttargz = env.ParseAndDownload( Boost_DownloadURL )
        boostdir   = env.Command(str(boosttargz).replace("[", "").replace("]", "").replace("'", "").replace(".tar.gz", ""), boosttargz, env["EXTRACT_CMD"]+env["extractsuffix"]+"library")
        boostbuild = Boost_BuildInstall(env, boostdir, zlibbuild, bzipbuild)
        lstbuild.append( boostbuild )
        
        #lstbuild.append( env.Command("boostnumericbindings", boostbuild, "svn checkout http://svn.boost.org/svn/boost/sandbox/numeric_bindings/ "+setpath(env, os.path.join("library", "build", "boost", "numeric_bindings"))) )
        lstdownload.append(boosttargz)
        
        
    if not("ginac" in skiplist) :
        clntarbz   = env.ParseAndDownload( CLN_DownloadURL )
        ginactarbz = env.ParseAndDownload( Ginac_DownloadURL )
        clndir     = env.Command(str(clntarbz).replace("[", "").replace("]", "").replace("'", "").replace(".tar.bz2", ""), clntarbz, env["EXTRACT_CMDBZ"]+env["extractsuffix"]+"library")
        ginacdir   = env.Command(str(ginactarbz).replace("[", "").replace("]", "").replace("'", "").replace(".tar.bz2", ""), ginactarbz, env["EXTRACT_CMDBZ"]+env["extractsuffix"]+"library")
        lstdownload.append(clntarbz)
        lstdownload.append(ginactarbz)
        lstbuild.append( CLNGiNaC_BuildInstall(env, clndir, ginacdir) )

    #download JSON library, extract & install
    if not("json" in skiplist) :
        jsontargz  = env.ParseAndDownload( JsonCPP_DownloadURL )
        jsonfiles  = env.Extract(str(jsontargz).replace("[", "").replace("]", "").replace("'", "").replace(".tar.gz", ""), jsontargz, extractsuffix=env["extractsuffix"]+"library")
        lstbuild.extend( JsonCPP_BuildInstall(env, jsontargz, jsonfiles) )
        lstdownload.append(jsontargz)
        
    # download HDF5, extract & install
    if not("hdf" in skiplist) :
        hdftargz = env.ParseAndDownload( HDF5_DownloadURL )
        hdfdir   = env.Command(str(hdftargz).replace("[", "").replace("]", "").replace("'", "").replace(".tar.gz", ""), hdftargz, env["EXTRACT_CMD"]+env["extractsuffix"]+"library")
        lstdownload.append(hdftargz)
        lstbuild.append( HDF5_BuildInstall(env, hdfdir) )
        
    # download LibXML, extract & install
    if not("xml" in skiplist) :
        xmltargz = env.ParseAndDownload( LibXML2_DownloadURL )
        xmldir   = env.Command(str(xmltargz).replace("[", "").replace("]", "").replace("'", "").replace(".tar.gz", ""), xmltargz, env["EXTRACT_CMD"]+env["extractsuffix"]+"library")
        lstdownload.append(xmltargz)
        lstbuild.append( LibXML2_BuildInstall(env, xmldir) )


env.Alias("librarydownload", lstdownload, FinishMessage, PRINT_CMD_LINE_FUNC=FinishMessage_print)
env.Clean(
    env.Alias("librarybuild", lstbuild, FinishMessage, PRINT_CMD_LINE_FUNC=FinishMessage_print), 
    [
        Glob(os.path.join("#", "library", "*"+env["SHLIBSUFFIX"])),
        Glob(os.path.join("#", "library", "*"+env["LIBSUFFIX"])),
        Glob(os.path.join("#", "library", "boost*")),
        Glob(os.path.join("#", "library", "bzip2*")),
        Glob(os.path.join("#", "library", "zlib*")),
        Glob(os.path.join("#", "library", "jsoncpp-src-*")),
        Glob(os.path.join("#", "library", "lapack*")),
        Glob(os.path.join("#", "library", "hdf*")),
        Glob(os.path.join("#", "library", "cln*")),
        Glob(os.path.join("#", "library", "ginac*")),
        Glob(os.path.join("#", "library", "libxml2*")),
        Glob(os.path.join("#", "library", "atlas*")),
        os.path.join("#", "library", "ATLAS")
    ]
) 
