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

#ifndef __MACHINELEARNING_DIMENSIONREDUCE_NONSUPERVISED_LLE_HPP
#define __MACHINELEARNING_DIMENSIONREDUCE_NONSUPERVISED_LLE_HPP


#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>

#include "reduce.hpp"
#include "../../neighborhood/neighborhood.h"
#include "../../errorhandling/exception.hpp"
#include "../../tools/tools.h"

#include <iostream>
#include <boost/numeric/ublas/io.hpp>


namespace machinelearning { namespace dimensionreduce { namespace nonsupervised {
    
    namespace ublas  = boost::numeric::ublas;
    
    
    /** create the local linear embedding (LLE) **/
    template<typename T> class lle : public reduce<T>
    {
        
        public :
        
            lle( const neighborhood::neighborhood<T>&, const std::size_t& );
            ublas::matrix<T> map( const ublas::matrix<T>& );
            std::size_t getDimension( void ) const;
        
        
        private :
        
            /** neighborhood object **/
            const neighborhood::neighborhood<T>& m_neighborhood;
            /** target dimension **/
            const std::size_t m_dim;
        
    };

    

    /** constructor
     * @param p_neighborhood neighborhood object
     * @param p_dim target dimension
     **/
    template<typename T> inline lle<T>::lle( const neighborhood::neighborhood<T>& p_neighborhood, const std::size_t& p_dim ) :
        m_neighborhood( p_neighborhood ),
        m_dim( p_dim )
    {
        if (p_dim == 0)
            throw exception::runtime(_("dimension must be greater than zero"), *this);
    }
    
    
    /** returns the target dimensione size
     * @return number of dimension
     **/
    template<typename T> inline std::size_t lle<T>::getDimension( void ) const
    {
        return m_dim;
    }
    
    
    /** caluate and project the input data
     * @todo project matrix change to a sparse matrix if arpack can used with boost
     * @bug it is uncomplete
     * @param p_data input datamatrix
     * @return matrix with mapped points
     **/
    template<typename T> inline ublas::matrix<T> lle<T>::map( const ublas::matrix<T>& p_data )
    {
        if (p_data.size2() <= m_dim)
            throw exception::runtime(_("data points are less than target dimension"), *this);
        
        // if number of neighborhood greate than data dimension (column size)
        // regularize weight-matrix
        const T l_tolerance = 1.0/10000.0;
        const ublas::matrix<T> l_regular = tools::matrix::diag<T>( ublas::vector<T>(m_neighborhood.getNeighborCount(), l_tolerance) );
        const bool l_regularize = m_neighborhood.getNeighborCount() > p_data.size2();
        
        // calculate neighborhood index and create some structires
        const ublas::scalar_vector<T> l_ones(m_neighborhood.getNeighborCount(), 1);
        const ublas::matrix<std::size_t> l_neighborhood = m_neighborhood.get( p_data );
        ublas::matrix<T> l_weight(p_data.size1(), m_neighborhood.getNeighborCount());
        
        // calculate weight matrix
        for(std::size_t i=0; i < p_data.size1(); ++i) {
        
            // subtract every point from their neighbors (centering neighbors to the point)
            ublas::matrix<T> l_local( m_neighborhood.getNeighborCount(), p_data.size2() );
            for(std::size_t j=0; j < m_neighborhood.getNeighborCount(); ++j)
                ublas::row(l_local, j) = ublas::row(p_data, l_neighborhood(i, j)) - ublas::row(p_data, i);
                    
            // symmetrize matrix (add tolerance)
            ublas::vector<T> l_result;
            ublas::matrix<T> l_localweight = ublas::prod(l_local, ublas::trans(l_local));
            
            if (l_regularize)
                l_localweight += l_regular * tools::matrix::trace<T>(l_localweight);

            // solve the lineare equation and normalize
            tools::lapack::solve<T>( l_localweight, l_ones, l_result );
            ublas::row(l_weight, i) = l_result / ublas::sum(l_result);
        }
 
        
        // create weight matrix (I-W)' * (I-W) for the data
        ublas::mapped_matrix<T> l_project = tools::matrix::eye<T>(p_data.size1());
        for(std::size_t i=0; i < l_weight.size1(); ++i) {
            
            const ublas::vector<std::size_t> l_ndidx = ublas::row(l_neighborhood, i);
            const ublas::vector<T> l_pointweights    = ublas::row(l_weight, i);
            const ublas::matrix<T> l_mat             = ublas::outer_prod(l_pointweights, l_pointweights);
            
            for(std::size_t j=0; j < l_pointweights.size(); ++j) {
                l_project(i, l_ndidx(j)) -= l_pointweights(j);
                l_project(l_ndidx(j), i) -= l_pointweights(j);
                
                for(std::size_t n=0; n < l_pointweights.size(); ++n)
                    l_project(l_ndidx(j),l_ndidx(n))    += l_mat(j, n);
            }
        }
        
        // calculate the eigenvalues & -vectors
        ublas::vector<T> l_eigenvalues;
        ublas::matrix<T> l_eigenvectors;
        tools::lapack::eigen<T>(l_project, l_eigenvalues, l_eigenvectors);
         
        std::cout << l_eigenvalues << std::endl;
        
        return l_eigenvectors;
        
    }

}}}
#endif
