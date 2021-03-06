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

# build script for the source example

import os
Import("*")

buildlist = []

if env["withsources"] :
    buildlist.append( env.Program( target=os.path.join("#build", env["buildtype"], "sources", "twitter"), source=defaultcpp+["twitter.cpp"] ) )
    buildlist.append( env.Program( target=os.path.join("#build", env["buildtype"], "sources", "newsgroup"), source=defaultcpp+["newsgroup.cpp"] ) )
    buildlist.append( env.Program( target=os.path.join("#build", env["buildtype"], "sources", "wikipedia"), source=defaultcpp+["wikipedia.cpp"] ) )

    if env["withfiles"] :
        buildlist.append( env.Program( target=os.path.join("#build", env["buildtype"], "sources", "cloud"), source=defaultcpp+["cloud.cpp"] ) )
    
if env["uselocallibrary"] or env["copylibrary"] :
    Depends(buildlist, env.LibraryCopy( os.path.join("#build", env["buildtype"], "sources"), [] ))
    
env.Alias( "sources", buildlist )