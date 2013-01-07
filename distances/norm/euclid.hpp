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



#ifndef __MACHINELEARNING_DISTANCES_NORM_EUCLID_HPP
#define __MACHINELEARNING_DISTANCES_NORM_EUCLID_HPP

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/bindings/blas.hpp>
#include <boost/numeric/bindings/ublas/vector.hpp>
#include <boost/numeric/bindings/ublas/matrix.hpp>

#include "../distance.hpp"
#include "../../errorhandling/exception.hpp"
#include "../../tools/tools.h"


namespace machinelearning { namespace distances { namespace norm {
    
    
    #ifndef SWIG
    namespace ublas  = boost::numeric::ublas;
    namespace blas   = boost::numeric::bindings::blas;
    #endif
    
    
    /** class for calculating euclid distance beween datapoints
     * @todo portage this class to the Intel Math Kernel Library http://software.intel.com/en-us/articles/intel-mkl/
     **/
    template<typename T> class euclid : public distance<T>
    {
        
        public:
        
            #ifndef SWIG
            void normalize( ublas::vector<T>& ) const;
            void normalize( ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const;        
            #endif
            ublas::vector<T> getNormalize( const ublas::vector<T>& ) const;
            ublas::matrix<T> getNormalize( const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const;
        
            T getLength( const ublas::vector<T>& ) const;
            ublas::vector<T> getLength( const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const;
            T getInvert( const T& ) const;
            ublas::vector<T> getAbs( const ublas::vector<T>& ) const;
            #ifndef SWIG
            void abs( ublas::vector<T>& ) const;
            #endif
        
            T getDistance( const ublas::vector<T>&, const ublas::vector<T>& ) const;        
            ublas::vector<T> getDistance( const ublas::matrix<T>&, const ublas::vector<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const;
            ublas::vector<T> getDistance( const ublas::matrix<T>&, const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const;
        
            #ifndef SWIG
            T getWeightedDistance( const ublas::vector<T>&, const ublas::vector<T>&, const ublas::vector<T>& ) const;        
            ublas::vector<T> getWeightedDistance( const ublas::matrix<T>&, const ublas::vector<T>&, const ublas::vector<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const;        
            ublas::vector<T> getWeightedDistance( const ublas::matrix<T>&, const ublas::matrix<T>&, const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const;
            ublas::vector<T> getWeightedDistance( const ublas::matrix<T>&, const ublas::vector<T>&, const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const;
            #endif

    };
    
    
    
    /** normalize a vector with euclidian norm
     * @param p_vec vector which should be normalized
     **/
    template<typename T> inline void euclid<T>::normalize( ublas::vector<T>& p_vec ) const 
    {
        p_vec /= blas::nrm2( p_vec );
    }    
    
    
   
    /** normalize a matrix on their rows or columns vectors
     * @param p_matrix matrix for normalization
     * @param p_row option for row or column iteration (default row)
     **/
    template<typename T> inline void euclid<T>::normalize( ublas::matrix<T>& p_matrix, const tools::matrix::rowtype& p_row ) const
    { 
        switch (p_row) {                
            case tools::matrix::row :
                
                    for(std::size_t i=0; i < p_matrix.size1(); ++i)
                        ublas::row(p_matrix, i) /= blas::nrm2( static_cast< ublas::vector<T> >(ublas::row(p_matrix, i)) );
                    break;
                
                
            case tools::matrix::column :  
                
                    for(std::size_t i=0; i < p_matrix.size2(); ++i)
                        ublas::column(p_matrix, i) /= blas::nrm2( static_cast< ublas::vector<T> >(ublas::column(p_matrix, i)) );
                    break;
        }
        
    }
    
    
    
    /** return the normalized vector with euclidian norm
     * @param p_vec vector which should be normalized
     * @return vector
     **/
    template<typename T> inline ublas::vector<T> euclid<T>::getNormalize( const ublas::vector<T>& p_vec ) const 
    {
        ublas::vector<T> l_vec = p_vec;
        normalize(l_vec);
        return l_vec;
    }
    
    
    
    /** normalize a matrix on their rows or columns vectors
     * @param p_matrix matrix for noamlization
     * @param p_row option for row or column iteration (default row)
     * @return normalized matrix
     **/
    template<typename T> inline ublas::matrix<T> euclid<T>::getNormalize( const ublas::matrix<T>& p_matrix, const tools::matrix::rowtype& p_row ) const
    {
        ublas::matrix<T> l_matrix = p_matrix;
        normalize(l_matrix, p_row);
        return l_matrix;
    }
    
    
    
    /** returns the length of a vector
     * @param p_vec vector
     * @return length of the vector
     **/
    template<typename T> inline T euclid<T>::getLength( const ublas::vector<T>& p_vec ) const
    {
        return blas::nrm2( p_vec );
    }

    
    
    /** returns for every row or column the length 
     * @param p_matrix matrix
     * @param p_row option for row or column iteration (default row)
     * @return vector with length
     **/
    template<typename T> inline ublas::vector<T> euclid<T>::getLength( const ublas::matrix<T>& p_matrix, const tools::matrix::rowtype& p_row ) const
    {
        ublas::vector<T> l_vec( (p_row==tools::matrix::row) ? p_matrix.size1() : p_matrix.size2()  );
        
        switch (p_row) {                
            case tools::matrix::row :
                
                for(std::size_t i=0; i < l_vec.size(); ++i)
                    l_vec(i) = blas::nrm2( static_cast< ublas::vector<T> >(ublas::row(p_matrix, i)) );
                break;
                
                
            case tools::matrix::column :  
                
                for(std::size_t i=0; i < l_vec.size(); ++i)
                    l_vec(i) = blas::nrm2( static_cast< ublas::vector<T> >(ublas::column(p_matrix, i)) );
                break;
        }
        
        return l_vec;
    }
    
    
    
    /** returns a invertet value
     * @param p_val value
     * @return inverted value
     **/
    template<typename T> inline T euclid<T>::getInvert( const T& p_val ) const
    {
        return std::pow( p_val, static_cast<T>(-2) );
    }    
    
    
    
    /** calculate absolut values for a vector like vec.^2
     * @param p_vec vector
     * @return absolut value
     **/
    template<typename T> inline ublas::vector<T> euclid<T>::getAbs( const ublas::vector<T>& p_vec ) const
    {
        return tools::vector::pow<T>(p_vec, 2);
    }    
    
    
    
    /** calculate absolut values for every element of the vector like vec.^2
     * @param p_vec vector
     **/
    template<typename T> inline void euclid<T>::abs( ublas::vector<T>& p_vec ) const
    {
        p_vec = tools::vector::pow<T>(p_vec, 2);
    }
   

    
    /** calculates the distance between two vectors
     * @param p_first first vector
     * @param p_second second vector
     * @return distance value
     **/
    template<typename T> inline T euclid<T>::getDistance( const ublas::vector<T>& p_first, const ublas::vector<T>& p_second ) const
    {
        ublas::vector<T> l_vec = p_first-p_second;
        return blas::nrm2( l_vec );
    }
    
    
    /** calculates the distance beween every row or column of the matrix and the vector [ norm(matrix({1..n},:) * vector)]
     * @param p_data matrix
     * @param p_vec vector
     * @param p_row row / column option (default row)
     * @return vector with distance values
     **/
    template<typename T> inline ublas::vector<T> euclid<T>::getDistance( const ublas::matrix<T>& p_data, const ublas::vector<T>& p_vec, const tools::matrix::rowtype& p_row ) const
    {
        ublas::vector<T> l_vec( (p_row==tools::matrix::row) ? p_data.size1() : p_data.size2()  );
        
        switch (p_row) {                
            case tools::matrix::row :
                
                for(std::size_t i=0; i < l_vec.size(); ++i)
                    l_vec(i) = getDistance( ublas::row(p_data, i), p_vec);
                    
                break;
                
                
            case tools::matrix::column :  
                
                for(std::size_t i=0; i < l_vec.size(); ++i)
                    l_vec(i) = getDistance( ublas::column(p_data, i), p_vec);
                
                break;
        }

        return l_vec;
    }
    
    
    
    /** calculates the distance between every row and column of the matrices [ norm(matrixA[i,:] - matrixB(i,:))]
     * @param p_first first matrix
     * @param p_second second matrix
     * @param p_row row / column option (default row)
     * @return distance vector
     **/
    template<typename T> inline ublas::vector<T> euclid<T>::getDistance( const ublas::matrix<T>& p_first, const ublas::matrix<T>& p_second, const tools::matrix::rowtype& p_row ) const
    {
        ublas::vector<T> l_vec( (p_row==tools::matrix::row) ? p_first.size1() : p_first.size2()  );
        ublas::matrix<T> l_matrix = p_first-p_second;
        
        switch (p_row) {                
            
            case tools::matrix::row :
                
                for(std::size_t i=0; i < l_vec.size(); ++i)
                    l_vec(i) = blas::nrm2( static_cast< ublas::vector<T> >(ublas::row(l_matrix, i)) );
                break;
            
                
            case tools::matrix::column :  for(std::size_t i=0; i < l_vec.size(); ++i)
                    l_vec(i) = blas::nrm2( static_cast< ublas::vector<T> >(ublas::column(l_matrix, i)) );
                break;
        }
        
        return l_vec;
    }
    
    
    
    /** calculates the weighted distance between two vectors [ norm2(weight .* (vectorA - vectorB)) ]
     * @param p_first first vector
     * @param p_second second vector
     * @param p_weight weight vector
     * @return distance value
     **/
    template<typename T> inline T euclid<T>::getWeightedDistance( const ublas::vector<T>& p_first, const ublas::vector<T>& p_second, const ublas::vector<T>& p_weight ) const
    {
        ublas::vector<T> l_vec = p_first-p_second;
        l_vec = ublas::element_prod( p_weight, l_vec );
        return blas::nrm2(  l_vec );
    }
    

    
    /** calculates the weighted distance between two vectors  [ norm2(weight(i,:) .* (matrix(i,:) - vector)) ]
     * @param p_data data matrix
     * @param p_vec vector
     * @param p_weight weight vector
     * @param p_row row / column option (default row)
     * @return distance vector
     **/
    template<typename T> inline ublas::vector<T> euclid<T>::getWeightedDistance( const ublas::matrix<T>& p_data, const ublas::vector<T>& p_vec, const ublas::vector<T>& p_weight, const tools::matrix::rowtype& p_row ) const
    {
        ublas::vector<T> l_vec( (p_row==tools::matrix::row) ? p_data.size1() : p_data.size2()  );
        ublas::vector<T> l_help;
        
        switch (p_row) {                
            case tools::matrix::row :
                
                for(std::size_t i=0; i < l_vec.size(); ++i) {
                    l_help   = ublas::row(p_data, i) - p_vec;
                    l_help   = ublas::element_prod( p_weight, l_help );
                    l_vec(i) = blas::nrm2( l_help );
                }
                
                break;
                
                
            case tools::matrix::column :  
                
                for(std::size_t i=0; i < l_vec.size(); ++i) {
                    l_help   = ublas::column(p_data, i) - p_vec;
                    l_help   = ublas::element_prod( p_weight, l_help );
                    l_vec(i) = blas::nrm2( l_help );
                }
                
                break;
        }
        
        return l_vec;        
    }
    
    
    
    /** calculates the weighted distance between two matrices [ weight .* (matrixA - matrixB) ]
     * @param p_first first matrix
     * @param p_second second matrix
     * @param p_weight weight matrix
     * @param p_row row / column option (default row)
     * @return distance vector
     **/
    template<typename T> inline ublas::vector<T> euclid<T>::getWeightedDistance( const ublas::matrix<T>& p_first, const ublas::matrix<T>& p_second, const ublas::matrix<T>& p_weight, const tools::matrix::rowtype& p_row ) const
    {
        ublas::vector<T> l_vec( (p_row==tools::matrix::row) ? p_first.size1() : p_first.size2()  );
        ublas::matrix<T> l_matrix = ublas::element_prod( p_weight, p_first-p_second );
        
        switch (p_row) {                
                
            case tools::matrix::row :
                
                for(std::size_t i=0; i < l_vec.size(); ++i)
                    l_vec(i) = blas::nrm2( static_cast< ublas::vector<T> >(ublas::row(l_matrix, i)) );
                break;
                
                
            case tools::matrix::column : 
                
                for(std::size_t i=0; i < l_vec.size(); ++i)
                    l_vec(i) = blas::nrm2( static_cast< ublas::vector<T> >(ublas::column(l_matrix, i)) );
                break;
        }
        
        return l_vec;
    }
    
    
    
    /** calculates the weighted distance between matrices, vector and weighted matrix  [ norm2(weight(i,:) .* (matrix(i,:) - vector)) ]
     * @param p_matrix matrix
     * @param p_vec vector
     * @param p_weight weight matrix
     * @param p_row row / column option (default row)
     * @return distance vector
     **/
    template<typename T> inline ublas::vector<T> euclid<T>::getWeightedDistance( const ublas::matrix<T>& p_matrix, const ublas::vector<T>& p_vec, const ublas::matrix<T>& p_weight, const tools::matrix::rowtype& p_row ) const
    {
        ublas::vector<T> l_vec( (p_row==tools::matrix::row) ? p_matrix.size1() : p_matrix.size2()  );
        
        switch (p_row) {                
                
            case tools::matrix::row :
                
                for(std::size_t i=0; i < l_vec.size(); ++i)
                    l_vec(i) = getWeightedDistance( ublas::row(p_matrix, i), p_vec, ublas::row(p_weight, i) );
                break;
                
            
            case tools::matrix::column :

                for(std::size_t i=0; i < l_vec.size(); ++i)
                    l_vec(i) = getWeightedDistance( ublas::column(p_matrix, i), p_vec, ublas::column(p_weight, i) );
                break;
        
        }
        
        return l_vec;
    }
    
    
} } }
#endif
