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

#ifndef __MACHINELEARNING_CLUSTERING_NONSUPERVISED_KMEANS_HPP
#define __MACHINELEARNING_CLUSTERING_NONSUPERVISED_KMEANS_HPP


#include <omp.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "clustering.hpp"
#include "../../errorhandling/exception.hpp"
#include "../../tools/tools.h"
#include "../../distances/distances.h"



namespace machinelearning { namespace clustering { namespace nonsupervised {
    
    #ifndef SWIG
    namespace ublas = boost::numeric::ublas;
    #endif
    
    
    /** class for calculate (batch) k-means
     * @todo determine best k with variance analyse
     **/
    template<typename T> class kmeans : public clustering<T>
    {
        
        public:
            
            kmeans( const distances::distance<T>&, const std::size_t&, const std::size_t& );
            void train( const ublas::matrix<T>&, const std::size_t& );
            ublas::matrix<T> getPrototypes( void ) const;
            void setLogging( const bool& );
            std::vector< ublas::matrix<T> > getLoggedPrototypes( void ) const;
            bool getLogging( void ) const;
            std::size_t getPrototypeSize( void ) const;
            std::size_t getPrototypeCount( void ) const;
            std::vector<T> getLoggedQuantizationError( void ) const;
            ublas::indirect_array<> use( const ublas::matrix<T>& ) const;
        
            
        private :
        
            /** distance object **/
            const distances::distance<T>& m_distance;        
            /** prototypes **/
            ublas::matrix<T> m_prototypes;                
            /** bool for logging prototypes **/
            bool m_logging;
            /** std::vector for prototypes for each iteration **/
            std::vector< ublas::matrix<T> > m_logprototypes;
            /** std::vector for quantisation error in each iteration **/
            std::vector<T> m_quantizationerror;
            
            T calculateQuantizationError( const ublas::matrix<T>& ) const;
        
    };
    
    

    /** contructor for initialization the k-means
     * @param p_distance distance object
     * @param p_prototypes number of prototypes
     * @param p_prototypesize size of each prototype (data dimension)
     **/
    template<typename T> inline kmeans<T>::kmeans( const distances::distance<T>& p_distance, const std::size_t& p_prototypes, const std::size_t& p_prototypesize ) :
        m_distance( p_distance ),
        m_prototypes( tools::matrix::random<T>(p_prototypes, p_prototypesize) ),
        m_logging( false ),
        m_logprototypes( std::vector< ublas::matrix<T> >() ),
        m_quantizationerror( std::vector<T>() )
    {
        if (p_prototypesize == 0)
            throw exception::runtime(_("prototype size must be greater than zero"), *this);
        if (p_prototypes == 0)
            throw exception::runtime(_("number of prototypes must be greater than zero"), *this);
    }
    
    
    /** returns the prototype matrix
     * @return matrix (rows = number of prototypes)
     **/
    template<typename T> inline ublas::matrix<T> kmeans<T>::getPrototypes( void ) const
    {
        return m_prototypes;
    }
    
    
    
    /** enabled logging for training
     * @param p_val bool
     **/
    template<typename T> inline void kmeans<T>::setLogging( const bool& p_val )
    {
        m_logging = p_val;
        m_logprototypes.clear();
        m_quantizationerror.clear();
    }
    
    
    
    /** shows the logging status
     * @return bool
     **/
    template<typename T> inline bool kmeans<T>::getLogging( void ) const
    {
        return m_logging && (m_logprototypes.size() > 0);
    }
    
    
    
