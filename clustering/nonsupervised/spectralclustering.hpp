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

#ifndef __MACHINELEARNING_CLUSTERING_NONSUPERVISED_SPECTRALCLUSTERING_HPP
#define __MACHINELEARNING_CLUSTERING_NONSUPERVISED_SPECTRALCLUSTERING_HPP

#include <omp.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/bindings/blas.hpp>

#include "clustering.hpp"
#include "kmeans.hpp"
#include "../../errorhandling/exception.hpp"
#include "../../tools/tools.h"
#include "../../distances/distances.h"


namespace machinelearning { namespace clustering { namespace nonsupervised {
    
    #ifndef SWIG
    namespace ublas = boost::numeric::ublas;
    namespace blas  = boost::numeric::bindings::blas;
    #endif
    
    
    /** class for normalized spectral clustering. This class calculates only the graph laplacian
     * and creates the general eigenvector decomposition. A neuralgas algorithm with euclidian
     * distancesis used for clustering the data
     * @todo set all routines to sparse matrix if arpack can be used with boost
     * @todo create eigengap heurstic
     **/
    template<typename T> class spectralclustering : public clustering<T>
    {
        public :

            spectralclustering( const std::size_t& );
            void train( const ublas::matrix<T>&, const std::size_t& );
            ublas::matrix<T> getPrototypes( void ) const;
            void setLogging( const bool& );
            std::vector< ublas::matrix<T> > getLoggedPrototypes( void ) const;
            bool getLogging( void ) const;
            std::size_t getPrototypeSize( void ) const;
            std::size_t getPrototypeCount( void ) const;
            std::vector<T> getLoggedQuantizationError( void ) const;
            ublas::indirect_array<> use( const ublas::matrix<T>& ) const;
            
            //static std::size_t getEigenGap( const ublas::matrix<T>& ) const;


        private :
        
            ublas::matrix<T> getEigenGraphLaplacian( const ublas::matrix<T>& p_adjacency ) const;
        
            /** distance object for clustering (we use euclidan distances) **/
            const distances::norm::euclid<T> m_distance;
            /** neural gas for clustering the graph laplacian **/
            kmeans<T> m_kmeans;
        
    };
    

    /** constructor 
     * @param p_prototypes number of prototypes
     **/
    template<typename T> inline spectralclustering<T>::spectralclustering( const std::size_t& p_prototypes ) :
        m_distance( distances::norm::euclid<T>() ),
        m_kmeans( kmeans<T>( m_distance, p_prototypes, p_prototypes) )
    {}
    
    
    /** returns the prototype matrix
     * @return matrix (rows = number of prototypes)
     **/
    template<typename T> inline ublas::matrix<T> spectralclustering<T>::getPrototypes( void ) const
    {
        return m_kmeans.getPrototypes();
    }
    
    
    
    /** enabled logging for training
     * @param p_log bool
     **/
    template<typename T> inline void spectralclustering<T>::setLogging( const bool& p_log )
    {
        m_kmeans.setLogging(p_log);
    }
    
    
    
    /** shows the logging status
     * @return bool
     **/
    template<typename T> inline bool spectralclustering<T>::getLogging( void ) const
    {
        return m_kmeans.getLogging();
    }
    
    
    
    /** returns every prototype step during training
     * @return std::vector with prototype matrix
     **/
    template<typename T> inline std::vector< ublas::matrix<T> > spectralclustering<T>::getLoggedPrototypes( void ) const
    {
        return m_kmeans.getLoggedPrototypes();
    }
    
    
    /** returns the dimension of prototypes
     * @return dimension of the prototypes
     **/
    template<typename T> inline std::size_t spectralclustering<T>::getPrototypeSize( void ) const 
    {
        return m_kmeans.getPrototypeSize();
    }
    
    
    /** returns the number of prototypes
     * @return number of the prototypes / classes
     **/
    template<typename T> inline std::size_t spectralclustering<T>::getPrototypeCount( void ) const 
    {
        return m_kmeans.getPrototypeCount();
    }
    
    
    /** returns the quantisation error 
     * @return error for each iteration
     **/
    template<typename T> inline std::vector<T> spectralclustering<T>::getLoggedQuantizationError( void ) const
    {
        return m_kmeans.getLoggedQuantizationError();
    }    
    
    
    /** creates the cluster matrix of the graph laplacian
     * @param p_adjacency adjacency matrix
     * @return data matrix for the k-means clustering
     **/
    template<typename T> inline ublas::matrix<T> spectralclustering<T>::getEigenGraphLaplacian( const ublas::matrix<T>& p_adjacency ) const
    {
        // get the normalized graph laplacian
        const ublas::matrix<T> l_laplacian = tools::lapack::normalizedGraphLaplacian( p_adjacency );
        
        // determine eigenvalues and -vector of the graph laplacian
        ublas::vector<T> l_eigenvalue;
        ublas::matrix<T> l_eigenvector;
        tools::lapack::eigen( l_laplacian, l_eigenvalue, l_eigenvector );
        
        // ranking eigenvalues and get the k smallest for the eigenvectors
        const ublas::indirect_array<> l_rank = tools::vector::rankIndex<T>(l_eigenvalue);
        ublas::matrix<T> l_eigenmatrix( p_adjacency.size1(), m_kmeans.getPrototypeCount() );

        for(std::size_t i=0; i < l_eigenmatrix.size2(); ++i)
            ublas::column(l_eigenmatrix, i) = ublas::column(l_eigenvector, l_rank(i));
        
        return l_eigenmatrix;
    }
        
    
    /** cluster the graph with the <strong>normalized</strong> graph laplacian
     * @param p_adjacency adjacency / distance matrix
     * @param p_iterations number of iterations
     **/
    template<typename T> inline void spectralclustering<T>::train( const ublas::matrix<T>& p_adjacency, const std::size_t& p_iterations )
    {
        m_kmeans.train( getEigenGraphLaplacian(p_adjacency), p_iterations );
    }
    
    
    /** returns the index for each datapoint to the prototype
     * @param p_data input matrix
     * @return array with index values
     **/
    template<typename T> inline ublas::indirect_array<> spectralclustering<T>::use( const ublas::matrix<T>& p_data ) const
    {
        return m_kmeans.use( getEigenGraphLaplacian(p_data) );
    }
    
        
}}}
#endif
