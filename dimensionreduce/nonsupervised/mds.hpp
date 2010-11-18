/** 
 @cond
 #########################################################################
 # GPL License                                                           #
 #                                                                       #
 # This file is part of the Machine Learning Framework.                  #
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

#ifndef MACHINELEARNING_DIMENSIONREDUCE_NONSUPERVISED_MDS_HPP
#define MACHINELEARNING_DIMENSIONREDUCE_NONSUPERVISED_MDS_HPP


#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>

#include "../dimensionreduce.hpp"
#include "../../exception/exception.h"
#include "../../tools/tools.h"


namespace machinelearning { namespace dimensionreduce { namespace nonsupervised {
    
    namespace ublas  = boost::numeric::ublas;
    
    
    /** create the Multidimensional scaling (MDS) with different algorithms **/
    template<typename T> class mds : public nonsupervisedreduce<T> {
        
        public :
        
            enum project {
                metric          = 0,
                sammon          = 1,
                hit             = 2,    // http://dig.ipk-gatersleben.de/hitmds/hitmds.html
            };
        
        
            mds( const std::size_t&, const project& = metric, const std::size_t& = 0 );
            ublas::matrix<T> map( const ublas::matrix<T>& );
            std::size_t getDimension( void ) const;
        
        
        private :
        
            /** number of iterations for sammon **/
            const std::size_t m_iteration;
            /** target dimension **/
            const std::size_t m_dim;
            /** project type **/
            const project m_type;
        
        
            ublas::matrix<T> project_metric( const ublas::matrix<T>& );
            ublas::matrix<T> project_sammon( const ublas::matrix<T>& );
        
            ublas::matrix<T> distance( const ublas::matrix<T>&, const bool& = false ) const;
            ublas::matrix<T> doublecentering( const ublas::matrix<T>& ) const;

    };


    /** constructor
     * @param p_dim target dimension
     * @param p_type project type
     * @param p_iteration number of iterations for sammon
     **/
    template<typename T> inline mds<T>::mds( const std::size_t& p_dim, const project& p_type, const std::size_t& p_iteration ) :
        m_iteration( p_iteration ),
        m_dim( p_dim ),
        m_type( p_type )
    {
        if (p_dim == 0)
            throw exception::runtime(_("dimension must be greater than zero"));
    }
    
    
    /** returns the target dimensione size
     * @return number of dimension
     **/
    template<typename T> inline std::size_t mds<T>::getDimension( void ) const
    {
        return m_dim;
    }
    
    
    /** caluate and project the input data
     * @param p_data input datamatrix (similarity matrix)
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::map( const ublas::matrix<T>& p_data )
    {
        if (p_data.size2() <= m_dim)
            throw exception::runtime(_("datapoint dimension are less than target dimension"));
        if (p_data.size1() != p_data.size2())
            throw exception::runtime( _("matrix must be square") );
        
        switch (m_type) {
                
            case metric:
                return project_metric(p_data);
                
            case sammon:
                return project_sammon(p_data);
                
                
            default :
                throw exception::runtime(_("project option is unkown"));
        };
    }
    
    
    
    /** caluate the metric MDS (for metric we use eigenvalues)
     * @param p_data input datamatrix (similarity matrix)
     * @return mapped data
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::project_metric( const ublas::matrix<T>& p_data )
    {
        // calculate the eigenvalues & -vectors
        ublas::vector<T> l_eigenvalues;
        ublas::matrix<T> l_eigenvectors;
        tools::lapack::eigen<T>(p_data, l_eigenvalues, l_eigenvectors);
        
        
        // rank the eigenvalues
        const ublas::indirect_array< std::vector<std::size_t> > l_rank = tools::vector::rankIndex( l_eigenvalues );
        
        // create projection (largest eigenvectors correspondends with the largest eigenvalues -> last values in rank)
        ublas::matrix<T> l_project( l_eigenvectors.size2(), m_dim );
        ublas::matrix<T> l_values(m_dim, m_dim, 0);
        for(std::size_t i=0; i < m_dim; ++i) {
            ublas::column(l_project, m_dim-i-1) = ublas::column(l_eigenvectors, l_rank(l_rank.size()-i-1));
            l_values(m_dim-i-1, m_dim-i-1) = std::pow(l_eigenvalues(l_rank(l_rank.size()-i-1)), static_cast<T>(0.5));
        }
        
        return ublas::prod(l_project, l_values);
    }
    
    
    /** caluate the sammon mapping on MDS (with newton method for optimization)
     * @note uses code idea of http://ticc.uvt.nl/~lvdrmaaten
     * @param p_data input datamatrix (similarity matrix)
     * @return mapped data
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::project_sammon( const ublas::matrix<T>& p_data )
    {
        if (m_iteration == 0)
            throw exception::runtime(_("iterations must be greater than zero"));
        
        // the similarity matrix must be double-centered
        const ublas::matrix<T> l_center = doublecentering( p_data );
        
        
        // create the distance for each row/colum (create distance matrix) of the matrix and sets the diagonal elements to one
        const ublas::mapped_matrix<T> l_DataOnes   = tools::matrix::eye<T>( l_center.size1() );   
        const ublas::matrix<T> l_data              = distance(l_center) + l_DataOnes;
        const ublas::matrix<T> l_dataInv           = tools::matrix::invert(l_data);
        
        // target point matrix und one matrix
        ublas::matrix<T> l_target                  = tools::matrix::random( l_data.size1(), m_dim, tools::random::uniform, -0.5, 0.5 );
        const ublas::mapped_matrix<T> l_TargetOnes = ublas::scalar_matrix<T>( l_target.size1(), l_target.size2(), static_cast<T>(1) );
        
        
        for(std::size_t i=0; i < m_iteration; ++i) {
            const ublas::matrix<T> l_Distances       = distance(l_target, true) + l_DataOnes;
            const ublas::matrix<T> l_DistancesInv    = tools::matrix::invert(l_target);
            const ublas::matrix<T> l_DistancesInv3   = tools::matrix::pow(l_DistancesInv, static_cast<T>(3));
            const ublas::matrix<T> l_target2         = tools::matrix::pow(l_target, static_cast<T>(2));
            
            
            const ublas::matrix<T> l_delta           = l_DistancesInv - l_dataInv;
            const ublas::matrix<T> l_deltaOne        = ublas::prod( l_delta, l_TargetOnes );
            
            const ublas::matrix<T> l_gradient        = ublas::prod( l_delta, l_target) - ublas::element_prod( l_target, l_deltaOne );
            const ublas::matrix<T> l_hesse           = ublas::prod(l_DistancesInv3, l_target2) -  l_deltaOne - 2 * ublas::element_prod(l_target, ublas::prod(l_DistancesInv3, l_target)); 
            
            
         /*delta    = dinv - Dinv;
         deltaone = delta * one;
         g        = delta * y - y .* deltaone;
         dinv3    = dinv .^ 3;
         y2       = y .^ 2;
         H        = dinv3 * y2 - deltaone - 2 * y .* (dinv3 * y) + y2 .* (dinv3 * one);
         s        = -g(:) ./ abs(H(:));
         y_old    = y;
         */   
        }
        
        return l_target;
    }
    
    
    /** calculate the distance between a every row of the matrix and the other rows
     * @param p_matrix input matrix
     * @param p_double
     * @return distance matrix
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::distance( const ublas::matrix<T>& p_matrix, const bool& p_double ) const
    {
        ublas::matrix<T> l_sse( p_matrix.size1(), p_matrix.size1(), 0 );
        if (p_double) {
            l_sse = tools::matrix::pow(p_matrix, static_cast<T>(2));
            const ublas::matrix<T> l_mat = ublas::outer_prod(tools::matrix::sum(l_sse), ublas::scalar_vector<T>(p_matrix.size1(),1));
            l_sse = l_mat + ublas::trans(l_mat) - 2*ublas::prod(p_matrix, ublas::trans(p_matrix));
            l_sse = tools::matrix::pow(l_sse, static_cast<T>(0.5));                       
        } else 
            for(std::size_t i=0; i < l_sse.size1(); ++i)
                for(std::size_t j=0; j < l_sse.size2(); ++j)
                    l_sse(i,j) = std::pow(ublas::sum( tools::vector::pow( static_cast< ublas::vector<T> >(ublas::row(p_matrix, j)-ublas::row(p_matrix, i)), static_cast<T>(2)) ), static_cast<T>(0.5));
            
        return l_sse;
    }
    
    
    /** create a double centering matrix
     * @param p_data input matrix
     * @return double centered matrix
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::doublecentering( const ublas::matrix<T>& p_data ) const
    {
        ublas::matrix<T> l_center(p_data.size1(), p_data.size2());
        
        for(std::size_t i=0; i < p_data.size1(); ++i)
            for(std::size_t j=0; j < p_data.size2(); ++j)
                l_center(i,j) = p_data(i,i) + p_data(j,j) - (p_data(i,j)+p_data(j,i));
        
        return l_center;
    }
    
    


};};};
#endif
