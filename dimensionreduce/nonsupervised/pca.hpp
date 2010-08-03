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

#ifndef MACHINELEARNING_DIMENSIONREDUCE_NONSUPERVISED_PCA_HPP
#define MACHINELEARNING_DIMENSIONREDUCE_NONSUPERVISED_PCA_HPP


#include <boost/numeric/ublas/matrix.hpp>

#include "../dimensionreduce.hpp"
#include "../../exception/exception.h"
#include "../../tools/tools.h"


namespace machinelearning { namespace dimensionreduce { namespace nonsupervised {
    
    namespace ublas  = boost::numeric::ublas;

    
    /** create the Principal Component Analysis (PCA) from data **/
    template<typename T> class pca : public nonsupervisedreduce<T> {
        
        public :
            
            pca( const std::size_t& );
            ublas::matrix<T> map( const ublas::matrix<T>& );
            ublas::matrix<T> getMapping( void ) const;
            std::size_t getDimension( void ) const;
        
        
        private :
            
            /** target dimension **/
            const std::size_t m_dim;
            /** matrix with projection vectors (row orientated) **/
            ublas::matrix<T> m_project;
        
    };
    
    
    
    /** constructor
     * @param p_dim target dimension
    **/
    template<typename T> inline pca<T>::pca( const std::size_t& p_dim ) :
        m_dim( p_dim ),
        m_project()
    {
        if (p_dim == 0)
            throw exception::parameter(_("dimension must be greater than zero"));
    }
    
    
    /** returns the target dimensione size
     * @return number of dimension
    **/
    template<typename T> inline std::size_t pca<T>::getDimension( void ) const
    {
        return m_dim;
    }
    
    
    /** returns the project vectors (largest eigenvectors)
     * @return matrix with eigenvector
     **/
    template<typename T> inline ublas::matrix<T> pca<T>::getMapping( void ) const
    {
        return m_project;
    }
    
    
    /** caluate and project the input data
     * @param p_data input datamatrix
    **/
    template<typename T> inline ublas::matrix<T> pca<T>::map( const ublas::matrix<T>& p_data )
    {
        if (p_data.size2() <= m_dim)
            throw exception::greaterthan("data dimension", "reduce dimension");
        
        // centering the data
        ublas::matrix<T> l_center = tools::matrix::centering<T>(p_data);
        
        // creates if needed the covarianz matrix or create matrix product
        ublas::matrix<T> l_data;
        if (l_center.size2() < l_center.size1())
            l_data = tools::matrix::cov<T>(l_center);
        else
            l_data =  (1.0 / l_center.size1()) * ublas::prod(ublas::trans(l_center), l_center);
        
        
        // calculate the eigenvalues & -vectors
        ublas::vector<T> l_eigenvalues;
        ublas::matrix<T> l_eigenvectors;
        tools::lapack::eigen<T>(l_data, l_eigenvalues, l_eigenvectors);
               
        // rank the eigenvalues
        const ublas::indirect_array< std::vector<std::size_t> > l_rank = tools::vector::rankIndex( l_eigenvalues );
                
        // create projection (largest eigenvectors correspondends with the largest eigenvalues -> last values in rank)
        m_project = ublas::matrix<T>( l_eigenvectors.size2(), m_dim );
        for(std::size_t i=0; i < m_dim; ++i)
            ublas::column(m_project, i) = ublas::column(l_eigenvectors, l_rank(l_rank.size()-i-1));
        
        return ublas::prod(l_center, m_project);
    }
    
};};};

#endif