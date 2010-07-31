/** 
 \verbatim
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
 \endverbatim
 **/

#ifndef MACHINELEARNING_CLUSTERING_NONSUPERVISED_NG_HPP
#define MACHINELEARNING_CLUSTERING_NONSUPERVISED_NG_HPP


#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "../clustering.hpp"
#include "../../exception/exception.h"
#include "../../tools/tools.h"
#include "../../distances/distances.h"



namespace machinelearning { namespace clustering { namespace nonsupervised {
    
    namespace ublas = boost::numeric::ublas;
    
    
    
    /** class for calculate (batch) neural gas **/
    template<typename T> class neuralgas : public nonsupervisedclustering<T> {
        
        public:
        
            neuralgas( const distances::distance<T>&, const std::size_t&, const std::size_t& );
            void train( const ublas::matrix<T>&, const std::size_t& );
            void train( const ublas::matrix<T>&, const std::size_t&, const T& );
            ublas::matrix<T> getPrototypes( void ) const;
            void setLogging( const bool& );
            std::vector< ublas::matrix<T> > getLoggedPrototypes( void ) const;
            bool getLogging( void ) const;
            std::size_t getPrototypeSize( void ) const;
            std::size_t getPrototypeCount( void ) const;
            std::vector<T> getLoggedQuantizationError( void ) const;
            ublas::indirect_array< std::vector<std::size_t> > use( const ublas::matrix<T>& ) const;
        
        
        private :
        
            /** distance object **/
            const distances::distance<T>* m_distance;        
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
    
    
    
    /** contructor for initialization the neural gas
     * @param p_distance distance object
     * @param p_prototypes number of prototypes
     * @param p_prototypesize size of each prototype (data dimension)
    **/
    template<typename T> inline neuralgas<T>::neuralgas( const distances::distance<T>& p_distance, const std::size_t& p_prototypes, const std::size_t& p_prototypesize ) :
        m_distance( &p_distance ),
        m_prototypes( tools::matrix::random<T>(p_prototypes, p_prototypesize) ),
        m_logging( false ),
        m_logprototypes( std::vector< ublas::matrix<T> >() ),
        m_quantizationerror( std::vector<T>() )
    {
        if (tools::function::isNumericalZero(p_prototypesize))
            throw exception::greaterthanzero("prototype size");
        if (tools::function::isNumericalZero(p_prototypes))
            throw exception::greaterthanzero("prototype number");
    }
    
     
    /** returns the prototype matrix
     * @return matrix (rows = number of prototypes)
     **/
    template<typename T> inline ublas::matrix<T> neuralgas<T>::getPrototypes( void ) const
    {
        return m_prototypes;
    }
    

    
    /** enabled logging for training
     * @param p bool
     **/
    template<typename T> inline void neuralgas<T>::setLogging( const bool& p )
    {
        m_logging = p;
    }
    
    

    /** shows the logging status
     * @return bool
     **/
    template<typename T> inline bool neuralgas<T>::getLogging( void ) const
    {
        return m_logging;
    }
    
    
    
    /** returns every prototype step during training
     * @return std::vector with prototype matrix
     **/
    template<typename T> inline std::vector< ublas::matrix<T> > neuralgas<T>::getLoggedPrototypes( void ) const
    {
        return m_logprototypes;
    }
    
    
    /** returns the dimension of prototypes
     * @return dimension of the prototypes
     **/
    template<typename T> inline std::size_t neuralgas<T>::getPrototypeSize( void ) const 
    {
        return m_prototypes.size2();
    };
    

    /** returns the number of prototypes
     * @return number of the prototypes / classes
     **/
    template<typename T> inline std::size_t neuralgas<T>::getPrototypeCount( void ) const 
    {
        return m_prototypes.size1();
    };
    
    
    /** returns the quantisation error 
     * @return error for each iteration
     **/
    template<typename T> inline std::vector<T> neuralgas<T>::getLoggedQuantizationError( void ) const
    {
        return m_quantizationerror;
    }    
    
    
    /** train the prototypes
     * @overload
     * @param p_data data matrix
     * @param p_iterations number of iterations
    **/
    template<typename T> inline void neuralgas<T>::train( const ublas::matrix<T>& p_data, const std::size_t& p_iterations )
    {
        train(p_data, p_iterations, m_prototypes.size1() * 0.5);
    }

    
    /** training the prototypes
     * @overload
     * @param p_data datapoints
     * @param p_iterations iterations
     * @param p_lambda max adapet size
    **/
    template<typename T> inline void neuralgas<T>::train( const ublas::matrix<T>& p_data, const std::size_t& p_iterations, const T& p_lambda )
    {
        if (p_data.size1() < m_prototypes.size1())
            throw exception::greaterthan("data", "neurons");
        if (p_iterations == 0)
            throw exception::greaterthanzero("iterations");
        if (p_data.size2() != m_prototypes.size2())
            throw exception::matrix("data and prototype dimension are not equal");
        if (p_lambda <= 0)
            throw exception::greaterthanzero("lambda");
        
            
        // creates logging
        if (m_logging) {
            m_logprototypes     = std::vector< ublas::matrix<T> >();
            m_quantizationerror = std::vector< T >();
            m_logprototypes.reserve(p_iterations);
            m_quantizationerror.reserve(p_iterations);
        }
        
        
        // run neural gas       
        const T l_multi = 0.01/p_lambda;
        ublas::scalar_vector<T> l_ones(p_data.size1(), 1);
        ublas::matrix<T> l_adaptmatrix( m_prototypes.size1(), p_data.size1() );

        for(std::size_t i=0; (i < p_iterations); ++i) {
          
            // create adapt values
            const T l_lambda = p_lambda * std::pow(l_multi, static_cast<T>(i)/static_cast<T>(p_iterations));
            
            // calculate for every prototype the distance
            for(std::size_t n=0; n < m_prototypes.size1(); ++n)
                ublas::row(l_adaptmatrix, n)  = m_distance->calculate( p_data,  ublas::outer_prod(l_ones, ublas::row(m_prototypes, n)) );

            // for every column ranks values and create adapts
            // we need rank and not randIndex, because we 
            // use the value of the ranking for calculate the 
            // adapt value
            for(std::size_t n=0; n < l_adaptmatrix.size2(); ++n) {
                ublas::vector<T> l_col = ublas::column(l_adaptmatrix, n);
                l_col                  = tools::vector::rank( l_col );
                
                // calculate adapt value
                BOOST_FOREACH( T& p, l_col)
                    p = std::exp( -p / l_lambda );
                
                // return value to matrix
                ublas::column(l_adaptmatrix, n) = l_col;
            }
           
            
            // adapt to prototypes and normalize (row orientated)
            m_prototypes = ublas::prod( l_adaptmatrix, p_data );
            
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
    
    
    /** calculate the quantization error
     * @param p_data matrix with data points
     * @return quantization error
     **/    
    template<typename T> inline T neuralgas<T>::calculateQuantizationError( const ublas::matrix<T>& p_data ) const
    {
        ublas::scalar_vector<T> l_ones(p_data.size1(), 1);
        ublas::matrix<T> l_distances( m_prototypes.size1(), p_data.size2() );
        
        for(std::size_t i=0; i < m_prototypes.size1(); ++i)
            ublas::row(l_distances, i) = m_distance->calculate( p_data, ublas::outer_prod( l_ones, ublas::row(m_prototypes, i) ));
        
        return ublas::sum(  m_distance->abs( tools::matrix::min(l_distances) )  );  
    }
    
    
    /** calulates distance between datapoints and prototypes and returns a std::vector
     * with index of the nearest prototype
     * @param p_data matrix
     * @return index array of prototype indices
    **/
    template<typename T> inline ublas::indirect_array< std::vector<std::size_t> > neuralgas<T>::use( const ublas::matrix<T>& p_data ) const
    {
        if (p_data.size1() < m_prototypes.size1())
            throw exception::greaterthan("data", "neurons");
        
        std::vector<std::size_t> l_vec(p_data.size1());
        ublas::scalar_vector<T> l_ones(p_data.size1(), 1);
        ublas::matrix<T> l_distance(m_prototypes.size1(), p_data.size1());
        
        // calculate distance for every prototype
        for(std::size_t i=0; i < m_prototypes.size1(); ++i)
            ublas::row(l_distance, i)  = m_distance->calculate( p_data,  ublas::outer_prod(l_ones, ublas::row(m_prototypes, i)) );
        
        // determine nearest prototype
        for(std::size_t i=0; i < m_prototypes.size2(); ++i) {
            ublas::vector<T> l_col                                          = ublas::column(l_distance, i);
            const ublas::indirect_array< std::vector<std::size_t> > l_rank  = tools::vector::rankIndex( l_col );
            l_vec.push_back( l_rank(0) );
        }
            
        return ublas::indirect_array< std::vector<std::size_t> >(l_vec.size(), l_vec);
                
    }
    
};};};


#endif
