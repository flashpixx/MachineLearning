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


#ifndef __MACHINELEARNING_GENETICALGORITHM_INDIVIDUAL_HPP
#define __MACHINELEARNING_GENETICALGORITHM_INDIVIDUAL_HPP

#include <boost/shared_ptr.hpp>



namespace machinelearning { namespace geneticalgorithm {
    
    /** abstract class of an indivdual of the population
     * $LastChangedDate: 2011-09-13 17:12:35 +0200 (Di, 13 Sep 2011) $
     **/
    template<typename T> class individual
    {

        public :
        
            /** method for cloning the object. The method should be create a new individual for the population
             * initialization on the heap and returns the smart-pointer to the heap object within the reference parameter. 
             **/
            virtual void clone( boost::shared_ptr< individual<T> >& ) const = 0;

            /** returns the data at the position of the individual **/
            virtual T operator[]( const std::size_t& ) const = 0;
        
            /** returns a reference of a data element at the position of the individual **/
            virtual T& operator[]( const std::size_t& ) = 0;
        
            /** mutates the individual **/
            virtual void mutate( void ) = 0;
        
            /** returns the number of positions / length **/
            virtual std::size_t size( void ) const = 0;
    
    };
    
};};

#endif

