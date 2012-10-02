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

import os
import re
import urllib2
Import("*")

workingpath = os.path.abspath(os.curdir)


#=== download packages ===============================================================================================================
def Boost_DownloadURL()  :
    # read download path of the Boost (latest version)
    f = urllib2.urlopen("http://www.boost.org/users/download/")
    html = f.read()
    f.close()
    
    found = re.search("<a href=\"http://sourceforge.net/projects/boost/files/(.*)\">Download</a>", html)
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
    
    
    
def JsonCPP_DownloadURL() :
    # read download path of the JsonCPP library (latest version)
    f = urllib2.urlopen("http://sourceforge.net/projects/jsoncpp/")
    html = f.read()
    f.close()
    
    found = re.search("<a href=\"/projects/jsoncpp/files/latest/download\" title=\"Download /jsoncpp/(.*)/jsoncpp-src-(.*)\.tar\.gz", html)
    if found == None :
        raise RuntimeError("JsonCPP Download URL not found")

    return "http://sourceforge.net/projects/jsoncpp/files/latest/download", "jsoncpp-src-" + found.group(1) + ".tar.gz"
    
   
     
def LibXML2_DownloadURL() :
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



def HDF5_DownloadURL() :
    # read download path of the HDF library (latest version)
    f = urllib2.urlopen("http://www.hdfgroup.org/ftp/HDF5/current/src/")
    html = f.read()
    f.close()
    
    found = re.search("<a href=\"(.*)tar.gz\">", html)
    if found == None :
        raise RuntimeError("HDF Download URL not found")
    
    filename = found.group(0).replace("<a href=\"./", "").replace("\">", "")

    return "http://www.hdfgroup.org/ftp/HDF5/current/src/"+filename, filename



def LaPack_DownloadURL() :
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


def Atlas_DownloadURL() :
    # read download path of the LAPack (latest version)
    f = urllib2.urlopen("http://sourceforge.net/projects/math-atlas/")
    html = f.read()
    f.close()
    
    found = re.search("<small title=\"(.*)\">(.*)</small>", html)
    if found == None :
        raise RuntimeError("Atlas Download URL not found")

    return "http://sourceforge.net/projects/math-atlas/files/latest/download?source=files", found.group(2)
    
    
def CLN_DownloadURL() :
    # read download path of the CLN (latest version)
    f = urllib2.urlopen("http://www.ginac.de/CLN/")
    html = f.read()
    f.close()
    
    found = re.search("<a href=\"cln-(.*)\.tar\.bz2\">", html)
    if found == None :
        raise RuntimeError("CLN Download URL not found")
        
    filename    = "cln-" + found.group(1) + ".tar.bz2"

    return "http://www.ginac.de/CLN/"+filename, filename
    
    
def Ginac_DownloadURL() :
    # read download path of the CLN (latest version)
    f = urllib2.urlopen("http://www.ginac.de/Download.html")
    html = f.read()
    f.close()
    
    found = re.search("<a href=\"http://www.ginac.de/ginac-(.*)\.tar\.bz2\">", html)
    if found == None :
        raise RuntimeError("GiNaC Download URL not found")

    filename = "ginac-" + found.group(1) + ".tar.bz2"

    return "http://www.ginac.de/"+filename, filename
    
    
def GZip_DownloadURL() :
    # read download path of the ZLib (latest version)
    f = urllib2.urlopen("http://www.zlib.net/")
    html = f.read()
    f.close()
    
    found = re.search("http://zlib.net/zlib-(.*)\.tar\.gz", html)
    if found == None :
        raise RuntimeError("ZLib Download URL not found")

    filename = "zlib-" + found.group(1) + ".tar.gz"

    return "http://zlib.net/"+filename, filename

    
