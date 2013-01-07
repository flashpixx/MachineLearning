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


#ifndef __MACHINELEARNING_CLUSTERING_NONSUPERVISED_RELATIONAL_NEURALGAS_HPP
#define __MACHINELEARNING_CLUSTERING_NONSUPERVISED_RELATIONAL_NEURALGAS_HPP

#include <omp.h>

#include <numeric>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#ifdef MACHINELEARNING_MPI
#include <boost/mpi.hpp>
#endif

#include "clustering.hpp"
#include "../../errorhandling/exception.hpp"
#include "../../tools/tools.h"
#include "../../neighborhood/neighborhood.h"



namespace machinelearning { namespace clustering { namespace nonsupervised {
    
    #ifndef SWIG
    namespace ublas = boost::numeric::ublas;
    #ifdef MACHINELEARNING_MPI
    namespace mpi   = boost::mpi;
    #endif
    #endif
    
    
    /** class for calculate (batch) relational neural gas with pseudo-euclidian data space
     * @note The MPI methods do not check the correct ranges / dimension of the prototype
     * data, so it is the task of the developer to use the correct ranges. Also the MPI
     * methods must be called in the correct order, so the MPI calls must be run
     * on each process.
     * @todo thinking about relation calculating transform to a own distance class
     * @todo add patch-clustering:
     * @code
         do k-approximation
         k-nearest:
         1. determine relational spanning set
         1a. k = 1 / neurons.size2()
         1b. std::vector ret
         1b. for(i in neurons.size1()) {
                std::vector pos
                for(j in neurons.size2())
                    if neurons(i,j) >= k)
                        ret.push_back(j)
                ret.push_back(pos);
            }
         2. sorting spanning set
         2a. calculate distance data between data and neurons (matrix with neurons x data)
         2b. std::vector newret;
         2c. for(i in ret.size()) {
                ublas::vector dist( ret(i).size() )
                for(j in 0..dist.size()-1)
                    dist(j) = distance(i, j)
                std::vector idx = rankIndex(dist);
         
                std::vector pos;
                std::vector line = ret(i);
                for(j in 0..ret(i).size()-1)
                    pos.push_back( line(idx(j)) );
                newret.push_back(pos);
            }
     * @endcode
     **/
    template<typename T> class relational_neuralgas : public clustering<T> 
        #ifdef MACHINELEARNING_MPI 
        , public mpiclustering<T>
        #endif
    {
        
        public:
        
            relational_neuralgas( const std::size_t&, const std::size_t& );
            //relational_neuralgas( const std::size_t&, const std::size_t&, const neighborhood::kapproximation<T>& );
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
        
        
            #ifdef MACHINELEARNING_MPI
            void train( const mpi::communicator&, const ublas::matrix<T>&, const std::size_t& );
            void train( const mpi::communicator&, const ublas::matrix<T>&, const std::size_t&, const T& );
            ublas::matrix<T> getPrototypes( const mpi::communicator& ) const;
            std::vector< ublas::matrix<T> > getLoggedPrototypes( const mpi::communicator& ) const;
            std::vector<T> getLoggedQuantizationError( const mpi::communicator& ) const;
            ublas::indirect_array<> use( const mpi::communicator&, const ublas::matrix<T>& ) const;
            void use( const mpi::communicator& ) const;
            #endif
        
        
        private:
        
            /** prototypes **/
            ublas::matrix<T> m_prototypes;
            /** bool for logging prototypes **/
            bool m_logging;
            /** std::vector for prototypes for each iteration **/
            std::vector< ublas::matrix<T> > m_logprototypes;
            /** std::vector for quantisation error in each iteration **/
            std::vector<T> m_quantizationerror;
        
            T calculateQuantizationError( const ublas::matrix<T>& ) const;
            ublas::matrix<T> calcDistance( const ublas::matrix<T>&, const ublas::matrix<T>& ) const;
        
            #ifdef MACHINELEARNING_MPI
            /** vector with information to every process and width of the prototype / data matrix **/
            std::vector< std::pair<std::size_t,std::size_t> > m_processdatainfo;        
            /** vector with information to every process and prototype**/
            std::vector< std::pair<std::size_t,std::size_t> > m_processprototypinfo;
        
            ublas::matrix<T> gatherAllPrototypes( const mpi::communicator& ) const;
            std::size_t getNumberPrototypes( const mpi::communicator& ) const;
            void setProcessDataPrototypInfo( const mpi::communicator&, const std::size_t& );
            ublas::matrix<T> synchronizePrototypes( const mpi::communicator&, const ublas::matrix<T>& ) const;
            ublas::matrix<T> extractLocalPrototypes( const mpi::communicator&, ublas::matrix<T>& );
            #endif
        
    };

    
    /** contructor for initialization the neural gas
     * @param p_prototypes number of prototypes
     * @param p_prototypesize size of each prototype (data dimension)
     **/
    template<typename T> inline relational_neuralgas<T>::relational_neuralgas( const std::size_t& p_prototypes, const std::size_t& p_prototypesize ) :
        m_prototypes( tools::matrix::random<T>(p_prototypes, p_prototypesize) ),
        m_logging( false ),
        m_logprototypes( std::vector< ublas::matrix<T> >() ),
        m_quantizationerror( std::vector<T>() )
        #ifdef MACHINELEARNING_MPI
        , m_processdatainfo(),
        m_processprototypinfo()
        #endif
    {
        if (p_prototypesize == 0)
            throw exception::runtime(_("prototype size must be greater than zero"), *this);
        
        // normalize the prototypes
        #pragma omp parallel for
        for(std::size_t i=0; i <  m_prototypes.size1(); ++i) {
            const T l_sum = ublas::sum( ublas::row( m_prototypes, i) );
            
            if (!tools::function::isNumericalZero(l_sum))
                ublas::row( m_prototypes, i) /= l_sum;
        }
    }   
    
    
    /** returns the prototype matrix
     * @return matrix (rows = number of prototypes)
     **/
    template<typename T> inline ublas::matrix<T> relational_neuralgas<T>::getPrototypes( void ) const
    {
        return m_prototypes;
    }
    
    
    /** enabled logging for training
     * @param p_log bool
     **/
    template<typename T> inline void relational_neuralgas<T>::setLogging( const bool& p_log )
    {
        m_logging = p_log;
        m_logprototypes.clear();
        m_quantizationerror.clear();
    }
    
    
    /** shows the logging status
     * @return bool
     **/
    template<typename T> inline bool relational_neuralgas<T>::getLogging( void ) const
    {
        return m_logging && (m_logprototypes.size() > 0);
    }
    
    
    /** returns every prototype step during training
     * @return std::vector with prototype matrix
     **/
    template<typename T> inline std::vector< ublas::matrix<T> > relational_neuralgas<T>::getLoggedPrototypes( void ) const
    {
        return m_logprototypes;
    }
    
    
    /** returns the dimension of prototypes
     * @return dimension of the prototypes
     **/
    template<typename T> inline std::size_t relational_neuralgas<T>::getPrototypeSize( void ) const 
    {
        return m_prototypes.size2();
    }
    
    
    /** returns the number of prototypes
     * @return number of the prototypes / classes
     **/
    template<typename T> inline std::size_t relational_neuralgas<T>::getPrototypeCount( void ) const 
    {
        return m_prototypes.size1();
    }
    
    
    /** returns the quantisation error 
     * @return error for each iteration
     **/
    template<typename T> inline std::vector<T> relational_neuralgas<T>::getLoggedQuantizationError( void ) const
    {
        return m_quantizationerror;
    }    
   
    
    /** 
     * train the prototypes
     * @param p_data data matrix
     * @param p_iterations number of iterations
     **/
    template<typename T> inline void relational_neuralgas<T>::train( const ublas::matrix<T>& p_data, const std::size_t& p_iterations )
    {
        train(p_data, p_iterations, m_prototypes.size1() * 0.5);
    }
    
    
    /** training the prototypes
     * @param p_data datapoints
     * @param p_iterations iterations
     * @param p_lambda max adapet size
     **/
    template<typename T> inline void relational_neuralgas<T>::train( const ublas::matrix<T>& p_data, const std::size_t& p_iterations, const T& p_lambda )
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
        if (p_data.size1() != p_data.size2())
            throw exception::runtime(_("matrix must be square"), *this);
        
        
        // creates logging
        if (m_logging) {
            m_logprototypes.clear();
            m_quantizationerror.clear();
            m_logprototypes.reserve(p_iterations);
            m_quantizationerror.reserve(p_iterations);
        }
        
        
        
