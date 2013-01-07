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


#ifndef __MACHINELEARNING_CLUSTERING_SUPERVISED_RLVQ_HPP
#define __MACHINELEARNING_CLUSTERING_SUPERVISED_RLVQ_HPP

#include <omp.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "clustering.hpp"
#include "../../errorhandling/exception.hpp"
#include "../../tools/tools.h"
#include "../../distances/distances.h"



namespace machinelearning { namespace clustering { namespace supervised {
    
    #ifndef SWIG
    namespace ublas   = boost::numeric::ublas;
    #endif
    
    
    /** class for calculate relevance vector quantisation (RLVQ).
     * RLVQ is not the best solution for overlapping cluster,
     * the class is created like a template class for free types
     * of the label structure
    **/
    template<typename T, typename L> class rlvq : public clustering<T, L> 
    {
        
        public:
        
            rlvq( const distances::distance<T>&, const std::vector<L>&, const std::size_t& );
            void train( const ublas::matrix<T>&, const std::vector<L>&, const std::size_t& );
            void train( const ublas::matrix<T>&, const std::vector<L>&, const std::size_t&, const T& );
            void train( const ublas::matrix<T>&, const std::vector<L>&, const std::size_t&, const T&, const T& );
            ublas::matrix<T> getPrototypes( void ) const;
            std::vector<L> getPrototypesLabel( void ) const;
            void setLogging( const bool& );
            bool getLogging( void ) const;
            std::vector< ublas::matrix<T> > getLoggedPrototypes( void ) const;
            std::size_t getPrototypeSize( void ) const; 
            std::size_t getPrototypeCount( void ) const;
            std::vector<T> getLoggedQuantizationError( void ) const;
            ublas::indirect_array<> use( const ublas::matrix<T>& ) const;
        
        
        private :
        
            /** distance object **/
            const distances::distance<T>& m_distance;
            /** prototypes **/
            ublas::matrix<T> m_prototypes;
            /** vector with neuron label information **/
            const std::vector<L> m_neuronlabels;
            /** bool for logging prototypes **/
            bool m_logging;
            /** std::vector with prototypes in each iteration **/
            std::vector< ublas::matrix<T> > m_logprototypes;
            /** std::vector with quantisation error in each iteration **/
            std::vector<T> m_quantizationerror;
        
            T calculateQuantizationError( const ublas::matrix<T>& ) const;
    };
   
    
    /** constructor with labeling
     * @param p_distance distance object
     * @param p_neuronlabels protoype labeling
     * @param p_prototypesize length of prototypes   
    **/
    template<typename T, typename L> inline rlvq<T, L>::rlvq( const distances::distance<T>& p_distance, const std::vector<L>& p_neuronlabels, const std::size_t& p_prototypesize ) :
        m_distance( p_distance ),    
        m_prototypes( tools::matrix::random<T>(p_neuronlabels.size(), p_prototypesize) ),
        m_neuronlabels( p_neuronlabels ),
        m_logging( false ),
        m_logprototypes( std::vector< ublas::matrix<T> >() ),
        m_quantizationerror( std::vector< T >() )
    {
        if (p_prototypesize == 0)
            throw exception::runtime(_("prototype size must be greater than zero"), *this);
    }
    
    
    /** returns the prototype matrix
     * @return matrix (rows = prototypes)
     **/
    template<typename T, typename L> inline ublas::matrix<T> rlvq<T, L>::getPrototypes( void ) const
    {
        return m_prototypes;
    }
    
    
    /** returns the prototypes labels
     * @return vector with label information
    **/
    template<typename T, typename L> inline std::vector<L> rlvq<T, L>::getPrototypesLabel( void ) const
    {
        return m_neuronlabels;
    }
    
    
    /** enabled / disable logging for training
     * @param p_log bool
    **/
    template<typename T, typename L> inline void rlvq<T, L>::setLogging( const bool& p_log )
    {
        m_logging = p_log;
        m_logprototypes.clear();
        m_quantizationerror.clear();
    }
    
