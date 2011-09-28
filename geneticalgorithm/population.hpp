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


#ifndef __MACHINELEARNING_GENETICALGORITHM_POPULATION_HPP
#define __MACHINELEARNING_GENETICALGORITHM_POPULATION_HPP

#include <limits>
#include <boost/numeric/ublas/vector.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/ref.hpp>

#include "../exception/exception.h"
#include "../tools/tools.h"

#include "individual/individual.h"
#include "crossover/crossover.h"
#include "selection/selection.h"
#include "fitness/fitness.h"


namespace machinelearning { namespace geneticalgorithm {
    
    namespace ublas = boost::numeric::ublas;
    

    /** class for the population / optimization structure
     * $LastChangedDate$
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
            /** boost mutex for running **/
            boost::mutex m_running;
            /** boost mutex for lock during fitting **/
            boost::mutex m_iterationlock;
            /** elite size **/
            std::size_t m_elitesize;
        
        
            void fitness( const std::size_t&, const std::size_t&, fitness::fitness<T,L>&, ublas::vector<T>&, bool& );
            void mutate( const std::size_t&, const std::size_t& );
            void buildelite( const std::size_t&, const std::size_t&, selection::selection<T,L>&, const ublas::vector<T>&, const ublas::vector<std::size_t>&, const ublas::vector<std::size_t>& );
            void buildpopulation( const std::size_t&, const std::size_t&, crossover::crossover<L>&, const ublas::vector<std::size_t>& );
        
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
        m_running(),
        m_iterationlock(),
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

        // we set a lock during calculation, because the object can't handle different local states
        boost::unique_lock<boost::mutex> l_lock( m_running );
        
        
        
        // create element ranges of the population and elite
        std::size_t l_inc = m_population.size() / boost::thread::hardware_concurrency();
        std::vector< std::pair<std::size_t, std::size_t> > l_populationparts;
        for( std::size_t i=0; i < m_population.size()-1; i+=l_inc )
            l_populationparts.push_back( std::pair<std::size_t, std::size_t>(i, i+l_inc) );
        
        if (l_populationparts.size() == 0)
            l_populationparts.push_back( std::pair<std::size_t, std::size_t>(0, m_population.size()) );
        else
            l_populationparts[l_populationparts.size()-1].second += m_population.size() % boost::thread::hardware_concurrency();
        
        
        l_inc = m_elitesize / boost::thread::hardware_concurrency();
        std::vector< std::pair<std::size_t, std::size_t> > l_eliteparts;
        for( std::size_t i=0; i < m_elitesize-1; i+=l_inc )
            l_eliteparts.push_back( std::pair<std::size_t, std::size_t>(i, i+l_inc) );
        
        if (l_eliteparts.size() == 0)
            l_eliteparts.push_back( std::pair<std::size_t, std::size_t>(0, m_elitesize) );
        else
            l_eliteparts[l_eliteparts.size()-1].second += m_elitesize % boost::thread::hardware_concurrency();

        
        
