############################################################################
# LGPL License                                                             #
#                                                                          #
# This file is part of the Machine Learning Framework.                     #
# Copyright (c) 2010, Philipp Kraus, <philipp.kraus@flashpixx.de>          #
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
    data["cheaders"]   = unique(data["cheaders"])
    data["cppheaders"] = unique(data["cppheaders"])
    data["libraries"]  = unique(data["libraries"])

    # check C++ environment
    if not conf.CheckCXX() :
        Exit(1)
        
    # check data (header & libraries)
    for i in data["cheaders"] :
        if not conf.CheckCHeader(i) :
            Exit(1)
            
    for i in data["cppheaders"] :
        if not conf.CheckCXXHeader(i) :
            Exit(1)
    
    for i in data["librariesWithHeader"] :

        # the "lang" is not set, because in CXX the library is not found
        if not conf.CheckLib(i["lib"], None) :
            Exit(1)
            
        if i["lang"] == "CXX" :
            if type([]) == type(i["header"]) :
                for n in i["header"] :
                    if not conf.CheckCXXHeader(n) :
                        Exit(1)
            elif type("") == type(i["header"]) :
                if not conf.CheckCXXHeader(i["header"]) :
                        Exit(1)
                    
        elif i["lang"] == "C" :
            if type([]) == type(i["header"]) :
                for n in i["header"] :
                    if not conf.CheckCHeader(n) :
                        Exit(1)
            elif type("") == type(i["header"]) :
                if not conf.CheckCHeader(i["header"]) :
                        Exit(1)

    for i in data["libraries"] :
        if not conf.CheckLib(i) :
            Exit(1)
    