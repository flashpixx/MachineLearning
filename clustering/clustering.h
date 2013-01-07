/** 
 @cond
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
 @endcond
 **/

/** header file to connect all clustering algorithm for one include **/

#ifndef __MACHINELEARNING_CLUSTERING_CLUSTERING_H
#define __MACHINELEARNING_CLUSTERING_CLUSTERING_H

namespace machinelearning { 
    
    /** namespace for all clustering algorithms
     * @todo adding SOM http://www.cis.hut.fi/research/som-research/
     **/
    namespace clustering { }
    
}


#include "nonsupervised/clustering.hpp"
#include "nonsupervised/neuralgas.hpp"
#include "nonsupervised/relational_neuralgas.hpp"
#include "nonsupervised/kmeans.hpp"
#include "nonsupervised/spectralclustering.hpp"

#include "supervised/clustering.hpp"
#include "supervised/rlvq.hpp"

#endif
