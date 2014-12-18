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

    found = re.search( "<a href=\"http://sourceforge.net/projects/boost/files/boost/(.*)\">Download</a>", html )
    if found == None :
        raise RuntimeError("Boost Download URL not found")

    # read url of the tar.bz2
    f = urllib2.urlopen(found.group(0).replace("<a href=\"", "").replace("\">Download</a>", ""))
    html = f.read()
    f.close()
    found = re.search( "http://sourceforge.net/projects/boost/files/boost/(.*).tar.gz/download", html )
    if found == None :
        raise RuntimeError("Boost file Download URL not found")

    # create download URL and version
    return "http://downloads.sourceforge.net/project/boost/boost/"+found.group(1)+".tar.gz", (found.group(1)+".tar.gz").split("/")[1]

    
    
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


def Atlas_DownloadURL(env) :
    if env["atlasversion"] == "stable" :     
        # read download path of the Atlas (latest stableversion)
        f = urllib2.urlopen("http://sourceforge.net/projects/math-atlas/")
        html = f.read()
        f.close()
    
        found = re.search("<small title=\"(.*)\">(.*)</small>", html)
        if found == None :
            raise RuntimeError("Atlas Download URL not found")
        return "http://sourceforge.net/projects/math-atlas/files/latest/download?source=files", found.group(2)

    elif env["atlasversion"] == "devel" : 
        # read download path of the Atlas (latest developer version)
        f = urllib2.urlopen("https://sourceforge.net/projects/math-atlas/files/Developer%20%28unstable%29/")
        html = f.read()
        f.close()

        found = re.search("<a href=\"/projects/math-atlas/files/Developer%20%28unstable%29/(.+)/\"", html)
        if found == None :
            raise RuntimeError("Atlas Version Download URL not found")
        version  = found.group(0).replace("\"", "").replace("<a href=", "").strip("/").split("/")[-1]     
        filename = "atlas" + version + ".tar.bz2"
        return "https://downloads.sourceforge.net/project/math-atlas/Developer%20%28unstable%29/"+version+"/"+filename, filename
        
    raise RuntimeError("Atlas Download unknown")
    
    
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
    
    
def GZip_DownloadURL(env) :
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
def Boost_BuildInstall(env, source, gzipbuild, bzipbuild)  :
    # extract path and version of the source name
    boostdir    = str(source).replace("extractdir-", "").replace("['", "").replace("']", "").replace(".", "_").replace("-", "_")
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
    if (gzipbuild <> None) and (bzipbuild <> None) :
        zipcmd   = "export BZIP2_BINARY=bz2 && export ZLIB_BINARY=z && "
        dependency.append(gzipbuild)
        dependency.append(bzipbuild)
        gzipversion = str(gzipbuild).replace("['", "").replace("']", "").replace("buildgzip-", "")
        bzipversion = str(bzipbuild).replace("['", "").replace("']", "").replace("buildbzip2-", "")
        
        zipcmd  += "export ZLIB_INCLUDE="  + setpath(env, os.path.abspath(os.path.join(os.curdir, "build", "zlib", gzipversion, "include"))) + " && "
        zipcmd  += "export ZLIB_LIBPATH="  + setpath(env, os.path.abspath(os.path.join(os.curdir, "build", "zlib", gzipversion, "lib"))) + " && "
        zipcmd  += "export BZIP2_INCLUDE=" + setpath(env, os.path.abspath(os.path.join(os.curdir, "build", "bzip2", bzipversion, "include"))) + " && "
        zipcmd  += "export BZIP2_LIBPATH=" + setpath(env, os.path.abspath(os.path.join(os.curdir, "build", "bzip2", bzipversion, "lib"))) + " && "
        cmd      = zipcmd + cmd
        
    return env.Command("buildboost-"+version, dependency, cmd)
      
      
def Gzip_BuildInstall(env, extract) :
    version = str(extract).replace("['", "").replace("']", "").replace("zlib-", "")
    prefix  = os.path.join("..", "build", "zlib", version)

    cmd = "cd $SOURCE && ";
    if env["TOOLKIT"] == "msys" :
        lib = setpath(env, os.path.join(prefix, "lib"))
        inc = setpath(env, os.path.join(prefix, "include"))
        cmd = cmd + "make -fwin32/Makefile.gcc && make install -fwin32/Makefile.gcc INCLUDE_PATH="+inc+" BINARY_PATH="+lib+" LIBRARY_PATH="+lib
    else :
        cmd = cmd + "./configure --prefix=" + prefix + " && make && make install"
    return env.Command("buildgzip-"+version, extract, cmd)
   
   
