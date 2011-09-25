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


#ifndef __MACHINELEARNING_GENETICALGORITHM_FITNESS_FITNESS_HPP
#define __MACHINELEARNING_GENETICALGORITHM_FITNESS_FITNESS_HPP

#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>

#include "../individual/individual.hpp"


namespace machinelearning { namespace geneticalgorithm { namespace fitness {
    
    /** abstract class of the fitness function
     * $LastChangedDate$
     **/
    template<typename T, typename L> class fitness
    {
        BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
        
        public :
        
            /** method for calculating the fitness value of an individual / return value should be [0, best value]
             * @param p_individual reference to an individual
             **/
            virtual T getFitness( const individual::individual<L>& p_individual ) const = 0;
       
            /** method for cloning the object, for using on multithread
             * @param p_ptr smart-pointer object
             **/
            virtual void clone( boost::shared_ptr< fitness<T,L> >& p_ptr ) const = 0;
        
            /** method that is called at the end of each iteration
             * @param p_population population
             **/
            virtual void onEachIteration( const std::vector< boost::shared_ptr< individual::individual<L> > >& p_population ) = 0;
        
    };
    
};};};

#endif

