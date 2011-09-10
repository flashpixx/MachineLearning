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

#include <limits>
#include <boost/numeric/ublas/vector.hpp>

#include <boost/thread.hpp>
#include <boost/ref.hpp>

#include "../exception/exception.h"
#include "../tools/tools.h"

#include "individual.h"
#include "crossover.h"
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
                fullBuildFromElite    = 0,
                overwriteEliteWithNew = 1,
                useEliteAndNewOnes    = 2
            };
        
        
            population( const individual&, const std::size_t&, const std::size_t& );
            ~population( void );
        
        
            std::size_t size( void ) const;
            void setEliteSize( const std::size_t& );
            std::size_t getEliteSize( void ) const;
            std::vector< individual > getElite( void ) const;
            void setMutalProbability( const T&, const tools::random::distribution& = tools::random::uniform, const T& = std::numeric_limits<T>::epsilon(), const T& = std::numeric_limits<T>::epsilon(), const T& = std::numeric_limits<T>::epsilon() );
            std::vector< individual > getElite( void ) const;
            void setPopulationBuild( const buildoption&, const tools::random::distribution& = tools::random::uniform );
            void iterate( const fitnessfunction<T>&, const eliteselection<T>&, const crossover&, const std::size_t& );
            //bool isConverged( const fitnessfunction&, const eliteselection<T>& );
        
        
        private :
        
            /** struct of the probabilities **/
            struct probability {
                tools::random::distribution distribution;
                T probability;
                T first;
                T second;
                T third;
                                
                probability() : 
                    distribution(tools::random::uniform),  
                    probability(0.5), 
                    first( std::numeric_limits<T>::epsilon() ),
                    second( std::numeric_limits<T>::epsilon() ),
                    third( std::numeric_limits<T>::epsilon() )
                {}
            };
        
      
            /** option in which way the new population is build **/
            buildoption m_buildoption;
            /** mutation probability **/
            probability m_mutateprobility;
            /** vector with elite individuals **/
            std::vector< individual > m_elite;
            /** pointer vector with individuals **/
            std::vector< individual* > m_population;
            /** boost mutex for running **/
            boost::mutex m_running;
        
    };
    
    
    
    /** constructor
     * @param p_individualref reference to the individual object, that is n-times cloned for the population
     * @param p_size size of the population
     * @param p_elite size of the elites
     **/
    template<typename T> inline population<T>::population( const individual& p_individualref, const std::size_t& p_size, const std::size_t& p_elite ) :
        m_buildoption( 0 ),
        m_mutateprobility(),
        m_elite( p_elite ),
        m_population(p_size),
        m_running()
    {
        if (p_size < 3)
            throw exception::runtime(_("population size must be greater than two"));
        
        if (p_elite < 2)
            throw exception::runtime(_("elite size must be greater than one"));
        
        if (p_elite > p_size)
            throw exception::runtime(_("elite size must be smaller than population size"));
        
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
     * @param p_distribution distribution 
     **/
    template<typename T> inline void population<T>::setPopulationBuild( const buildoption& p_opt, const tools::random::distribution& p_distribution )
    {
        m_buildoption = p_opt;
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
    template<typename T> inline std::vector< individual > population<T>::getElite( void ) const
    {
        return m_elite;
    }
    
    
    /** changes the mutal probability
     * @param p_prop probility
     * @param p_distribution distribution
     * @param p_first first distribution value
     **/
    template<typename T> inline void population<T>::setMutalProbability( const T& p_prop, const tools::random::distribution& p_distribution, const T& p_first, const T& p_second, const T& p_third )
    {
        m_mutateprobility.distribution = p_distribution;
        m_mutateprobility.probability  = p_prop;
        m_mutateprobility.first        = p_first;
        m_mutateprobility.second       = p_second;
        m_mutateprobility.third        = p_third;
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
     **/
    template<typename T> inline void population<T>::iterate( const fitnessfunction<T>& p_fitness, const eliteselection<T>& p_elite, const crossover& p_crossover, const std::size_t& p_iteration )
    {
        if (p_iteration == 0)
            throw exception::runtime(_("iterations must be greater than zero"));
        
        
        if (boost::thread::hardware_concurrency() > 1) {
            // we set a lock during calculation, because the object can't handle different local states
            boost::unique_lock<boost::mutex> l_lock( m_running );
            
        } else {
            tools::random l_rand;
            for(std::size_t i=0; i < p_iteration; ++i)
            {
                // determin the fitness value for each individual
                ublas::vector<T> l_fitness(m_population.size(), 0);
                for(std::size_t j=0; j < m_population.size(); ++j)
                    l_fitness(j) = m_population[j] ? p_fitness.getFitness( &m_population[j] ) : 0;
                
                // rank the fitness values
                const ublas::vector<std::size_t> l_rank(tools::vector::rankIndexVector(l_fitness));
                
                // determine elite values and create a local copy of the elements
                m_elite.clear();
                std::vector< individual* > l_elite = p_elite.getElite( m_population, l_fitness, l_rank, m_elite.capacity() );
                for(std::size_t j=0; j < l_elite.size(); ++j)
                    if (l_elite[j])
                        m_elite.push_back( *l_elite[j] );
                
                
                // create new individuals (read two individuals with uniform distribution and combine)
                switch (m_buildoption) {
                        
                    case fullBuildFromElite :
                        for(std::size_t j=0; j < m_population.size(); ++j) {
                            delete( m_population[j] );
                            
                            for(std::size_t j=0; j < p_crossover.getNumberOfIndividuals(); ++j)
                                p_crossover.setIndividual( m_elite[static_cast<std::size_t>(l_rand.get<T>(tools::random::uniform, 0, m_elite.size()))] );
                            p_crossover.create(m_population[j]);
                        }
                        break;

                        
                    case overwriteEliteWithNew :
                        for(std::size_t j=0; j < m_elite.size(); ++j) {
                            delete(l_elite[j]);
                            
                            for(std::size_t j=0; j < p_crossover.getNumberOfIndividuals(); ++j)
                                p_crossover.setIndividual( m_elite[static_cast<std::size_t>(l_rand.get<T>(tools::random::uniform, 0, m_elite.size()))] );
                            p_crossover.create(l_elite[j]);
                        }
                        break;
                        
                        
                    case useEliteAndNewOnes :
                        for(std::size_t j=0; j < m_elite.size(); ++j) {
                            delete(m_population[l_rank[j]]);
                            
                            for(std::size_t j=0; j < p_crossover.getNumberOfIndividuals(); ++j)
                                p_crossover.setIndividual( m_elite[static_cast<std::size_t>(l_rand.get<T>(tools::random::uniform, 0, m_elite.size()))] );
                            p_crossover.create(m_population[l_rank[j]]);
                        }
                        break;
                    
                }
                
                // run over the new population and mutate some individuals
                for(std::size_t j=0; j < m_population.size(); ++j)
                    if (l_rand.get<T>( m_mutateprobility.distribution, m_mutateprobility.first, m_mutateprobility.second, m_mutateprobility.third ) <= m_mutateprobility.probability)
                        m_population[j]->mutate();
            }
        }
    
    }
    
    
    
};};

#endif