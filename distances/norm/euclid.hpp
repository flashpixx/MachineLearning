/** 
 \verbatim
 #########################################################################
 # GPL License                                                           #
 #                                                                       #
 # Copyright (c) 2010, Philipp Kraus, <philipp.kraus@flashpixx.de>       #
 # This program is free software: you can redistribute it and/or modify  #
 # it under the terms of the GNU General Public License as published by  #
 # the Free Software Foundation, either version 3 of the License, or     #
 # (at your option) any later version.                                   #
 #                                                                       #
 # This program is distributed in the hope that it will be useful,       #
 # but WITHOUT ANY WARRANTY; without even the implied warranty of        #
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
 # GNU General Public License for more details.                          #
 #                                                                       #
 # You should have received a copy of the GNU General Public License     #
 # along with this program.  If not, see <http://www.gnu.org/licenses/>. #
 #########################################################################
 \endverbatim
 **/



#ifndef MACHINELEARNING_DISTANCES_EUCLID_H
#define MACHINELEARNING_DISTANCES_EUCLID_H

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/bindings/ublas/matrix.hpp>
#include <boost/numeric/bindings/ublas/vector.hpp>
#include <boost/numeric/bindings/blas.hpp>

#include "../distance.hpp"
#include "../../exception/exception.h"
#include "../../tools/tools.h"


namespace machinelearning { namespace distances {
    