        // run neural gas       
        const T l_multi = 0.01/p_lambda;
        ublas::vector<T> l_lambda(m_prototypes.size1());
        
        for(std::size_t i=0; i < p_iterations; ++i) {
            
            // create adapt values
            const T l_lambdahelp = p_lambda * std::pow(l_multi, static_cast<T>(i)/static_cast<T>(p_iterations));
            
            #pragma omp parallel for shared(l_lambda)
            for(std::size_t n=0; n < l_lambda.size(); ++n)
                l_lambda(n) = std::exp( -static_cast<T>(n) / l_lambdahelp );
            
            // create adapt values
            ublas::matrix<T> l_adaptmatrix  = calcDistance( m_prototypes, p_data );

            
            // determine quantization error for logging (adaption matrix)
            if (m_logging) {
                m_quantizationerror.push_back( calculateQuantizationError(l_adaptmatrix) );
                m_logprototypes.push_back( m_prototypes );
            }
            
            
            // for every column ranks values and create adapts
            // we need rank and not randIndex, because we 
            // use the value of the ranking for calculate the 
            // adapt value
            #pragma omp parallel for shared(l_adaptmatrix)
            for(std::size_t n=0; n < l_adaptmatrix.size2(); ++n) {
                ublas::vector<T> l_column                = ublas::column(l_adaptmatrix, n);
                const ublas::vector<std::size_t> l_rank  = tools::vector::rank(l_column);
                
                for(std::size_t j=0; j < l_rank.size(); ++j)
                    l_adaptmatrix(j,n) = l_lambda(l_rank(j));
            }
            
 
            // adapt values are the new prototypes (and run normalization)
            #pragma omp parallel for
            for(std::size_t n=0; n < l_adaptmatrix.size1(); ++n) {
                const T l_sum                = ublas::sum( ublas::row( l_adaptmatrix, n) );
                ublas::row(m_prototypes, n ) = ublas::row( l_adaptmatrix, n );
                
                if (!tools::function::isNumericalZero(l_sum))
                    ublas::row( m_prototypes, n ) /= l_sum;
            }
        }
    }
    
    
    /** calculate the quantization error
     * @param p_distance distance matrix (adaption matrix)
     * @return quantization error
     **/    
    template<typename T> inline T relational_neuralgas<T>::calculateQuantizationError( const ublas::matrix<T>& p_distance ) const
    {
        return 0.5 * ublas::sum( tools::matrix::min( p_distance, tools::matrix::column ) );
    }

    
    /** calulates distance between datapoints and prototypes and returns a indirect array
     * with index of the nearest prototype
     * @param p_data matrix
     * @return index array of prototype indices
     **/
    template<typename T> inline ublas::indirect_array<> relational_neuralgas<T>::use( const ublas::matrix<T>& p_data ) const
    {
        if (m_prototypes.size1() == 0)
            throw exception::runtime(_("number of prototypes must be greater than zero"), *this);
        if (p_data.size2() != m_prototypes.size2())
            throw exception::runtime(_("data and prototype dimension are not equal"), *this);
        
        ublas::indirect_array<> l_idx(p_data.size1());
        const ublas::matrix<T> l_distance = ublas::prod( p_data, ublas::trans(m_prototypes) );
        
        #pragma omp parallel for shared(l_idx)
        for(std::size_t i=0; i < l_distance.size1(); ++i) {
            ublas::vector<T> l_col                = ublas::row(l_distance, i);
            const ublas::indirect_array<> l_rank  = tools::vector::rankIndex( l_col );
            l_idx[i] = l_rank(0);
        }
        
        return l_idx;
    }
    
    
    /** calculates the distance values between neurons and data.
     * @todo thinking for own distance class
     * @param p_prototypes prototype matrix
     * @param p_data data matrix
     * @return matrix with distance values (number of prototypes X data dimension)
     **/
    template<typename T> inline ublas::matrix<T> relational_neuralgas<T>::calcDistance( const ublas::matrix<T>& p_prototypes, const ublas::matrix<T>& p_data ) const
    {
        // calculate for every prototype the distance
        // relational: (D * alpha_i)_j - 0.5 * alpha_i^t * D * alpha_i = || x^j - w^i || 
        // D = distance, alpha = weight of the prototype for the convex combination
        ublas::matrix<T> l_adaptmatrix = ublas::prod(p_prototypes, p_data);
       
        #pragma omp parallel for shared(l_adaptmatrix)
        for(std::size_t n=0; n < l_adaptmatrix.size1(); ++n) {
            const T l_val = 0.5 * ublas::inner_prod( ublas::row(p_prototypes, n), ublas::row(l_adaptmatrix, n) );
            
            for(std::size_t j=0; j < l_adaptmatrix.size2(); ++j)
                l_adaptmatrix(n, j) -= l_val;
        }        
    
        return l_adaptmatrix;
    }
    