    /** returns every prototype step during training
     * @return std::vector with prototype matrix
     **/
    template<typename T> inline std::vector< ublas::matrix<T> > kmeans<T>::getLoggedPrototypes( void ) const
    {
        return m_logprototypes;
    }
    
    
    /** returns the quantisation error 
     * @return error for each iteration
     **/
    template<typename T> inline std::vector<T> kmeans<T>::getLoggedQuantizationError( void ) const
    {
        return m_quantizationerror;
    }    
    
    
    /** train the prototypes
     * @param p_data data matrix
     * @param p_iterations number of iterations
     **/
    template<typename T> inline void kmeans<T>::train( const ublas::matrix<T>& p_data, const std::size_t& p_iterations )
    {
        if (p_iterations == 0)
            throw exception::runtime(_("iterations must be greater than zero"), *this);
        if (p_data.size2() != m_prototypes.size2())
            throw exception::runtime(_("data and prototype dimension are not equal"), *this);
        if (p_data.size1() < m_prototypes.size1())
            throw exception::runtime(_("number of datapoints are less than prototypes"), *this);
        
        
        // creates logging
        if (m_logging) {
            m_logprototypes.clear();
            m_quantizationerror.clear();
            m_logprototypes.reserve(p_iterations);
            m_quantizationerror.reserve(p_iterations);
        }
        
        
        // run kmeans       
        ublas::matrix<T> l_distances( m_prototypes.size1(), p_data.size1() );
        // the matrix for adaption is a sparse matrix, because there are only 0 or 1 values
        ublas::mapped_matrix<T> l_adaptmatrix( m_prototypes.size1(), p_data.size1(), m_prototypes.size1()*p_data.size1() );
        
        for(std::size_t i=0; i < p_iterations; ++i) {
            
            // calculate for every prototype the distance
            #pragma omp parallel for shared(l_distances)
            for(std::size_t n=0; n < m_prototypes.size1(); ++n)
                ublas::row(l_distances, n)  = m_distance.getDistance( p_data,  ublas::row(m_prototypes, n) );
            
            // determine winner and set the winner to 1
            // iterate over the columns and ranks every column
            l_adaptmatrix.clear();
            #pragma omp parallel for shared(l_adaptmatrix)
            for(std::size_t n=0; n < l_distances.size2(); ++n) {
                ublas::vector<T> l_vec = ublas::column(l_distances, n);

                // set winner
                l_adaptmatrix(tools::vector::rankIndex( l_vec )(0), n) = static_cast<T>(1);
            }
            
            
            // adapt to prototypes and normalize the winner row (row orientated)
            m_prototypes = ublas::prod( l_adaptmatrix, p_data );
   
            #pragma omp parallel for
            for(std::size_t n=0; n < m_prototypes.size1(); ++n) {
                const T l_norm = ublas::sum( ublas::row(l_adaptmatrix, n) );
                
                if (!tools::function::isNumericalZero(l_norm))
                    ublas::row(m_prototypes, n) /= l_norm;
            }
            
            
            // determine quantization error for logging
            if (m_logging) {
                m_logprototypes.push_back( m_prototypes );
                m_quantizationerror.push_back( calculateQuantizationError(p_data) );
            }            
        }
    }
    
    
    /** returns the dimension of prototypes
     * @return dimension of the prototypes
     **/
    template<typename T> inline std::size_t kmeans<T>::getPrototypeSize( void ) const 
    {
        return m_prototypes.size2();
    }

    
    /** returns the number of prototypes
     * @return number of the prototypes / classes
     **/
    template<typename T> inline std::size_t kmeans<T>::getPrototypeCount( void ) const 
    {
        return m_prototypes.size1();
    }
    
    
    /** calculate the quantization error
     * @param p_data matrix with data points
     * @return quantization error
     **/    
    template<typename T> inline T kmeans<T>::calculateQuantizationError( const ublas::matrix<T>& p_data ) const
    {
        ublas::matrix<T> l_distances( m_prototypes.size1(), p_data.size1() );
        
        #pragma omp parallel for
        for(std::size_t i=0; i < m_prototypes.size1(); ++i)
            ublas::row(l_distances, i) = m_distance.getDistance( p_data, ublas::row(m_prototypes, i) );
        
        return 0.5 * ublas::sum(  m_distance.getAbs(tools::matrix::min(l_distances, tools::matrix::column))  );  
    }
    
    
    /** calulates distance between datapoints and prototypes and returns a indirect array
     * with index of the nearest prototype
     * @param p_data matrix
     * @return index array of prototype indices
     **/
    template<typename T> inline ublas::indirect_array<> kmeans<T>::use( const ublas::matrix<T>& p_data ) const
    {
        if (p_data.size1() < m_prototypes.size1())
            throw exception::runtime(_("number of datapoints are less than prototypes"), *this);        
        
        ublas::indirect_array<> l_idx(p_data.size1());
        ublas::matrix<T> l_distance(m_prototypes.size1(), p_data.size1());
        
        // calculate distance for every prototype
        #pragma omp parallel for shared(l_distance)
        for(std::size_t i=0; i < m_prototypes.size1(); ++i)
            ublas::row(l_distance, i)  = m_distance.getDistance( p_data, ublas::row(m_prototypes, i) );
        
        // determine nearest prototype
        #pragma omp parallel for shared(l_distance, l_idx)
        for(std::size_t i=0; i < l_distance.size2(); ++i) {
            ublas::vector<T> l_col                = ublas::column(l_distance, i);
            const ublas::indirect_array<> l_rank  = tools::vector::rankIndex( l_col );
            l_idx[i] = l_rank(0);
        }
        
        return l_idx;
        
    }

    

}}}
#endif
