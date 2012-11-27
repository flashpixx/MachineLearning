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


#ifndef __MACHINELEARNING_DISTANCES_DISTANCE_HPP
#define __MACHINELEARNING_DISTANCES_DISTANCE_HPP

#include <boost/static_assert.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "../tools/tools.h"



namespace machinelearning { namespace distances {
        
        #ifndef SWIG
        namespace ublas = boost::numeric::ublas;
        #endif
        
        
        /** abstract class for distance classes. Calculates 
         * between matrix-, vectordata and weighted data
         * $LastChangedDate$
         **/      
        template<typename T> class distance
        {
            #ifndef SWIG
            BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );            
            #endif
            
            
            public :
            
                #ifndef SWIG
                /** normalize a vector */
                virtual void normalize( ublas::vector<T>& ) const = 0;

                /** normalize a matrix on the rows or columns **/
                virtual void normalize( ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const = 0;  
                #endif
            
                /** returns a normalized vector **/
                virtual ublas::vector<T> getNormalize( const ublas::vector<T>& ) const = 0;
            
                /** returns a normalized matrix on the rows or columns **/
                virtual ublas::matrix<T> getNormalize( const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const = 0;           
                
            
                
                /** return the length of the vector **/
                virtual T getLength( const ublas::vector<T>& ) const = 0;
            
                /** return the length of every row or column vector within the matrix **/
                virtual ublas::vector<T> getLength( const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const = 0;
            
                /** invert a value **/
                virtual T getInvert( const T& ) const = 0;
                   
                /** returns a vector with their absolute values **/
                virtual ublas::vector<T> getAbs( const ublas::vector<T>& ) const = 0;
            
                #ifndef SWIG
                /** changes every element of the vector to their absolute value **/
                virtual void abs( ublas::vector<T>& ) const = 0;
                #endif
            
                   
            
                /** distance between two vectors **/
                virtual T getDistance( const ublas::vector<T>&, const ublas::vector<T>& ) const = 0;

                /** distances between row / column vectors of matrix and vector **/
                virtual ublas::vector<T> getDistance( const ublas::matrix<T>&, const ublas::vector<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const = 0;

                /** distances between row / column vectors of matrix and  row / column vectors of the other matrix **/
                virtual ublas::vector<T> getDistance( const ublas::matrix<T>&, const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const = 0;
            
            
                #ifndef SWIG
                /** weight distance between two vectors **/
                virtual T getWeightedDistance( const ublas::vector<T>&, const ublas::vector<T>&, const ublas::vector<T>& ) const = 0;       
            
                /** distances between row / column vectors of matrix and weighted vector **/
                virtual ublas::vector<T> getWeightedDistance( const ublas::matrix<T>&, const ublas::vector<T>&, const ublas::vector<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const = 0;
                    
                /** distances between row / column vectors of matrix and  row / column weighted vectors of the other matrix **/
                virtual ublas::vector<T> getWeightedDistance( const ublas::matrix<T>&, const ublas::matrix<T>&, const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const = 0;       
            
                /** distances between row / column vectors of matrix and  row / column of the weighted matrix **/
                virtual ublas::vector<T> getWeightedDistance( const ublas::matrix<T>&, const ublas::vector<T>&, const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const = 0;
                #endif

        };

} }
#endif
