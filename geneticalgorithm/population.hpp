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


#ifndef __MACHINELEARNING_GENETICALGORITHM_POPULATION_HPP
#define __MACHINELEARNING_GENETICALGORITHM_POPULATION_HPP

#include <omp.h>
#include <limits>
#include <boost/numeric/ublas/vector.hpp>

#include <boost/shared_ptr.hpp>

#include "../errorhandling/exception.hpp"
#include "../tools/tools.h"

#include "individual/individual.h"
#include "crossover/crossover.h"
#include "selection/selection.h"
#include "fitness/fitness.h"


namespace machinelearning { namespace geneticalgorithm {
    
    namespace ublas = boost::numeric::ublas;
    

    /** class for the population / optimization structure
     * @todo check memory allocation and clearing on big datasets
     **/
    template<typename T, typename L> class population
    {
        BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
        
        public :
        
            enum buildoption {
                eliteonly      = 0,
                steadystates   = 1,
                random         = 2
            };
        
        
            population( const individual::individual<L>&, const std::size_t&, const std::size_t& );
        
            std::size_t size( void ) const;
            void setEliteSize( const std::size_t& );
            std::size_t getEliteSize( void ) const;
            std::vector< boost::shared_ptr< individual::individual<L> > > getElite( void ) const;
            void setMutalProbability( const T&, const tools::random::distribution& = tools::random::uniform, const T& = std::numeric_limits<T>::epsilon(), const T& = std::numeric_limits<T>::epsilon(), const T& = std::numeric_limits<T>::epsilon() );
            void setPopulationBuild( const buildoption&, const tools::random::distribution& = tools::random::uniform );
            void iterate( const std::size_t&, fitness::fitness<T,L>&, selection::selection<T,L>&, crossover::crossover<L>& );
            //bool iterateUntilConverged( const std::size_t&, const fitness::fitness<T>&, const selection::selection<T>&, const crossover& );
        
        
        private :
        
            /** struct of the probabilities **/
            struct probability {
                tools::random::distribution distribution;
                T probabilityvalue;
                T first;
                T second;
                T third;
                                
                probability() : 
                    distribution(tools::random::uniform),  
                    probabilityvalue( 0.4 ), 
                    first( 0.0 ),
                    second( 1.0 ),
                    third( std::numeric_limits<T>::epsilon() )
                {}
            };
        
            /** reference of the individual **/
            const individual::individual<L>& m_individualref;
            /** vector with smart-pointer of individuals **/
            std::vector< boost::shared_ptr< individual::individual<L> > > m_population;
            /** vector with smart-pointer of elite-individuals **/
            std::vector< boost::shared_ptr< individual::individual<L> > > m_elite;
            /** option in which way the new population is build **/
            buildoption m_buildoption;
            /** mutation probability **/
            probability m_mutateprobility;
            /** elite size **/
            std::size_t m_elitesize;
    };
    
    
    
