/** 
 @cond
 ############################################################################
 # LGPL License                                                             #
 #                                                                          #
 # This file is part of the Machine Learning Framework.                     #
 # Copyright (c) 2010, Philipp Kraus, <philipp.kraus@flashpixx.de>          #
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
 # along with this program.  If not, see <http://www.gnu.org/licenses/>.    #
 ############################################################################
 @endcond
 **/

#ifndef MACHINELEARNING_CLUSTERING_NONSUPERVISED_SPECTRALCLUSTERING_HPP
#define MACHINELEARNING_CLUSTERING_NONSUPERVISED_SPECTRALCLUSTERING_HPP


#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/bindings/blas.hpp>

#include "../clustering.hpp"
#include "kmeans.hpp"
#include "../../exception/exception.h"
#include "../../tools/tools.h"
#include "../../distances/distances.h"


namespace machinelearning { namespace clustering { namespace nonsupervised {
    
    namespace ublas = boost::numeric::ublas;
    namespace blas  = boost::numeric::bindings::blas;
    
    
    /** class for normalized spectral clustering. This class calculates only the graph laplacian
     * and creates the general eigenvector decomposition. A neuralgas algorithm with euclidian
     * distancesis used for clustering the data
     * @todo set all routines to sparse matrix if arpack can be used with boost
     **/
    template<typename T> class spectralclustering : public nonsupervisedclustering<T> {
        
        
        public :
            
            spectralclustering( const std::size_t&, const std::size_t& );
            void train( const ublas::matrix<T>&, const std::size_t& );
            void train( const ublas::matrix<T>&, const std::size_t&, const T& );
            ublas::matrix<T> getPrototypes( void ) const;
            void setLogging( const bool& );
            std::vector< ublas::matrix<T> > getLoggedPrototypes( void ) const;
            bool getLogging( void ) const;
            std::size_t getPrototypeSize( void ) const;
            std::size_t getPrototypeCount( void ) const;
            std::vector<T> getLoggedQuantizationError( void ) const;
            std::vector<T> use( const ublas::matrix<T>& ) const;
            
            //static std::size_t getEigenGap( const ublas::matrix<T>& ) const;
            //static std::size_t getEigenGap( const ublas::matrix<T>&, const ublas::matrix<T>& ) const;


        private :
        
            /** distance object for ng **/
            const distances::euclid<T> m_distance;
            /** neural gas for clustering the graph laplacian **/
            kmeans<T> m_kmeans;
        
    };
    

    /** constructor 
     * @param p_prototypes number of prototypes
     * @param p_prototypesize size of each prototype (row / column length of the adjacency matrix)
     **/
    template<typename T> inline spectralclustering<T>::spectralclustering( const std::size_t& p_prototypes, const std::size_t& p_prototypesize ) :
        m_distance( distances::euclid<T>() ),
        m_kmeans( neuralgas<T>( m_distance, p_prototypes, p_prototypesize) )
    {}
    
    
    /** returns the prototype matrix
     * @return matrix (rows = number of prototypes)
     **/
    template<typename T> inline ublas::matrix<T> spectralclustering<T>::getPrototypes( void ) const
    {
        return m_kmeans.getPrototypes();
    }
    
    
    
    /** enabled logging for training
     * @param p bool
     **/
    template<typename T> inline void spectralclustering<T>::setLogging( const bool& p )
    {
        m_kmeans.setLogging(p);
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
    };
    
    
    /** returns the number of prototypes
     * @return number of the prototypes / classes
     **/
    template<typename T> inline std::size_t spectralclustering<T>::getPrototypeCount( void ) const 
    {
        return m_kmeans.getPrototypeCount();
    };
    
    
    /** returns the quantisation error 
     * @return error for each iteration
     **/
    template<typename T> inline std::vector<T> spectralclustering<T>::getLoggedQuantizationError( void ) const
    {
        return m_kmeans.getLoggedQuantizationError();
    }    
    
    
    /** cluster the graph with the <strong>normalized</strong> graph laplacian
     * @todo copy function for matrix musst be optimized
     * @param p_similarity similarity NxN matrix, needs to be squared and non-negative
     * @param p_iterations number of iterations
     **/
    template<typename T> inline void spectralclustering<T>::train( const ublas::matrix<T>& p_similarity, const std::size_t& p_iterations )
    {
        if (p_similarity.size1() != p_similarity.size2())
            throw exception::runtime(_("matrix must be square"));
        if (p_similarity.size2() < m_kmeans.getPrototypeCount())
            throw exception::runtime(_("data and prototype dimension are not equal"));

        
        // create squared degree and normalized graph laplacian
        const ublas::matrix<T> l_sqrtdegree   = tools::matrix::pow( tools::matrix::diag(tools::matrix::sum(p_similarity)), static_cast<T>(-0.5));
        const ublas::matrix<T> l_tmp          = ublas::prod(l_sqrtdegree, p_similarity);
        const ublas::matrix<T> l_laplacian    = tools::matrix::eye<T>(p_similarity.size1()) - ublas::prod(l_tmp, l_sqrtdegree);
        
        // determine eigenvalues and -vector of the graph laplacian
        ublas::vector<T> l_eigenvalue;
        ublas::matrix<T> l_eigenvector;
        tools::lapack::eigen( l_laplacian, l_eigenvalue, l_eigenvector );
         
        // ranking eigenvalues and get the k smallest for the eigenvectors
        const ublas::indirect_array< std::vector<std::size_t> > l_rank = tools::vector::rankIndex<T>(l_eigenvalue);
        
        // extrakt the k eigenvectors, change the normalized laplacian back and normalize
        ublas::matrix<T> l_eigenmatrix(m_kmeans.getPrototypeCount(), l_laplacian.size1());
        const ublas::vector<T> l_sum = tools::matrix::sum(p_similarity);
        
        for(std::size_t i=0; i < m_kmeans.getPrototypeCount(); ++i) {
            ublas::row(l_eigenmatrix, i)  = ublas::element_div( ublas::column(l_eigenvector, l_rank(i)), l_sum );
            ublas::row(l_eigenmatrix, i) /= blas::nrm2( static_cast< ublas::vector<T> >(ublas::row(l_eigenmatrix, i)) ); 
        }
            
        // clustering
        m_kmeans.train(l_eigenmatrix, p_iterations);
    }
    
        
};};};
#endif
