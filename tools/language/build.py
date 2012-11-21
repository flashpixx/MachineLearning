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


# build script for the multilanguage of the framework

import os
Import("*")


env.Tool("gettext") 
env.AppendUnique(MSGMERGEFLAGS   = ["--no-wrap", "--update"])
env.AppendUnique(XGETTEXTFLAGS   = ["--keyword=_", "--language=c++", "--no-wrap"])

po = env.Translate(["de"], GlobRekursiv(os.path.join("..", ".."), env["CPPSUFFIXES"], ["swig", "examples", "documentation", "library", "buildenvironment"]), POAUTOINIT = True) 
mo = env.MOFiles(po)

env.Clean(
    env.Alias("language", [
        env.InstallAs( os.path.join("#build", "language", "de_DE.UTF-8", "LC_MESSAGES", "machinelearning.mo"), "de.mo")
    ]),
    [
        "messages.pot",
        Glob("*.po~")
    ]
)
