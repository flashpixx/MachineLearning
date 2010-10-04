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


#ifndef MACHINELEARNING_CLUSTERING_NONSUPERVISED_NG_HPP
#define MACHINELEARNING_CLUSTERING_NONSUPERVISED_NG_HPP

#include <algorithm>
#include <numeric>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#ifdef CLUSTER
#include <boost/mpi.hpp>
#endif

#include "../clustering.hpp"
#include "../../exception/exception.h"
#include "../../tools/tools.h"
#include "../../distances/distances.h"



namespace machinelearning { namespace clustering { namespace nonsupervised {
    
    namespace ublas = boost::numeric::ublas;
    #ifdef CLUSTER
    namespace mpi   = boost::mpi;
    #endif
    
    /** class for calculate (batch) neural gas
     **/
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
        
            #ifdef CLUSTER
            void train( const mpi::communicator&, const ublas::matrix<T>&, const std::size_t& );
            void train( const mpi::communicator&, const ublas::matrix<T>&, const std::size_t&, const T& );
            ublas::matrix<T> getPrototypes( const mpi::communicator& ) const;
            std::vector< ublas::matrix<T> > getLoggedPrototypes( const mpi::communicator& ) const;
            std::vector<T> getLoggedQuantizationError( const mpi::communicator& ) const;
            #endif
        
        
        
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
        
            T calculateQuantizationError( const ublas::matrix<T>&, const ublas::matrix<T>& ) const;
        
            #ifdef CLUSTER
            /** map with information **/
            std::map<int, std::pair<std::size_t,std::size_t> > m_processprototypinfo;