    /** constructor
     * @param p_individualref reference to the individual object, that is n-times cloned for the population
     * @param p_size size of the population
     * @param p_elite size of the elites
     **/
    template<typename T, typename L> inline population<T,L>::population( const individual::individual<L>& p_individualref, const std::size_t& p_size, const std::size_t& p_elite ) :
        m_individualref( p_individualref ), 
        m_population(),
        m_elite(),
        m_buildoption( eliteonly ),
        m_mutateprobility(),
        m_elitesize(p_elite)
    {
        if (p_size < 3)
            throw exception::runtime(_("population size must be greater than two"), *this);
        
        if (p_elite < 2)
            throw exception::runtime(_("elite size must be greater than one"), *this);
        
        if (p_elite >= p_size)
            throw exception::runtime(_("elite size must be smaller than population size"), *this);
        
        // create individuals
        for(std::size_t i=0; i < p_size; ++i) {
            boost::shared_ptr< individual::individual<L> > l_ptr;
            m_individualref.clone( l_ptr );
            m_population.push_back( l_ptr );
        }
    }
    
    
    /** method for setting population-building-option
     * @param p_opt build option
     * @param p_distribution distribution 
     **/
    template<typename T, typename L> inline void population<T,L>::setPopulationBuild( const buildoption& p_opt, const tools::random::distribution& p_distribution )
    {
        m_buildoption = p_opt;
    }
    
    
    /** change the elite size
     * @param p_size size number
     **/
    template<typename T, typename L> inline void population<T,L>::setEliteSize( const std::size_t& p_size )
    {
        if (p_size < 2)
            throw exception::runtime(_("elite size must be greater than one"), *this);
        
        if (p_size >= m_population.size())
            throw exception::runtime(_("elite size must be smaller than population size"), *this);
        
        m_elitesize = p_size;
    }
    
    
    /** returns the elite size
     * @return size of elite
     **/
    template<typename T, typename L> inline std::size_t population<T,L>::getEliteSize( void ) const
    {
        return m_elitesize;
    }
    
    
    /** returns a copy of the elite individuals 
     * @return vector with smart-pointer objects of the elite individuals
     **/
    template<typename T, typename L> inline std::vector< boost::shared_ptr< individual::individual<L> > > population<T,L>::getElite( void ) const
    {
        std::vector< boost::shared_ptr< individual::individual<L> > > l_result;
        
        for(std::size_t i=0; i < m_elite.size(); ++i) {
            boost::shared_ptr< individual::individual<L> > l_ind;
            m_individualref.clone( l_ind );
            
            if (l_ind->size() != m_elite[i]->size())
                throw exception::runtime(_("element sizes are not equal"), *this);
        
            for(std::size_t n=0; n < m_elite[i]->size(); ++n) {
                // we need a copy of the value - the [] operator returns a reference so we force the value-copy
                T l_value = (*(m_elite[i]))[n];
                (*l_ind)[n] = l_value;
            }
            l_result.push_back( l_ind );
        }
        
        return l_result;
    }
    
    
    /** changes the mutal probability
     * @param p_prop probility
     * @param p_distribution distribution
     * @param p_first first distribution value
     * @param p_second second distribution value
     * @param p_third third distribution value
     **/
    template<typename T, typename L> inline void population<T,L>::setMutalProbability( const T& p_prop, const tools::random::distribution& p_distribution, const T& p_first, const T& p_second, const T& p_third )
    {
        m_mutateprobility.distribution      = p_distribution;
        m_mutateprobility.probabilityvalue  = p_prop;
        m_mutateprobility.first             = p_first;
        m_mutateprobility.second            = p_second;
        m_mutateprobility.third             = p_third;
    }
        

