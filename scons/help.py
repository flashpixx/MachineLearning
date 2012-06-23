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
import sys

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
def unique(seq) :
    noDupes = []
    [ noDupes.append(i) for i in seq if not noDupes.count(i) ]
    return noDupes
    
    
# function for checking the conviguration options
def checkConfiguratin( conf, data ) :

    # uniquify all lists
    data["cheaders"]            = unique(data["cheaders"])
    data["cppheaders"]          = unique(data["cppheaders"])

    data["cpplibraries"]        = unique(data["cpplibraries"])
    data["cppdylibrariesonly"]  = unique(data["cppdylibrariesonly"])
    data["cdylibrariesonly"]    = unique(data["cdylibrariesonly"])
    data["clibraries"]          = unique(data["clibraries"])
    


    # check C++ environment
    if not conf.CheckCXX() :
        sys.exit(1)
        
    # check data (header & libraries)
    for i in data["cheaders"] :
        if not conf.CheckCHeader(i) :
            sys.exit(1)
            
    for i in data["cppheaders"] :
        if not conf.CheckCXXHeader(i) :
            sys.exit(1)
    
    
    # on static libs we must the check a little bit different
    for i in data["staticlinkonly"] :
        lcLib  = conf.env["LIBPREFIX"] + i + conf.env["LIBSUFFIX"]   
        lxPath = conf.env.FindFile(lcLib, conf.env["LIBPATH"])
           
        lcMsg = ""
        if lxPath == None :
            print "Checking for static C library " + lcLib + "... no"
            sys.exit(1)
        else :
            conf.env.Append(LINKFLAGS = str(lxPath))
            print "Checking for static C/C++ library " + lcLib + "... yes (found in " + os.path.dirname(str(lxPath)) + ")"
    
    
    
    
    if conf.env["staticlink"] :
        for i in data["cdylibrariesonly"] :
            if not conf.CheckLib(i, language="C") :
                sys.exit(1)
        for i in data["cppdylibrariesonly"] :
            if not conf.CheckLib(i, language="C++") :
                sys.exit(1)

                
        for i in data["cpplibraries"]+data["clibraries"] :
            lcLib  = conf.env["LIBPREFIX"] + i + conf.env["LIBSUFFIX"]   
            lxPath = conf.env.FindFile(lcLib, conf.env["LIBPATH"])
           
            lcMsg = ""
            if lxPath == None :
                print "Checking for static C library " + lcLib + "... no"
                sys.exit(1)
            else :
                conf.env.Append(LINKFLAGS = str(lxPath))
                print "Checking for static C/C++ library " + lcLib + "... yes (found in " + os.path.dirname(str(lxPath)) + ")"
    
    else :
    
        for i in data["cdylibrariesonly"]+data["clibraries"] :
            if not conf.CheckLib(i, language="C") :
                sys.exit(1)
    
        for i in data["cppdylibrariesonly"]+data["cpplibraries"] :
            if i == "boost_mpi" :
                conf.env.Append(LIBS = i)
                continue
            if not conf.CheckLib(i, language="C++") :
                sys.exit(1)
