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
import sys
sys.path.append("..")
import help
import os
Import("*")


# create target of a list of sources
def createTarget(env, path, sources, framework) :
    lst = []
    if sources.count == 0 :
        return

    add = []
    for i in framework :
        add.append( os.path.join("..", "..", i) )

    for i in sources :
        compilesource = [os.path.join("..", "..", "examples", path, i)]
        compilesource.extend(add)

        lst.append( env.Program( target=os.path.join("#build", os.path.splitext(i)[0]), source=compilesource ) )

    return lst
    
    
#=====================================================================================================================================
# default cpps that must be compiled on each run
framework = []
if env["withlogger"] or env["withrandomdevice"] :
    framework.append( "machinelearning.cpp" )



# genetic algorithm examples
listGA = createTarget(env, "geneticalgorithm", ["knapsack.cpp"], framework)

# other examples
srcOther = []
if env["withfiles"] and env["withsources"] :
    srcOther.extend( ["mds_nntp.cpp", "mds_wikipedia.cpp", "mds_twitter.cpp"] )
if env["withfiles"] :
    srcOther.append("mds_file.cpp")
listOther = createTarget(env, "other", srcOther, framework)

# classifier examples
srcClassifier = []
if env["withfiles"] :
    srcClassifier.extend( ["lazy.cpp"] )
listClassifier = createTarget(env, "classifier", srcClassifier, framework)

# distance examples
srcDistance = []
if env["withfiles"] :
    srcDistance.extend( ["ncd.cpp"] )
listDistance = createTarget(env, "distance", srcDistance, framework)

# reduce examples
srcReduce = []
if env["withfiles"] :
    srcReduce.extend( ["lda.cpp", "mds.cpp", "pca.cpp"] )
listReduce = createTarget(env, "reducing", srcReduce, framework)

#clustering examples
srcCluster = []
if env["withfiles"] :
    #srcCluster.extend( ["rlvq.cpp", "kmeans.cpp", "neuralgas.cpp", "patch_neuralgas.cpp", "relational_neuralgas.cpp", "spectral.cpp"] )
    srcCluster.extend( ["relational_neuralgas.cpp"] )
listCluster = createTarget(env, "clustering", srcCluster, framework)

srcSources = []
if env["withsources"] :
    srcSources.extend( ["twitter.cpp", "newsgroup.cpp", "wikipedia.cpp"] )
if env["withfiles"] :
    srcSources.append( "cloud.cpp" )
listSources = createTarget(env, "sources", srcSources, framework)

# create language files if multilanguage is used and copy to the builddir
if env["withmultilanguage"] :
    lst = []
    lang = help.getRekusivFiles( os.path.join("..", "..", "tools", "language"), ".mo")
    
    for i in lang :
        langfile = os.path.sep.join(i.split(os.path.sep)[4:])
        langpath = os.path.dirname(os.path.sep.join(i.split(os.path.sep)[4:]))
       
        lst.append( env.Command("mkdirlang", "", Mkdir(os.path.join("build", "language", langpath))) )
        lst.append( env.Command("copylang", "", Copy(os.path.join("build", "language", langfile), os.path.join("tools", "language", langfile))) )
        
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