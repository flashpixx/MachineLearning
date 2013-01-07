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


#ifndef __MACHINELEARNING_TOOLS_LAPACK_HPP
#define __MACHINELEARNING_TOOLS_LAPACK_HPP

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/bindings/blas.hpp>
#include <boost/numeric/bindings/ublas/vector.hpp>
#include <boost/numeric/bindings/ublas/matrix.hpp>
#include <boost/numeric/bindings/lapack/driver/geev.hpp>
#include <boost/numeric/bindings/lapack/driver/ggev.hpp>
#include <boost/numeric/bindings/lapack/driver/gesv.hpp> 
#include <boost/numeric/bindings/lapack/driver/gesvd.hpp>
#include <boost/numeric/bindings/lapack/computational/hseqr.hpp>


#include "../errorhandling/exception.hpp"
#include "language/language.h"


namespace machinelearning { namespace tools {
    
    #ifndef SWIG
    namespace ublas     = boost::numeric::ublas;
    namespace blas      = boost::numeric::bindings::blas;
    namespace linalg    = boost::numeric::bindings::lapack;
    #endif
    
    
    /** class for connection LAPACK calls with numerical bindings.
     * We can handle only dense / full matrix data with LAPACK
     * @see http://svn.boost.org/svn/boost/sandbox/numeric_bindings
     * @todo add Johnson Lindenstrauss Lemma for orthognal high dimensional matrices http://en.wikipedia.org/wiki/Johnson%E2%80%93Lindenstrauss_lemma
     * @todo try to change the matrix copy in eigen / svd to a better option (eg bindings::trans or traits )
     * @todo add matrix exponential via Pade approximation (shown in Matlab with expmdemo)
     * @todo add support for http://en.wikipedia.org/wiki/Matrix_Template_Library
     * @todo add support for http://icl.cs.utk.edu/plasma/index.html
     * @todo add support for http://icl.cs.utk.edu/magma/
     * @todo add support for http://www.netlib.org/scalapack/
     * @todo portage this class to the Intel Math Kernel Library http://software.intel.com/en-us/articles/intel-mkl/
     * @todo adding support for PETSc http://www.mcs.anl.gov/petsc/
     * @todo adding support for http://en.wikipedia.org/wiki/List_of_numerical_libraries
     **/
    class lapack
    {
        
        public :
        
            template<typename T> static void eigen( const ublas::matrix<T>&, ublas::vector<T>&, ublas::matrix<T>&, const bool& = true );
            template<typename T> static void eigen( const ublas::matrix<T>&, const ublas::matrix<T>&, ublas::vector<T>&, ublas::matrix<T>&, const bool& = true );
            template<typename T> static void svd( const ublas::matrix<T>&, ublas::vector<T>&, ublas::matrix<T>&, ublas::matrix<T>&, const bool& = true );
            template<typename T> static void solve( const ublas::matrix<T>&, const ublas::vector<T>&, ublas::vector<T>& );
            //template<typename T> static ublas::matrix<T> expm( const ublas::matrix<T>& );
            template<typename T> static ublas::vector<T> perronfrobenius( const ublas::matrix<T>&, const std::size_t& );
            template<typename T> static ublas::vector<T> perronfrobenius( const ublas::matrix<T>&, const std::size_t&, const ublas::vector<T>& );
            template<typename T> static ublas::matrix<T> unnormalizedGraphLaplacian( const ublas::matrix<T>& );
            template<typename T> static ublas::matrix<T> normalizedGraphLaplacian( const ublas::matrix<T>& );
        
    };

    
    
