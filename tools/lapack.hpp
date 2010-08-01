/** 
 @cond
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
 @endcond
 **/


#ifndef MACHINELEARNING_TOOLS_LAPACK_HPP
#define MACHINELEARNING_TOOLS_LAPACK_HPP

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/bindings/blas.hpp>
#include <boost/numeric/bindings/lapack/driver/geev.hpp>
#include <boost/numeric/bindings/lapack/driver/ggev.hpp>
#include <boost/numeric/bindings/lapack/driver/gesv.hpp> 


#include "../exception/exception.h"



namespace machinelearning { namespace tools {
    
    namespace ublas     = boost::numeric::ublas;
	namespace blas      = boost::numeric::bindings::blas;
    namespace bindings  = boost::numeric::bindings;
    namespace linalg    = boost::numeric::bindings::lapack;
    
    
    
    /** class for connection LAPACK calls with numerical bindings.
     * We can handle only dense / full matrix data with LAPACK
     **/
    class lapack {
        
        public :
        
            template<typename T> static void eigen( const ublas::matrix<T>&, ublas::vector<T>&, ublas::matrix<T>& );
            template<typename T> static void eigen( const ublas::matrix<T>&, const ublas::matrix<T>&, ublas::vector<T>&, ublas::matrix<T>& );
            template<typename T> static void solve( const ublas::matrix<T>&, const ublas::vector<T>&, ublas::vector<T>& );
        
    };

    
    
    /** calulate from a NxN and a diagonalized matrix the generalized eigenvalues and -vectors
     * @overload
     * @param p_matrix input matrix
     * @param p_diag diagonal matrix
     * @param p_eigval blas vector for sorted eigenvalues [initialisation is not needed]
     * @param p_eigvec blas matrix for (normalized) eigenvectors (every column is a eigenvector) [initialisation is not needed]
     **/
    template<typename T> inline void lapack::eigen( const ublas::matrix<T>& p_matrix, const ublas::matrix<T>& p_diag, ublas::vector<T>& p_eigval, ublas::matrix<T>& p_eigvec )
    {
        if (  (p_matrix.size1() != p_matrix.size2()) || (p_diag.size1() != p_diag.size2())  )
            throw exception::matrix("matrix are not symmetric");
        if ( (p_matrix.size1() != p_diag.size1()) || (p_matrix.size2() != p_diag.size2()) )
            throw exception::matrix("both matrices have not the same size");
        
        // copy matrix for LAPACK
        ublas::matrix<T, ublas::column_major> l_matrix(p_matrix);
        ublas::matrix<T, ublas::column_major> l_diag(p_diag);
        
        // create result structures
        ublas::matrix<T, ublas::column_major> l_eigvec(l_matrix.size1(), l_matrix.size2());
        ublas::vector<T> l_eigval(l_matrix.size1());
        ublas::vector<T> l_div(l_matrix.size1());
        
        // need temporary structures
        ublas::vector<T> l_tmp1(l_eigval.size());
        ublas::matrix<T, ublas::column_major> l_tmp2(l_matrix.size1(),l_matrix.size2());
        
        // determine eigenvector without sorting and calculate eigenvalues
        linalg::ggev( 'N', 'V', l_matrix, l_diag, l_eigval,  l_tmp1,  l_div,  l_tmp2,  l_eigvec, linalg::optimal_workspace() );
        
        // calculate eigenvalues
        for(std::size_t i=0; i < l_eigval.size(); ++i)
            if (!function::isNumericalZero<T>(l_div(i)))
                l_eigval(i) /= l_div(i);
  		
		// normalize every eigenvector
		for(std::size_t i=0; i < l_eigvec.size2(); ++i) {
			const T l_val = blas::nrm2( static_cast< ublas::vector<T> >(ublas::column(l_eigvec, i)) );
            if (!function::isNumericalZero<T>(l_val))
                ublas::column(l_eigvec, i) /= l_val;
        }
		
        
        // we must copy the reference
        p_eigvec = l_eigvec;
        p_eigval = l_eigval;
    }
    
    
    /** calculates from a NxN matrix eigenvalues and eigenvector
     * @overload
     * @param p_matrix input matrix
     * @param p_eigval blas vector for sorted eigenvalues [initialisation is not needed]
     * @param p_eigvec blas matrix for (normalized) eigenvectors (every column is a eigenvector) [initialisation is not needed]
    **/
    template<typename T> inline void lapack::eigen( const ublas::matrix<T>& p_matrix, ublas::vector<T>& p_eigval, ublas::matrix<T>& p_eigvec )
    {
        if (p_matrix.size1() != p_matrix.size2())
            throw exception::matrix("matrix are not symmetric");
        
        // copy matrix for LAPACK
        ublas::matrix<T, ublas::column_major> l_matrix(p_matrix);
        
        // create result structures
        ublas::matrix<T, ublas::column_major> l_eigvec(l_matrix.size1(), l_matrix.size2());
        ublas::vector<T> l_eigval(l_matrix.size1());
        
        // need temporary structures
        ublas::vector<T> l_tmp1(l_eigval.size());
        ublas::matrix<T, ublas::column_major> l_tmp2(l_matrix.size1(),l_matrix.size2());
        ublas::matrix<T, ublas::column_major> l_tmp3(l_matrix.size1(),l_matrix.size2());
        
        // determine eigenvector without sorting
        linalg::geev( 'N', 'V', l_matrix, l_eigval,  l_tmp1,l_tmp2,  l_eigvec, linalg::optimal_workspace() );
        
		// normalize every eigenvector
		for(std::size_t i=0; i < l_eigvec.size2(); ++i) {
			const T l_val = blas::nrm2( static_cast< ublas::vector<T> >(ublas::column(l_eigvec, i)) );
            if (!function::isNumericalZero<T>(l_val))
                ublas::column(l_eigvec, i) /= l_val;
		}
        
        // we must copy the reference
        p_eigvec = l_eigvec;
        p_eigval = l_eigval;
    }
    
    
    /** solve a lineare equation system like Ax=b
     * @param p_matrix left side matrix
     * @param p_vec right side vector
     * @param p_solve solved equation
    **/
    template<typename T> inline void lapack::solve( const ublas::matrix<T>& p_matrix, const ublas::vector<T>& p_vec, ublas::vector<T>& p_solve )
    {
        // copy matrix for LAPACK
        ublas::matrix<T, ublas::column_major> l_matrix(p_matrix);
        ublas::matrix<T, ublas::column_major> l_right(p_vec.size(),1);
        ublas::column(l_right,0) = p_vec;
        
         // need temporary structures
        ublas::vector<std::ptrdiff_t> l_pivots(p_vec.size());
        
        // solve the lineare equation system
        linalg::gesv(l_matrix, l_pivots, l_right);
        
        // we must copy the reference
        p_solve = static_cast< ublas::vector<T> >(ublas::column(l_right,0));
    }
    
    

};};
#endif