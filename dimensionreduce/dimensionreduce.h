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

/** header file to connect all dimensional reducing algorithm for one include **/

#ifndef __MACHINELEARNING_DIMENSIONREDUCE_DIMENSIONREDUCE_H
#define __MACHINELEARNING_DIMENSIONREDUCE_DIMENSIONREDUCE_H


namespace machinelearning { 
    
    /** namespace for all algorithms to reduce data dimension **/    
    namespace dimensionreduce { }
    
}


#include "supervised/reduce.hpp"
#include "supervised/lda.hpp"

#include "nonsupervised/reduce.hpp"
#include "nonsupervised/pca.hpp"
#include "nonsupervised/lle.hpp"
#include "nonsupervised/mds.hpp"

#endif