def BZip2_DownloadURL() :
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
    boostpath   = str(source).replace("['", "").replace("']", "")
    version     = boostpath.replace("boost-", "")
    boostpath   = os.path.join(workingpath, boostpath.replace(".", "_").replace("-", "_"))

    # set the toolset and compile the bjam and build boost
    boostoptions = "--with-exception --with-filesystem --with-math --with-random --with-regex --with-date_time --with-thread --with-system --with-program_options --with-serialization --with-iostreams --disable-filesystem2 link=shared runtime-link=shared threading=multi variant=%V% install --layout=system --prefix="+os.path.abspath(os.path.join(workingpath, "build_"+env["buildtype"], "boost", version))
    boostoptions = boostoptions.replace("%V%", env["buildtype"])
    if env["withmpi"] :
        boostoptions = "--with-mpi " + boostoptions
    
    cmd = None
    if env["TOOLKIT"] in ["darwin", "posix", "cygwin"] :
        cmd = "cd " + boostpath + ";"
        if env["withmpi"] :
            cmd = cmd + " echo \"using mpi ;\" >> " + os.path.join(boostpath, "tools", "build", "v2", "user-config.jam") + ";"
        cmd = cmd + " ./bootstrap.sh; ./b2 " + boostoptions
    
    else :
        raise RuntimeError("toolkit not known")

    # build Boost with Bzip2 and ZLib support and set the dependency
    dependency = [source]
    if (gzipbuild <> None) and (bzipbuild <> None) :
        dependency.append(gzipbuild)
        dependency.append(bzipbuild)
        
        gzipversion = str(gzipbuild).replace("['", "").replace("']", "").replace("buildgzip-", "")
        bzipversion = str(bzipbuild).replace("['", "").replace("']", "").replace("buildbzip2-", "")
        zipcmd   = "export BZIP2_BINARY=bz2; export ZLIB_BINARY=z; "
        zipcmd  += "export ZLIB_INCLUDE="+os.path.join(workingpath, "build_"+env["buildtype"], "zlib", gzipversion, "include")+"; "
        zipcmd  += "export ZLIB_LIBPATH="+os.path.join(workingpath, "build_"+env["buildtype"], "zlib", gzipversion, "lib")+"; "
        zipcmd  += "export BZIP2_INCLUDE="+os.path.join(workingpath, "build_"+env["buildtype"], "bzip2", bzipversion, "include")+"; "
        zipcmd  += "export BZIP2_LIBPATH="+os.path.join(workingpath, "build_"+env["buildtype"], "bzip2", bzipversion, "lib")+"; "
        cmd      = zipcmd + cmd
        
    return env.Command("buildboost-"+version, dependency, cmd)
      
      
def Gzip_BuildInstall(env, extract) :
    version = str(extract).replace("['", "").replace("']", "").replace("zlib-", "")
    return env.Command("buildgzip-"+version, extract, "cd $SOURCE; ./configure --prefix="+os.path.abspath(os.path.join(workingpath, "build_"+env["buildtype"], "zlib", version))+"; make; make install")
   
   
def BZip2_BuildInstall(env, extract) :
    version = str(extract).replace("['", "").replace("']", "").replace("bzip2-", "")
    return env.Command("buildbzip2-"+version, extract, "cd $SOURCE; make install PREFIX="+os.path.abspath(os.path.join(workingpath, "build_"+env["buildtype"], "bzip2", version)))
      
        
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

    envjson = env.Clone()
    envjson.Append(CPPPATH = os.path.join(sourcepath, "include"))
    
    libbuild = []
    if env["TOOLKIT"] in ["posix", "cygwin", "darwin"] :
        libbuild.append( envjson.Library(target="json", source=libsrc) )
        libbuild.append( envjson.SharedLibrary(target="json", source=libsrc) )
        
    else :
        raise RuntimeError("no library jsoncpp build target found in toolkit ["+env["TOOLKIT"]+"]")
        
    # install header and libraries
    libinstall      = env.Install( os.path.join(workingpath, "build_"+env["buildtype"], "jsoncpp", version, "lib"), libbuild )
    headerdir       = env.Command( os.path.join(workingpath, "build_"+env["buildtype"], "jsoncpp", version, "include", "json"), "", Mkdir("$TARGET"))
    headerinstall   = []
    for i in libheader :
        headerinstall.append( env.Command( os.path.join(str(headerdir).replace("']", "").replace("['", ""), os.path.basename(str(i))), i, Copy("$TARGET", "$SOURCE")) )
    
    # prevent libs and headers during clean
    env.NoClean(libinstall)
    env.NoClean(headerinstall)
    
    return libinstall + headerinstall


def Atlas_BuildInstall(env, atlasdir, lapacktargz) :
    # Atlas need a temporary build directory
    builddir = env.Command( str(atlasdir).replace("['", "").replace("']", "")+"-buildtmp", atlasdir, Mkdir("$TARGET"))
    version  = str(atlasdir).replace("atlas", "").replace("['", "").replace("']", "")
    
    cmd = "cd " + os.path.join(workingpath, str(builddir).replace("['", "").replace("']", "")) + "; ../ATLAS/configure --dylibs"
    if env["atlaspointerwidth"] == "32" :
        cmd += " -b 32"
    elif env["atlaspointerwidth"] == "64" :
        cmd += " -b 64"
    cmd += " --with-netlib-lapack-tarfile=../" + str(lapacktargz).replace("['", "").replace("']", "") + " --prefix=" + os.path.join(workingpath, "build_"+env["buildtype"], "atlas", version) + "; make; make install"
    
    return env.Command("buildatlas-"+version, [atlasdir, lapacktargz, builddir], cmd)


def HDF5_BuildInstall(env, hdfdir) :
    version = str(hdfdir).replace("']", "").replace("['", "").replace("hdf5-", "")
    
    return env.Command("buildhdf5-"+version, hdfdir, "cd $SOURCE; ./configure --enable-cxx --prefix=" + os.path.join(workingpath, "build_"+env["buildtype"], "hdf", version) + "; make; make install")


