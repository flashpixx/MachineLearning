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
                shepardkruskal  = 3,     // http://de.wikipedia.org/wiki/Multidimensionale_Skalierung#Shepard-Kruskal_Algorithmus
                energie         = 4
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
    
    
    /** caluate the sammon mapping on MDS
     * @param p_data input datamatrix (similarity matrix)
     * @return mapped data
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::project_sammon( const ublas::matrix<T>& p_data )
    {
        if (m_iteration == 0)
            throw exception::runtime(_("iterations must be greater than zero"));
        
        // the similarity matrix must be double-centered
        ublas::matrix<T> l_data(p_data.size1(), p_data.size2());
        for(std::size_t i=0; i < l_data.size1(); ++i)
            for(std::size_t j=0; j < l_data.size2(); ++j)
                l_data(i,j) = p_data(i,i) + p_data(j,j) - (p_data(i,j)+p_data(j,i));
        
        
        for(std::size_t i=0; i < m_iteration; ++i) {
            
        }
    }
    

};};};
#endif