    namespace ublas  = boost::numeric::ublas;
    namespace blas   = boost::numeric::bindings::blas;

    
    /** class for calculating euclid distance beween datapoints **/
    template<typename T> class euclid : public distance<T> {
        

        public:

            ublas::vector<T> calculate( const ublas::matrix<T>&, const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const;
            ublas::vector<T> calculate( const ublas::matrix<T>&, const ublas::matrix<T>&, const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const;
            ublas::vector<T> abs( const ublas::vector<T>&  ) const;
            T calculate( const ublas::vector<T>&, const ublas::vector<T>& ) const;
            T calculate( const ublas::vector<T>&, const ublas::vector<T>&, const ublas::vector<T>& ) const;
            void norm( ublas::vector<T>& ) const ;
            void norm( ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const; 
            T length( const ublas::vector<T>& p_vec ) const;
        
    };


    
    /** calculate distance between matrix, wherever rows or columns
     * are read like vectors (sqrt( sum ( (vec1[i] - vec2[i]).^2 ) )
     * @overload
     * @param p_first first matrix
     * @param p_second second matrix
     * @param p_row enum for creating matrix sum over rows or columns
     * @return vector with distances
    **/
    template<typename T> inline ublas::vector<T> euclid<T>::calculate( const ublas::matrix<T>& p_first, const ublas::matrix<T>& p_second, const tools::matrix::rowtype& p_row ) const
    {
        ublas::vector<T> l_vec( (p_row==tools::matrix::row) ? p_first.size1() : p_first.size2()  );
        ublas::matrix<T> l_matrix = p_first-p_second;
        
        switch (p_row) {                
            case tools::matrix::row :     for(std::size_t i=0; i < l_vec.size(); ++i)
                                            l_vec(i) = blas::nrm2( static_cast< ublas::vector<T> >(ublas::row(l_matrix, i)) );
                                          break;
            
            case tools::matrix::column :  for(std::size_t i=0; i < l_vec.size(); ++i)
                                            l_vec(i) = blas::nrm2( static_cast< ublas::vector<T> >(ublas::column(l_matrix, i)) );
                                          break;
        }
        
        return l_vec;
    }
    
    
    
    /** calculate weight distance between matrix, wherever rows or columns
     * are read like vectors (sqrt( sum ( weights.^2 .* (vec1[i] - vec2[i]).^2 ) )
     * @overload
     * @param p_first first matrix
     * @param p_second second matrix
     * @param p_weights weights 
     * @param p_row enum for creating matrix sum over rows or columns
     * @return vector with distances
     **/
    template<typename T> inline ublas::vector<T> euclid<T>::calculate( const ublas::matrix<T>& p_first, const ublas::matrix<T>& p_second, const ublas::matrix<T>& p_weights, const tools::matrix::rowtype& p_row ) const
    {
        ublas::vector<T> l_vec( (p_row==tools::matrix::row) ? p_first.size1() : p_first.size2()  );
        ublas::matrix<T> l_matrix = ublas::element_prod( p_weights, p_first-p_second );      
        
        switch (p_row) {                
            case tools::matrix::row :     for(std::size_t i=0; i < l_vec.size(); ++i)
                                            l_vec(i) = blas::nrm2( static_cast< ublas::vector<T> >(ublas::row(l_matrix, i)) );
                                          break;
                
            case tools::matrix::column :  for(std::size_t i=0; i < l_vec.size(); ++i)
                                            l_vec(i) = blas::nrm2( static_cast< ublas::vector<T> >(ublas::column(l_matrix, i)) );
                                          break;
        }
        
        return l_vec;
    }
    
    
    /** normalize a vector with euclidian norm
     * @param p_vec vector which should be normalized
    **/
    template<typename T> inline void euclid<T>::norm( ublas::vector<T>& p_vec ) const 
    {
        p_vec /= blas::nrm2( p_vec );
    };
    
    
    /** normalize a matrix on their row or column vectors
     * @param p_matrix matrix for noamlization
     * @param p_row option for row or column iteration (default row)
    **/
    template<typename T> inline void euclid<T>::norm( ublas::matrix<T>& p_matrix, const tools::matrix::rowtype& p_row ) const
    { 
        switch (p_row) {                
            case tools::matrix::row :     for(std::size_t i=0; i < p_matrix.size1(); ++i)
                                            ublas::row(p_matrix, i) /= blas::nrm2( static_cast< ublas::vector<T> >(ublas::row(p_matrix, i)) );
                                          break;
                
            case tools::matrix::column :  for(std::size_t i=0; i < p_matrix.size2(); ++i)
                                            ublas::column(p_matrix, i) /= blas::nrm2( static_cast< ublas::vector<T> >(ublas::column(p_matrix, i)) );
                                          break;
        }
        
    };
    
    
    /** calculate absolut values for a vector like vec.^2
     * @param p_vec vector
     * @return absolut value
     **/
    template<typename T> inline ublas::vector<T> euclid<T>::abs( const ublas::vector<T>& p_vec ) const
    {
        return tools::vector::pow<T>(p_vec, 2);
    }    
    
    
    
    /** calculate distance between two vectors like
     * sqrt( (vec1 - vec2).^2 )
     * @overload
     * @param p_first first vector
     * @param p_second second vector
     * return distance value
    **/
    template<typename T> inline T euclid<T>::calculate( const ublas::vector<T>& p_first, const ublas::vector<T>& p_second ) const
    {
        ublas::vector<T> l_vec = p_first-p_second;
        return blas::nrm2( l_vec );
    }
    
    
    
    /** calculate distance between two vectors like
     * sqrt( weights.^2 .* (vec1 - vec2).^2 )
     * @overload
     * @param p_first first vector
     * @param p_second second vector
     * @param p_weights weights
     * return distance value
     **/
    template<typename T> inline T euclid<T>::calculate( const ublas::vector<T>& p_first, const ublas::vector<T>& p_second, const ublas::vector<T>& p_weights ) const
    {
        ublas::vector<T> l_vec = ublas::element_prod( p_weights, p_first-p_second );
        return blas::nrm2( l_vec );
    }    
    
    
    
    /** returns the length of a vector
     * @param p_vec vector
     * @return length of the vector
    **/
    template<typename T> inline T euclid<T>::length( const ublas::vector<T>& p_vec ) const
    {
        return blas::nrm2( p_vec );
    }

};};

#endif