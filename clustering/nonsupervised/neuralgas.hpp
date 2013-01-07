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


#ifndef __MACHINELEARNING_CLUSTERING_NONSUPERVISED_NEURALGAS_HPP
#define __MACHINELEARNING_CLUSTERING_NONSUPERVISED_NEURALGAS_HPP


#include <omp.h>

#include <numeric>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/bindings/blas.hpp>
#ifdef MACHINELEARNING_MPI
#include <boost/mpi.hpp>
#endif

#include "clustering.hpp"
#include "../../errorhandling/exception.hpp"
#include "../../tools/tools.h"
#include "../../distances/distances.h"



namespace machinelearning { namespace clustering { namespace nonsupervised {
    
    #ifndef SWIG
    namespace ublas = boost::numeric::ublas;
    #ifdef MACHINELEARNING_MPI
    namespace mpi   = boost::mpi;
    #endif
    #endif

    
    /** class for calculate (batch) neural gas
     * @note The MPI methods do not check the correct ranges / dimension of the prototype
     * data, so it is the task of the developer to use the correct ranges. Also the MPI
     * methods must be called in the correct order, so the MPI calls must be run
     * on each process.
     **/
    template<typename T> class neuralgas : public clustering<T>, public patchclustering<T>
        #ifdef MACHINELEARNING_MPI 
        , public mpiclustering<T>, public mpipatchclustering<T>
        #endif
    {
        
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
            ublas::indirect_array<> use( const ublas::matrix<T>& ) const;
        
            // derived from patch clustering
            ublas::vector<T> getPrototypeWeights( void ) const;
            void trainpatch( const ublas::matrix<T>&, const std::size_t& );
            void trainpatch( const ublas::matrix<T>&, const std::size_t&, const T& );
            std::vector< ublas::vector<T> > getLoggedPrototypeWeights( void ) const;
             
            #ifdef MACHINELEARNING_MPI
            void train( const mpi::communicator&, const ublas::matrix<T>&, const std::size_t& );
            void train( const mpi::communicator&, const ublas::matrix<T>&, const std::size_t&, const T& );
            ublas::matrix<T> getPrototypes( const mpi::communicator& ) const;
            std::vector< ublas::matrix<T> > getLoggedPrototypes( const mpi::communicator& ) const;
            std::vector<T> getLoggedQuantizationError( const mpi::communicator& ) const;
            ublas::indirect_array<> use( const mpi::communicator&, const ublas::matrix<T>& ) const;
            void use( const mpi::communicator& ) const;
        
            // derived from patch clustering
            ublas::vector<T> getPrototypeWeights( const mpi::communicator& ) const;
            void trainpatch( const mpi::communicator&, const ublas::matrix<T>&, const std::size_t& );
            void trainpatch( const mpi::communicator&, const ublas::matrix<T>&, const std::size_t&, const T& );
            std::vector< ublas::vector<T> > getLoggedPrototypeWeights( const mpi::communicator& ) const;
            #endif
        
        
        
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
            /** prototype weights for patch clustering **/
            ublas::vector<T> m_prototypeWeights;
            /** std::vector for logging the prototype weights **/
            std::vector< ublas::vector<T> > m_logprototypeWeights;
            /** bool for check initialized patch **/
            bool m_firstpatch;
            
            T calculateQuantizationError( const ublas::matrix<T>&, const ublas::matrix<T>& ) const;
            
            #ifdef MACHINELEARNING_MPI
            /** map with information to every process and prototype**/
            std::vector< std::pair<std::size_t,std::size_t> > m_processprototypinfo;
            
            void synchronizePrototypes( const mpi::communicator&, ublas::matrix<T>&, ublas::vector<T>& );
            void synchronizePrototypeWeights( const mpi::communicator&, ublas::vector<T>& );
            ublas::matrix<T> gatherAllPrototypes( const mpi::communicator& ) const;
            std::size_t getNumberPrototypes( const mpi::communicator& ) const;
            void setProcessPrototypeInfo( const mpi::communicator& );
            #endif
    };
    
    
    
    /** contructor for initialization the neural gas
     * @param p_distance distance object
     * @param p_prototypes number of prototypes
     * @param p_prototypesize size of each prototype (data dimension)
     **/
    template<typename T> inline neuralgas<T>::neuralgas( const distances::distance<T>& p_distance, const std::size_t& p_prototypes, const std::size_t& p_prototypesize ) :
        m_distance( p_distance ),
        m_prototypes( tools::matrix::random<T>(p_prototypes, p_prototypesize) ),
        m_logging( false ),
        m_logprototypes( std::vector< ublas::matrix<T> >() ),
        m_quantizationerror( std::vector<T>() ),
        m_prototypeWeights( p_prototypes, 0 ),
        m_logprototypeWeights(),
        m_firstpatch(true)
        #ifdef MACHINELEARNING_MPI
        , m_processprototypinfo()
        #endif
    {
        if (p_prototypesize == 0)
            throw exception::runtime(_("prototype size must be greater than zero"), *this);
    }
    
    
    /** returns the prototype matrix
     * @return matrix (rows = number of prototypes)
     **/
    template<typename T> inline ublas::matrix<T> neuralgas<T>::getPrototypes( void ) const
    {
        return m_prototypes;
    }
    
    
    
    /** enabled logging for training
     * @param p_log bool
     **/
    template<typename T> inline void neuralgas<T>::setLogging( const bool& p_log )
    {
        m_logging = p_log;
        m_logprototypeWeights.clear();
        m_logprototypes.clear();
        m_quantizationerror.clear();
    }
    
    
    