def LibXML2_BuildInstall(env, libxmldir) :
    version = str(libxmldir).replace("']", "").replace("['", "").replace("libxml2-", "")
    
    cmd = "cd $SOURCE; ./configure --without-python --without-threads --prefix="+os.path.abspath(os.path.join(workingpath, "build_"+env["buildtype"], "xml", version))
    if env["buildtype"] == "release" :
        cmd = cmd + " --without-debug"
    cmd = cmd + "; make; make install"
    
    return env.Command("buildlibxml2-"+version, libxmldir, cmd)
    
        
def GiNaC_BuildInstall(env, ginacdir, clnbuild) :
    version    = str(ginacdir).replace("']", "").replace("['", "").replace("ginac-", "")
    clnversion = str(clnbuild).replace("']", "").replace("['", "").replace("buildcln-", "")
    clninclude = os.path.join(workingpath, "build_"+env["buildtype"], "cln", clnversion, "include")
    clnlib     = os.path.join(workingpath, "build_"+env["buildtype"], "cln", clnversion, "lib")
    
    return env.Command("buildginac-"+version, [ginacdir, clnbuild], "cd $SOURCE; export CLN_CFLAGS=-I" + clninclude + "; export CLN_LIBS=\"-L" + clnlib + " -lcln\"; ./configure --prefix="+os.path.abspath(os.path.join(workingpath, "build_"+env["buildtype"], "ginac", version))+ "; make; make install")
        
        
def CLN_BuildInstall(env, clndir) :
    version = str(clndir).replace("']", "").replace("['", "").replace("cln-", "")
    
    return env.Command("buildcln-"+version, clndir, "cd $SOURCE; ./configure --prefix="+os.path.abspath(os.path.join(workingpath, "build_"+env["buildtype"], "cln", version))+ "; make; make install")
    

#=== target structure ================================================================================================================
def FinishMessage_print(s, target, source, env):
    pass
def FinishMessage(target, source, env) :
    alias = str(target[0])
    if alias == "librarydownload" :
        print "\n==> library sources have been downloaded, run for building the target [librarybuild]\n"
    if alias == "librarybuild" :
        print "\n==> libraries have been built and stored under ["+os.path.join("library", "build_"+env["buildtype"])+"]\n"
    
    

skiplist = str(env["skiplibrary"]).split(",")
if (("librarybuild" in COMMAND_LINE_TARGETS) or ("librarydownload" in COMMAND_LINE_TARGETS)) and ("all" in skiplist) :
    raise RuntimeError("nothing to build")

#build into seperate directory, check needed installation tools and get the command line for extracting tar.gz files
lstbuild    = []
lstdownload = []

# build target list
if ("librarybuild" in COMMAND_LINE_TARGETS) or ("librarydownload" in COMMAND_LINE_TARGETS) :

    if " " in workingpath :
        raise RuntimeError("there are spaces within the path, use a path without spaces")

    # download Atlas & LaPack, extract & install
    if not("atlas" in skiplist) :
        lapacktargz = env.ParseAndDownload( LaPack_DownloadURL )
        atlastarbz  = env.ParseAndDownload( Atlas_DownloadURL )
        atlasdir    = env.Command(str(atlastarbz).replace("[", "").replace("]", "").replace("'", "").replace(".tar.bz2", ""), atlastarbz, env["EXTRACT_CMDBZ"]+env["extractsuffix"]+"library")
        lstdownload.append(lapacktargz)
        lstdownload.append(atlastarbz)
        lstbuild.append( Atlas_BuildInstall(env, atlasdir, lapacktargz) )

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
        boostdir   = env.Command(str(boosttargz).replace("[", "").replace("]", "").replace("'", "").replace(".tar.gz", ""), boosttargz, env["EXTRACT_CMD"]+env["extractsuffix"]+"library")
        boostbuild = Boost_BuildInstall(env, boostdir, gzipbuild, bzipbuild)
        
        lstbuild.append( env.Command("boostnumericbindings", boostbuild, "svn checkout http://svn.boost.org/svn/boost/sandbox/numeric_bindings/ "+os.path.join(workingpath, "build_"+env["buildtype"], "boost", "sandbox", "numeric_bindings")) )
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
        Glob(os.path.join("#", "library", "boost*")),
        Glob(os.path.join("#", "library", "bzip2*")),
        Glob(os.path.join("#", "library", "zlib*")),
        Glob(os.path.join("#", "library", "jsoncpp-src-*")),
        Glob(os.path.join("#", "library", "hdf*")),
        Glob(os.path.join("#", "library", "cln*")),
        Glob(os.path.join("#", "library", "ginac*")),
        Glob(os.path.join("#", "library", "libxml2*")),
        Glob(os.path.join("#", "library", "atlas*")),
        os.path.join("#", "library", "ATLAS")
    ]
) 