def BZip2_BuildInstall(env, extract) :
    version = str(extract).replace("['", "").replace("']", "").replace("bzip2-", "")
    prefix  = setpath(env, os.path.join("..", "build", "bzip2", version))
    
    if env["TOOLKIT"] == "msys" :
        cmd = "cd $SOURCE && grep -v chmod Makefile | grep -v ln > Makefile.msys && make -fMakefile.msys install PREFIX=" + setpath(env, prefix)
    else :
        cmd = "cd $SOURCE && make CFLAGS=\"-Wall -Winline -O2 -g -D_FILE_OFFSET_BITS=64 -fPIC\" LDFLAGS=\"-fPIC\" install PREFIX=" + prefix
    
    return env.Command("buildbzip2-"+version, extract, cmd)
      
        
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
    
    return libinstall + headerinstall

def Lapack_BuildInstall(env, lapackdir) :
    # build Lapack only with MSYS
    dir     = str(lapackdir).replace("']", "").replace("['", "")
    version = dir.replace("lapack-", "")
    
    cmd = "cd $SOURCE && cmake . -G \"MSYS Makefiles\" -DCMAKE_BUILD_TYPE:String=release -DLAPACKE:BOOL=\"1\" -DCMAKE_INSTALL_PREFIX:PATH=\"" + setpath(env, os.path.abspath(os.path.join("build", "lapack", version))) + "\" && make && make install"
    return env.Command("buildlapack-"+version, lapackdir, cmd)

def LapackAtlas_BuildInstall(env, atlasdir, lapacktargz) :
    # Atlas need a temporary build directory
    builddir = env.Command( str(atlasdir).replace("['", "").replace("']", "")+"-buildtmp", atlasdir, Mkdir("$TARGET"))
    version  = str(atlasdir).replace("atlas", "").replace("['", "").replace("']", "")
    prefix   = os.path.join("..", "build", "atlas", version)
    
    cmd = "cd " + os.path.join("library", str(builddir).replace("['", "").replace("']", "")) + " && " + os.path.join("..", "ATLAS", "configure") + " --dylibs"
    if env["atlaspointerwidth"] == "32" :
        cmd += " -b 32"
    elif env["atlaspointerwidth"] == "64" :
        cmd += " -b 64"
    cmd += " --with-netlib-lapack-tarfile=" + os.path.join("..", str(lapacktargz).replace("['", "").replace("']", "")) + " --prefix=" + prefix
    
    # change Soname of the Linux library
    if env["TOOLKIT"] == "posix" :
        cmd = cmd + " && sed -e \"s/soname \\$$(LIBINSTdir)\\/\\$$(outso)/soname \\$$(outso).\\$$(VER)/\" lib/Makefile > lib/Makefile.tmp && mv -f lib/Makefile.tmp lib/Makefile"
    cmd = cmd + " && make && make install"
    if env["TOOLKIT"] == "posix" :
        cmd = cmd + " && cd "+prefix+"/lib && mv libsatlas.so libsatlas.so."+version+" && mv libtatlas.so libtatlas.so."+version+" && ln -s libsatlas.so."+version+" libsatlas.so && ln -s libtatlas.so."+version+" libtatlas.so"
    
    return env.Command("buildatlas-"+version, [atlasdir, lapacktargz, builddir], cmd)


def HDF5_BuildInstall(env, hdfdir) :
    version = str(hdfdir).replace("']", "").replace("['", "").replace("hdf5-", "")
    prefix  = setpath(env, os.path.abspath(os.path.join(os.curdir, "build", "hdf", version)))
    
    if env["TOOLKIT"] == "msys" :
        cmd = "cd $SOURCE && cmake . -G \"MSYS Makefiles\" -DCMAKE_BUILD_TYPE:String=release -DBUILD_SHARED_LIBS:BOOL=\"1\" -DHDF5_BUILD_HL_LIB:BOOL=\"1\" -DHDF5_BUILD_CPP_LIB:BOOL=\"1\" -DCMAKE_INSTALL_PREFIX:PATH=\"" + prefix + "\" && make && make install"
    else :
        cmd = "cd $SOURCE && ./configure --enable-cxx --prefix=" + prefix + " && make && make install"
    return env.Command("buildhdf5-"+version, hdfdir, cmd)


def LibXML2_BuildInstall(env, libxmldir) :
    version   = str(libxmldir).replace("']", "").replace("['", "").replace("libxml2-", "")
    prefix    = setpath(env, os.path.abspath(os.path.join(os.curdir, "build", "xml", version)))
    
    cmd = "cd $SOURCE && ./configure --without-python --without-threads --without-debug"
    cmd = cmd + " --prefix=" + prefix + " && make && make install"
    return env.Command("buildlibxml2-"+version, libxmldir, cmd)
    
        