    /** calulate from a NxN and a diagonalized matrix the generalized eigenvalues and -vectors
     * @param p_matrix input matrix
     * @param p_diag diagonal matrix
     * @param p_eigval blas vector for eigenvalues [initialisation is not needed]
     * @param p_eigvec blas matrix for (normalized) eigenvectors (every column is a eigenvector) [initialisation is not needed]
     * @param p_normalize normalize eigenvectors
     **/
    template<typename T> inline void lapack::eigen( const ublas::matrix<T>& p_matrix, const ublas::matrix<T>& p_diag, ublas::vector<T>& p_eigval, ublas::matrix<T>& p_eigvec, const bool& p_normalize )
    {
        if (  (p_matrix.size1() != p_matrix.size2()) || (p_diag.size1() != p_diag.size2())  )
            throw exception::runtime( _("matrix must be square") );
        if ( (p_matrix.size1() != p_diag.size1()) || (p_matrix.size2() != p_diag.size2()) )
            throw exception::runtime( _("both matrices have not the same size") );
        
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
        
        // determine generalized eigenvector without sorting and calculate eigenvalues
        linalg::ggev( 'N', 'V', l_matrix, l_diag, l_eigval,  l_tmp1,  l_div,  l_tmp2,  l_eigvec, linalg::optimal_workspace() );
        
        // calculate eigenvalues
        for(std::size_t i=0; i < l_eigval.size(); ++i)
            l_eigval(i) /= l_div(i);
        
        // normalize every eigenvector
        if (p_normalize)
            for(std::size_t i=0; i < l_eigvec.size2(); ++i)
                ublas::column(l_eigvec, i) /= blas::nrm2( static_cast< ublas::vector<T> >(ublas::column(l_eigvec, i)) );        
        
        // we must copy the reference
        p_eigvec = l_eigvec;
        p_eigval = l_eigval;
    }
    
    
    /** calculates from a NxN matrix eigenvalues and eigenvector
     * @param p_matrix input matrix
     * @param p_eigval blas vector for eigenvalues [initialisation is not needed]
     * @param p_eigvec blas matrix for (normalized) eigenvectors (every column is a eigenvector) [initialisation is not needed]
     * @param p_normalize normalize eigenvectors
    **/
    template<typename T> inline void lapack::eigen( const ublas::matrix<T>& p_matrix, ublas::vector<T>& p_eigval, ublas::matrix<T>& p_eigvec, const bool& p_normalize )
    {
        if (p_matrix.size1() != p_matrix.size2())
            throw exception::runtime(_("matrix must be square"));
        
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
        if (p_normalize)
            for(std::size_t i=0; i < l_eigvec.size2(); ++i)
                ublas::column(l_eigvec, i) /= blas::nrm2( static_cast< ublas::vector<T> >(ublas::column(l_eigvec, i)) );
        
        // we must copy the reference
        p_eigvec = l_eigvec;
        p_eigval = l_eigval;
    }
    
    
    /** singular value decomposition
     * @param p_matrix input matrix
     * @param p_svdval blas vector for eigenvalues [initialisation is not needed]
     * @param p_svdvec1 blas matrix for singular values vectors (every column is a vector) [initialisation is not needed]
     * @param p_svdvec2 blas matrix for singular values vectors (every column is a vector) [initialisation is not needed]
     * @param p_normalize normalize svd vectors
     **/
    template<typename T> inline void lapack::svd( const ublas::matrix<T>& p_matrix, ublas::vector<T>& p_svdval, ublas::matrix<T>& p_svdvec1, ublas::matrix<T>& p_svdvec2, const bool& p_normalize )
    {
        // copy matrix for LAPACK
        ublas::matrix<T, ublas::column_major> l_matrix(p_matrix);
        
        // create result structures
        ublas::matrix<T, ublas::column_major> l_svdvec1(l_matrix.size1(), l_matrix.size1());
        ublas::matrix<T, ublas::column_major> l_svdvec2(l_matrix.size2(), l_matrix.size2());
        ublas::vector<T> l_svdval( l_matrix.size1() );

        // determine svd without sorting
        linalg::gesvd( 'A', 'A', l_matrix, l_svdval, l_svdvec1, l_svdvec2, linalg::optimal_workspace() );
        
        // second matrix must be transpose
        l_svdvec2 = ublas::trans( l_svdvec2 );
        
        // normalize
        if (p_normalize)
        {
            for(std::size_t i=0; i < l_svdvec1.size2(); ++i)
                ublas::column(l_svdvec1, i) /= blas::nrm2( static_cast< ublas::vector<T> >(ublas::column(l_svdvec1, i)) );
            for(std::size_t i=0; i < l_svdvec2.size2(); ++i)
                ublas::column(l_svdvec2, i) /= blas::nrm2( static_cast< ublas::vector<T> >(ublas::column(l_svdvec2, i)) );
        }
        
        // we must copy the reference
        p_svdval    = l_svdval;
        p_svdvec1   = l_svdvec1;
        p_svdvec2   = l_svdvec2;
    }
    
    
    /** solve a lineare equation system like Ax=b
     * @param p_matrix left side matrix
     * @param p_vec right side vector
     * @param p_solve solved equation
     * @bug gesv does not work, create compiler error 
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
  
     
    /** generates the eigenvalue / -vector with the perronforbenius theorem
     * @see http://en.wikipedia.org/wiki/Perron–Frobenius_theorem
     * @param p_matrix input matrix
     * @param p_iteration number of iterations
     * @return eigenvector
     **/
    template<typename T> inline ublas::vector<T> lapack::perronfrobenius( const ublas::matrix<T>& p_matrix, const std::size_t& p_iteration )
    {
        return perronfrobenius( p_matrix, p_iteration, vector::random<T>(p_matrix.size1()) );
    }
    
    
    /** generates the eigenvalue / -vector with the perronforbenius theorem
     * @see http://en.wikipedia.org/wiki/Perron–Frobenius_theorem
     * @param p_matrix input matrix
     * @param p_iteration number of iterations
     * @param p_init initialization vector
     * @return eigenvector
     **/
    template<typename T> inline ublas::vector<T> lapack::perronfrobenius( const ublas::matrix<T>& p_matrix, const std::size_t& p_iteration, const ublas::vector<T>& p_init )
    {
        if (p_matrix.size1() != p_matrix.size2())
            throw exception::runtime(_("matrix must be square"));
        if (p_matrix.size1() != p_init.size())
            throw exception::runtime(_("number of rows / columns of the matrix must have the same dimension like the initialisation vector"));
        
        ublas::vector<T> l_vec = p_init;
        for(std::size_t i=0; i < p_iteration; ++i) {
            l_vec  = ublas::prod(p_matrix, l_vec);
            l_vec /= blas::nrm2( l_vec );
        }
        
        return l_vec;
    }
    
    
    /** creates the unnormalized graph laplacian of a distance matrix
     * @param p_adjacency adjacency matrix
     * @return graph laplacian
     **/
    template<typename T> inline ublas::matrix<T> lapack::unnormalizedGraphLaplacian( const ublas::matrix<T>& p_adjacency )
    {
        if (p_adjacency.size1() != p_adjacency.size2())
            throw exception::runtime(_("matrix must be square"));
        
        // change the distance matrix to a degree matrix
        const ublas::vector<T> l_vertexdegree       = matrix::sum(p_adjacency);
        const ublas::matrix<T> l_degree             = matrix::diag(l_vertexdegree);
        
        // degree minus adjacency matrix
        return l_degree - p_adjacency;
    }
    
    
    /** creates the normalized graph laplacian of a distance matrix
     * @param p_adjacency adjacency matrix
     * @return normalized graph laplacian
     **/
    template<typename T> inline ublas::matrix<T> lapack::normalizedGraphLaplacian( const ublas::matrix<T>& p_adjacency )
    {
        if (p_adjacency.size1() != p_adjacency.size2())
            throw exception::runtime(_("matrix must be square"));
        
        // change the distance matrix to a degree matrix
        const ublas::vector<T> l_vertexdegree       = matrix::sum(p_adjacency);
        const ublas::matrix<T> l_degree             = matrix::diag(l_vertexdegree);
        
        // degree minus adjacency matrix
        const ublas::matrix<T> l_unnormlaplacian    = l_degree - p_adjacency;
        
        // normalized laplacian: multiply l_degree^-1 * l_unnormlaplacian
        // degree matrix is a diagnomal matrix, so invert the elements and do a matrix product
        return ublas::prod( matrix::invert(l_degree), l_unnormlaplacian );
    }

}}
#endif