    /** shows the logging status
     * @return bool
     **/
    template<typename T> inline bool neuralgas<T>::getLogging( void ) const
    {
        return m_logging && (m_logprototypes.size() > 0);
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
    }
    
    
    /** returns the number of prototypes
     * @return number of the prototypes / classes
     **/
    template<typename T> inline std::size_t neuralgas<T>::getPrototypeCount( void ) const 
    {
        return m_prototypes.size1();
    }
    
    
    /** returns the quantisation error 
     * @return error for each iteration
     **/
    template<typename T> inline std::vector<T> neuralgas<T>::getLoggedQuantizationError( void ) const
    {
        return m_quantizationerror;
    }    
    
    
    /** train the prototypes
     * @param p_data data matrix
     * @param p_iterations number of iterations
     **/
    template<typename T> inline void neuralgas<T>::train( const ublas::matrix<T>& p_data, const std::size_t& p_iterations )
    {
        train(p_data, p_iterations, m_prototypes.size1() * 0.5);
    }
    
    
    /** returns the weights of prototypes on patch clustering
     * @return weights vector
     **/
    template<typename T> inline ublas::vector<T> neuralgas<T>::getPrototypeWeights( void ) const
    {
        return m_prototypeWeights;
    }
    
    
    /** returns the log of the prototype weights
     * @return std::vector with weight vector
     **/
    template<typename T> inline std::vector< ublas::vector<T> > neuralgas<T>::getLoggedPrototypeWeights( void ) const
    {
        return m_logprototypeWeights;
    }
    
