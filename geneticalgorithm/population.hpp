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


#ifndef MACHINELEARNING_GENETICALGORITHM_POPULATION_H
#define MACHINELEARNING_GENETICALGORITHM_POPULATION_H

#include "individual.hpp"
#include "fitnessfunction.hpp"

#include "../exception/exception.h"


namespace machinelearning { namespace geneticalgorithm {

    /** class for the population / optimization structure
     * $LastChangedDate: 2011-09-06 21:32:01 +0200 (Di, 06 Sep 2011) $
     **/
    template<typename T> class population {
        BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
        
        public :
        
            population( const individual<T>&, const std::size_t&, const std::size_t&, const T& );
        
            std::size_t size( void ) const;
            void setEliteSize( const std::size_t& );
            void setMutalProbability( const T& );
            //std::vector< individual<T> > getElite( void ) const;
            void setParentsDie( const bool& );
            bool getParentDie( void ) const;
            //void iterate( const fitnessfunction&, const std::size_t& );
            //bool isConverged( const fitnessfunction& );
        
        
        private :
      
            T m_mutateprobility;
            bool m_parentdie;
            std::size_t m_elitesize;
            std::size_t m_populationsize;
            individual<T>* m_population;
        
        
    };
    
    
    
    
    template<typename T> inline population<T>::population( const individual<T>& p_individualref, const std::size_t& p_size, const std::size_t& p_elite, const T& p_mutate ) :
        m_mutateprobility( p_mutate ),
        m_parentdie( true ),
        m_elitesize( p_elite ),
        m_populationsize( p_size ),
        m_population( static_cast< individual<T>* >(calloc(p_size,sizeof(p_individualref))) )
    {
        if (m_elitesize < 2)
            throw exception::runtime(_("elite size must be greater than one"));
        
        if (m_elitesize > m_populationsize)
            throw exception::runtime(_("elite size must be smaller than population size"));
        
        if ( !((m_mutateprobility >= 0) && (m_mutateprobility <= 1)) )
            throw exception::runtime(_("mutation probility must be between zero and one"));
    }
    
    
    template<typename T> inline void population<T>::setParentsDie( const bool& p_die )
    {
        m_parentdie = p_die;
    }
    
    
    template<typename T> inline bool population<T>::getParentDie( void ) const
    {
        return m_parentdie;
    }
    
    
    template<typename T> inline void population<T>::setEliteSize( const std::size_t& p_size )
    {
        if (p_size < 2)
            throw exception::runtime(_("elite size must be greater than one"));
        
        if (m_elitesize > m_populationsize)
            throw exception::runtime(_("elite size must be smaller than population size"));
        
        m_elitesize = p_size;
    }
    
    
    template<typename T> inline void population<T>::setMutalProbability( const T& p_prop )
    {
        if ( !((p_prop >= 0) && (p_prop <= 1)) )
            throw exception::runtime(_("mutation probility must be between zero and one"));
        
        m_mutateprobility = p_prop;
    }
        

    template<typename T> inline std::size_t population<T>::size( void ) const
    {
        return m_populationsize;
    }
    
};};

#endif