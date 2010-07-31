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


/** template header file with implementation for some matrix functions **/


#ifndef MACHINELEARNING_TOOLS_MATRIX_HPP
#define MACHINELEARNING_TOOLS_MATRIX_HPP

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

#include "../exception/exception.h"
//#include "../distances/distances.h"
#include "vector.hpp"
#include "function.hpp"



namespace machinelearning { namespace tools {
    
    namespace ublas     = boost::numeric::ublas;
    
    
    /** class for matrix operations **/
    class matrix {
    
        public :
    
            enum rowtype
            {
                row     = 0,
                column  = 1
            };
        
        
            template<typename T> static ublas::matrix<T> random( const std::size_t&, const std::size_t&, const tools::random::distribution& = tools::random::uniform );
            template<typename T> static ublas::matrix<T> random( const std::size_t&, const tools::random::distribution& = tools::random::uniform );
            template<typename T> static ublas::matrix<T> eye( const std::size_t&, const std::size_t& );
            template<typename T> static ublas::matrix<T> eye( const std::size_t& );
            template<typename T> static ublas::matrix<T> pow( const ublas::matrix<T>&, const T& );
            template<typename T> static ublas::vector<T> min( const ublas::matrix<T>&, const rowtype& = row );
            template<typename T> static ublas::vector<T> mean( const ublas::matrix<T>&, const rowtype& = row );
            template<typename T> static ublas::vector<T> sum( const ublas::matrix<T>&, const rowtype& = row );
            template<typename T> static ublas::matrix<T> diag( const ublas::vector<T>& );
            template<typename T> static ublas::vector<T> diag( const ublas::matrix<T>& );
            template<typename T> static T trace( const ublas::matrix<T>& );
			template<typename T> static ublas::matrix<T> centering( const ublas::matrix<T>&, const rowtype& = column );
            template<typename T> static ublas::matrix<T> sort( const ublas::matrix<T>&, const ublas::vector<std::size_t>&, const rowtype& p_which = row);
            template<typename T> static ublas::matrix<T> cov( const ublas::matrix<T>& );
            template<typename T> static ublas::matrix<T> setNumericalZero( const ublas::matrix<T>&, const T& = 0);
        
    };
    
    
    
    
    /** creates a random NxM matrix of type T
     * @overload
     * @param p_row rows
     * @param p_col columns
     * @param p_distribution name of the distribution
     * @return matrix of type T
     **/
    template<typename T> inline ublas::matrix<T> matrix::random( const std::size_t& p_row, const std::size_t& p_col, const tools::random::distribution& p_distribution )
    {
        if (tools::function::isNumericalZero(p_row))
            throw exception::greaterthanzero("row size");
        if (tools::function::isNumericalZero(p_col))
            throw exception::greaterthanzero("column size");
                
        // initialisation of prototypes
        ublas::matrix<T> l_matrix(p_row, p_col);
        for (std::size_t i=0; i < p_row; ++i)
            for (std::size_t j=0; j < p_col; ++j)
                l_matrix(i,j) = tools::random::get<T>( p_distribution );
        
        return l_matrix;
    }
    
    
    /** create a random symmetric matrix of type T
     * @overload
     * @param p_size element size
     * @param p_distribution name of the distribution
     * @return matrix of type T
     **/
    template<typename T> inline ublas::matrix<T> matrix::random( const std::size_t& p_size, const tools::random::distribution& p_distribution )
    {
        return random<T>(p_size, p_size, p_distribution);
    }
    
    
    /** creates a matrix and sets "1" on the diagonal elements
     * @overload
     * @param p_row rows
     * @param p_col columns
     * @return matrix of type T
     **/      
    template<typename T> inline ublas::matrix<T> matrix::eye( const std::size_t& p_row, const std::size_t& p_col )
    {
        if (tools::function::isNumericalZero(p_row))
            throw exception::greaterthanzero("row size");
        if (tools::function::isNumericalZero(p_col))
            throw exception::greaterthanzero("column size");
        
        const std::size_t l_diag = (p_row < p_col) ? p_row : p_col;
        
        // initialisation of prototypes
        ublas::matrix<T> l_matrix(p_row, p_col, 0);
        for (std::size_t i=0; i < l_diag; ++i)
            l_matrix(i,i) = 1;
        
        return l_matrix;
    }   
    
    
    /** create a symmetric matrix of type T and
     * sets "1" on the diagonal elements
     * @overload
     * @param p_size element size
     * @return matrix of type T
     **/
    template<typename T> inline ublas::matrix<T> matrix::eye( const std::size_t& p_size )
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
                for(std::size_t i=0; i < p_matrix.size1(); ++i)
                    l_min(i) = tools::vector::min( static_cast< ublas::vector<T> >(ublas::row(p_matrix, i)) );
                break;
                
