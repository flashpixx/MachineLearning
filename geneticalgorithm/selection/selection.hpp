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


#ifndef __MACHINELEARNING_GENETICALGORITHM_SELECTION_SELECTION_HPP
#define __MACHINELEARNING_GENETICALGORITHM_SELECTION_SELECTION_HPP

#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "../individual/individual.hpp"



namespace machinelearning { namespace geneticalgorithm { namespace selection {
    
    
    namespace ublas = boost::numeric::ublas;
    
    
    /** abstract class of the selection function
     * $LastChangedDate$
     **/
    template<typename T, typename L> class selection
    {
        BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
        
        public :
        
            /** returns a vector with smart-pointer to the individuals that are elite. The vector must be return the elite objects within the range [start,end)
             * @param p_start start value of the elite values
             * @param p_end end value of the elite values ([start, end) elite elements must be created)
             * @param p_population const reference to the population
             * @param p_fitness vector with fitnss values (index is equal to the index of the population)
             * @param p_rankIndex rank index (first index has the position of the population element, that has the smalles fitness value)
             * @param p_rank rank values (first element equal to polulation index has the rank value of the first individual)
             **/
            virtual std::vector< boost::shared_ptr< individual::individual<L> > > 
                    getElite( 
                              const std::size_t& p_start, 
                              const std::size_t& p_end,
                              const std::vector< boost::shared_ptr< individual::individual<L> > >& p_population,
                              const ublas::vector<T>& p_fitness,
                              const ublas::vector<std::size_t>& p_rankIndex,
                              const ublas::vector<std::size_t>& p_rank
                            ) const = 0;
        
    };
    
};};};

#endif

