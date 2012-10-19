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


#ifndef __MACHINELEARNING_GENETICALGORITHM_CROSSOVER_CROSSOVER_HPP
#define __MACHINELEARNING_GENETICALGORITHM_CROSSOVER_CROSSOVER_HPP

#include <boost/static_assert.hpp>

#include "../individual/individual.hpp"


namespace machinelearning { namespace geneticalgorithm { namespace crossover {
    
    /** abstract class of the crossover function **/
    template<typename T> class crossover
    {
        
        public :
        
            /** returns the number how many individuals are needed for the crossover (default should be two)
             * @return number of needed individuals
             **/
            virtual std::size_t getNumberOfIndividuals( void ) const = 0;     
        
            /** set the individuals with a smart-pointer
             * @param p_individual smart-pointer object to an individual
             **/
            virtual void setIndividual( const boost::shared_ptr< individual::individual<T> >& p_individual ) = 0;
        
            /** creates a new smart-pointer object with the new individual data
             * @return a new smart-pointer object of the individual that should be replaced
             **/
            virtual boost::shared_ptr< individual::individual<T> > combine( void ) = 0;
        
            /** method for cloning the object, for using on multithread
             * @param p_ptr smart-pointer object
             **/
            virtual void clone( boost::shared_ptr< crossover<T> >& p_ptr ) const = 0;
        
            /** method that is called at the end of each iteration
             * @param p_population population
             **/
            virtual void onEachIteration( const std::vector< boost::shared_ptr< individual::individual<T> > >& p_population ) = 0;
    };
    
}}}
#endif

