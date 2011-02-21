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
 # along with this program. If not, see <http://www.gnu.org/licenses/>.     #
 ############################################################################
 @endcond
 **/


#ifndef MACHINELEARNING_CLUSTERING_NONSUPERVISED_NG_HPP
#define MACHINELEARNING_CLUSTERING_NONSUPERVISED_NG_HPP

#include <numeric>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#ifdef MACHINELEARNING_MPI
#include <boost/mpi.hpp>
#endif

#include "../clustering.hpp"
#include "../../exception/exception.h"
#include "../../tools/tools.h"
#include "../../distances/distances.h"



namespace machinelearning { namespace clustering { namespace nonsupervised {
    
    namespace ublas = boost::numeric::ublas;
    #ifdef MACHINELEARNING_MPI
    namespace mpi   = boost::mpi;
    #endif
    
    /** class for calculate (batch) neural gas
     * $LastChangedDate$
     * @note The MPI methods do not check the correct ranges / dimension of the prototype
     * data, so it is the task of the developer to use the correct ranges. Also the MPI
     * methods must be called in the correct order, so the MPI calls must be run
     * on each process.
     **/
    template<typename T> class neuralgas : public clustering<T>, public patch<T> {
        
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
            ublas::indirect_array< std::vector<std::size_t> > use( const mpi::communicator&, const ublas::matrix<T>& ) const;
            void use( const mpi::communicator& ) const;
        
            // derived from patch clustering
            ublas::vector<T> getPrototypeWeights( const mpi::communicator& ) const;
            void trainpatch( const mpi::communicator&, const ublas::matrix<T>&, const std::size_t& );
            void trainpatch( const mpi::communicator&, const ublas::matrix<T>&, const std::size_t&, const T& );
            std::vector< ublas::vector<T> > getLoggedPrototypeWeights( const mpi::communicator& ) const;
            #endif
        
        
        
        private :
        
            /** distance object
             * @todo switch pointer eg to auto_ptr or smartpointer
             **/
            const distances::distance<T>* m_distance;        
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
            std::map<int, std::pair<std::size_t,std::size_t> > m_processprototypinfo;
            
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
        m_distance( &p_distance ),
        m_prototypes( tools::matrix::random<T>(p_prototypes, p_prototypesize) ),
        m_logging( false ),
        m_logprototypes( std::vector< ublas::matrix<T> >() ),
        m_quantizationerror( std::vector<T>() ),
        m_prototypeWeights( p_prototypes, 1 ),
        m_logprototypeWeights(),
        m_firstpatch(true)
        #ifdef MACHINELEARNING_MPI
        , m_processprototypinfo()
        #endif
    {
        if (p_prototypesize == 0)
            throw exception::runtime(_("prototype size must be greater than zero"));
            
            // normalize the prototypes
            m_distance->normalize( m_prototypes );
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
        
        //remove temporary datastructures
        if (m_logging)
            m_logprototypeWeights.clear();
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
    
    
    /** 
     * train the prototypes
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
            throw exception::runtime(_("number of prototypes must be greater than zero"));
        if (p_data.size1() < m_prototypes.size1())
            throw exception::runtime(_("number of datapoints are less than prototypes"));
        if (p_iterations == 0)
            throw exception::runtime(_("iterations must be greater than zero"));
        if (p_data.size2() != m_prototypes.size2())
            throw exception::runtime(_("data and prototype dimension are not equal"));
        if (p_lambda <= 0)
            throw exception::runtime(_("lambda must be greater than zero"));
        
        // creates logging
        if (m_logging) {
            m_logprototypes.clear();
            m_quantizationerror.clear();
            m_logprototypes.reserve(p_iterations);
            m_quantizationerror.reserve(p_iterations);
        }
        
        
        // run neural gas       
        const T l_multi = 0.01/p_lambda;
        T l_lambda      = 0;
        T l_norm        = 0;
        ublas::matrix<T> l_adaptmatrix( m_prototypes.size1(), p_data.size1() );
        ublas::vector<T> l_rank;
        
        for(std::size_t i=0; (i < p_iterations); ++i) {
            
            // create adapt values
            l_lambda = p_lambda * std::pow(l_multi, static_cast<T>(i)/static_cast<T>(p_iterations));
            
            
            // calculate for every prototype the distance
            for(std::size_t n=0; n < m_prototypes.size1(); ++n)
                ublas::row(l_adaptmatrix, n)  = m_distance->getDistance( p_data, ublas::row(m_prototypes, n) ) ;
            
            // for every column ranks values and create adapts
            // we need rank and not randIndex, because we 
            // use the value of the ranking for calculate the 
            // adapt value
            for(std::size_t n=0; n < l_adaptmatrix.size2(); ++n) {
                l_rank = ublas::column(l_adaptmatrix, n);
                l_rank = tools::vector::rank( l_rank );
                
                // calculate adapt value
                BOOST_FOREACH( T& p, l_rank)
                    p = std::exp( -p / l_lambda );
                
                // return value to matrix
                ublas::column(l_adaptmatrix, n) = l_rank;
            }
            
            
            // create prototypes
            m_prototypes = ublas::prod( l_adaptmatrix, p_data );
            
            // normalize prototypes
            for(std::size_t n=0; n < m_prototypes.size1(); ++n) {
                l_norm = ublas::sum( ublas::row(l_adaptmatrix, n) );
                
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
        ublas::matrix<T> l_distances( p_prototypes.size1(), p_data.size1() );
        
        for(std::size_t i=0; i < p_prototypes.size1(); ++i)
            ublas::row(l_distances, i) = m_distance->getDistance( p_data, ublas::row(p_prototypes, i) );
        
        return 0.5 * ublas::sum(  m_distance->getAbs(tools::matrix::min(l_distances, tools::matrix::column))  );  
    }
    
    
    /** calulates distance between datapoints and prototypes and returns a std::vector
     * with index of the nearest prototype
     * @param p_data matrix
     * @return index array of prototype indices
     **/
    template<typename T> inline ublas::indirect_array< std::vector<std::size_t> > neuralgas<T>::use( const ublas::matrix<T>& p_data ) const
    {
        if (m_prototypes.size1() == 0)
            throw exception::runtime(_("number of prototypes must be greater than zero"));
        if (p_data.size2() != m_prototypes.size2())
            throw exception::runtime(_("data and prototype dimension are not equal"));
        
        std::vector<std::size_t> l_vec(p_data.size1());
        ublas::matrix<T> l_distance(m_prototypes.size1(), p_data.size1());
        
        // calculate distance for every prototype
        for(std::size_t i=0; i < m_prototypes.size1(); ++i)
            ublas::row(l_distance, i)  = m_distance->getDistance( p_data,  ublas::row(m_prototypes, i) );
        
        // determine nearest prototype
        for(std::size_t i=0; i < l_distance.size2(); ++i) {
            ublas::vector<T> l_col                                          = ublas::column(l_distance, i);
            const ublas::indirect_array< std::vector<std::size_t> > l_rank  = tools::vector::rankIndex( l_col );
            l_vec[i] = l_rank(0);
         }
        
        return ublas::indirect_array< std::vector<std::size_t> >(l_vec.size(), l_vec);
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
        //xxx
        if (m_prototypes.size1() == 0)
            throw exception::runtime(_("number of prototypes must be greater than zero"));
        if (p_data.size1() < m_prototypes.size1())
            throw exception::runtime(_("number of datapoints are less than prototypes"));
        if (p_iterations == 0)
            throw exception::runtime(_("iterations must be greater than zero"));
        if (p_data.size2() != m_prototypes.size2())
            throw exception::runtime(_("data and prototype dimension are not equal"));
        if (p_lambda <= 0)
            throw exception::runtime(_("lambda must be greater than zero"));
        
        // creates logging
        if (m_logging) {
            m_logprototypes.clear();
            m_quantizationerror.clear();
            m_logprototypes.reserve(p_iterations);
            m_quantizationerror.reserve(p_iterations);
        }
        
        
        // if not the first patch add prototypes to data at the end and set the multiplier
        ublas::matrix<T> l_data(p_data);
        ublas::vector<std::size_t> l_multiplier(p_data.size1(), 1);
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
        m_firstpatch = false;
     

        // run neural gas       
        const T l_multi = 0.01/p_lambda;
        ublas::matrix<T> l_adaptmatrix( m_prototypes.size1(), l_data.size1() );
        ublas::vector<T> l_rank;
                
        for(std::size_t i=0; (i < p_iterations); ++i) {
            
            // create adapt values
            const T l_lambda = p_lambda * std::pow(l_multi, static_cast<T>(i)/static_cast<T>(p_iterations));
            
            
            // calculate for every prototype the distance
            for(std::size_t n=0; n < m_prototypes.size1(); ++n)
                ublas::row(l_adaptmatrix, n)  = m_distance->getDistance( l_data, ublas::row(m_prototypes, n) ) ;
            
            // for every column ranks values and create adapts
            // we need rank and not randIndex, because we 
            // use the value of the ranking for calculate the 
            // adapt value
            for(std::size_t n=0; n < l_adaptmatrix.size2(); ++n) {
                l_rank = ublas::column(l_adaptmatrix, n);
                l_rank = tools::vector::rank( l_rank );
                
                // calculate adapt value
                BOOST_FOREACH( T& p, l_rank)
                    p = std::exp( -p / l_lambda );
                
                // return value to matrix
                ublas::column(l_adaptmatrix, n) = l_rank;
            }
            
            
            // add multiplier
            for(std::size_t n=0; n < l_adaptmatrix.size1(); ++n)
                ublas::row(l_adaptmatrix, n) = ublas::element_prod( ublas::row(l_adaptmatrix, n), l_multiplier );
            
            // create prototypes
            m_prototypes = ublas::prod( l_adaptmatrix, l_data );
            
            // normalize prototypes
            for(std::size_t n=0; n < m_prototypes.size1(); ++n) {
                const T l_norm = ublas::sum( ublas::row(l_adaptmatrix, n) );
                
                if (!tools::function::isNumericalZero(l_norm))
                    ublas::row(m_prototypes, n) /= l_norm;
            }
            
            // determine quantization error for logging
            if (m_logging) {
                m_logprototypes.push_back( m_prototypes );
                m_quantizationerror.push_back( calculateQuantizationError(l_data, m_prototypes) );
            }
        }
        
        // determine size of receptive fields
        const ublas::indirect_array< std::vector<std::size_t> > l_winner = use(l_data);
        for(std::size_t i=0; i < l_winner.size(); ++i)
            m_prototypeWeights( l_winner(i) )++;
        
        if (m_logging)
            m_logprototypeWeights.push_back(m_prototypeWeights);
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
    
    
    
    /** set the std::map with the begin position and size of the prototypes matrix. Is needed for extracting the prototypes
     * of the full matrix for every process
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
            m_processprototypinfo[static_cast<int>(i)]  = std::pair<std::size_t,std::size_t>(l_sum, l_processdata[i]);
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
        //ddd
        if (p_data.size1() < m_prototypes.size1())
            throw exception::runtime(_("number of datapoints are less than prototypes"));
        if (p_iterations == 0)
            throw exception::runtime(_("iterations must be greater than zero"));
        if (p_data.size2() != m_prototypes.size2())
            throw exception::runtime(_("data and prototype dimension are not equal"));
        if (p_lambda <= 0)
            throw exception::runtime(_("lambda must be greater than zero"));
        
        
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
        ublas::matrix<T> l_adaptmatrix( l_normvec.size(), p_data.size1() );
        
        for(std::size_t i=0; (i < l_iterationsMPI); ++i) {
            
            // create adapt values
            const T l_lambda = l_lambdaMPI * std::pow(l_multi, static_cast<T>(i)/static_cast<T>(l_iterationsMPI));
            
            // we get all prototypes of every process
            ublas::matrix<T> l_prototypes = gatherAllPrototypes( p_mpi );
            
            
            // calculate for every prototype the distance (of the actually prototypes).
            // within the adapt matrix, we must specify the position of the prototypes 
            for(std::size_t n=0; n < l_prototypes.size1(); ++n)
                ublas::row(l_adaptmatrix, n)  = m_distance->getDistance( p_data, ublas::row(l_prototypes, n) ) ;
            
            // for every column ranks values and create adapts
            // we need rank and not randIndex, because we 
            // use the value of the ranking for calculate the 
            // adapt value
            for(std::size_t n=0; n < l_adaptmatrix.size2(); ++n) {
                ublas::vector<T> l_rank = ublas::column(l_adaptmatrix, n);
                l_rank = tools::vector::rank( l_rank );
                
                // calculate adapt value
                BOOST_FOREACH( T& p, l_rank)
                    p = std::exp( -p / l_lambda );
                
                // return value to matrix
                ublas::column(l_adaptmatrix, n) = l_rank;
            }
            
            
            // create local prototypes
            l_prototypes = ublas::prod( l_adaptmatrix, p_data );
            
            
            // normalize prototypes
            for(std::size_t n=0; n < l_prototypes.size1(); ++n)
                l_normvec(n) = ublas::sum( ublas::row(l_adaptmatrix, n) );
            
            synchronizePrototypes(p_mpi, l_prototypes, l_normvec);
            
            
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
    
    
    /** calulates distance between datapoints and prototypes and returns a std::vector
     * with index of the nearest prototype
     * @param p_mpi MPI object for communication     
     * @param p_data matrix
     * @return index array of prototype indices
     **/
    template<typename T> inline ublas::indirect_array< std::vector<std::size_t> > neuralgas<T>::use( const mpi::communicator& p_mpi, const ublas::matrix<T>& p_data ) const
    {
        if (p_data.size2() != m_prototypes.size2())
            throw exception::runtime(_("data and prototype dimension are not equal"));
        
        //first we gathering all other prototypes
        const ublas::matrix<T> l_prototypes = gatherAllPrototypes( p_mpi );
        
        std::vector<std::size_t> l_vec(p_data.size1());
        ublas::scalar_vector<T> l_ones(p_data.size1(), 1);
        ublas::matrix<T> l_distance(l_prototypes.size1(), p_data.size1());
        
        // calculate distance for every prototype
        for(std::size_t i=0; i < l_prototypes.size1(); ++i)
            ublas::row(l_distance, i)  = m_distance->getDistance( p_data, ublas::outer_prod(l_ones, ublas::row(l_prototypes, i)) ) ;
        
        // determine nearest prototype
        for(std::size_t i=0; i < l_prototypes.size2(); ++i) {
            ublas::vector<T> l_col                                          = ublas::column(l_distance, i);
            const ublas::indirect_array< std::vector<std::size_t> > l_rank  = tools::vector::rankIndex( l_col );
            l_vec[i] = l_rank(0);
        }
        
        return ublas::indirect_array< std::vector<std::size_t> >(l_vec.size(), l_vec);
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
            throw exception::runtime(_("number of datapoints are less than prototypes"));
        if (p_iterations == 0)
            throw exception::runtime(_("iterations must be greater than zero"));
        if (p_data.size2() != m_prototypes.size2())
            throw exception::runtime(_("data and prototype dimension are not equal"));
        if (p_lambda <= 0)
            throw exception::runtime(_("lambda must be greater than zero"));
        
        
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
        
        // we need first all prototypes and their weights of each process
        ublas::matrix<T> l_prototypes       = gatherAllPrototypes( p_mpi );
        ublas::vector<T> l_prototypeWeights(l_prototypes.size1(), 0);
        
        // if not the first patch add prototypes to data at the end and set the multiplier
        ublas::matrix<T> l_data(p_data);
        ublas::vector<std::size_t> l_multiplier(p_data.size1(), 1);
        if (!m_firstpatch) {
            
            // read weights of each process (each prototype) and 
            l_prototypeWeights = getPrototypeWeights( p_mpi );
            
            // the weight vector must be divided with the number of CPUs for correct scaling (see note)
            l_prototypeWeights /= p_mpi.size();
            
            // resize data matrix
            l_data.resize( l_data.size1()+l_prototypes.size1(), l_data.size2());
            ublas::matrix_range< ublas::matrix<T> > l_datarange(l_data, 
                                                                ublas::range( l_data.size1()-l_prototypes.size1(), l_data.size1() ), 
                                                                ublas::range( 0, l_data.size2() )
                                                                );
            l_datarange.assign(l_prototypes);
            
            // resize multiplier
            l_multiplier.resize( l_multiplier.size()+l_prototypeWeights.size() );
            ublas::vector_range< ublas::vector<std::size_t> > l_multiplierrange( l_multiplier, ublas::range( l_multiplier.size()-l_prototypeWeights.size(), l_multiplier.size()) );
            
            // each prototype is used n (=CPU count) times, because each CPU uses a own data block and the prototypes are added to them,
            // so the multipliers are also added n (=CPU count) time. To correct this, we divide the weights with the number of CPUs
            l_multiplierrange.assign(l_prototypeWeights);
        }
        m_firstpatch = false;
        
        
        // run neural gas       
        const T l_multi = 0.01/l_lambdaMPI;
        ublas::vector<T> l_normvec( getNumberPrototypes(p_mpi), 0 );
        ublas::matrix<T> l_adaptmatrix( l_prototypes.size1(), l_data.size1() );
        ublas::vector<T> l_rank;
        
        
        for(std::size_t i=0; (i < l_iterationsMPI); ++i) {
            
            // create adapt values
            const T l_lambda = l_lambdaMPI * std::pow(l_multi, static_cast<T>(i)/static_cast<T>(l_iterationsMPI));
            
            // we get all prototypes of every process
            l_prototypes = gatherAllPrototypes( p_mpi );
            
            // calculate for every prototype the distance
            for(std::size_t n=0; n < l_prototypes.size1(); ++n)
                ublas::row(l_adaptmatrix, n)  = m_distance->getDistance( l_data, ublas::row(l_prototypes, n) ) ;
            
            // for every column ranks values and create adapts
            // we need rank and not randIndex, because we 
            // use the value of the ranking for calculate the 
            // adapt value
            for(std::size_t n=0; n < l_adaptmatrix.size2(); ++n) {
                l_rank = ublas::column(l_adaptmatrix, n);
                l_rank = tools::vector::rank( l_rank );
                
                // calculate adapt value
                BOOST_FOREACH( T& p, l_rank)
                    p = std::exp( -p / l_lambda );
                
                // return value to matrix
                ublas::column(l_adaptmatrix, n) = l_rank;
            }
            
            
            // add multiplier
            for(std::size_t n=0; n < l_adaptmatrix.size1(); ++n)
                ublas::row(l_adaptmatrix, n) = ublas::element_prod( ublas::row(l_adaptmatrix, n), l_multiplier );
            
            // create local prototypes
            l_prototypes = ublas::prod( l_adaptmatrix, l_data );
            
            // normalize prototypes and synch them on each process
            for(std::size_t n=0; n < l_prototypes.size1(); ++n)
                l_normvec(n) = ublas::sum( ublas::row(l_adaptmatrix, n) );
            synchronizePrototypes(p_mpi, l_prototypes, l_normvec);
            
            // determine quantization error for logging
            if (m_logging) {
                m_logprototypes.push_back( m_prototypes );
                m_quantizationerror.push_back( calculateQuantizationError(l_data, m_prototypes) );
            }
        }
        
        // determine size of receptive fields for the local data space
        const ublas::indirect_array< std::vector<std::size_t> > l_winner = use(p_mpi, l_data);
        for(std::size_t i=0; i < l_winner.size(); ++i)
            l_prototypeWeights( l_winner(i) )++;
        
        // synchronize the weights for each process to get weights for the whole data space
        synchronizePrototypeWeights(p_mpi, l_prototypeWeights);
        
        // do logging
        if (m_logging)
            m_logprototypeWeights.push_back(m_prototypeWeights);
        
    }
    
    #endif
    
};};};


#endif
