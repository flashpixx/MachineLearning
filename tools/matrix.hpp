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


#ifndef __MACHINELEARNING_TOOLS_MATRIX_HPP
#define __MACHINELEARNING_TOOLS_MATRIX_HPP

#include <omp.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>

#include "../errorhandling/exception.hpp"
#include "vector.hpp"
#include "function.hpp"
#include "language/language.h"



namespace machinelearning { namespace tools {
    
    #ifndef SWIG
    namespace ublas     = boost::numeric::ublas;
    #endif
    
    
    /** class for matrix operations **/
    class matrix
    {
    
        public :
    
            /** row type **/
            enum rowtype
            {
                row,
                column
            };
        
            template<typename T> static ublas::matrix<T> random( const std::size_t&, const std::size_t&, const tools::random::distribution& = tools::random::uniform, const T& = std::numeric_limits<T>::epsilon(), const T& = std::numeric_limits<T>::epsilon(), const T& = std::numeric_limits<T>::epsilon() );
            template<typename T> static ublas::matrix<T> random( const std::size_t&, const tools::random::distribution& = tools::random::uniform, const T& = std::numeric_limits<T>::epsilon(), const T& = std::numeric_limits<T>::epsilon(), const T& = std::numeric_limits<T>::epsilon() );
            template<typename T> static ublas::mapped_matrix<T> eye( const std::size_t&, const std::size_t&, const T& = 1 );
            template<typename T> static ublas::mapped_matrix<T> eye( const std::size_t& );
            template<typename T> static ublas::matrix<T> pow( const ublas::matrix<T>&, const T& );
            template<typename T> static ublas::vector<T> max( const ublas::matrix<T>&, const rowtype& = row );
            template<typename T> static ublas::vector<T> min( const ublas::matrix<T>&, const rowtype& = row );
            template<typename T> static ublas::vector<T> mean( const ublas::matrix<T>&, const rowtype& = row );
            template<typename T> static ublas::vector<T> variance( const ublas::matrix<T>&, const rowtype& = row );
            template<typename T> static ublas::vector<T> sum( const ublas::matrix<T>&, const rowtype& = row );
            template<typename T> static ublas::mapped_matrix<T> diag( const ublas::vector<T>& );
            template<typename T> static ublas::vector<T> diag( const ublas::matrix<T>& );
            template<typename T> static T trace( const ublas::matrix<T>& );
            template<typename T> static ublas::matrix<T> doublecentering( const ublas::matrix<T>& );
            template<typename T> static ublas::matrix<T> centering( const ublas::matrix<T>&, const rowtype& = column );
            template<typename T> static ublas::matrix<T> sort( const ublas::matrix<T>&, const ublas::vector<std::size_t>&, const rowtype& p_which = row);
            template<typename T> static ublas::matrix<T> cov( const ublas::matrix<T>& );
            template<typename T> static ublas::matrix<T> setNumericalZero( const ublas::matrix<T>&, const T& = 0);
            template<typename T> static ublas::matrix<T> invert( const ublas::matrix<T>&);
            template<typename T> static ublas::matrix<T> repeat( const ublas::vector<T>&, const rowtype& p_which = row);
            template<typename T> static ublas::matrix<T> repeat( const ublas::vector<T>&, const std::size_t&, const rowtype& p_which = row);
    };
    
    
    
    
    /** creates a random NxM matrix of type T
     * @param p_row rows
     * @param p_col columns
     * @param p_distribution name of the distribution
     * @param p_a first value for distribution
     * @param p_b second value for distribution
     * @param p_c third value for distribution
     * @return matrix of type T
     **/
    template<typename T> inline ublas::matrix<T> matrix::random( const std::size_t& p_row, const std::size_t& p_col, const tools::random::distribution& p_distribution, const T& p_a, const T& p_b, const T& p_c )
    {         
        if ((p_row == 0) || (p_col == 0))
            return ublas::matrix<T>(p_row, p_col);

        // random object must be created within the thread, because
        // initialization runs wrong if the pragma option private / firstprivate is used
        ublas::matrix<T> l_matrix(p_row, p_col);
        #pragma omp parallel shared(l_matrix)
        {
            tools::random l_rand;

            #pragma omp for
            for (std::size_t i=0; i < p_row; ++i)
                for (std::size_t j=0; j < p_col; ++j)
                    l_matrix(i,j) = l_rand.get<T>( p_distribution, p_a, p_b, p_c );
        }
        
        return l_matrix;
    }
    
    
    /** create a random symmetric matrix of type T
     * @param p_size element size
     * @param p_distribution name of the distribution
     * @param p_a first value for distribution
     * @param p_b first value for distribution
     * @param p_c first value for distribution
     * @return matrix of type T
     **/
    template<typename T> inline ublas::matrix<T> matrix::random( const std::size_t& p_size, const tools::random::distribution& p_distribution, const T& p_a, const T& p_b, const T& p_c )
    {
        return random<T>(p_size, p_size, p_distribution, p_a, p_b, p_c);
    }
    
    
    /** creates a matrix and sets "1" on the diagonal elements
     * @param p_row rows
     * @param p_col columns
     * @param p_val value for set (default 1)
     * @return matrix of type T
     **/  
    template<typename T> inline ublas::mapped_matrix<T> matrix::eye( const std::size_t& p_row, const std::size_t& p_col, const T& p_val )
    {
        if (p_row == 0)
            throw exception::runtime(_("row size must be greater than zero"));
        if (p_col == 0)
            throw exception::runtime(_("column size must be greater than zero"));
        
        const std::size_t l_diag = (p_row < p_col) ? p_row : p_col;
        
        // initialisation of prototypes
        ublas::mapped_matrix<T> l_matrix(p_row, p_col, l_diag);
        #pragma omp parallel for shared(l_matrix)
        for (std::size_t i=0; i < l_diag; ++i)
            l_matrix(i,i) = p_val;
        
        return l_matrix;
    }
    
    
    /** create a symmetric matrix of type T and
     * sets "1" on the diagonal elements
     * @param p_size element size
     * @return matrix of type T
     **/
    template<typename T> inline ublas::mapped_matrix<T> matrix::eye( const std::size_t& p_size )
    {
        return eye<T>(p_size, p_size);
    }
    
    
    /** creates a blas vector in which every element hold the minimum of the row / column elements of the matrix
     * @param p_matrix blas matrix
     * @param p_which row / column option (default row)
     * @return vector with minimum elements
     **/
    template<typename T> inline ublas::vector<T> matrix::min( const ublas::matrix<T>& p_matrix, const rowtype& p_which )
    {
        ublas::vector<T> l_min( (p_which==row) ? p_matrix.size1() : p_matrix.size2() );
        
        switch (p_which) {                
            case row :
                #pragma omp parallel for shared(l_min)
                for(std::size_t i=0; i < p_matrix.size1(); ++i)
                    l_min(i) = tools::vector::min( static_cast< ublas::vector<T> >(ublas::row(p_matrix, i)) );
                break;
                
            case column :
                #pragma omp parallel for shared(l_min)
                for(std::size_t i=0; i < p_matrix.size2(); ++i)
                    l_min(i) = tools::vector::min( static_cast< ublas::vector<T> >(ublas::column(p_matrix, i)) );
                break;
        }
        
        return l_min;
    }

    
    /** calulates from a blas matrix the mean values on the rows or columns
     * @param p_matrix blas matrix
     * @param p_which row / column option (default row)
     * @return vector with mean elements
     **/
    template<typename T> inline ublas::vector<T> matrix::mean( const ublas::matrix<T>& p_matrix, const rowtype& p_which )
    {
        ublas::vector<T> l_mean( (p_which==row) ? p_matrix.size1() : p_matrix.size2() );
        
        switch (p_which) {                
            case row :
                #pragma omp parallel for shared(l_mean)
                for(std::size_t i=0; i < p_matrix.size1(); ++i)
                    l_mean(i) = tools::vector::mean( static_cast< ublas::vector<T> >(ublas::row(p_matrix, i)) );
                break;
                
            case column :
                #pragma omp parallel for shared(l_mean)
                for(std::size_t i=0; i < p_matrix.size2(); ++i)
                    l_mean(i) = tools::vector::mean( static_cast< ublas::vector<T> >(ublas::column(p_matrix, i)) );
                break;
        }
        
        return l_mean;
    }
    
    
    /** calulates from a blas matrix the variance values on the rows or columns
     * @param p_matrix blas matrix
     * @param p_which row / column option (default row)
     * @return vector with mean elements
     **/
    template<typename T> inline ublas::vector<T> matrix::variance( const ublas::matrix<T>& p_matrix, const rowtype& p_which )
    {
        ublas::vector<T> l_var( (p_which==row) ? p_matrix.size1() : p_matrix.size2() );
        
        switch (p_which) {                
            case row :
                #pragma omp parallel for shared(l_var)
                for(std::size_t i=0; i < p_matrix.size1(); ++i)
                    l_var(i) = tools::vector::variance( static_cast< ublas::vector<T> >(ublas::row(p_matrix, i)) );
                break;
                
            case column :
                #pragma omp parallel for shared(l_var)
                for(std::size_t i=0; i < p_matrix.size2(); ++i)
                    l_var(i) = tools::vector::variance( static_cast< ublas::vector<T> >(ublas::column(p_matrix, i)) );
                break;
        }
        
        return l_var;
    }
    
    
    /** centering the matrix data (row or column orientated)
     * @param p_data input matrix
     * @param p_which row / column option (default column)
     * @return centered matrix
    **/
    template<typename T> inline ublas::matrix<T> matrix::centering( const ublas::matrix<T>& p_data, const rowtype& p_which )
    {
        ublas::matrix<T> l_center(p_data);
        
        switch (p_which) { 
            case row :
                #pragma omp parallel for shared(l_center)
                for(std::size_t i=0; i < l_center.size1(); ++i) {
                    T l_mean = tools::vector::mean( static_cast< ublas::vector<T> >(ublas::row(p_data,i)) );
                    for(std::size_t j=0; j < l_center.size2(); ++j)
                        l_center(i,j) -= l_mean;
                }
                break;
                
            case column :
                #pragma omp parallel for shared(l_center)
                for(std::size_t i=0; i < l_center.size2(); ++i) {
                    T l_mean = tools::vector::mean( static_cast< ublas::vector<T> >(ublas::column(p_data,i)) );
                    for(std::size_t j=0; j < l_center.size1(); ++j)
                        l_center(j,i) -= l_mean;
                }
                break;
        }
        
        return l_center;
    }
    
    
    /** centering the matrix data (row or column orientated)
     * @param p_data input matrix
     * @return doublecentered matrix
     **/
    template<typename T> inline ublas::matrix<T> matrix::doublecentering( const ublas::matrix<T>& p_data )
    {
        if (p_data.size1() != p_data.size2())
            throw exception::runtime( _("matrix must be square") );
        
        ublas::matrix<T> l_center(p_data.size1(), p_data.size2());
        
        #pragma omp parallel for shared(l_center)
        for(std::size_t i=0; i < p_data.size1(); ++i)
            for(std::size_t j=0; j < p_data.size2(); ++j)
                l_center(i,j) = p_data(i,i) + p_data(j,j) - (p_data(i,j)+p_data(j,i));
                
        return l_center;
    }
    
    
    /** sorts the rows or columns with the index elements of the vector 
     * @param p_matrix input matrix
     * @param p_idx index vector
     * @param p_which row / column option (default row)
    **/
    template<typename T> inline ublas::matrix<T> matrix::sort( const ublas::matrix<T>& p_matrix, const ublas::vector<std::size_t>& p_idx, const rowtype& p_which )
    {
        ublas::matrix<T> l_mat( p_matrix.size1(), p_matrix.size2() );
        std::size_t n=0;
        
        switch (p_which) {                
            case row    :   BOOST_FOREACH( std::size_t i, p_idx)
                                ublas::row(l_mat, n++) = ublas::row(p_matrix, i);
                            break;
                                
            case column :   BOOST_FOREACH( std::size_t i, p_idx)
                                ublas::column(l_mat, n++) = ublas::column(p_matrix, i);
                            break;
        }
        
        return l_mat;
    }
    
    
    /** creates a NxN matrix, in which the diagonal elements are set form the vector
     * @param p_vec input vector
     * @return NxN matrix
    **/
    template<typename T> inline ublas::mapped_matrix<T> matrix::diag( const ublas::vector<T>& p_vec )
    {
        ublas::mapped_matrix<T> l_mat(p_vec.size(), p_vec.size(), p_vec.size());
        
        #pragma omp parallel for shared(l_mat)
        for(std::size_t i=0; i < p_vec.size(); ++i)
            l_mat(i,i) = p_vec(i);
        
        return l_mat;
    }
    
    
    /** creates a vector with the diagonal elements of the matrix
     * @param p_matrix input matrix
     * @return ublas vector
    **/
    template<typename T> inline ublas::vector<T> matrix::diag( const ublas::matrix<T>& p_matrix )
    {
        ublas::vector<T> l_diag( std::min(p_matrix.size1(), p_matrix.size2()) );
    
        #pragma omp parallel for shared(l_diag)
        for(std::size_t i=0; i < l_diag.size(); ++i)
            l_diag(i) = p_matrix(i,i);
        
        return l_diag;
    }
    
    
    /** returns the trace of a matrix (sum diagonal elements)
     * @param p_matrix input matrix
     * @return trace value
     **/
    template<typename T> inline T matrix::trace( const ublas::matrix<T>& p_matrix )
    {
        return ublas::sum( diag(p_matrix) );
    }
    
    
    /** std::pow operator each element in the matrix
     * @param p_mat matrix
     * @param p_ex exponent
     * @return matrix which std::pow elements
     **/
    template<typename T> inline ublas::matrix<T> matrix::pow( const ublas::matrix<T>& p_mat, const T& p_ex )
    {
        ublas::matrix<T> l_mat(p_mat);
        
        #pragma omp parallel for shared(l_mat)
        for(std::size_t i=0; i < l_mat.size1(); ++i)
            for(std::size_t j=0; j < l_mat.size2(); ++j)
                l_mat(i,j) = std::pow(l_mat(i,j), p_ex);

        return l_mat;
    }
    
    
    /** returns a vector with the maximum values of each row or column
     * @param p_matrix input matrix
     * @param p_which row or column
     * @return maximum vector
     **/
    template<typename T> inline ublas::vector<T> matrix::max( const ublas::matrix<T>& p_matrix, const rowtype& p_which )
    {
        ublas::vector<T> l_max( (p_which==row) ? p_matrix.size1() : p_matrix.size2() );
        
        switch (p_which) {                
            case row :
                #pragma omp parallel for shared(l_max)
                for(std::size_t i=0; i < p_matrix.size1(); ++i)
                    l_max(i) = vector::max( static_cast< ublas::vector<T> >(ublas::row(p_matrix, i)) );
                break;
                
            case column :
                #pragma omp parallel for shared(l_max)
                for(std::size_t i=0; i < p_matrix.size2(); ++i)
                    l_max(i) = vector::max( static_cast< ublas::vector<T> >(ublas::column(p_matrix, i)) );
                break;
        }
        
        return l_max;
    }

    
    /** calculates sum of rows or columns
     * @param p_matrix input matrix
     * @param p_which row / column option (default row)
     * @return vector with row/coumn sum
    **/
    template<typename T> inline ublas::vector<T> matrix::sum( const ublas::matrix<T>& p_matrix, const rowtype& p_which )
    {
        ublas::vector<T> l_sum( (p_which==row) ? p_matrix.size1() : p_matrix.size2() );
        
        switch (p_which) {                
            case row :
                #pragma omp parallel for shared(l_sum)
                for(std::size_t i=0; i < p_matrix.size1(); ++i)
                    l_sum(i) = ublas::sum( static_cast< ublas::vector<T> >(ublas::row(p_matrix, i)) );
                break;
                
            case column :
                #pragma omp parallel for shared(l_sum)
                for(std::size_t i=0; i < p_matrix.size2(); ++i)
                    l_sum(i) = ublas::sum( static_cast< ublas::vector<T> >(ublas::column(p_matrix, i)) );
                break;
        }

        return l_sum;
    }
    
    
    