    /** shows the logging status
     * @return bool
    **/
    template<typename T, typename L> inline bool rlvq<T, L>::getLogging( void ) const
    {
        return m_logging && (m_logprototypes.size() > 0);
    }
    
    
    /** returns every prototype step during training
     * @return std::vector with prototype matrix
    **/
    template<typename T, typename L> inline std::vector< ublas::matrix<T> > rlvq<T, L>::getLoggedPrototypes( void ) const
    {
        return m_logprototypes;
    }
    
    
    /** returns the dimension of prototypes
     * @return dimension of the prototypes
     **/
    template<typename T, typename L> inline std::size_t rlvq<T, L>::getPrototypeSize( void ) const 
    {
        return m_prototypes.size2();
    }

    
    /** returns the number of prototypes
     * @return number of the prototypes / classes
     **/
    template<typename T, typename L> inline std::size_t rlvq<T, L>::getPrototypeCount( void ) const 
    {
        return m_prototypes.size1();
    }
    
    
    /** returns the quantisation error 
     * @return error for each iteration
    **/
    template<typename T, typename L> inline std::vector<T> rlvq<T, L>::getLoggedQuantizationError( void ) const
    {
        return m_quantizationerror;
    }
    
    
    /** trains the prototypes from the data
     * @param p_data Matrix with data (rows are the vectors)
     * @param p_labels vector for labels
     * @param p_iterations iterations
     **/
    template<typename T, typename L> inline void rlvq<T, L>::train( const ublas::matrix<T>& p_data, const std::vector<L>& p_labels, const std::size_t& p_iterations )
    {
        train(p_data, p_labels, p_iterations, 0.01/m_prototypes.size1());
    }
    
    
    /** trains the prototypes from the data
     * @param p_data Matrix with data (rows are the vectors)
     * @param p_labels vector for labels
     * @param p_iterations iterations
     * @param p_lambda multiplicator for adaption for prototypes
     **/
    template<typename T, typename L> inline void rlvq<T, L>::train( const ublas::matrix<T>& p_data, const std::vector<L>& p_labels, const std::size_t& p_iterations, const T& p_lambda )
    {
        train(p_data, p_labels, p_iterations, p_lambda, 0.1*p_lambda);
    }
    

