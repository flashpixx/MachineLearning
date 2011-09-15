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


#ifndef __MACHINELEARNING_GENETICALGORITHM_FITNESSFUNCTION_HPP
#define __MACHINELEARNING_GENETICALGORITHM_FITNESSFUNCTION_HPP

#include <boost/static_assert.hpp>

#include "individual.hpp"
#include "population.hpp"


namespace machinelearning { namespace geneticalgorithm {
    
    /** abstract class of the fitness function
     * $LastChangedDate$
     **/
    template<typename T, typename L> class fitnessfunction
    {
        BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
        
        public :
        
            /** method for calculating the fitness value of an individual / return value must be >= 0 and 0 == worst value **/
            virtual T getFitness( const individual<L>& ) const = 0;
        
           // virtual T getFitness( const population<T>& ) const;
        
    };
    
};};

#endif

