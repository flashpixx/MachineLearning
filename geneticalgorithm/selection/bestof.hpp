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


#ifndef __MACHINELEARNING_GENETICALGORITHM_SELECTION_BESTOF_HPP
#define __MACHINELEARNING_GENETICALGORITHM_SELECTION_BESTOF_HPP

#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "selection.hpp"
#include "../individual/individual.hpp"
#include "../../tools/tools.h"



namespace machinelearning { namespace geneticalgorithm { namespace selection {
    
    
    namespace ublas = boost::numeric::ublas;
    
    
    /** class of the bestof-selection. uses the n best elements of the population **/
    template<typename T, typename L> class bestof : public selection<T,L>
    {
        BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
        
        public :
        
            bestof( const std::size_t& );
        
            void clone( boost::shared_ptr< selection<T,L> >& p_ptr ) const;
            void getElite( const std::size_t&, const std::size_t&, const std::vector< boost::shared_ptr< individual::individual<L> > >&, const ublas::vector<T>&, const ublas::vector<std::size_t>&, const ublas::vector<std::size_t>&,std::vector< boost::shared_ptr< individual::individual<L> > >&  );
        
            void onEachIteration( const std::vector< boost::shared_ptr< individual::individual<L> > >& ) {}
        
        
        private :
        
            /** number of elements **/
            const std::size_t m_number;
        
    };
    
    
    
    /** constructor
     * @param p_num number of elements that should be used
     **/
    template<typename T, typename L> inline bestof<T,L>::bestof( const std::size_t& p_num ) :
        m_number( p_num )
    {
        if (p_num == 0)
            throw exception::runtime(_("number must be greater than zero"), *this);
    }
    
    
    /** method for cloning the object, for using on multithread
     * @param p_ptr smart-pointer object
     **/
    template<typename T, typename L> inline void bestof<T,L>::clone( boost::shared_ptr< selection<T,L> >& p_ptr ) const
    {
        p_ptr = boost::shared_ptr< selection<T,L> >( new bestof<T,L>(m_number) );
    }
    
    
    /** returns the first n elites
     * @param p_start start value of the elite values
     * @param p_end end value of the elite values ([start, end) elite elements must be created)
     * @param p_population const reference to the population
     * @param p_rankIndex rank index (first index has the position of the population element, that has the smalles fitness value)
     * @param p_elite vector with elite individual
     **/
    template<typename T, typename L> inline void bestof<T,L>::getElite( const std::size_t& p_start, const std::size_t& p_end, const std::vector< boost::shared_ptr< individual::individual<L> > >& p_population, const ublas::vector<T>&, const ublas::vector<std::size_t>& p_rankIndex, const ublas::vector<std::size_t>&, std::vector< boost::shared_ptr< individual::individual<L> > >& p_elite )
    {
        const std::size_t l_end = std::min(p_end, m_number);

        std::size_t n = p_start;
        for(std::size_t i=p_start; i < p_end; ++i) {
            p_elite.push_back( p_population[p_rankIndex[p_rankIndex.size()-1-n]] );
            n++;
            if (n >= l_end)
                n = p_start;
        }
    }
    
    
}}}
#endif