            ublas::matrix<T> gatherPrototypes( const mpi::communicator& ) const;
            void gatherLocalPrototypes( const mpi::communicator&, ublas::matrix<T>&, ublas::vector<T>& );
            std::size_t gatherNumberPrototypes( const mpi::communicator& ) const;
            void setProcessPrototypeInfo( const mpi::communicator& );
            #endif
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
        #ifdef CLUSTER
        , m_processprototypinfo()
        #endif
    {
        if (p_prototypesize == 0)
            throw exception::parameter(_("prototype size must be greater than zero"));
        if (p_prototypes == 0)
            throw exception::parameter(_("number of prototypes must be greater than zero"));
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
            throw exception::parameter(_("number of datapoints are less than prototypes"));
        if (p_iterations == 0)
            throw exception::parameter(_("iterations must be greater than zero"));
        if (p_data.size2() != m_prototypes.size2())
            throw exception::matrix(_("data and prototype dimension are not equal"));
        if (p_lambda <= 0)
            throw exception::parameter(_("lambda must be greater than zero"));
        
        // creates logging
        if (m_logging) {
            m_logprototypes.clear();
            m_quantizationerror.clear();
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
                m_quantizationerror.push_back( calculateQuantizationError(p_data, m_prototypes) );
            }
        }
    }
    
    
    /** calculate the quantization error
     * @param p_data matrix with data points
     * @param p_prototypes prototype matrix
     * @return quantization error
     **/    
    template<typename T> inline T neuralgas<T>::calculateQuantizationError( const ublas::matrix<T>& p_data, const ublas::matrix<T>& p_prototypes ) const
    {
        ublas::scalar_vector<T> l_ones(p_data.size1(), 1);
        ublas::matrix<T> l_distances( p_prototypes.size1(), p_data.size1() );
        
        for(std::size_t i=0; i < p_prototypes.size1(); ++i)
            ublas::row(l_distances, i) = m_distance->calculate( p_data, ublas::outer_prod( l_ones, ublas::row(p_prototypes, i) ));
        
        return 0.5 * ublas::sum(  m_distance->abs(tools::matrix::min(l_distances, tools::matrix::column))  );  
    }
    
    
    /** calulates distance between datapoints and prototypes and returns a std::vector
     * with index of the nearest prototype
     * @param p_data matrix
     * @return index array of prototype indices
    **/
    template<typename T> inline ublas::indirect_array< std::vector<std::size_t> > neuralgas<T>::use( const ublas::matrix<T>& p_data ) const
    {
        if (p_data.size1() < m_prototypes.size1())
            throw exception::parameter(_("number of datapoints are less than prototypes"));
        
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
    
    
    #ifdef CLUSTER
    
    /** gathering prototypes of every process and return the full prototypes matrix (row oriantated)
     * @param p_mpi MPI object for communication
     * @return full prototypes matrix
     **/
    template<typename T> inline ublas::matrix<T> neuralgas<T>::gatherPrototypes( const mpi::communicator& p_mpi ) const
    {
        // gathering in this way, that every process get all prototypes
        std::vector< ublas::matrix<T> > l_processdata;
        for(int i=0; i < p_mpi.size(); ++i)
            mpi::gather(p_mpi, m_prototypes, l_processdata, i);
        
        // create full prototype matrix with processprotos
        ublas::matrix<T> l_prototypes = l_processdata[0];
        for(std::size_t i=1; i < l_processdata.size(); ++i) {
            l_prototypes.resize( l_prototypes.size1()+l_processdata[i].size1(), l_prototypes.size2());

            ublas::matrix_range< ublas::matrix<T> > l_range(l_prototypes, 
                    ublas::range( l_prototypes.size1()-l_processdata[i].size1(), l_prototypes.size1() ), 
                    ublas::range( 0, l_prototypes.size2() )
            );
            l_range.assign(l_processdata[i]);
        }
        
        return l_prototypes;
    }


    /** gathering prototypes of every process and set with them the local prototypematrix.
     * We can not use const references because of the range
     * @param p_MPI MPI object for communication
     * @param p_localprototypes local prototype matrix
     * @param p_localnorm normalize vector
     **/
    template<typename T> inline void neuralgas<T>::gatherLocalPrototypes( const mpi::communicator& p_mpi, ublas::matrix<T>& p_localprototypes, ublas::vector<T>& p_localnorm )
    {
        std::vector< ublas::matrix<T> > l_localprototypes;
		std::vector< ublas::vector<T> > l_localnorm;
        for(int i=0; i < p_mpi.size(); ++i) {

            // gather prototypes
            ublas::matrix_range< ublas::matrix<T> > l_protorange(p_localprototypes, 
                    ublas::range( m_processprototypinfo[i].first, m_processprototypinfo[i].first + m_processprototypinfo[i].second ), 
                    ublas::range( 0, p_localprototypes.size2() )
            );
            ublas::matrix<T> l_protomatrix = l_protorange;
            mpi::gather(p_mpi, l_protomatrix, l_localprototypes, i);

            // gather norm
            ublas::vector_range< ublas::vector<T> > l_normrange(p_localnorm,
                    ublas::range( m_processprototypinfo[i].first, m_processprototypinfo[i].first + m_processprototypinfo[i].second )
            );
            ublas::vector<T> l_normvec = l_normrange;
            mpi::gather(p_mpi, l_normvec, l_localnorm, i);
        }

        // create local prototypes and normalize
        m_prototypes             = l_localprototypes[0];
        ublas::vector<T> l_norm  = l_localnorm[0];
        for(std::size_t i=1; i < l_localprototypes.size(); ++i) {
            m_prototypes += l_localprototypes[i];
            l_norm       += l_localnorm[i];
        }

        for(std::size_t i=0; i < m_prototypes.size1(); ++i)
            if (!tools::function::isNumericalZero(l_norm(i)))
                ublas::row(m_prototypes, i) /= l_norm(i);
    }


    /** set the std::map with the begin position and size of the prototypes matrix. Is needed for extracting the prototypes
     * of the full matrix for every process
     * @param p_mpi MPI object for communication
    **/
    template<typename T> inline void neuralgas<T>::setProcessPrototypeInfo( const mpi::communicator& p_mpi )
    {
        // gathering the number of prototypes
        std::vector< std::size_t > l_processdata;
        for(int i=0; i < p_mpi.size(); ++i)
            mpi::gather(p_mpi, m_prototypes.size1(), l_processdata, i);
        
        // create map
        std::size_t l_sum = 0;
        for(std::size_t i=0; i < l_processdata.size(); ++i) {
            m_processprototypinfo[static_cast<int>(i)]  = std::pair<std::size_t,std::size_t>(l_sum, l_processdata[i]);
            
            l_sum += l_processdata[i];
        }
    }
    
    
    /** returns the number of all prototypes
     * @param p_mpi MPI object for communication
     * @return number of prototypes
     **/
    template<typename T> inline std::size_t neuralgas<T>::gatherNumberPrototypes( const mpi::communicator& p_mpi ) const
    {
        // gathering number of prototypes
        std::vector<std::size_t> l_numbers;
        for(int i=0; i < p_mpi.size(); ++i)
            mpi::gather(p_mpi, m_prototypes.size1(), l_numbers, i);
        
        // calculate the sum and return
        return std::accumulate( l_numbers.begin(), l_numbers.end(), 0 );;
    }
    
    
    /** train the data on the cluster
     * @overload
     * @param p_mpi MPI object for communication
     * @param p_data datapoints
     * @param p_iterations iterations
     **/
    template<typename T> inline void neuralgas<T>::train( const mpi::communicator& p_mpi, const ublas::matrix<T>& p_data, const std::size_t& p_iterations )
    {
        train(p_mpi, p_data, p_iterations, m_prototypes.size1() * 0.5);
    }

    
    /** train the data on the cluster
     * @overload
     * @param p_mpi MPI object for communication
     * @param p_data datapoints
     * @param p_iterations iterations
     * @param p_lambda max adapet size
     **/
    template<typename T> inline void neuralgas<T>::train( const mpi::communicator& p_mpi, const ublas::matrix<T>& p_data, const std::size_t& p_iterations, const T& p_lambda )
    {
        if (p_data.size1() < m_prototypes.size1())
            throw exception::parameter(_("number of datapoints are less than prototypes"));
        if (p_iterations == 0)
            throw exception::parameter(_("iterations must be greater than zero"));
        if (p_data.size2() != m_prototypes.size2())
            throw exception::matrix(_("data and prototype dimension are not equal"));
        if (p_lambda <= 0)
            throw exception::parameter(_("lambda must be greater than zero"));
        
        
        // process 0 sets the iteration and the lambda and we collect all needed data
        std::size_t l_iterationsBrd = p_iterations;
        T l_lambdaBrd               = p_lambda;
        
        mpi::broadcast(p_mpi, l_iterationsBrd, 0);
        mpi::broadcast(p_mpi, l_lambdaBrd, 0);
        mpi::broadcast(p_mpi, m_logging, 0);
        
        ublas::matrix<T> l_adaptmatrix( gatherNumberPrototypes(p_mpi), p_data.size1() );
        setProcessPrototypeInfo(p_mpi);
         
        
        // creates logging
        if (m_logging) {
            m_logprototypes     = std::vector< ublas::matrix<T> >();
            m_quantizationerror = std::vector< T >();
            m_logprototypes.reserve(p_iterations);
            m_quantizationerror.reserve(p_iterations);
        }
        
        
        // run neural gas       
        const T l_multi = 0.01/l_lambdaBrd;
        ublas::scalar_vector<T> l_ones(p_data.size1(), 1);
        
        for(std::size_t i=0; (i < l_iterationsBrd); ++i) {
            
            // create adapt values
            const T l_lambda = l_lambdaBrd * std::pow(l_multi, static_cast<T>(i)/static_cast<T>(l_iterationsBrd));
            
            // we get all prototypes of every process
            ublas::matrix<T> l_prototypes = gatherPrototypes( p_mpi );
            
            // calculate for every prototype the distance (of the actually prototypes).
            // within the adapt matrix, we must specify the position of the prototypes 
            for(std::size_t n=0; n < l_prototypes.size1(); ++n)
                ublas::row(l_adaptmatrix, n)  = m_distance->calculate( p_data,  ublas::outer_prod(l_ones, ublas::row(l_prototypes, n)) );

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
                
            // calculate all local prototype for the local data points
            l_prototypes = ublas::prod( l_adaptmatrix, p_data );

            // calculating local norm for the prototypes
            ublas::vector<T> l_normvec(l_prototypes.size1(),0);
            for(std::size_t n=0; n < l_prototypes.size1(); ++n)
                l_normvec(n) = ublas::sum( ublas::row(l_adaptmatrix, n) );

            gatherLocalPrototypes(p_mpi, l_prototypes, l_normvec);
            
            
            // determine quantization error for logging
            if (m_logging) {
                
                // we must normalize the local prototypes (only on logging, in other cases gatherLocalPrototypes do this)
                for(std::size_t n=0; n < l_prototypes.size1(); ++n)
                    if (!tools::function::isNumericalZero(l_normvec(n)))
                        ublas::row(l_prototypes, n) /= l_normvec(n);
                
                m_logprototypes.push_back( m_prototypes );
                m_quantizationerror.push_back( calculateQuantizationError(p_data, l_prototypes) );
            }
        }
    }
    
    
    /** return all prototypes of the cluster
     * @overload
     * @param p_mpi MPI object for communication
     * @return matrix (rows = prototypes)
     **/
    template<typename T> inline ublas::matrix<T> neuralgas<T>::getPrototypes( const mpi::communicator& p_mpi ) const
    {
        return gatherPrototypes( p_mpi );
    }
    
    
    /** returns all logged prototypes in all processes
     * @overload
     * @param p_mpi MPI object for communication
     * @retun std::vector with all logged prototypes
     **/
    template<typename T> inline std::vector< ublas::matrix<T> > neuralgas<T>::getLoggedPrototypes( const mpi::communicator& p_mpi ) const
    {
        // we must gather every logged prototype and create the full prototype matrix
        std::vector< std::vector< ublas::matrix<T> > > l_gatherProto;
        for(int i=0; i < p_mpi.size(); ++i)
            mpi::gather(p_mpi, m_logprototypes, l_gatherProto, i);
        
        // now we create the full prototype matrix for every log
        std::vector< ublas::matrix<T> > l_logProto = l_gatherProto[0];
        for(std::size_t i=1; i < l_gatherProto.size(); ++i)
            for(std::size_t n=0; n < l_logProto.size(); ++n) {
            
                l_logProto[n].resize( l_logProto[n].size1()+l_gatherProto[i][n].size1(), l_logProto[n].size2());
        
                ublas::matrix_range< ublas::matrix<T> > l_range(l_logProto[n], 
                                                                ublas::range( l_logProto[n].size1()-l_gatherProto[i][n].size1(), l_logProto[n].size1() ), 
                                                                ublas::range( 0, l_logProto[n].size2() )
                                                                );
                l_range.assign(l_gatherProto[i][n]);
            }
        
        
        return l_logProto;
    }
    
    
    /** returns the logged quantisation error
     * @overload
     * @param p_mpi MPI object for communication
     * @return std::vector with quantization error
     **/
    template<typename T> inline std::vector<T> neuralgas<T>::getLoggedQuantizationError( const mpi::communicator& p_mpi ) const
    {
        // we must call the quantization error of every process and sum all values for the main error
        std::vector< std::vector<T> > l_gatherError;
        for(int i=0; i < p_mpi.size(); ++i)
            mpi::gather(p_mpi, m_quantizationerror, l_gatherError, i);
        
        // we get every quantization error in 0.5 * sum-over-local-datapoints,
        // so we must for full sum multiplicate with 2
        std::vector<T> l_error = l_gatherError[0];
        for(std::size_t i=1; i < l_gatherError.size(); ++i)
            for(std::size_t n=0; n < l_error.size(); ++n)
                l_error[n] += 2.0*l_gatherError[i][n];
            
        // now we can create the correct error
        for(std::size_t i=0; i < l_error.size(); ++i)
            l_error[i] *= 0.5;
        
        return l_error;
            
    }
    
    #endif
    
};};};


#endif
