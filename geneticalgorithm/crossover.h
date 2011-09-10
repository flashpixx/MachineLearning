/** 
 @cond
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
 @endcond
 **/


#ifndef MACHINELEARNING_GENETICALGORITHM_CROSSOVER_H
#define MACHINELEARNING_GENETICALGORITHM_CROSSOVER_H

#include <boost/static_assert.hpp>

#include "individual.h"


namespace machinelearning { namespace geneticalgorithm {
    
    /** abstract class of the crossover function
     * $LastChangedDate: 2011-09-10 20:05:39 +0200 (Sa, 10 Sep 2011) $
     **/
    class crossover {
        
        public :
        
            /** returns the number how many individuals are needed for the crossover (default should be two) **/
            virtual std::size_t getNumberOfIndividuals( void ) const;        
        
            /** set the individuals with a pointer **/
            virtual void setIndividual( const individual& );
        
            /** create the new individual into the pointer structure, after creating the list of individuals must be cleared **/
            virtual void create( const individual* );
        
    };
    
};};

#endif

