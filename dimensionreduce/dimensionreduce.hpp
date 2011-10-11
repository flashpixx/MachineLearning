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


#ifndef __MACHINELEARNING_DIMENSIONREDUCE_HPP
#define __MACHINELEARNING_DIMENSIONREDUCE_HPP


#include <boost/static_assert.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#ifdef MACHINELEARNING_MPI
#include <boost/mpi.hpp>
#endif

#include "../exception/exception.h"
#include "../tools/tools.h"


namespace machinelearning { 
    
    /** namespace for all algorithms to reduce data dimension
     * $LastChangedDate$
     **/    
    namespace dimensionreduce {
 
        
        namespace ublas = boost::numeric::ublas;
        #ifdef MACHINELEARNING_MPI
        namespace mpi   = boost::mpi;
        #endif
        
        
        /** namespace for all non-supervised reducing algorithms
         * $LastChangedDate$
         **/
        namespace nonsupervised {
   
            
            /** abstract class for nonsupervised dimension reducing classes
             * $LastChangedDate$
             * @todo implement serializable interface
             **/      
            template<typename T> class reduce
            {
                BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
                
                
                public :

                    /** maps data to target dimension **/
                    virtual ublas::matrix<T> map( const ublas::matrix<T>& ) = 0;
                      
                    /** returns the mapped dimension **/
                    virtual std::size_t getDimension( void ) const = 0;

            };
            
            
            #ifdef MACHINELEARNING_MPI
            
            /** abstract class for nonsupervised dimension reducing classes with MPI support
             * $LastChangedDate$
             * @todo implement serializable interface
             **/      
            template<typename T> class reducempi
            {
                BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
                
                
                public :
                
                    /** maps data to target dimension **/
                    virtual ublas::matrix<T> map( const mpi::communicator&, const ublas::matrix<T>& ) = 0;
                
            };

            #endif

        }
        
        
        
        
        /** namespace for all supervised reducing algorithms
         * $LastChangedDate$
         * @todo implement serializable interface
         **/
        namespace supervised {
        
            /** abstract class for supervised dimension reducing classes
             * $LastChangedDate$
             **/      
            template<typename T, typename L> class reduce
            {
                
                public :
                
                    /** maps data to target dimension **/
                    virtual ublas::matrix<T> map( const ublas::matrix<T>&, const std::vector<L>& ) = 0; 
                
                    /** returns the mapped dimension **/
                    virtual std::size_t getDimension( void ) const = 0; 
                
            };
            
        }
        
    }
}
#endif