def GiNaC_BuildInstall(env, ginacdir, clnbuild) :
    version    = str(ginacdir).replace("']", "").replace("['", "").replace("ginac-", "")
    prefix     = setpath(env, os.path.abspath(os.path.join(os.curdir, "build", "ginac", version)))
    
    clnversion = str(clnbuild).replace("']", "").replace("['", "").replace("buildcln-", "")
    clninclude = setpath(env, os.path.abspath(os.path.join(os.curdir, "build", "cln", clnversion, "include")))
    clnlib     = setpath(env, os.path.abspath(os.path.join(os.curdir, "build", "cln", clnversion, "lib")))
    
    cmd = "cd $SOURCE && export CLN_CFLAGS=-I" + clninclude + " && export CLN_LIBS=\"-L" + clnlib + " -lcln\" && ./configure --prefix=" + prefix + " && make && make install"
    return env.Command("buildginac-"+version, [ginacdir, clnbuild], cmd)
        
        
def CLN_BuildInstall(env, clndir) :
    version = str(clndir).replace("']", "").replace("['", "").replace("cln-", "")
    prefix  = setpath(env, os.path.abspath(os.path.join(os.curdir, "build", "cln", version)))
    
    cmd = "cd $SOURCE && ./configure --prefix=" + prefix + " && make && make install"
    return env.Command("buildcln-"+version, clndir, cmd)
    
    
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

    # download Atlas / LaPack, extract & install
    if not("lapack" in skiplist) :
        lapacktargz = env.ParseAndDownload( LaPack_DownloadURL )
        lstdownload.append(lapacktargz)
        if env["TOOLKIT"] == "msys" :
            lstbuild.append( Lapack_BuildInstall(env, env.Command(str(lapacktargz).replace("[", "").replace("]", "").replace("'", "").replace(".tgz", ""), lapacktargz, env["EXTRACT_CMD"]+env["extractsuffix"]+"library")) )
        else :
            atlastarbz  = env.ParseAndDownload( Atlas_DownloadURL )
            lstdownload.append(atlastarbz)
            atlasdir    = env.Command(str(atlastarbz).replace("[", "").replace("]", "").replace("'", "").replace(".tar.bz2", ""), atlastarbz, env["EXTRACT_CMDBZ"]+env["extractsuffix"]+"library")
            lstbuild.append( LapackAtlas_BuildInstall(env, atlasdir, lapacktargz) )

    # download Boost, extract & install
    if not("boost" in skiplist) :
        bzipbuild = None
        gzipbuild = None
        if env["zipsupport"] :
            gziptargz = env.ParseAndDownload( GZip_DownloadURL )
            bziptargz = env.ParseAndDownload( BZip2_DownloadURL )
            bzipbuild = BZip2_BuildInstall(env, env.Command(str(bziptargz).replace("[", "").replace("]", "").replace("'", "").replace(".tar.gz", ""), bziptargz, env["EXTRACT_CMD"]+env["extractsuffix"]+"library") )
            gzipbuild = Gzip_BuildInstall(env,  env.Command(str(gziptargz).replace("[", "").replace("]", "").replace("'", "").replace(".tar.gz", ""), gziptargz, env["EXTRACT_CMD"]+env["extractsuffix"]+"library") )
            lstdownload.append(gziptargz)
            lstdownload.append(bziptargz)
    
        boosttargz = env.ParseAndDownload( Boost_DownloadURL )
        boostdir   = env.Command("extractdir-"+str(boosttargz).replace("[", "").replace("]", "").replace("'", "").replace(".tar.gz", ""), boosttargz, env["EXTRACT_CMD"]+env["extractsuffix"]+"library")
        boostbuild = Boost_BuildInstall(env, boostdir, gzipbuild, bzipbuild)
        
        lstbuild.append( env.Command("boostnumericbindings", boostbuild, "svn checkout http://svn.boost.org/svn/boost/sandbox/numeric_bindings/ "+setpath(env, os.path.join("library", "build", "boost", "numeric_bindings"))) )
        lstdownload.append(boosttargz)
        
        
    if not("ginac" in skiplist) :
        clntarbz   = env.ParseAndDownload( CLN_DownloadURL )
        ginactarbz = env.ParseAndDownload( Ginac_DownloadURL )
        clndir     = env.Command(str(clntarbz).replace("[", "").replace("]", "").replace("'", "").replace(".tar.bz2", ""), clntarbz, env["EXTRACT_CMDBZ"]+env["extractsuffix"]+"library")
        ginacdir   = env.Command(str(ginactarbz).replace("[", "").replace("]", "").replace("'", "").replace(".tar.bz2", ""), ginactarbz, env["EXTRACT_CMDBZ"]+env["extractsuffix"]+"library")
        lstdownload.append(clntarbz)
        lstdownload.append(ginactarbz)
        lstbuild.append( GiNaC_BuildInstall(env, ginacdir, CLN_BuildInstall(env, clndir)) )

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
