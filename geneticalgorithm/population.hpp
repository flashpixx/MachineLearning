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


#ifndef MACHINELEARNING_GENETICALGORITHM_POPULATION_HPP
#define MACHINELEARNING_GENETICALGORITHM_POPULATION_HPP

#include <boost/numeric/ublas/vector.hpp>

#include "../exception/exception.h"
#include "../tools/tools.h"

#include "individual.hpp"
#include "fitnessfunction.hpp"
#include "eliteselection.hpp"


namespace machinelearning { namespace geneticalgorithm {
    
    namespace ublas = boost::numeric::ublas;
    

    /** class for the population / optimization structure
     * $LastChangedDate$
     **/
    template<typename T> class population {
        BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
        
        public :
        
            enum buildoption {
                overwriteEliteWithNew = 0,
                useEliteAndNew        = 1,
                fullNewBuild          = 2
            };
        
        
            population( const individual<T>&, const std::size_t&, const std::size_t&, const T& );
            ~population( void );
        
        
            std::size_t size( void ) const;
            void setEliteSize( const std::size_t& );
            std::size_t getEliteSize( void ) const;
            std::vector< individual<T> > getElite( void ) const;
            void setMutalProbability( const T& );
            T getMutalProbability( void ) const;
            std::vector< individual<T> > getElite( void ) const;
            void setPopulationBuild( const buildoption& );
            buildoption getPopulationBuild( void ) const;
            void iterate( const fitnessfunction<T>&, const eliteselection<T>&, const std::size_t&, const tools::random::distribution& = tools::random::uniform );
            //bool isConverged( const fitnessfunction&, const eliteselection<T>& );
        
        
        private :
      
            /** option in which way the new population is build **/
            buildoption m_buildoption;
            /** mutation probability **/
            T m_mutateprobility;
            /** pointer vector with elite individuals **/
            std::vector< individual<T>* > m_elite;
            /** pointer vector with individuals **/
            std::vector< individual<T>* > m_population;
        
        
    };
    
    
    
    /** constructor
     * @param p_individualref reference to the individual object, that is n-times cloned for the population
     * @param p_size size of the population
     * @param p_elite size of the elites
     * @param p_mutate mutation probility
     **/
    template<typename T> inline population<T>::population( const individual<T>& p_individualref, const std::size_t& p_size, const std::size_t& p_elite, const T& p_mutate ) :
        m_buildoption( fullNewBuild ),
        m_mutateprobility( p_mutate ),
        m_parentdie( true ),
        m_elite( p_elite ),
        m_population(p_size)
    {
        if (p_size < 3)
            throw exception::runtime(_("population size must be greater than two"));
        
        if (m_elite < 2)
            throw exception::runtime(_("elite size must be greater than one"));
        
        if (m_elite > p_size)
            throw exception::runtime(_("elite size must be smaller than population size"));
        
        if ( !((m_mutateprobility >= 0) && (m_mutateprobility <= 1)) )
            throw exception::runtime(_("mutation probility must be between zero and one"));
        
        // create individuals
        for(std::size_t i=0; i < p_size; ++i)
            m_population.push_back( p_individualref.clone() );
    }
    
    
    /** destructor for clearing population **/
    template<typename T> inline population<T>::~population( void )
    {
        for(std::size_t i=0; i < m_population.size(); ++i)
            delete( m_population[i] );
    }
    
    
    /** method for setting population-building-option
     * @param p_die bool for enabeling the option
     **/
    template<typename T> inline void population<T>::setPopulationBuild( const buildoption& p_opt )
    {
        m_buildoption = p_opt;
    }
    
    
    /** returns the value oft the population-building-option
     * @return bool
     **/
    template<typename T> inline population<T>::buildoption population<T>::getPopulationBuild( void ) const
    {
        return m_buildoption;
    }
    
    
    /** change the elite size
     * @param p_size size number
     **/
    template<typename T> inline void population<T>::setEliteSize( const std::size_t& p_size )
    {
        if (p_size < 2)
            throw exception::runtime(_("elite size must be greater than one"));
        
        if (p_size >= m_population.capacity())
            throw exception::runtime(_("elite size must be smaller than population size"));
        
        m_elite.resize(p_size);
    }
    
    
    /** returns the elite size
     * @return size of elite
     **/
    template<typename T> inline std::size_t population<T>::getEliteSize( void ) const
    {
        return m_elite.capacity();
    }
    
    
    /** returns a copy of the individuals objects of the elites
     * @return vector with individual objects
     **/
    template<typename T> inline std::vector< individual<T> > population<T>::getElite( void ) const
    {
        std::vector< individual<T> > l_elite;
        
        for(std::size_t i=0; i < m_elite.size(); ++i)
            l_elite.push_back( &m_elite[i] );
        
        return l_elite;
    }
    
    
    /** changes the mutal probability
     * @param p_prop probility
     **/
    template<typename T> inline void population<T>::setMutalProbability( const T& p_prop )
    {
        if ( !((p_prop >= 0) && (p_prop <= 1)) )
            throw exception::runtime(_("mutation probility must be between zero and one"));
        
        m_mutateprobility = p_prop;
    }
    
    
    /** returns the mutal probability
     * @return probability
     **/
    template<typename T> inline T population<T>::getMutalProbability( void ) const
    {
        return m_mutateprobility;
    }
        

    /** returns the population size
     * @param size of the population
     **/
    template<typename T> inline std::size_t population<T>::size( void ) const
    {
        return m_population.capacity();
    }
    
    
    /** executes the algorithm iteratively
     * @param p_fitness fitness function object
     * @param p_elite elite selection object
     * @param p_iteration number of iterations
     * @param p_distribution distribution of the mutation and build (if full build is used) probability [default uniform distribution]
     **/
    template<typename T> inline void population<T>::iterate( const fitnessfunction<T>& p_fitness, const eliteselection<T>& p_elite, const std::size_t& p_iteration, const tools::random::distribution& p_distribution )
    {
        if (p_iteration == 0)
            throw exception::runtime(_("iterations must be greater than zero"));
        
        
        ublas::vector<T> l_fitness(m_population.size(), 0);
        tools::random l_rand;
        
        for(std::size_t i=0; i < p_iteration; ++i)
        {
            // determin the fitness value for each individual
            for(std::size_t j=0; j < m_population.size(); ++j)
                l_fitness(j) = m_population[j] ? p_fitness.getFitness( &m_population[j] ) : 0;
            
            // determine elite values
            m_elite = p_elite.getElite( m_population, l_fitness, m_elite.capacity() );
            
            // run over the new population and mutate some individuals
            for(std::size_t j=0; j < m_population.size(); ++j)
                if (l_rand.get<T>( p_distribution, 0, 1 ) <= m_mutateprobility)
                    m_population[j]->mutate();
        }
    
    }
    
    
    
};};

#endif