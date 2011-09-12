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

#include <boost/shared_ptr.hpp>
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
                fullBuildFromElite      = 0,
                removeWorst             = 1,
                replaceRandom           = 2
            };
        
        
            population( const individual&, const std::size_t&, const std::size_t& );
        
            std::size_t size( void ) const;
            void setEliteSize( const std::size_t& );
            std::size_t getEliteSize( void ) const;
            std::vector< individual > getElite( void ) const;
            void setMutalProbability( const T&, const tools::random::distribution& = tools::random::uniform, const T& = std::numeric_limits<T>::epsilon(), const T& = std::numeric_limits<T>::epsilon(), const T& = std::numeric_limits<T>::epsilon() );
            std::vector< individual > getElite( void ) const;
            void setPopulationBuild( const buildoption&, const tools::random::distribution& = tools::random::uniform );
            void iterate( const std::size_t&, const fitnessfunction<T>&, const eliteselection<T>&, const crossover& );
            //bool isConverged( const std::size_t&, const fitnessfunction<T>&, const eliteselection<T>&, const crossover& );
        
        
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
        
            /** vector with smart-pointer of individuals **/
            std::vector< boost::shared_ptr<individual> > m_population;
            /** vector with smart-pointer of elite-individuals **/
            std::vector< boost::shared_ptr<individual> > m_elite;
            /** option in which way the new population is build **/
            buildoption m_buildoption;
            /** mutation probability **/
            probability m_mutateprobility;
            /** boost mutex for running **/
            boost::mutex m_running;
            /** boost mutex for lock during fitting **/
            boost::mutex m_iterationlock;
        
        
            void fitness( const std::size_t&, const std::size_t&, const fitnessfunction<T>, ublas::vector<T>& ) const;
            void mutate( const std::size_t&, const std::size_t& ) const;
            void buildelite( const std::size_t&, const std::size_t&, const eliteselection<T>, const ublas::vector<T>&, const ublas::vector<std::size_t>& );
            void buildpopulation( const std::size_t&, const std::size_t&, const crossover, const ublas::vector<std::size_t>& ) const;
        
    };
    
    
    
    /** constructor
     * @param p_individualref reference to the individual object, that is n-times cloned for the population
     * @param p_size size of the population
     * @param p_elite size of the elites
     **/
    template<typename T> inline population<T>::population( const individual& p_individualref, const std::size_t& p_size, const std::size_t& p_elite ) :
        m_population( p_size ),
        m_elite( p_elite ),
        m_buildoption( 0 ),
        m_mutateprobility(),
        m_running(),
        m_iterationlock()
    {
        if (p_size < 3)
            throw exception::runtime(_("population size must be greater than two"));
        
        if (p_elite < 2)
            throw exception::runtime(_("elite size must be greater than one"));
        
        if (p_elite > p_size)
            throw exception::runtime(_("elite size must be smaller than population size"));
        
        // create individuals
        for(std::size_t i=0; i < p_size; ++i) {
            boost::shared_ptr<individual> l_ptr;
            p_individualref.clone( l_ptr );
            m_population.push_back( l_ptr );
        }
    }
    
    
    /** method for setting population-building-option
     * @param p_opt bool for enabeling the option
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
     * @param p_second second distribution value
     * @param p_third third distribution value
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
     * @return size of the population
     **/
    template<typename T> inline std::size_t population<T>::size( void ) const
    {
        return m_population.size();
    }
    
    
    /** executes the algorithm iteratively
     * @param p_iteration number of iterations
     * @param p_fitness fitness function object
     * @param p_elite elite selection object
     * @param p_crossover crossover object
     **/
    template<typename T> inline void population<T>::iterate( const std::size_t& p_iteration, const fitnessfunction<T>& p_fitness, const eliteselection<T>& p_elite, const crossover& p_crossover )
    {
        if (p_iteration == 0)
            throw exception::runtime(_("iterations must be greater than zero"));
        
        // we set a lock during calculation, because the object can't handle different local states
        boost::unique_lock<boost::mutex> l_lock( m_running );
        
        
        
        // create element ranges of the population and elite
        std::size_t l_inc = m_population.size() / boost::thread::hardware_concurrency();
        std::vector< std::pair<std::size_t, std::size_t> > l_populationparts;
        for( std::size_t i=0; i < m_population.size(); i+= l_inc )
            l_populationparts.push_back( std::pair<std::size_t, std::size_t>(i, i+l_inc) );
        l_populationparts[l_populationparts.size()-1].second += m_population.size() % boost::thread::hardware_concurrency();
        
        l_inc = m_elite.capacity() / boost::thread::hardware_concurrency();
        std::vector< std::pair<std::size_t, std::size_t> > l_eliteparts;
        for( std::size_t i=0; i < m_elite.capacity(); i+= l_inc )
            l_eliteparts.push_back( std::pair<std::size_t, std::size_t>(i, i+l_inc) );
        l_eliteparts[l_eliteparts.size()-1].second += m_elite.capacity() % boost::thread::hardware_concurrency();
        

        
        // run iteration process (each thread group must be recreated on the iteration, because after the join_all() the group is empty)
        for(std::size_t i=0; i < p_iteration; ++i) {
            
            // vector with fitness values and thread object
            ublas::vector<T> l_fitness(m_population.size(), 0);
            boost::thread_group l_threads;
            
            
            
            // create and run fitness threads
            for(std::size_t j=0; j < l_populationparts.size(); ++j)
                l_threads.create_thread(  boost::bind( &population<T>::fitness, this, l_populationparts[j].first, l_populationparts[j].second, p_fitness, boost::ref(l_fitness) )  );
            l_threads.join_all();
                        
            
            // rank the fitness values (not multithread)
            const ublas::vector<std::size_t> l_rank( tools::vector::rankIndexVector(l_fitness) );
        
            
            // create elite multithreaded
            m_elite.clear();
            
            
            // create build new population threads and run
            switch (  ((m_buildoption == fullBuildFromElite) || (m_buildoption == replaceRandom)) ? 0 : 1  ) {
                    
                case 0 :
                    for(std::size_t j=0; j < l_populationparts.size(); ++j)
                        l_threads.create_thread(  boost::bind( &population<T>::buildpopulation, this, l_populationparts[j].first, l_populationparts[j].second, p_crossover, l_rank )  );
                    break;
                    
                case 1 :
                    for(std::size_t j=0; j < l_eliteparts.size(); ++j)
                        l_threads.create_thread(  boost::bind( &population<T>::buildpopulation, this, l_eliteparts[j].first, l_eliteparts[j].second, p_crossover, l_rank )  );
                    break;
            }
            l_threads.join_all();
            
            
            // create and run mutation threads
            for(std::size_t j=0; j < l_populationparts.size(); ++j)
                l_threads.create_thread(  boost::bind( &population<T>::mutate, this, l_populationparts[j].first, l_populationparts[j].second )  );
            l_threads.join_all();
        }
        
    }
    
    
    /** multithread method for calculating fitness values (start & end values must be disjuct over all threads)
     * @param p_start start value of the population values
     * @param p_end end value of the population values
     * @param p_fitnessfunction fitness function object
     * @param p_fitness reference to the fitness vector
     **/
    template<typename T> inline void population<T>::fitness( const std::size_t& p_start, const std::size_t& p_end, const fitnessfunction<T> p_fitnessfunction, ublas::vector<T>& p_fitness ) const
    {
        for(std::size_t i=0; i < p_end; ++i)
            p_fitness(i) = m_population[i] ? p_fitnessfunction.getFitness( *m_population[i] ) : 0;
    }
    
    
    /** multithread method for mutating some population elements (start & end values must be disjuct over all threads)
     * @param p_start start value of the population values
     * @param p_end end value of the population values
     **/
    template<typename T> inline void population<T>::mutate( const std::size_t& p_start, const std::size_t& p_end ) const
    {
        tools::random l_rand;
        for(std::size_t i=0; i < p_end; ++i)
            if (l_rand.get<T>( m_mutateprobility.distribution, m_mutateprobility.first, m_mutateprobility.second, m_mutateprobility.third ) <= m_mutateprobility.probability)
                (*m_population[i]).mutate();
    }
    
    
    /** multithread method for building new population elements (start & end values must be disjuct over all threads)
     * @param p_start start value of the population / elite values
     * @param p_end end value of the population  / elite 
     * @param p_crossover crossover function object
     * @param p_rank ublas vector with rank values of all population elements
     **/
    template<typename T> inline void population<T>::buildpopulation( const std::size_t& p_start, const std::size_t& p_end, const crossover p_crossover, const ublas::vector<std::size_t>& p_rank ) const
    {
        tools::random l_rand;
        switch (m_buildoption) {
                
            case fullBuildFromElite :
                for(std::size_t i=p_start; i < p_end; ++i) {
                    for(std::size_t j=0; j < p_crossover.getNumberOfIndividuals(); ++j)
                        p_crossover.setIndividual( *m_elite[static_cast<std::size_t>(l_rand.get<T>(tools::random::uniform, 0, m_elite.size()))] );
                    
                    m_population[i] = p_crossover.combine();
                }
                break;
                
                
            case removeWorst :
                for(std::size_t i=p_start; i < p_end; ++i) {
                    for(std::size_t j=0; j < p_crossover.getNumberOfIndividuals(); ++j)
                        p_crossover.setIndividual( *m_elite[static_cast<std::size_t>(l_rand.get<T>(tools::random::uniform, 0, m_elite.size()))] );

                    m_population[p_rank(i)] = p_crossover.combine();
                }
                break;
                
                
            case replaceRandom :
                for(std::size_t i=p_start; i < p_end; ++i) {
                    for(std::size_t j=0; j < p_crossover.getNumberOfIndividuals(); ++j)
                        p_crossover.setIndividual( *m_elite[static_cast<std::size_t>(l_rand.get<T>(tools::random::uniform, 0, m_elite.size()))] );
                    
                    m_population[static_cast<std::size_t>(l_rand.get<T>(tools::random::uniform, 0, m_population.size()))] = p_crossover.combine();
                }
                break;
        }
    }
    
    
    /*
     // determine elite values and create a local copy of the elements
     m_elite.clear();
     std::vector< individual* > l_elite = ;
     for(std::size_t j=0; j < l_elite.size(); ++j)
     if (l_elite[j])
     m_elite.push_back( *l_elite[j] );
     */
    
    
    template<typename T> inline void population<T>::buildelite( const std::size_t& p_start, const std::size_t& p_end, const eliteselection<T> p_eliteselection, const ublas::vector<T>& p_fitness, const ublas::vector<std::size_t>& p_rank )
    {
        //p_eliteselection.getElite( p_start, p_end, m_population, p_fitness, p_rank, m_elite.capacity() );
        
        // we need a mutex, because different threads can modify the same pointer structure (vector of elite pointer and copy elite data)
        //boost::unique_lock<boost::mutex> l_lock( m_iterationlock );
    }
    
    
    
};};

#endif