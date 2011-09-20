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


#ifndef __MACHINELEARNING_GENETICALGORITHM_CROSSOVER_HPP
#define __MACHINELEARNING_GENETICALGORITHM_CROSSOVER_HPP

#include <boost/static_assert.hpp>

#include "individual/individual.hpp"


namespace machinelearning { namespace geneticalgorithm {
    
    /** abstract class of the crossover function
     * $LastChangedDate$
     **/
    template<typename T> class crossover
    {
        
        public :
        
            /** returns the number how many individuals are needed for the crossover (default should be two) **/
            virtual std::size_t getNumberOfIndividuals( void ) const = 0;     
        
            /** set the individuals with a smart-pointer **/
            virtual void setIndividual( const boost::shared_ptr< individual::individual<T> >& ) = 0;
        
            /** creates a new smart-pointer object with the new individual data **/
            virtual boost::shared_ptr< individual::individual<T> > combine( void ) = 0;
    };
    
};};

#endif

