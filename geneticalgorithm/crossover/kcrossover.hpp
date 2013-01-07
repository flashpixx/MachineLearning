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


#ifndef __MACHINELEARNING_GENETICALGORITHM_CROSSOVER_KCROSSOVER_HPP
#define __MACHINELEARNING_GENETICALGORITHM_CROSSOVER_KCROSSOVER_HPP

#include <boost/static_assert.hpp>
#include <boost/shared_ptr.hpp>

#include "crossover.hpp"
#include "../individual/individual.hpp"
#include "../../errorhandling/exception.hpp"
#include "../../tools/tools.h"


namespace machinelearning { namespace geneticalgorithm { namespace crossover {

    /** k-crossover algorithm **/
    template<typename T> class kcrossover : public crossover<T>
    {
        public :
        
            kcrossover( const std::size_t& );
        
            void clone( boost::shared_ptr< crossover<T> >& ) const;
            std::size_t getNumberOfIndividuals( void ) const;    
            boost::shared_ptr< individual::individual<T> > combine( void );
            void setIndividual( const boost::shared_ptr< individual::individual<T> >& );
            
            void onEachIteration( const std::vector< boost::shared_ptr< individual::individual<T> > >& ) {}
        
        
        private :
        
            /** random object **/
            tools::random m_random;
            /** cut positions of the elements **/
            const std::size_t m_cuts;
            /** list with elements **/
            std::vector< boost::shared_ptr< individual::individual<T> > > m_individuals;
    };

    
    
    /** constructor
     * @param p_cuts number of cut positions that are used
     **/
    template<typename T> inline kcrossover<T>::kcrossover( const std::size_t& p_cuts ) :
        m_random(),
        m_cuts(p_cuts),
        m_individuals()
    {
        if (p_cuts == 0)
            throw exception::runtime(_("cuts must be greater than zero"), *this);
    }
    
    
    /** method for cloning the object, for using on multithread
     * @param p_ptr smart-pointer object
     **/
    template<typename T> inline void kcrossover<T>::clone( boost::shared_ptr< crossover<T> >& p_ptr ) const
    {
        p_ptr = boost::shared_ptr< crossover<T> >( new kcrossover<T>(m_cuts) );
    }

    
    /** returns the number of elements that are used for the crossover
     * @return elements
     **/
    template<typename T> inline std::size_t kcrossover<T>::getNumberOfIndividuals( void ) const
    {
        return m_cuts+1;
    }     
    
    
    /** sets a individual to the list
     * @param p_individual individual object
     **/
    template<typename T> inline void kcrossover<T>::setIndividual( const boost::shared_ptr< individual::individual<T> >& p_individual )
    {
        m_individuals.push_back( p_individual );
    }
    
    
    /** creates a new child of the elements
     * @note we must not check the ranges, because it will be correct with the constructor set and
     * the population object breaks down until it has added the number of individuals that are resolved 
     * with getNumberOfIndividuals call
     * @return new smart-pointer object with the individual
     **/
    template<typename T> inline boost::shared_ptr< individual::individual<T> > kcrossover<T>::combine( void )
    {
        // we create a new individual and overwrite the parameter data
        boost::shared_ptr< individual::individual<T> > l_new;
        m_individuals[0]->clone( l_new );

        // create crossover parts and add them to the new individual
        std::size_t l_pos = 0;
        for(std::size_t i=0; (i < m_individuals.size()) && (l_pos < l_new->size()); ++i) {
            std::size_t l_old = l_pos;
            l_pos = static_cast<std::size_t>(m_random.get<double>(tools::random::uniform, l_pos+1, l_new->size()+1));
            
            for(std::size_t n=l_old; n < l_pos; ++n) {
                // we need a copy of the value - the [] operator returns a reference so we force the value-copy
                T l_value = (*(m_individuals[i]))[n];
                (*l_new)[n] = l_value;
            }
        }
        
        // after create, we clear the internal list
        m_individuals.clear();
        return l_new;
    }

}}}
#endif