            case column :
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
        ublas::vector<T> l_min( (p_which==row) ? p_matrix.size1() : p_matrix.size2() );
        
        switch (p_which) {                
            case row :
                for(std::size_t i=0; i < p_matrix.size1(); ++i)
                    l_min(i) = tools::vector::mean( static_cast< ublas::vector<T> >(ublas::row(p_matrix, i)) );
                break;
                
                case column :
                for(std::size_t i=0; i < p_matrix.size2(); ++i)
                    l_min(i) = tools::vector::mean( static_cast< ublas::vector<T> >(ublas::column(p_matrix, i)) );
                break;
        }
        
        return l_min;
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
					for(std::size_t i=0; i < l_center.size1(); ++i) {
						T l_mean = tools::vector::mean( static_cast< ublas::vector<T> >(ublas::row(p_data,i)) );
						for(std::size_t j=0; j < l_center.size2(); ++j)
							l_center(i,j) -= l_mean;
					}
					break;
				
			case column :
				for(std::size_t i=0; i < l_center.size2(); ++i) {
					T l_mean = tools::vector::mean( static_cast< ublas::vector<T> >(ublas::column(p_data,i)) );
					for(std::size_t j=0; j < l_center.size1(); ++j)
						l_center(j,i) -= l_mean;
				}
				break;
		}
		
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
    template<typename T> inline ublas::matrix<T> matrix::diag( const ublas::vector<T>& p_vec )
    {
        ublas::matrix<T> l_mat(p_vec.size(), p_vec.size(), 0);
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
        
        for(std::size_t i=0; i < l_mat.size1(); ++i)
            for(std::size_t j=0; j < l_mat.size2(); ++j)
                l_mat(i,j) = std::pow(l_mat(i,j), p_ex);
        
        return p_mat;
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
                for(std::size_t i=0; i < p_matrix.size1(); ++i)
                    l_sum(i) = ublas::sum( static_cast< ublas::vector<T> >(ublas::row(p_matrix, i)) );
                break;
                
            case column :
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
        if (tools::function::isNumericalZero(p_input.size1()))
            throw exception::greaterthanzero("row size");
        
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
        
        for(std::size_t i=0; i < l_mat.size1(); ++i)
            for(std::size_t j=0; j < l_mat.size2(); ++j)
                if (tools::function::isNumericalZero(l_mat(i,j)))
                    l_mat(i,j) = p_val; 
        
        return l_mat;
    }
    
    
    /** creates a similarity matrix from a dissimilarity matrix
     * @param p_dissimilarity input dissimilarity matrix
     * @param p_distance distance object for converting
     * @return similarity matrix
     **
    template<typename T> inline ublas::matrix<T> matrix::similarity( const ublas::matrix<T>& p_dissimilarity) //, const distances::distance<T>& p_distance )
    {
        return p_dissimilarity;
    }
    
    
    ** creates a dissimilarity matrix from a similarity matrix
     * @param p_similarity input similarity matrix
     * @param p_distance distance object for converting
     * @return dissimilarity matrix
     **
    template<typename T> inline ublas::matrix<T> matrix::dissimilarity( const ublas::matrix<T>& p_similarity) //, const distances::distance<T>& p_distance )
    {
        // as.dist(sqrt(outer(diag(mx), diag(mx), "+") - 2*mx)) 
        return p_similarity;
    }*/
       
};};

#endif