    /** training the prototypes
     * @param p_data datapoints
     * @param p_iterations iterations
     * @param p_lambda max adapet size
     **/
    template<typename T> inline void neuralgas<T>::train( const ublas::matrix<T>& p_data, const std::size_t& p_iterations, const T& p_lambda )
    {
        if (m_prototypes.size1() == 0)
            throw exception::runtime(_("number of prototypes must be greater than zero"), *this);
        if (p_data.size1() < m_prototypes.size1())
            throw exception::runtime(_("number of datapoints are less than prototypes"), *this);
        if (p_iterations == 0)
            throw exception::runtime(_("iterations must be greater than zero"), *this);
        if (p_data.size2() != m_prototypes.size2())
            throw exception::runtime(_("data and prototype dimension are not equal"), *this);
        if (p_lambda <= 0)
            throw exception::runtime(_("lambda must be greater than zero"), *this);
        
        // creates logging
        if (m_logging) {
            m_logprototypes.clear();
            m_quantizationerror.clear();
            m_logprototypes.reserve(p_iterations);
            m_quantizationerror.reserve(p_iterations);
        }

        
        // run neural gas       
        const T l_multi = 0.01/p_lambda;
        ublas::matrix<T> l_adaptmatrix( m_prototypes.size1(), p_data.size1() );
        ublas::vector<T> l_lambda(m_prototypes.size1());
        
        for(std::size_t i=0; i < p_iterations; ++i) {
            
            // determine quantization error for logging
            if (m_logging) {
                m_logprototypes.push_back( m_prototypes );
                m_quantizationerror.push_back( calculateQuantizationError(p_data, m_prototypes) );
            }
            
            
            // create adapt values
            const T l_lambdahelp = p_lambda * std::pow(l_multi, static_cast<T>(i)/static_cast<T>(p_iterations));

            #pragma omp parallel for shared(l_lambda)
            for(std::size_t n=0; n < l_lambda.size(); ++n)
                l_lambda(n) = std::exp( -static_cast<T>(n) / l_lambdahelp );

                                
            // calculate for every prototype the distance
            #pragma omp parallel for shared(l_adaptmatrix)
            for(std::size_t n=0; n < m_prototypes.size1(); ++n)
                ublas::row(l_adaptmatrix, n)  = m_distance.getDistance( p_data, ublas::row(m_prototypes, n) );

            
            // for every column ranks values and create adapts
            // we need rank and not randIndex, because we 
            // use the value of the ranking for getting the 
            // adapt value
            #pragma omp parallel for shared(l_adaptmatrix)
            for(std::size_t n=0; n < l_adaptmatrix.size2(); ++n) {
                ublas::vector<T> l_column                = ublas::column(l_adaptmatrix, n);
                const ublas::vector<std::size_t> l_rank  = tools::vector::rank(l_column);
                
                for(std::size_t j=0; j < l_rank.size(); ++j)
                    l_adaptmatrix(j,n) = l_lambda(l_rank(j));
            }

            

            // create prototypes
            m_prototypes = ublas::prod( l_adaptmatrix, p_data );
            
            // normalize prototypes
            #pragma omp parallel for
            for(std::size_t n=0; n < m_prototypes.size1(); ++n) {
                const T l_norm = ublas::sum( ublas::row(l_adaptmatrix, n) );
                
                if (!tools::function::isNumericalZero(l_norm))
                    ublas::row(m_prototypes, n) /= l_norm;
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
        ublas::matrix<T> l_distances( p_prototypes.size1(), p_data.size1() );
        
        #pragma omp parallel for shared(l_distances)
        for(std::size_t i=0; i < p_prototypes.size1(); ++i)
            ublas::row(l_distances, i) = m_distance.getDistance( p_data, ublas::row(p_prototypes, i) );
        
        return 0.5 * ublas::sum(  m_distance.getAbs(tools::matrix::min(l_distances, tools::matrix::column))  );  
    }
    
    
    /** calulates distance between datapoints and prototypes and returns a indirect array
     * with index of the nearest prototype
     * @param p_data matrix
     * @return index array of prototype indices
     **/
    template<typename T> inline ublas::indirect_array<> neuralgas<T>::use( const ublas::matrix<T>& p_data ) const
    {
        if (m_prototypes.size1() == 0)
            throw exception::runtime(_("number of prototypes must be greater than zero"), *this);
        if (p_data.size2() != m_prototypes.size2())
            throw exception::runtime(_("data and prototype dimension are not equal"), *this);
        
        ublas::indirect_array<> l_idx(p_data.size1());
        ublas::matrix<T> l_distance(m_prototypes.size1(), p_data.size1());
        
        // calculate distance for every prototype
        #pragma omp parallel for shared(l_distance)
        for(std::size_t i=0; i < m_prototypes.size1(); ++i)
            ublas::row(l_distance, i)  = m_distance.getDistance( p_data,  ublas::row(m_prototypes, i) );
        
        // determine nearest prototype
        #pragma omp parallel for shared(l_idx)
        for(std::size_t i=0; i < l_distance.size2(); ++i) {
            ublas::vector<T> l_col                = ublas::column(l_distance, i);
            const ublas::indirect_array<> l_rank  = tools::vector::rankIndex( l_col );
            l_idx[i] = l_rank(0);
         }
        
        return l_idx;
    }
  
    
    /** train a patch (input data) with the data (include the weights)
     * @param p_data datapoints
     * @param p_iterations iterations
     **/
    template<typename T> inline void neuralgas<T>::trainpatch( const ublas::matrix<T>& p_data, const std::size_t& p_iterations )
    {
        trainpatch(p_data, p_iterations, m_prototypes.size1() * 0.5);
    }
    
     
    /** train a patch (input data) with the data (include the weights)
     * @param p_data datapoints
     * @param p_iterations iterations
     * @param p_lambda max adapet size
     **/
    template<typename T> inline void neuralgas<T>::trainpatch( const ublas::matrix<T>& p_data, const std::size_t& p_iterations, const T& p_lambda )
    {
        if (m_prototypes.size1() == 0)
            throw exception::runtime(_("number of prototypes must be greater than zero"), *this);
        if (p_data.size1() < m_prototypes.size1())
            throw exception::runtime(_("number of datapoints are less than prototypes"), *this);
        if (p_iterations == 0)
            throw exception::runtime(_("iterations must be greater than zero"), *this);
        if (p_data.size2() != m_prototypes.size2())
            throw exception::runtime(_("data and prototype dimension are not equal"), *this);
        if (p_lambda <= 0)
            throw exception::runtime(_("lambda must be greater than zero"), *this);
        
        // creates logging
        if (m_logging) {
            m_logprototypes.clear();
            m_quantizationerror.clear();
            m_logprototypes.reserve(p_iterations);
            m_quantizationerror.reserve(p_iterations);
        }
        
        // if not the first patch add prototypes to data at the end and set the multiplier
        ublas::matrix<T> l_data(p_data);
        ublas::vector<std::size_t> l_multiplier(l_data.size1(), 1);
        if (!m_firstpatch) {
            
            // resize data matrix
            l_data.resize( l_data.size1()+m_prototypes.size1(), l_data.size2());
            ublas::matrix_range< ublas::matrix<T> > l_datarange(l_data, 
                                                            ublas::range( l_data.size1()-m_prototypes.size1(), l_data.size1() ), 
                                                            ublas::range( 0, l_data.size2() )
                                                            );
            l_datarange.assign(m_prototypes);
            
            // resize multiplier
            l_multiplier.resize( l_multiplier.size()+m_prototypeWeights.size() );
            ublas::vector_range< ublas::vector<std::size_t> > l_multiplierrange( l_multiplier, ublas::range( l_multiplier.size()-m_prototypeWeights.size(), l_multiplier.size()) );
            l_multiplierrange.assign(m_prototypeWeights);
        }
     

        // run neural gas       
        const T l_multi = 0.01/p_lambda;
        ublas::matrix<T> l_adaptmatrix( m_prototypes.size1(), p_data.size1() );
        ublas::vector<T> l_lambda(m_prototypes.size1());
        
        for(std::size_t i=0; i < p_iterations; ++i) {
            
            // determine quantization error for logging
            if (m_logging) {
                m_logprototypes.push_back( m_prototypes );
                m_quantizationerror.push_back( calculateQuantizationError(l_data, m_prototypes) );
            }
            
            
            // create adapt values
            const T l_lambdahelp = p_lambda * std::pow(l_multi, static_cast<T>(i)/static_cast<T>(p_iterations));
            
            #pragma omp parallel for shared(l_lambda)
            for(std::size_t n=0; n < l_lambda.size(); ++n)
                l_lambda(n) = std::exp( -static_cast<T>(n) / l_lambdahelp );
            
            
            // calculate for every prototype the distance
            #pragma omp parallel for shared(l_adaptmatrix)
            for(std::size_t n=0; n < m_prototypes.size1(); ++n)
                ublas::row(l_adaptmatrix, n)  = m_distance.getDistance( l_data, ublas::row(m_prototypes, n) ) ;
            
            
            // for every column ranks values and create adapts
            // we need rank and not randIndex, because we 
            // use the value of the ranking for getting the 
            // adapt value
            #pragma omp parallel for shared(l_adaptmatrix)
            for(std::size_t n=0; n < l_adaptmatrix.size2(); ++n) {
                ublas::vector<T> l_column                = ublas::column(l_adaptmatrix, n);
                const ublas::vector<std::size_t> l_rank  = tools::vector::rank(l_column);
                
                for(std::size_t j=0; j < l_rank.size(); ++j)
                    l_adaptmatrix(j,n) = l_lambda(l_rank(j));
            }
            
            
            // add multiplier
            #pragma omp parallel for shared(l_adaptmatrix)
            for(std::size_t n=0; n < l_adaptmatrix.size1(); ++n)
                ublas::row(l_adaptmatrix, n) = ublas::element_prod( ublas::row(l_adaptmatrix, n), l_multiplier );
            
            // create prototypes
            m_prototypes = ublas::prod( l_adaptmatrix, l_data );
            
            
            // normalize prototypes
            #pragma omp parallel for
            for(std::size_t n=0; n < m_prototypes.size1(); ++n) {
                const T l_norm = ublas::sum( ublas::row(l_adaptmatrix, n) );
                
                if (!tools::function::isNumericalZero(l_norm))
                    ublas::row(m_prototypes, n) /= l_norm;
            }
        }
        
        // determine size of receptive fields, but we use only the data points
        const ublas::indirect_array<> l_winner = use(p_data);
        #pragma omp parallel for
        for(std::size_t i=0; i < l_winner.size(); ++i)
            #pragma omp critical
            m_prototypeWeights( l_winner(i) )++;
        
        if (m_logging)
            m_logprototypeWeights.push_back(m_prototypeWeights);
        
        // set bool for the first patch
        m_firstpatch = false;
    }
    

    
    //======= MPI ==================================================================================================================================
    #ifdef MACHINELEARNING_MPI
    
    /** gathering prototypes of every process and return the full prototypes matrix (row oriantated)
     * @param p_mpi MPI object for communication
     * @return full prototypes matrix
     **/
    template<typename T> inline ublas::matrix<T> neuralgas<T>::gatherAllPrototypes( const mpi::communicator& p_mpi ) const
    {
        // gathering in this way, that every process get all prototypes
        std::vector< ublas::matrix<T> > l_prototypedata;
        mpi::all_gather(p_mpi, m_prototypes, l_prototypedata);
        
        // create full prototype matrix with processprotos
        ublas::matrix<T> l_prototypes = l_prototypedata[0];
        for(std::size_t i=1; i < l_prototypedata.size(); ++i) {
            l_prototypes.resize( l_prototypes.size1()+l_prototypedata[i].size1(), l_prototypes.size2());
            
            ublas::matrix_range< ublas::matrix<T> > l_range(l_prototypes, 
                                                            ublas::range( l_prototypes.size1()-l_prototypedata[i].size1(), l_prototypes.size1() ), 
                                                            ublas::range( 0, l_prototypes.size2() )
                                                            );
            l_range.assign(l_prototypedata[i]);
        }
        
        return l_prototypes;
    }
    
    
    
    /** synchronize prototypes of every process and set with them the local prototypematrix.
     * We can not use const references because of the range. The idea is the commutativity
     * of the dot product of every prototype dimension. The prototypes are a matrix-matrix-product of the
     * adaption value and the data values. The matrix-matrix-product is a dot product of rows and columns so
     * the commutativity is used for parallelism / the gathering uses the commutativity for create the correct prototype values
     * @param p_mpi MPI object for communication
     * @param p_localprototypes local prototype matrix
     * @param p_localnorm normalize vector
     **/
    template<typename T> inline void neuralgas<T>::synchronizePrototypes( const mpi::communicator& p_mpi, ublas::matrix<T>& p_localprototypes, ublas::vector<T>& p_localnorm )
    {
        // create for each process the norm and prototypes
        // we need two vectors, in which the index is the process ID and sends the data back to the process
        std::vector< ublas::matrix<T> > l_prototypes;
		std::vector< ublas::vector<T> > l_norm;
        for(int i=0; i < p_mpi.size(); ++i) {
            
            // prototypes
            ublas::matrix_range< ublas::matrix<T> > l_protorange(p_localprototypes, 
                                                                 ublas::range( m_processprototypinfo[i].first, m_processprototypinfo[i].first + m_processprototypinfo[i].second ), 
                                                                 ublas::range( 0, p_localprototypes.size2() )
                                                                 );
            l_prototypes.push_back(l_protorange);
            
            //norm
            ublas::vector_range< ublas::vector<T> > l_normrange(p_localnorm,
                                                                ublas::range( m_processprototypinfo[i].first, m_processprototypinfo[i].first + m_processprototypinfo[i].second )
                                                                );
            l_norm.push_back(l_normrange);
        }
        
        
        // collect the norm and prototype data in each process
		std::vector< ublas::vector<T> > l_collectnorm;
        std::vector< ublas::matrix<T> > l_collectprototypes;
        
        mpi::all_to_all( p_mpi, l_norm, l_collectnorm );
        mpi::all_to_all( p_mpi, l_prototypes, l_collectprototypes );
        
        
        // both std::vectors will be summerized
        m_prototypes = std::accumulate( l_collectprototypes.begin(), l_collectprototypes.end(), ublas::matrix<T>(m_prototypes.size1(), m_prototypes.size2(), 0) );
        const ublas::vector<T> l_sumnorm    = std::accumulate( l_collectnorm.begin(), l_collectnorm.end(), ublas::vector<T>(l_collectnorm[0].size(), 0) );
        
        // normalize the prototypes
        for(std::size_t i=0; i < l_sumnorm.size(); ++i)
            if (!tools::function::isNumericalZero(l_sumnorm(i)))
                ublas::row(m_prototypes, i) /= l_sumnorm(i);
    }
    
    
    
    /** sets the std::vector with the begin position and size of the prototypes matrix. Is required for the extraction of prototypes
     * of the full matrix for each process
     * @param p_mpi MPI object for communication
     **/
    template<typename T> inline void neuralgas<T>::setProcessPrototypeInfo( const mpi::communicator& p_mpi )
    {
        m_processprototypinfo.clear();
        // gathering the number of prototypes
        std::vector< std::size_t > l_processdata;
        mpi::all_gather(p_mpi, m_prototypes.size1(), l_processdata);
        
        // create map
        std::size_t l_sum = 0;
        for(std::size_t i=0; i < l_processdata.size(); ++i) {
            m_processprototypinfo.push_back( std::pair<std::size_t,std::size_t>(l_sum, l_processdata[i]) );
            l_sum += l_processdata[i];
        }
    }
    
    
    /** returns the number of all prototypes
     * @param p_mpi MPI object for communication
     * @return number of prototypes
     **/
    template<typename T> inline std::size_t neuralgas<T>::getNumberPrototypes( const mpi::communicator& p_mpi ) const
    {
        std::size_t l_count = 0;
        mpi::all_reduce(p_mpi, m_prototypes.size1(), l_count, std::plus<std::size_t>());
        return l_count;
    }
    
    
    /** train the data on the cluster
     * @param p_mpi MPI object for communication
     * @param p_data datapoints
     * @param p_iterations iterations
     **/
    template<typename T> inline void neuralgas<T>::train( const mpi::communicator& p_mpi, const ublas::matrix<T>& p_data, const std::size_t& p_iterations )
    {
        // if the process has no prototypes, than lambda need not be zero, so we set it to a minimal numerical value, so the exception is not thrown
        train(p_mpi, p_data, p_iterations, ((m_prototypes.size1() == 0) ? std::numeric_limits<T>::epsilon() :  m_prototypes.size1() * 0.5) );
    }
    
    
    /** train the data on the cluster
     * @param p_mpi MPI object for communication
     * @param p_data datapoints
     * @param p_iterations iterations
     * @param p_lambda max adapet size
     **/
    template<typename T> inline void neuralgas<T>::train( const mpi::communicator& p_mpi, const ublas::matrix<T>& p_data, const std::size_t& p_iterations, const T& p_lambda )
    {
        if (p_data.size1() < m_prototypes.size1())
            throw exception::runtime(_("number of datapoints are less than prototypes"), *this);
        if (p_iterations == 0)
            throw exception::runtime(_("iterations must be greater than zero"), *this);
        if (p_data.size2() != m_prototypes.size2())
            throw exception::runtime(_("data and prototype dimension are not equal"), *this);
        if (p_lambda <= 0)
            throw exception::runtime(_("lambda must be greater than zero"), *this);
        
        
        // we use the max. of all values of each process
        const std::size_t l_iterationsMPI = mpi::all_reduce(p_mpi, p_iterations, mpi::maximum<std::size_t>());
        const T l_lambdaMPI               = mpi::all_reduce(p_mpi, p_lambda, mpi::maximum<T>());
        m_logging                         = mpi::all_reduce(p_mpi, m_logging, std::multiplies<bool>());
        setProcessPrototypeInfo(p_mpi);
        
        // creates logging
        if (m_logging) {
            m_logprototypes     = std::vector< ublas::matrix<T> >();
            m_quantizationerror = std::vector< T >();
            m_logprototypes.reserve(l_iterationsMPI);
            m_quantizationerror.reserve(l_iterationsMPI);
        }
        
        
        // run neural gas       
        const T l_multi = 0.01/l_lambdaMPI;
        ublas::vector<T> l_normvec( getNumberPrototypes(p_mpi), 0 );
        ublas::vector<T> l_lambda(l_normvec.size());
        ublas::matrix<T> l_adaptmatrix( l_normvec.size(), p_data.size1() );
        
        for(std::size_t i=0; (i < l_iterationsMPI); ++i) {
            
            // create adapt values
            const T l_lambdahelp = l_lambdaMPI * std::pow(l_multi, static_cast<T>(i)/static_cast<T>(l_iterationsMPI));
            
            #pragma omp parallel for shared(l_lambda)
            for(std::size_t n=0; n < l_lambda.size(); ++n)
                l_lambda(n) = std::exp( -static_cast<T>(n) / l_lambdahelp );

            
            // we get all prototypes of every process
            ublas::matrix<T> l_prototypes = gatherAllPrototypes( p_mpi );
            
            
            // determine quantization error for logging
            if (m_logging) {
                m_logprototypes.push_back( m_prototypes );
                m_quantizationerror.push_back( calculateQuantizationError(p_data, l_prototypes) );
            }
            
            
            // calculate for every prototype the distance (of the actually prototypes).
            // within the adapt matrix, we must specify the position of the prototypes 
            #pragma omp parallel for shared(l_adaptmatrix)
            for(std::size_t n=0; n < l_prototypes.size1(); ++n)
                ublas::row(l_adaptmatrix, n)  = m_distance.getDistance( p_data, ublas::row(l_prototypes, n) ) ;
            
            
            // for every column ranks values and create adapts
            // we need rank and not randIndex, because we 
            // use the value of the ranking for getting the 
            // adapt value
            #pragma omp parallel for shared(l_adaptmatrix)
            for(std::size_t n=0; n < l_adaptmatrix.size2(); ++n) {
                ublas::vector<T> l_column                = ublas::column(l_adaptmatrix, n);
                const ublas::vector<std::size_t> l_rank  = tools::vector::rank(l_column);
                
                for(std::size_t j=0; j < l_rank.size(); ++j)
                    l_adaptmatrix(j,n) = l_lambda(l_rank(j));
            }
            
            
            // create local prototypes
            l_prototypes = ublas::prod( l_adaptmatrix, p_data );
            
            
            // normalize prototypes
            #pragma omp parallel for shared(l_normvec)
            for(std::size_t n=0; n < l_prototypes.size1(); ++n)
                l_normvec(n) = ublas::sum( ublas::row(l_adaptmatrix, n) );
            
            synchronizePrototypes(p_mpi, l_prototypes, l_normvec);
        }
    }
    
    
    /** return all prototypes of the cluster
     * @param p_mpi MPI object for communication
     * @return matrix (rows = prototypes)
     **/
    template<typename T> inline ublas::matrix<T> neuralgas<T>::getPrototypes( const mpi::communicator& p_mpi ) const
    {
        return gatherAllPrototypes( p_mpi );
    }
    
    
    /** returns all logged prototypes in all processes
     * @param p_mpi MPI object for communication
     * @return std::vector with all logged prototypes
     **/
    template<typename T> inline std::vector< ublas::matrix<T> > neuralgas<T>::getLoggedPrototypes( const mpi::communicator& p_mpi ) const
    {
        // we must gather every logged prototype and create the full prototype matrix
        std::vector< std::vector< ublas::matrix<T> > > l_gatherProto;
        mpi::all_gather(p_mpi, m_logprototypes, l_gatherProto);

        // now we create the full prototype matrix for every log
        std::vector< ublas::matrix<T> > l_logProto = l_gatherProto[0];
        for(std::size_t i=1; i < l_gatherProto.size(); ++i)
            for(std::size_t n=0; n < l_gatherProto[i].size(); ++n) {
                
                // resizing must be in the correct way, so we check the dimensions (the condition 
                // must check if the prototypes are empty)
                if (l_gatherProto[i][n].size2() < l_logProto[n].size2())
                    l_logProto[n].resize( l_logProto[n].size1()+l_gatherProto[i][n].size1(), l_logProto[n].size2());
                else
                    l_logProto[n].resize( l_logProto[n].size1()+l_gatherProto[i][n].size1(), l_gatherProto[i][n].size2());
                
                
                ublas::matrix_range< ublas::matrix<T> > l_range(l_logProto[n], 
                                                                ublas::range( l_logProto[n].size1()-l_gatherProto[i][n].size1(), l_logProto[n].size1() ), 
                                                                ublas::range( 0, l_logProto[n].size2() )
                                                                );
                l_range.assign(l_gatherProto[i][n]);
            }
        
        
        return l_logProto;
    }
    
    
    /** returns the logged quantisation error
     * @param p_mpi MPI object for communication
     * @return std::vector with quantization error
     **/
    template<typename T> inline std::vector<T> neuralgas<T>::getLoggedQuantizationError( const mpi::communicator& p_mpi ) const
    {
        // we must call the quantization error of every process and sum all values for the main error
        std::vector< std::vector<T> > l_gatherError;
        mpi::all_gather(p_mpi, m_quantizationerror, l_gatherError);
        
        // we get every quantization error (if the prototypes are empty on the process, the quantization error exists for all other prototypes)
        std::vector<T> l_error = l_gatherError[0];
        for(std::size_t i=1; i < l_gatherError.size(); ++i)
            for(std::size_t n=0; n < l_gatherError[i].size(); ++n)
                l_error[n] += l_gatherError[i][n];
        
        return l_error;
        
    }
    
    
    /** calulates distance between datapoints and prototypes and returns a indirect array
     * with index of the nearest prototype
     * @param p_mpi MPI object for communication     
     * @param p_data matrix
     * @return index array of prototype indices
     **/
    template<typename T> inline ublas::indirect_array<> neuralgas<T>::use( const mpi::communicator& p_mpi, const ublas::matrix<T>& p_data ) const
    {
        if (p_data.size2() != m_prototypes.size2())
            throw exception::runtime(_("data and prototype dimension are not equal"), *this);
        
        
        //first we gathering all other prototypes
        const ublas::matrix<T> l_prototypes = gatherAllPrototypes( p_mpi );
        
        ublas::indirect_array<> l_idx(p_data.size1());
        ublas::matrix<T> l_distance(l_prototypes.size1(), p_data.size1());
        
        // calculate distance for every prototype
        for(std::size_t i=0; i < l_prototypes.size1(); ++i)
            ublas::row(l_distance, i)  = m_distance.getDistance( p_data, ublas::row(l_prototypes, i) ) ;
        
        // determine nearest prototype
        for(std::size_t i=0; i < l_distance.size2(); ++i) {
            ublas::vector<T> l_col                = ublas::column(l_distance, i);
            const ublas::indirect_array<> l_rank  = tools::vector::rankIndex( l_col );
            l_idx[i] = l_rank(0);
        }
        
        return l_idx;
    }
    
    
    /** blank method for receiving all prototypes. The method can be used in combination with the use-method and the matrix parameter, so
     * only one process can calculate the distances between prototypes and its data, all other process must call only this methode
     * @param p_mpi MPI object for communication 
     **/
    template<typename T> inline void neuralgas<T>::use( const mpi::communicator& p_mpi ) const
    {
        gatherAllPrototypes( p_mpi );
    }
    
    
    /** returns the log of the prototype weight
     * @param p_mpi MPI object for communication 
     * @return std::vector with weight vector
     **/
    template<typename T> inline std::vector< ublas::vector<T> > neuralgas<T>::getLoggedPrototypeWeights( const mpi::communicator& p_mpi ) const
    {
        // we must gather every logged weight
        std::vector< std::vector< ublas::vector<T> > > l_gatherWeights;
        mpi::all_gather(p_mpi, m_logprototypeWeights, l_gatherWeights);
        
        // now we create the full weight vector for every log
        std::vector< ublas::vector<T> > l_logWeight = l_gatherWeights[0];
        for(std::size_t i=1; i < l_gatherWeights.size(); ++i)
            for(std::size_t n=0; n < l_gatherWeights[i].size(); ++n) {
                l_logWeight[n].resize( l_logWeight[n].size()+l_gatherWeights[i][n].size() );
                
                ublas::vector_range< ublas::vector<T> > l_range(l_logWeight[n],
                                                                ublas::range( l_logWeight[n].size()-l_gatherWeights[i][n].size(), l_logWeight[n].size() )
                                                               );
                
                l_range.assign(l_gatherWeights[i][n]);
            }
        
        
        return l_logWeight;
    }
    
    
    /** returns the weights of prototypes on patch clustering
     * @param p_mpi MPI object for communication
     * @return weights vector
     **/
    template<typename T> inline ublas::vector<T> neuralgas<T>::getPrototypeWeights( const mpi::communicator& p_mpi ) const
    {
        std::vector< ublas::vector<T> > l_weightdata;
        mpi::all_gather(p_mpi, m_prototypeWeights, l_weightdata);
        
        // create full weight vector with processweights
        ublas::vector<T> l_weights = l_weightdata[0];
        for(std::size_t i=1; i < l_weightdata.size(); ++i) {
            l_weights.resize( l_weights.size()+l_weightdata[i].size() );
            
            ublas::vector_range< ublas::vector<T> > l_range(l_weights,
                                                            ublas::range( l_weights.size()-l_weightdata[i].size(), l_weights.size() )
                                                           );
            l_range.assign(l_weightdata[i]);
        }
        
        return l_weights;
    }
    
    
    /** train a patch (input data) with the data (include the weights)
     * @param p_mpi MPI object for communication 
     * @param p_data datapoints
     * @param p_iterations iterations
     **/
    template<typename T> inline void neuralgas<T>::trainpatch( const mpi::communicator& p_mpi, const ublas::matrix<T>& p_data, const std::size_t& p_iterations )
    {
        trainpatch( p_mpi, p_data, p_iterations, m_prototypes.size1() * 0.5);
    }
    
    /** synchronize the weights of each prototype
     * @param p_mpi MPI object for communication
     * @param p_weight weight vector
    **/
    template<typename T> inline void neuralgas<T>::synchronizePrototypeWeights( const mpi::communicator& p_mpi, ublas::vector<T>& p_weight )
    {
        // create the weights for each process
        std::vector< ublas::vector<T> > l_weights;
        for(int i=0; i < p_mpi.size(); ++i) {
            ublas::vector_range< ublas::vector<T> > l_weightrange(p_weight,
                                                                ublas::range( m_processprototypinfo[i].first, m_processprototypinfo[i].first + m_processprototypinfo[i].second )
                                                                );
            l_weights.push_back(l_weightrange);
        }
        
        // create vector for receiving
        std::vector< ublas::vector<T> > l_collectweight;
        mpi::all_to_all( p_mpi, l_weights, l_collectweight );
        
        // std::vectors will be summerized
        m_prototypeWeights = std::accumulate( l_collectweight.begin(), l_collectweight.end(), ublas::vector<T>(m_prototypeWeights.size(), 0) );
    }
    
    /** train a patch (input data) with the data (include the weights)
     * @note each prototype is used n (=CPU count) times, because each CPU uses a own data block and the prototypes are added to them,
     * so the multipliers are also added n (=CPU count) time. To correct this, we divide the weights with the number of CPUs
     * @param p_mpi MPI object for communication 
     * @param p_data datapoints
     * @param p_iterations iterations
     * @param p_lambda max adapet size
     **/
    template<typename T> inline void neuralgas<T>::trainpatch( const mpi::communicator& p_mpi, const ublas::matrix<T>& p_data, const std::size_t& p_iterations, const T& p_lambda )
    {
        if (p_data.size1() < m_prototypes.size1())
            throw exception::runtime(_("number of datapoints are less than prototypes"), *this);
        if (p_iterations == 0)
            throw exception::runtime(_("iterations must be greater than zero"), *this);
        if (p_data.size2() != m_prototypes.size2())
            throw exception::runtime(_("data and prototype dimension are not equal"), *this);
        if (p_lambda <= 0)
            throw exception::runtime(_("lambda must be greater than zero"), *this);
        
        
        // we use the max. of all values of each process
        const std::size_t l_iterationsMPI = mpi::all_reduce(p_mpi, p_iterations, mpi::maximum<std::size_t>());
        const T l_lambdaMPI               = mpi::all_reduce(p_mpi, p_lambda, mpi::maximum<T>());
        m_logging                         = mpi::all_reduce(p_mpi, m_logging, std::multiplies<bool>());
        m_firstpatch                      = mpi::all_reduce(p_mpi, m_firstpatch, std::multiplies<bool>());
        setProcessPrototypeInfo(p_mpi);
        
        // creates logging
        if (m_logging) {
            m_logprototypes     = std::vector< ublas::matrix<T> >();
            m_quantizationerror = std::vector< T >();
            m_logprototypes.reserve(l_iterationsMPI);
            m_quantizationerror.reserve(l_iterationsMPI);
        }

        // if not the first patch add prototypes to data at the end and set the multiplier
        ublas::matrix<T> l_data(p_data);
        ublas::vector<T> l_multiplier(l_data.size1(), 1);
        
        ublas::vector<T> l_prototypeWeights = getPrototypeWeights( p_mpi );
        
        if (!m_firstpatch) {
            
            // resize data matrix with prototypes
            const ublas::matrix<T> l_prototypes       = gatherAllPrototypes( p_mpi );
            l_data.resize( l_data.size1()+l_prototypes.size1(), l_data.size2());
            ublas::matrix_range< ublas::matrix<T> > l_datarange(l_data, 
                                                                ublas::range( l_data.size1()-l_prototypes.size1(), l_data.size1() ), 
                                                                ublas::range( 0, l_data.size2() )
                                                                );
            l_datarange.assign(l_prototypes);
            
            
            
            // resize multiplier with prototype weights and divided them with the number of CPUs for correct scaling (see note)
            l_prototypeWeights /= p_mpi.size();
            
            l_multiplier.resize( l_multiplier.size()+l_prototypeWeights.size() );
            ublas::vector_range< ublas::vector<T> > l_multiplierrange( l_multiplier, ublas::range( l_multiplier.size()-l_prototypeWeights.size(), l_multiplier.size()) );
            
            // each prototype is used n (=CPU count) times, because each CPU uses a own data block and the prototypes are added to them,
            // so the multipliers are also added n (=CPU count) time. To correct this, we divide the weights with the number of CPUs
            l_multiplierrange.assign(l_prototypeWeights);
        }
        
        
        // run neural gas       
        const T l_multi = 0.01/l_lambdaMPI;
        ublas::vector<T> l_normvec( getNumberPrototypes(p_mpi), 0 );
        ublas::vector<T> l_lambda(l_normvec.size());
        ublas::matrix<T> l_adaptmatrix( l_normvec.size(), l_data.size1() );
        
        for(std::size_t i=0; (i < l_iterationsMPI); ++i) {
            
            // create adapt values
            const T l_lambdahelp = l_lambdaMPI * std::pow(l_multi, static_cast<T>(i)/static_cast<T>(l_iterationsMPI));
            
            #pragma omp parallel for shared(l_lambda)
            for(std::size_t n=0; n < l_lambda.size(); ++n)
                l_lambda(n) = std::exp( -static_cast<T>(n) / l_lambdahelp );

            
            // we get all prototypes of every process
            ublas::matrix<T> l_prototypes = gatherAllPrototypes( p_mpi );
            
            
            // determine quantization error for logging
            if (m_logging) {
                m_logprototypes.push_back( m_prototypes );
                m_quantizationerror.push_back( calculateQuantizationError(l_data, m_prototypes) );
            }
            
            
            // calculate for every prototype the distance
            #pragma omp parallel for shared(l_adaptmatrix)
            for(std::size_t n=0; n < l_prototypes.size1(); ++n)
                ublas::row(l_adaptmatrix, n)  = m_distance.getDistance( l_data, ublas::row(l_prototypes, n) ) ;
            
            
            // for every column ranks values and create adapts
            // we need rank and not randIndex, because we 
            // use the value of the ranking for getting the 
            // adapt value
            #pragma omp parallel for shared(l_adaptmatrix)
            for(std::size_t n=0; n < l_adaptmatrix.size2(); ++n) {
                ublas::vector<T> l_column                = ublas::column(l_adaptmatrix, n);
                const ublas::vector<std::size_t> l_rank  = tools::vector::rank(l_column);
                
                for(std::size_t j=0; j < l_rank.size(); ++j)
                    l_adaptmatrix(j,n) = l_lambda(l_rank(j));
            }
            
            
            // add multiplier
            #pragma omp parallel for shared(l_adaptmatrix)
            for(std::size_t n=0; n < l_adaptmatrix.size1(); ++n)
                ublas::row(l_adaptmatrix, n) = ublas::element_prod( ublas::row(l_adaptmatrix, n), l_multiplier );
            
            // create local prototypes
            l_prototypes = ublas::prod( l_adaptmatrix, l_data );
            
            
            // normalize prototypes and sync them on each process
            #pragma omp parallel for shared(l_normvec)
            for(std::size_t n=0; n < l_prototypes.size1(); ++n)
                l_normvec(n) = ublas::sum( ublas::row(l_adaptmatrix, n) );
            synchronizePrototypes(p_mpi, l_prototypes, l_normvec);
        }
        
        // determine size of receptive fields, but we use only the data points
        const ublas::indirect_array<> l_winner = use(p_mpi, p_data);
		for(std::size_t i=0; i < l_winner.size(); ++i)
             l_prototypeWeights( l_winner(i) )++;

        // synchronize the weights for each process to get weights for the whole data space
        synchronizePrototypeWeights(p_mpi, l_prototypeWeights);

        // do logging
        if (m_logging)
            m_logprototypeWeights.push_back(m_prototypeWeights);
        
        // set bool for the first patch
        m_firstpatch = false;
        
    }
    
    #endif
    
}}}
#endif