    /** create a covariance matrix from input matrix. Each row is an observation, and each column is a variable
     * @param p_input input data matrix
     * @return covariance matrix
    **/
    template<typename T> inline ublas::matrix<T> matrix::cov( const ublas::matrix<T>& p_input )
    {
        if (p_input.size1() == 0)
            throw exception::runtime(_("row size must be greater than zero"));
        
        ublas::vector<T> l_sum = sum(p_input, column);
        return (ublas::prod(ublas::trans(p_input),p_input) - ublas::outer_prod(l_sum, l_sum) / p_input.size1()) / (p_input.size1()-1);
    }
    
    
    /** changes numerical zero / datatype limit to a fixed value
     * @param p_mat input matrix
     * @param p_val fixed vakue
     * @return changed matrix
     **/
    template<typename T> inline ublas::matrix<T> matrix::setNumericalZero( const ublas::matrix<T>& p_mat, const T& p_val)
    {
        ublas::matrix<T> l_mat( p_mat );
        
        #pragma omp parallel for shared(l_mat)
        for(std::size_t i=0; i < l_mat.size1(); ++i)
            for(std::size_t j=0; j < l_mat.size2(); ++j)
                if (tools::function::isNumericalZero(l_mat(i,j)))
                    l_mat(i,j) = p_val; 
        
        return l_mat;
    }
    
    
    /** inverts a matrix for each element, if a element is numerical zero the inverted value is set to zero
     * @param p_matrix matrix
     * @return inverted matrix
     **/
    template<typename T> inline ublas::matrix<T> matrix::invert( const ublas::matrix<T>& p_matrix )
    {
        ublas::matrix<T> l_mat( p_matrix.size1(), p_matrix.size2(), 0 );
        
        #pragma omp parallel for shared(l_mat)
        for(std::size_t i=0; i < l_mat.size1(); ++i)
            for(std::size_t j=0; j < l_mat.size2(); ++j)
                if (!tools::function::isNumericalZero(p_matrix(i,j)))
                    l_mat(i,j) = static_cast<T>(1) / p_matrix(i,j);
        
        return l_mat;
    }
    
    
    /** repeats a column / row with a vector
     * @param p_vec input vector
     * @param p_which row / column option (default row)
     * @return squared matrix
    **/
    template<typename T> inline ublas::matrix<T> matrix::repeat( const ublas::vector<T>& p_vec, const rowtype& p_which )
    {
        ublas::matrix<T> l_mat( p_vec.size(), p_vec.size() );
        
        switch (p_which) {                
            case row :
                #pragma omp parallel for shared(l_mat)
                for(std::size_t i=0; i < p_vec.size(); ++i)
                    ublas::row(l_mat, i) = p_vec;
                break;
                
            case column :
                #pragma omp parallel for shared(l_mat)
                for(std::size_t i=0; i < p_vec.size(); ++i)
                    ublas::column(l_mat, i) = p_vec;
                break;                
        }
        
        return l_mat;
    }
    
    
    /** repeats a column / row with a vector
     * @param p_vec input vector
     * @param p_num number of repeats
     * @param p_which row / column option (default row)
     * @return matrix with repeated vector (p_num times)
     **/
    template<typename T> inline ublas::matrix<T> matrix::repeat( const ublas::vector<T>& p_vec, const std::size_t& p_num, const rowtype& p_which )
    {
        ublas::matrix<T> l_mat( ((p_which == row) ? p_num : p_vec.size()) , ((p_which == column) ? p_num : p_vec.size()) );
        
        switch (p_which) {                
            case row :
                #pragma omp parallel for shared(l_mat)
                for(std::size_t i=0; i < p_num; ++i)
                    ublas::row(l_mat, i) = p_vec;
                break;
                
            case column :
                #pragma omp parallel for shared(l_mat)
                for(std::size_t i=0; i < p_num; ++i)
                    ublas::column(l_mat, i) = p_vec;
                break;                
        }
        
        return l_mat;
    }
        
}}
#endif