    //======= MPI ==================================================================================================================================
    #ifdef MACHINELEARNING_MPI
    
    /** gathering prototypes of every process and return the full prototypes matrix (row oriantated)
     * @param p_mpi MPI object for communication
     * @return full prototypes matrix
     **/
    template<typename T> inline ublas::matrix<T> relational_neuralgas<T>::gatherAllPrototypes( const mpi::communicator& p_mpi ) const
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
    
    /** returns the number of all prototypes
     * @param p_mpi MPI object for communication
     * @return number of prototypes
     **/
    template<typename T> inline std::size_t relational_neuralgas<T>::getNumberPrototypes( const mpi::communicator& p_mpi ) const
    {
        std::size_t l_count = 0;
        mpi::all_reduce(p_mpi, m_prototypes.size1(), l_count, std::plus<std::size_t>());
        return l_count;
    }
    
    
    /** create a map with information which prototype are on which CPU and which parts of the data matrix
     * is saved on the CPUs
     * @param p_mpi MPI object for communication
     * @param p_datacol number of columns of the data matrix
    **/
    template<typename T> inline void relational_neuralgas<T>::setProcessDataPrototypInfo( const mpi::communicator& p_mpi, const std::size_t& p_datacol )
    {
        // create process data info
        m_processdatainfo.clear();
        // gathering the column number of the data matrix
        std::vector< std::size_t > l_processdata;
        mpi::all_gather(p_mpi, p_datacol, l_processdata);
        
        // create map
        std::size_t l_sum = 0;
        for(std::size_t i=0; i < l_processdata.size(); ++i) {
            m_processdatainfo.push_back( std::pair<std::size_t,std::size_t>(l_sum, l_processdata[i]) );
            l_sum += l_processdata[i];
        }
        
        
        // we do the same for the prototypes (which prototyp is primary on which CPU)
        m_processprototypinfo.clear();
        // gathering the number of prototypes
        l_processdata.clear();
        mpi::all_gather(p_mpi, m_prototypes.size1(), l_processdata);
        
        // create map
        l_sum = 0;
        for(std::size_t i=0; i < l_processdata.size(); ++i) {
            m_processprototypinfo.push_back( std::pair<std::size_t,std::size_t>(l_sum, l_processdata[i]) );
            l_sum += l_processdata[i];
        }
    }
    
    
    /** synchronize the prototypes. The adapt matrix has the values for the dimension of the prototypes, which are synchronize over
     * all CPUs, because a CPU contains only some parts of the prototype
     * @param p_mpi MPI object for communication
     * @param p_adapetmatrix adapt matrix with rankings
     * @return new full prototype matrix
     **/
    template<typename T> inline ublas::matrix<T> relational_neuralgas<T>::synchronizePrototypes( const mpi::communicator& p_mpi, const ublas::matrix<T>& p_adapetmatrix ) const
    {
        // gathering all adapt values
        std::vector< ublas::matrix<T> > l_adaptmatrix;
        mpi::all_gather(p_mpi, p_adapetmatrix, l_adaptmatrix);
        
        // all elements of the l_adaptmatrix must be connected in columns to create the new prototype matrix
        ublas::matrix<T> l_prototypes = l_adaptmatrix[0];
        for (std::size_t i=1; i < l_adaptmatrix.size(); ++i) {
            l_prototypes.resize( l_prototypes.size1(), l_prototypes.size2()+l_adaptmatrix[i].size2() );
            
            ublas::matrix_range< ublas::matrix<T> > l_range(l_prototypes, 
                                                            ublas::range( 0, l_prototypes.size1() ), 
                                                            ublas::range( l_prototypes.size2()-l_adaptmatrix[i].size2(), l_prototypes.size2() )
                                                            );
            
            l_range.assign(l_adaptmatrix[i]);
        }
        
        return l_prototypes;
    }
    
    
    /** returns the prototype matrix only of this node
     * @param p_mpi MPI object for communication
     * @param p_prototypes prototype matrix of all nodes
     * @returns prototype matrix of the node
     **/
    template<typename T> inline ublas::matrix<T> relational_neuralgas<T>::extractLocalPrototypes( const mpi::communicator& p_mpi, ublas::matrix<T>& p_prototypes )
    {
        ublas::matrix_range< ublas::matrix<T> > l_range( p_prototypes, 
                                                         ublas::range( m_processprototypinfo[p_mpi.rank()].first, m_processprototypinfo[p_mpi.rank()].first+m_processprototypinfo[p_mpi.rank()].second ), 
                                                         ublas::range( 0, p_prototypes.size2() )
                                                       );    
        
        return l_range;
    }
    
    
    /** train the data on the cluster
     * @param p_mpi MPI object for communication
     * @param p_data datapoints
     * @param p_iterations iterations
     **/
    template<typename T> inline void relational_neuralgas<T>::train( const mpi::communicator& p_mpi, const ublas::matrix<T>& p_data, const std::size_t& p_iterations )
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
    template<typename T> inline void relational_neuralgas<T>::train( const mpi::communicator& p_mpi, const ublas::matrix<T>& p_data, const std::size_t& p_iterations, const T& p_lambda )
    {
        if (p_data.size1() < m_prototypes.size1())
            throw exception::runtime(_("number of datapoints are less than prototypes"), *this);
        if (p_iterations == 0)
            throw exception::runtime(_("iterations must be greater than zero"), *this);
        if (p_lambda <= 0)
            throw exception::runtime(_("lambda must be greater than zero"), *this); 
        
        // we check data dimension (data matrix over all CPUs must be squared and the column size of
        // the prototype matrix must be equal to the data size)
        std::size_t l_col      = 0;
        mpi::all_reduce(p_mpi, p_data.size2(), l_col, std::plus<std::size_t>());
        
        if (l_col != p_data.size1())
            throw exception::runtime(_("matrix must be square"), *this);
        if (l_col != m_prototypes.size2())
            throw exception::runtime(_("data and prototype dimension are not equal"), *this);
        
        
        // we use the max. of all values of each process
        const std::size_t l_iterationsMPI = mpi::all_reduce(p_mpi, p_iterations, mpi::maximum<std::size_t>());
        const T l_lambdaMPI               = mpi::all_reduce(p_mpi, p_lambda, mpi::maximum<T>());
        m_logging                         = mpi::all_reduce(p_mpi, m_logging, std::multiplies<bool>());
        setProcessDataPrototypInfo(p_mpi, p_data.size2());
        
        // creates logging
        if (m_logging) {
            m_logprototypes     = std::vector< ublas::matrix<T> >();
            m_quantizationerror = std::vector< T >();
            m_logprototypes.reserve(l_iterationsMPI);
            m_quantizationerror.reserve(l_iterationsMPI);
        }
        
        
        // run neural gas 
        const T l_multi               = 0.01/l_lambdaMPI;
        ublas::matrix<T> l_prototypes = gatherAllPrototypes( p_mpi );
        ublas::vector<T> l_lambda(l_prototypes.size1());
        
        for(std::size_t i=0; i < l_iterationsMPI; ++i) {
            
            // create adapt values
            const T l_lambdahelp = l_lambdaMPI * std::pow(l_multi, static_cast<T>(i)/static_cast<T>(l_iterationsMPI));
            
            #pragma omp parallel for shared(l_lambda)
            for(std::size_t n=0; n < l_lambda.size(); ++n)
                l_lambda(n) = std::exp( -static_cast<T>(n) / l_lambdahelp );
        
            // calculate for every prototype the distance (only the parts of the data matrix)
            // relational: (D * alpha_i)_j - 0.5 * alpha_i^t * D * alpha_i = || x^j - w^i || 
            // D = distance, alpha = weight of the prototype for the konvex combination
            ublas::matrix<T> l_adaptmatrix = ublas::prod(l_prototypes, p_data);

            // the adapt matrix holds all values on the block of the current CPU, so CPU 0 holds the values 0..k, CPU 1 k+1..n and so on.
            // In the next step we must calculate the inner product of each prototyp and each row of the adapt matrix, but we can't because
            // we have only parts of the rows on each CPU, so we split the prototype matrix (with the information of the processdata) in
            // the correct part, create the "local" inner product and sums over all CPUs. Each CPU gets so the correct value for subtract
            // it from their local adapt matrix
            ublas::matrix_range< ublas::matrix<T> > l_protorange( l_prototypes, 
                                                                  ublas::range(0, l_prototypes.size1()), 
                                                                  ublas::range( m_processdatainfo[p_mpi.rank()].first, m_processdatainfo[p_mpi.rank()].first+m_processdatainfo[p_mpi.rank()].second ) 
                                                                );

            for(std::size_t n=0; n < l_protorange.size1(); ++n) {
                const T l_val = 0.5 * mpi::all_reduce( p_mpi, 
                                                       ublas::inner_prod( ublas::row(l_protorange, n), ublas::row(l_adaptmatrix, n) ), 
                                                       std::plus<T>()
                                                     );
                
                for(std::size_t j=0; j < l_adaptmatrix.size2(); ++j)
                    l_adaptmatrix(n, j) -= l_val;
            }
            
            
            // determine quantization error for logging (adaption matrix)
            if (m_logging) {
                m_quantizationerror.push_back( calculateQuantizationError( extractLocalPrototypes(p_mpi, l_adaptmatrix)) );
                m_logprototypes.push_back( extractLocalPrototypes(p_mpi, l_prototypes) );
            }
            
            // for every column ranks values and create adapts
            // we need rank and not randIndex, because we 
            // use the value of the ranking for calculate the 
            // adapt value
            #pragma omp parallel for shared(l_adaptmatrix)
            for(std::size_t n=0; n < l_adaptmatrix.size2(); ++n) {
                ublas::vector<T> l_column                = ublas::column(l_adaptmatrix, n);
                const ublas::vector<std::size_t> l_rank  = tools::vector::rank(l_column);
                
                for(std::size_t j=0; j < l_rank.size(); ++j)
                    l_adaptmatrix(j,n) = l_lambda(l_rank(j));
            }
            
            
            // adapt values are the new prototypes (and run normalization)
            l_prototypes = synchronizePrototypes( p_mpi, l_adaptmatrix );
            
            #pragma omp parallel for shared(l_prototypes)
            for(std::size_t n=0; n < l_prototypes.size1(); ++n) {
                const T l_sum = ublas::sum( ublas::row( l_prototypes, n) );
                
                if (!tools::function::isNumericalZero(l_sum))
                    ublas::row( l_prototypes, n ) /= l_sum;
            }
        }
        
        // extract only the relevant prototypes to the member variable
        m_prototypes = extractLocalPrototypes(p_mpi, l_prototypes);
    }
    
    
    /** calulates distance between datapoints and prototypes and returns a indirect array
     * with index of the nearest prototype
     * @param p_mpi MPI object for communication     
     * @param p_data matrix
     * @return index array of prototype indices
     **/
    template<typename T> inline ublas::indirect_array<> relational_neuralgas<T>::use( const mpi::communicator& p_mpi, const ublas::matrix<T>& p_data ) const
    {
        const ublas::matrix<T> l_prototypes = ublas::trans(gatherAllPrototypes( p_mpi ));
        
        if (l_prototypes.size2() == 0)
            throw exception::runtime(_("number of prototypes must be greater than zero"), *this);
        if (p_data.size2() != l_prototypes.size1())
            throw exception::runtime(_("data and prototype dimension are not equal"), *this);
        
        ublas::indirect_array<> l_idx(p_data.size1());
        const ublas::matrix<T> l_distance = ublas::prod( p_data, l_prototypes );

        #pragma omp parallel for shared(l_idx)
        for(std::size_t i=0; i < l_distance.size1(); ++i) {
            ublas::vector<T> l_col                = ublas::row(l_distance, i);
            const ublas::indirect_array<> l_rank  = tools::vector::rankIndex( l_col );
            l_idx[i] = l_rank(0);
        }
        
        return l_idx;
    }
    
    
    /** blank method for receiving all prototypes. The method can be used in combination with the use-method and the matrix parameter, so
     * only one process can calculate the distances between prototypes and its data, all other process must call only this methode
     * @param p_mpi MPI object for communication 
     **/
    template<typename T> inline void relational_neuralgas<T>::use( const mpi::communicator& p_mpi ) const
    {
        gatherAllPrototypes( p_mpi );
    }
    
    
    /** return all prototypes of the cluster
     * @param p_mpi MPI object for communication
     * @return matrix (rows = prototypes)
     **/
    template<typename T> inline ublas::matrix<T> relational_neuralgas<T>::getPrototypes( const mpi::communicator& p_mpi ) const
    {
        return gatherAllPrototypes( p_mpi );
    }
    
    
    /** returns the logged quantisation error
     * @param p_mpi MPI object for communication
     * @return std::vector with quantization error
     **/
    template<typename T> inline std::vector<T> relational_neuralgas<T>::getLoggedQuantizationError( const mpi::communicator& p_mpi ) const
    {
        // we must call the quantization error of every process and sum all values for the main error
        std::vector< std::vector<T> > l_gatherError;
        mpi::all_gather(p_mpi, m_quantizationerror, l_gatherError);
        
        // we get every quantization error (if the prototypes are empty on the process, the quantization error exists for all other prototypes)
        std::vector<T> l_error = l_gatherError[0];
        for(std::size_t i=0; i < l_gatherError.size(); ++i) {
            T l_min = l_error[i];

            for(std::size_t n=1; n < l_gatherError.size(); ++n)
                l_min = std::min( l_min, l_gatherError[n][i]);
            
            l_error[i] = l_min;
        }
        
        return l_error;
    }
    
    
    /** returns all logged prototypes in all processes
     * @param p_mpi MPI object for communication
     * @return std::vector with all logged prototypes
     **/
    template<typename T> inline std::vector< ublas::matrix<T> > relational_neuralgas<T>::getLoggedPrototypes( const mpi::communicator& p_mpi ) const
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
    
    #endif

}}}
#endif
