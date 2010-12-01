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


#ifndef MACHINELEARNING_DISTANCES_DISTANCE_HPP
#define MACHINELEARNING_DISTANCES_DISTANCE_HPP

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "../exception/exception.h"
#include "../tools/tools.h"



namespace machinelearning { namespace distances {
    
    namespace ublas = boost::numeric::ublas;
    
    
    /** abstract class for distance classes. Calculates 
     * between matrix-, vectordata and weighted data
     **/      
    template<typename T> class distance {
        
        
        public :
        
            /** normalize a vector */
            virtual void normalize( ublas::vector<T>& ) const { throw exception::classmethod(_("method is not implementated in the base class")); };

            /** returns a normalized vector **/
            virtual ublas::vector<T> getNormize( const ublas::vector<T>& ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
        
            /** normalize a matrix on the rows or columns **/
            virtual void normalize( ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const { throw exception::classmethod(_("method is not implementated in the base class")); };        
            
            /** returns a normalized matrix on the rows or columns **/
            virtual ublas::matrix<T> getNormalize( const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
        
            
            
            /** return the length of the vector **/
            virtual T getLength( const ublas::vector<T>& ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
        
            /** return the length of every row or column vector within the matrix **/
            virtual ublas::vector<T> getLength( const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
            /** invert a value **/
            virtual T getInvert( const T& ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
               
            /** returns a vector with their absolute values **/
            virtual ublas::vector<T> getAbs( const ublas::vector<T>& ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
        
            /** changes every element of the vector to their absolute value **/
            virtual void abs( ublas::vector<T>& ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
        
               
        
            /** distance between two vectors **/
            virtual T getDistance( const ublas::vector<T>&, const ublas::vector<T>& ) const { throw exception::classmethod(_("method is not implementated in the base class")); };

            /** distances between row / column vectors of matrix and vector **/
            virtual ublas::vector<T> getDistance( const ublas::matrix<T>&, const ublas::vector<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const { throw exception::classmethod(_("method is not implementated in the base class")); };        

            /** distances between row / column vectors of matrix and  row / column vectors of the other matrix **/
            virtual ublas::vector<T> getDistance( const ublas::matrix<T>&, const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const { throw exception::classmethod(_("method is not implementated in the base class")); };   
        
        
        
            /** weight distance between two vectors **/
            virtual T getWeightedDistance( const ublas::vector<T>&, const ublas::vector<T>&, const ublas::vector<T>& ) const { throw exception::classmethod(_("method is not implementated in the base class")); };        
        
            /** distances between row / column vectors of matrix and weighted vector **/
            virtual ublas::vector<T> getWeightedDistance( const ublas::matrix<T>&, const ublas::vector<T>&, const ublas::vector<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
                
            /** distances between row / column vectors of matrix and  row / column weighted vectors of the other matrix **/
            virtual ublas::vector<T> getWeightedDistance( const ublas::matrix<T>&, const ublas::matrix<T>&, const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const { throw exception::classmethod(_("method is not implementated in the base class")); };        
        
            /** distances between row / column vectors of matrix and  row / column of the weighted matrix **/
            virtual ublas::vector<T> getWeightedDistance( const ublas::matrix<T>&, const ublas::vector<T>&, const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const { throw exception::classmethod(_("method is not implementated in the base class")); }; 


        
        
        protected :
        
            /** destructor **/
            virtual ~distance( void ) {}; 
        
    };

    
};};

#endif