    /** returns the population size
     * @return size of the population
     **/
    template<typename T, typename L> inline std::size_t population<T,L>::size( void ) const
    {
        return m_population.size();
    }
    
    
    /** executes the algorithm iteratively
     * @param p_iteration number of iterations
     * @param p_fitness fitness function object
     * @param p_elite elite selection object
     * @param p_crossover crossover object
     **/
    template<typename T, typename L> inline void population<T,L>::iterate( const std::size_t& p_iteration, fitness::fitness<T,L>& p_fitness, selection::selection<T,L>& p_elite, crossover::crossover<L>& p_crossover )
    {
        if (p_iteration == 0)
            throw exception::runtime(_("iterations must be greater than zero"), *this);
        
        // create a map with the parts of the elites that are build on each thread (eg: elites [0,k) on thread 0, [k,n) on thread 1, ...)
        std::size_t l_inc = m_elitesize / omp_get_max_threads();
        l_inc = l_inc == 0 ? m_elitesize : l_inc;
        std::vector< std::pair<std::size_t, std::size_t> > l_eliteparts;
        for( std::size_t i=0; i < m_elitesize-1; i+=l_inc )
            l_eliteparts.push_back( std::pair<std::size_t, std::size_t>(i, i+l_inc) );
        
        if (l_eliteparts.size() == 0)
            l_eliteparts.push_back( std::pair<std::size_t, std::size_t>(0, m_elitesize) );
        else
            l_eliteparts[l_eliteparts.size()-1].second += m_elitesize % omp_get_max_threads();
        l_eliteparts[l_eliteparts.size()-1].second = std::min( l_eliteparts[l_eliteparts.size()-1].second, m_elitesize );

        // create local random generator
        tools::random l_random;
        
        
        // run iteration process (each thread group must be recreated on the iteration, because after the join_all() the threads are "out-of-range")
        for(std::size_t i=0; i < p_iteration; ++i) {
            
            // vector with fitness values and thread object
            ublas::vector<T> l_fitness(m_population.size(), 0);
            
            // create and run fitness threads
            bool l_optimumreached = false;
            
            // OpenMP can't break the thread loop, so we run over all elements within the population
            // and if the optimum is reached we don't break the loop
            #pragma omp parallel shared(l_optimumreached, l_fitness)
            {
                boost::shared_ptr< fitness::fitness<T,L> > l_fitnessfunction;
                p_fitness.clone( l_fitnessfunction );
            
                #pragma omp for
                for(std::size_t i=0; i < m_population.size(); ++i) {
                    l_fitness(i) = l_fitnessfunction->getFitness( *m_population[i] );
                    
                    if (l_fitnessfunction->isOptimumReached())
                        #pragma omp critical
                        l_optimumreached = true;
                }
            }

        
            // scales the fitness values to [0,x]
            const T l_min = tools::vector::min( l_fitness );
            
            // scales the fitness values to zero
            #pragma omp parallel for shared(l_fitness)
            for(std::size_t i=0; i < l_fitness.size(); ++i)
                l_fitness(i) -= l_min;

             
            // rank the fitness values (not multithread)
            const ublas::vector<std::size_t> l_rankIndex( tools::vector::rankIndexVector(l_fitness) );
            const ublas::vector<std::size_t> l_rank( tools::vector::rank(l_fitness) );

            
            
            // create elite multithreaded
            m_elite.clear();
            
            #pragma omp parallel shared(l_fitness)
            {
                boost::shared_ptr< selection::selection<T,L> > l_selection;
                p_elite.clone( l_selection );
                
                std::vector< boost::shared_ptr< individual::individual<L> > > l_elite;
                l_selection->getElite(l_eliteparts[omp_get_thread_num()].first, l_eliteparts[omp_get_thread_num()].second, m_population, l_fitness, l_rankIndex, l_rank, l_elite);

                if (l_elite.size() > 0)
                    #pragma omp critical
                    std::copy( l_elite.begin(), l_elite.end(), std::back_inserter(m_elite));
            }
            
            // updateing elite size and break if optimum is found
            m_elitesize = m_elite.size();
            if (l_optimumreached)
                break;

            
            // build the new population
            switch (m_buildoption) {
                    
                case eliteonly :
                    #pragma omp parallel shared(l_random)
                    {
                        boost::shared_ptr< crossover::crossover<L> > l_crossover;
                        p_crossover.clone( l_crossover );
                
                        #pragma omp for 
                        for(std::size_t i=0; i < m_population.size(); ++i) {
                            for(std::size_t j=0; j < l_crossover->getNumberOfIndividuals(); ++j)
                                l_crossover->setIndividual( m_elite[static_cast<std::size_t>(l_random.get<T>(tools::random::uniform, 0, m_elite.size()))] );
                        
                            m_population[i] = l_crossover->combine();
                        }
                    }
                    break;
                    
                    
                case steadystates :
                    #pragma omp parallel shared(l_random)
                    {
                        boost::shared_ptr< crossover::crossover<L> > l_crossover;
                        p_crossover.clone( l_crossover );
                        
                        #pragma omp for 
                        for(std::size_t i=0; i < m_elite.size(); ++i) {
                            for(std::size_t j=0; j < l_crossover->getNumberOfIndividuals(); ++j)
                                l_crossover->setIndividual( m_elite[static_cast<std::size_t>(l_random.get<T>(tools::random::uniform, 0, m_elite.size()))] );
                            
                            m_population[l_rankIndex(i)] = l_crossover->combine();
                        }
                    }
                    break;
                    
                    
                case random :
                    #pragma omp parallel shared(l_random)
                    {
                        boost::shared_ptr< crossover::crossover<L> > l_crossover;
                        p_crossover.clone( l_crossover );
                        
                        #pragma omp for
                        for(std::size_t i=0; i < m_population.size(); ++i) {
                            for(std::size_t j=0; j < l_crossover->getNumberOfIndividuals(); ++j)
                                l_crossover->setIndividual( m_elite[static_cast<std::size_t>(l_random.get<T>(tools::random::uniform, 0, m_elite.size()))] );
                            
                            #pragma omp critical
                            m_population[static_cast<std::size_t>(l_random.get<T>(tools::random::uniform, 0, m_elite.size()))] = l_crossover->combine();
                        }
                    }
                    break;
            }
            
            
            // create and run mutation threads
            #pragma omp parallel for shared(l_random)
            for(std::size_t i=0; i < m_population.size(); ++i)
                if (l_random.get<T>( m_mutateprobility.distribution, m_mutateprobility.first, m_mutateprobility.second, m_mutateprobility.third ) <= m_mutateprobility.probabilityvalue)
                    m_population[i]->mutate();
            
            // call the "eachIteration" method of each object for updating local object properties (not multithreaded, because of synchronization)
            p_fitness.onEachIteration( m_population );
            p_elite.onEachIteration( m_population );
            p_crossover.onEachIteration( m_population );
        }
    }
    
    
}}
#endif
