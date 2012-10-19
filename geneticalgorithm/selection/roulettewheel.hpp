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


#ifndef __MACHINELEARNING_GENETICALGORITHM_SELECTION_ROULETTEWHEEL_HPP
#define __MACHINELEARNING_GENETICALGORITHM_SELECTION_ROULETTEWHEEL_HPP

#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "selection.hpp"
#include "../individual/individual.hpp"
#include "../../tools/tools.h"



namespace machinelearning { namespace geneticalgorithm { namespace selection {
    
    
    namespace ublas = boost::numeric::ublas;
    
    
    /** class of the roulette-wheel-selection **/
    template<typename T, typename L> class roulettewheel : public selection<T,L>
    {
        BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
        
        public :
        
            roulettewheel( void );
        
            void clone( boost::shared_ptr< selection<T,L> >& p_ptr ) const;
            void getElite( const std::size_t&, const std::size_t&, const std::vector< boost::shared_ptr< individual::individual<L> > >&, const ublas::vector<T>&, const ublas::vector<std::size_t>&, const ublas::vector<std::size_t>&,std::vector< boost::shared_ptr< individual::individual<L> > >&  );
        
            void onEachIteration( const std::vector< boost::shared_ptr< individual::individual<L> > >& ) {}
        
        
        private :
        
            /** random object **/
            tools::random m_random;
        
    };
    
    
    
    /** constructor **/
    template<typename T, typename L> inline roulettewheel<T,L>::roulettewheel( void ) :
        m_random()
    {}
    
    
    /** method for cloning the object, for using on multithread
     * @param p_ptr smart-pointer object
     **/
    template<typename T, typename L> inline void roulettewheel<T,L>::clone( boost::shared_ptr< selection<T,L> >& p_ptr ) const
    {
        p_ptr = boost::shared_ptr< selection<T,L> >( new roulettewheel<T,L>() );
    }

        
    /** returns the roulette-wheel-selection elites
     * @param p_start start value of the elite values
     * @param p_end end value of the elite values ([start, end) elite elements must be created)
     * @param p_population const reference to the population
     * @param p_fitness vector with fitnss values (index is equal to the index of the population)
     * @param p_elite vector with elite individual
     **/
    template<typename T, typename L> inline void roulettewheel<T,L>::getElite( const std::size_t& p_start, const std::size_t& p_end, const std::vector< boost::shared_ptr< individual::individual<L> > >& p_population, const ublas::vector<T>& p_fitness, const ublas::vector<std::size_t>&, const ublas::vector<std::size_t>&, std::vector< boost::shared_ptr< individual::individual<L> > >& p_elite )
    {
        // calculate probability
        const T l_max = ublas::sum(p_fitness);
        if (tools::function::isNumericalZero(l_max))
            throw exception::runtime(_("fitness values are all zero"), *this);
        
        ublas::vector<T> l_probability( p_fitness / l_max );
        ublas::vector<std::size_t> l_index( tools::vector::rankIndexVector( l_probability ) );
       
        // get elements
        for(std::size_t i=p_start; i < p_end; ++i) {

            // determine random value
            const T l_rand = m_random.get<T>(tools::random::uniform, 0.0, 1.0);
            
            // determine next element with the probability
            std::size_t n  = 0;
            for( ; (n < l_index.size()) && (l_probability[l_index[n]] <= l_rand ); ++n)
                ;
            n = std::min(n, l_index.size()-1);
            
            p_elite.push_back( p_population[n] );
        }
    }
    
    
}}}
#endif

