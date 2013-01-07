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



#ifndef __MACHINELEARNING_NEIGHBORHOOD_NEIGHBORHOOD_HPP
#define __MACHINELEARNING_NEIGHBORHOOD_NEIGHBORHOOD_HPP


#include <boost/static_assert.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "../tools/tools.h"


namespace machinelearning {
    
    
    /** neighborhood namespace for all structures to calculate neighbors
     **/
    namespace neighborhood {
    
        
        namespace ublas   = boost::numeric::ublas;
        
        
        
        /** abstract class for neighborhood classes
         * @todo recreate base class for neighborhood
         **/      
        template<typename T> class neighborhood
        {
            BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
            
            
            public :
            
                /** function for calculating the neighborhoods **/
                virtual ublas::matrix<std::size_t> get( const ublas::matrix<T>& ) const = 0;
            
                /** function for calculating the neighborhoods between different datapoints **/
                virtual ublas::matrix<std::size_t> get( const ublas::matrix<T>&, const ublas::matrix<T>& ) const = 0;
            
                /** calculates the distance between two vectors **/
                virtual T calculateDistance( const ublas::vector<T>&, const ublas::vector<T>& ) const = 0;
            
                /** invert a value **/
                virtual T invert( const T& ) const = 0;
            
                /** returns the number of neighbors **/
                virtual std::size_t getNeighborCount( void ) const = 0;
            
        };
    
    }
}
#endif