        // run iteration process (each thread group must be recreated on the iteration, because after the join_all() the threads are "out-of-range")
        for(std::size_t i=0; i < p_iteration; ++i) {
            
            // vector with fitness values and thread object
            ublas::vector<T> l_fitness(m_population.size(), 0);
            boost::thread_group l_threads;
            
            
            
            // create and run fitness threads
            bool l_optimumreached = false;
            for(std::size_t j=0; j < l_populationparts.size(); ++j)
                l_threads.create_thread(  boost::bind( &population<T,L>::fitness, this, l_populationparts[j].first, l_populationparts[j].second, boost::ref(p_fitness), boost::ref(l_fitness), boost::ref(l_optimumreached) )  );
            l_threads.join_all();
            

        
            // scales the fitness values to [0,x] (not multithread)
            const T l_min = tools::vector::min( l_fitness);
            BOOST_FOREACH( T& p, l_fitness )
                p -= l_min;
             
            // rank the fitness values (not multithread)
            const ublas::vector<std::size_t> l_rankIndex( tools::vector::rankIndexVector(l_fitness) );
            const ublas::vector<std::size_t> l_rank( tools::vector::rank(l_fitness) );

            
            
            // create elite multithreaded
            m_elite.clear();
            for(std::size_t j=0; j < l_eliteparts.size(); ++j)
                l_threads.create_thread(  boost::bind( &population<T,L>::buildelite, this, l_eliteparts[j].first, l_eliteparts[j].second, boost::ref(p_elite), boost::ref(l_fitness), boost::ref(l_rankIndex), boost::ref(l_rank) )  );
            l_threads.join_all();
                  
            if (l_optimumreached)
                break;
            
            
            
            // create build new population threads and run
            switch (  ((m_buildoption == eliteonly) || (m_buildoption == random)) ? 0 : 1  ) {
                    
                case 0 :
                    for(std::size_t j=0; j < l_populationparts.size(); ++j)
                        l_threads.create_thread(  boost::bind( &population<T,L>::buildpopulation, this, l_populationparts[j].first, l_populationparts[j].second, boost::ref(p_crossover), boost::ref(l_rankIndex) )  );
                    break;
                    
                case 1 :
                    for(std::size_t j=0; j < l_eliteparts.size(); ++j)
                        l_threads.create_thread(  boost::bind( &population<T,L>::buildpopulation, this, l_eliteparts[j].first, l_eliteparts[j].second, boost::ref(p_crossover), boost::ref(l_rankIndex) )  );
                    break;
                    
            }
            l_threads.join_all();

            
            
            // create and run mutation threads
            for(std::size_t j=0; j < l_populationparts.size(); ++j)
                l_threads.create_thread(  boost::bind( &population<T,L>::mutate, this, l_populationparts[j].first, l_populationparts[j].second )  );
            l_threads.join_all();
            
            
            
            // call the "eachIteration" method of each object for updating local object properties (not multithreaded, because of synchronization)
            p_fitness.onEachIteration( m_population );
            p_elite.onEachIteration( m_population );
            p_crossover.onEachIteration( m_population );
        }
    }
    
    
    /** multithread method for calculating fitness values (start & end values must be disjuct over all threads)
     * @param p_start start value of the population values
     * @param p_end end value of the population values
     * @param p_fitnessfunction fitness function object
     * @param p_fitness reference to the fitness vector
     **/
    template<typename T, typename L> inline void population<T,L>::fitness( const std::size_t& p_start, const std::size_t& p_end, fitness::fitness<T,L>& p_fitnessfunction, ublas::vector<T>& p_fitness, bool& p_optimumreached )
    {
        // create local selection object
        boost::shared_ptr< fitness::fitness<T,L> > l_fitness;
        p_fitnessfunction.clone( l_fitness );
        
        for(std::size_t i=p_start; (i < p_end) && (!p_optimumreached); ++i) {
            p_fitness(i) = l_fitness->getFitness( *m_population[i] );
            if (l_fitness->isOptimumReached()) {
                boost::unique_lock<boost::mutex> l_lock( m_iterationlock );
                p_optimumreached = true;
            }
        }
    }
    
    
    /** multithread method for mutating some population elements (start & end values must be disjuct over all threads)
     * @param p_start start value of the population values
     * @param p_end end value of the population values
     **/
    template<typename T, typename L> inline void population<T,L>::mutate( const std::size_t& p_start, const std::size_t& p_end )
    {
        tools::random l_rand;
        for(std::size_t i=p_start; i < p_end; ++i)
            if (l_rand.get<T>( m_mutateprobility.distribution, m_mutateprobility.first, m_mutateprobility.second, m_mutateprobility.third ) <= m_mutateprobility.probabilityvalue)
                m_population[i]->mutate();
    }
    
    
    /** multithread method for building new population elements (start & end values must be disjuct over all threads)
     * @param p_start start value of the population / elite values
     * @param p_end end value of the population  / elite 
     * @param p_crossover crossover function object
     * @param p_rankIdx ublas vector with rank index values of population elements
     **/
    template<typename T, typename L> inline void population<T,L>::buildpopulation( const std::size_t& p_start, const std::size_t& p_end, crossover::crossover<L>& p_crossover, const ublas::vector<std::size_t>& p_rankIdx ) 
    {
        if (m_elite.size() == 0)
            return;
        
        // create local selection object
        boost::shared_ptr< crossover::crossover<L> > l_crossover;
        p_crossover.clone( l_crossover );
        
        tools::random l_rand;
        
        switch (m_buildoption) {
                
            case eliteonly :
                for(std::size_t i=p_start; i < p_end; ++i) {
                    for(std::size_t j=0; j < l_crossover->getNumberOfIndividuals(); ++j)
                        l_crossover->setIndividual( m_elite[static_cast<std::size_t>(l_rand.get<T>(tools::random::uniform, 0, m_elite.size()))] );
                    
                    m_population[i] = l_crossover->combine();
                }
                break;
                
                
            case steadystates :
                for(std::size_t i=p_start; i < p_end; ++i) {
                    for(std::size_t j=0; j < l_crossover->getNumberOfIndividuals(); ++j)
                        l_crossover->setIndividual( m_elite[static_cast<std::size_t>(l_rand.get<T>(tools::random::uniform, 0, m_elite.size()))] );

                    m_population[p_rankIdx(i)] = l_crossover->combine();
                }
                break;
                
                
            case random :
                for(std::size_t i=p_start; i < p_end; ++i) {
                    for(std::size_t j=0; j < l_crossover->getNumberOfIndividuals(); ++j)
                        l_crossover->setIndividual( m_elite[static_cast<std::size_t>(l_rand.get<T>(tools::random::uniform, 0, m_elite.size()))] );
                    
                    m_population[static_cast<std::size_t>(l_rand.get<T>(tools::random::uniform, p_start, p_end))] = l_crossover->combine();
                }
                break;
        }
    }
    
    
    /** multithread for building the elite data
     * @param p_start start value of the elite values
     * @param p_end end value of the elite
     * @param p_eliteselection elite selection object
     * @param p_fitness fitness values
     * @param p_rankIndex vector with index values in ascending order (0. element has the index of the smalles fitness value within the population)
     * @param p_rank rank index (0. elements = 0. element within the population has the rank value (position) within the population )
     **/
    template<typename T, typename L> inline void population<T,L>::buildelite( const std::size_t& p_start, const std::size_t& p_end, selection::selection<T,L>& p_eliteselection, const ublas::vector<T>& p_fitness, const ublas::vector<std::size_t>& p_rankIndex, const ublas::vector<std::size_t>& p_rank )
    {
        // create local selection object
        boost::shared_ptr< selection::selection<T,L> > l_selection;
        p_eliteselection.clone( l_selection );
        
        // build elite
        std::vector< boost::shared_ptr< individual::individual<L> > > l_elite;
        l_selection->getElite( p_start, p_end, m_population, p_fitness, p_rankIndex, p_rank, l_elite );
        
        if (l_elite.size() == 0)
            return;
        
        // we need a mutex, because different threads modify the elite property, so we must create a lock during resizing
        boost::unique_lock<boost::mutex> l_lock( m_iterationlock );
        std::copy( l_elite.begin(), l_elite.end(), std::back_inserter(m_elite));
    }
    
    
    
};};

#endif