    /** trains the prototypes from the data
     * @param p_data Matrix with data (rows are the vectors)
     * @param p_labels vector for labels
     * @param p_iterations iterations
     * @param p_lambda multiplicator for adaption for prototypes
     * @param p_eta multiplicator for adaption for the dimension weights
    **/
    template<typename T, typename L> inline void rlvq<T, L>::train( const ublas::matrix<T>& p_data, const std::vector<L>& p_labels, const std::size_t& p_iterations, const T& p_lambda, const T& p_eta )
    {
        if (p_data.size1() < m_prototypes.size1())
            throw exception::runtime(_("number of datapoints are less than prototypes"), *this);
        if (p_iterations == 0)
            throw exception::runtime(_("iterations must be greater than zero"), *this);
        if (p_labels.size() != p_data.size1())
            throw exception::runtime(_("matrix rows and label size are not equal"), *this);
        if (p_data.size2() != m_prototypes.size2())
            throw exception::runtime(_("data and prototype dimension are not equal"), *this);
        if (p_lambda <= 0)
            throw exception::runtime(_("lambda must be greater than zero"), *this);
        if (p_eta <= 0)
            throw exception::runtime(_("eta must be greater than zero"), *this);
        
        // for every prototype create a own lambda, initialisate with 1 and normalize prototypes
        ublas::matrix<T> l_lambda(m_neuronlabels.size(), p_data.size2(), 1);
        m_distance.normalize( l_lambda );
        
        // creates logging
        if (m_logging) {
            m_logprototypes     = std::vector< ublas::matrix<T> >();
            m_quantizationerror = std::vector< T >();
            m_logprototypes.reserve(p_iterations);
            m_quantizationerror.reserve(p_iterations);
        }
        
        
        for(std::size_t i=0; i < p_iterations; ++i) {
            
            // determine quantization error for logging
            if (m_logging) {
                m_logprototypes.push_back( m_prototypes );
                m_quantizationerror.push_back( calculateQuantizationError(p_data) );
            }
            
            #pragma omp parallel for shared(l_lambda)
            for (std::size_t j=0; j < p_data.size1(); ++j) {
                
                // calculate weighted distance and rank vector elements, the first element is the index of the winner prototype
                ublas::vector<T> l_distance          = m_distance.getWeightedDistance( m_prototypes, ublas::row(p_data, j), l_lambda );
                const ublas::indirect_array<> l_rank = tools::vector::rankIndex( l_distance );
                
                // calculate adapt values
                const ublas::vector<T> l_winnerdelta    = p_lambda * (ublas::row(p_data, j) - ublas::row(m_prototypes, l_rank(0) ));
                const ublas::vector<T> l_lambdaadapt    = p_eta    * ublas::element_prod(ublas::row(l_lambda, l_rank(0)), m_distance.getAbs(l_winnerdelta));
                
                // label checking and adaption for winner and lambda
                #pragma omp critical
                {
                    if (  m_neuronlabels[l_rank(0)] == p_labels[j] ) {
                        ublas::row(m_prototypes, l_rank(0) ) += l_winnerdelta;
                        ublas::row(l_lambda, l_rank(0))      -= l_lambdaadapt;
                    } else {
                        ublas::row(m_prototypes, l_rank(0) ) -= l_winnerdelta;
                        ublas::row(l_lambda, l_rank(0))      += l_lambdaadapt;
                    }
                    
                    // normalize lambda (only one row, which has been changed)
                    ublas::row(l_lambda, l_rank(0))  /= m_distance.getLength( static_cast< ublas::vector<T> >(ublas::row(l_lambda, l_rank(0))) );
                }
            }
        }
    }
    
    
    
    /** calculate the quantization error
     * @param p_data matrix with data points
     * @return quantization error
     **/
    template<typename T, typename L> inline T rlvq<T, L>::calculateQuantizationError( const ublas::matrix<T>& p_data ) const
    {
        ublas::matrix<T> l_distances( m_prototypes.size1(), p_data.size1() );
        
        #pragma omp parallel for shared(l_distances)
        for(std::size_t i=0; i < m_prototypes.size1(); ++i)
            ublas::row(l_distances, i) = m_distance.getDistance( p_data, ublas::row(m_prototypes, i) );
        
        return 0.5 * ublas::sum(  m_distance.getAbs(tools::matrix::min(l_distances, tools::matrix::column))  );  
    }
    
    
    
    /** labels unkown data (row orientated)
     * @param p_data unkwon datamatrix
     * @return index position for every datapoint and its prototype / label
    **/
    template<typename T, typename L> inline ublas::indirect_array<> rlvq<T, L>::use( const ublas::matrix<T>& p_data ) const
    {
        if (p_data.size2() != m_prototypes.size2())
            throw exception::runtime( _("data and prototype dimension are not equal"), *this );
        
        ublas::indirect_array<> l_idx(p_data.size1());
        #pragma omp parallel for shared(l_idx)
        for(std::size_t i=0; i < p_data.size1(); ++i) {
            
            // calculate distance from datapoint to all prototyps and rank position
            ublas::vector<T> l_distance       = m_distance.getDistance( m_prototypes, ublas::row(p_data, i)  );
            ublas::indirect_array<> l_rank    = tools::vector::rankIndex( l_distance );
            
            // add index
            l_idx[i] = l_rank(0);
        }
        return l_idx;
    }


}}}
#endif
