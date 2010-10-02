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
    namespace mpi	= boost::mpi;
    #endif
    
    /** class for calculate (batch) neural gas
     * @todo parallelism with threading and MPI in this case: every CPU gets some prototypes (randomly).
     * Every CPU holds his own dataset and calculates the distance between prototype and datapoints.
     * After calculating the prototypes on the k-th CPU will send to the (k+1)-th CPU (the n-th send to the first CPU, building a circle)
     * and calculates the distance again. Finish is than, if the prototypes from the first CPU are back on them (on full circle run).
     * Every CPU hold now the distances for every prototype and can calculate their rankings. After we summarize the adaption for every
     * prototype and send the information to the CPU on which the prototype is saved and do the adaption.
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
        
            T calculateQuantizationError( const ublas::matrix<T>& ) const;
        
            #ifdef CLUSTER
            ublas::matrix<T> gatherPrototypes( const mpi::communicator&, const ublas::matrix<T>& ) const;
            std::size_t gatherNumberPrototypes( const mpi::communicator& ) const;
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
     * @param p_MPI MPI object for communication
     * @param p_Prototypes local prototypes
     * @return full prototypes matrix
     **/
    template<typename T> inline ublas::matrix<T> neuralgas<T>::gatherPrototypes( const mpi::communicator& p_MPI, const ublas::matrix<T>& p_Prototypes ) const
    {
        // gathering in this way, that every process get all prototypes
        std::vector< ublas::matrix<T> > l_processdata;
        for(int i=0; i < p_MPI.size(); ++i)
            mpi::gather(p_MPI, p_Prototypes, l_processdata, i);
        
        // create full prototype matrix with processprotos
        ublas::matrix<T> l_prototypes = l_processdata[0];
        for(std::size_t i=1; i < l_processdata.size(); ++i) {
            l_prototypes.resize( l_prototypes.size1()+l_processdata[i].size1(), l_prototypes.size2());
            l_prototypes.project().assign( l_processdata[i] );
        }
        
        return l_prototypes;
        
    }
    
    
    /** returns the number of all prototypes
     * @param p_MPI MPI object for communication
     * @return number of prototypes
     **/
    template<typename T> inline std::size_t neuralgas<T>::gatherNumberPrototypes( const mpi::communicator& ) const
    {
        // gathering number of prototypes
        ublas::vector<std::size_t> l_numbers;
        for(int i=0; i < p_MPI.size(); ++i)
            mpi::gather(p_MPI, m_prototypes.size1(), l_numbers, i);
        
        return ublas::sum( l_numbers );
    }
    
    
    /** train the data on the cluster
     * @overload
     * @param p_MPI MPI object for communication
     * @param p_data datapoints
     * @param p_iterations iterations
     **/
    template<typename T> inline void neuralgas<T>::train( const mpi::communicator& p_MPI, const ublas::matrix<T>& p_data, const std::size_t& p_iterations )
    {
        train(p_MPI, p_data, p_iterations, m_prototypes.size1() * 0.5);
    }

    
    /** train the data on the cluster
     * @overload
     * @param p_MPI MPI object for communication
     * @param p_data datapoints
     * @param p_iterations iterations
     * @param p_lambda max adapet size
     **/
    template<typename T> inline void neuralgas<T>::train( const mpi::communicator& p_MPI, const ublas::matrix<T>& p_data, const std::size_t& p_iterations, const T& p_lambda )
    {
        std::cout << gatherNumberPrototypes(p_MPI) << std::endl;
        
        if (p_data.size1() < m_prototypes.size1())
            throw exception::parameter(_("number of datapoints are less than prototypes"));
        if (p_iterations == 0)
            throw exception::parameter(_("iterations must be greater than zero"));
        if (p_data.size2() != m_prototypes.size2())
            throw exception::matrix(_("data and prototype dimension are not equal"));
        if (p_lambda <= 0)
            throw exception::parameter(_("lambda must be greater than zero"));
        
        
        // process 0 sets the iteration and the lambda
        std::size_t l_iterationsBrd = p_iterations;
        T l_lambdaBrd               = p_lambda;
        
        mpi::broadcast(p_MPI, l_iterationsBrd, 0);
        mpi::broadcast(p_MPI, l_lambdaBrd, 0);
        
        // create matrix for adaption (we need the number of prototype of every process)
        // we collcting the number of prototypes of every process into a std::map
        std::size_t l_protonum =  0;
        std::map<int, std::pair<std::size_t,std::size_t> > l_processinfo;
        for(int l=0; l < p_MPI.size(); ++l) {
            std::size_t l_num = m_prototypes.size1();
            mpi::broadcast(p_MPI, l_num, l);
            
            // process rank and in the first element of the pair is set the number of prototypes and
            // the second is the start value in the full prototype matrix
            // [0, number of first process prototypes-1], [number of first process prototypes, number of second process prototypes-1]
            l_processinfo.insert( std::pair<int, std::pair<std::size_t, std::size_t> >( l, std::pair<std::size_t, std::size_t>(l_num, l_protonum) ) );
            l_protonum += l_num;
        }
        ublas::matrix<T> l_adaptmatrix( l_protonum, p_data.size1() );
        
         
        
        // run neural gas       
        const T l_multi = 0.01/l_lambdaBrd;
        ublas::scalar_vector<T> l_ones(p_data.size1(), 1);
        
        for(std::size_t i=0; (i < l_iterationsBrd); ++i) {
            
            // create adapt values
            const T l_lambda = l_lambdaBrd * std::pow(l_multi, static_cast<T>(i)/static_cast<T>(l_iterationsBrd));
            
            // first we need the local prototypes
			ublas::matrix<T> l_prototypes = m_prototypes;
                        
            // we iterate over every process
            for(int l=0; l < p_MPI.size(); ++l) {
            
                // we get the prototypes of every process
                mpi::broadcast(p_MPI, l_prototypes, l);
                                
                // calculate for every prototype the distance (of the actually prototypes).
                // within the adapt matrix, we must specify the position of the prototypes 
                for(std::size_t n=0; n < l_prototypes.size1(); ++n)
                    ublas::row(l_adaptmatrix, l_processinfo[l].second+n)  = m_distance->calculate( p_data,  ublas::outer_prod(l_ones, ublas::row(l_prototypes, n)) );

            }
            
            // at this point, every process holds the adaptmatrix
            // for every prototype and their local datapoints
            
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
                
            // calculate the "prototype" for the local points
            ublas::matrix<T> l_localprototype = ublas::prod( l_adaptmatrix, p_data );

			std::cout << l_localprototype << std::endl;

            // now we send every local prototypes with their adaption to the process back 
            // we do this a gather for prototypes and adaption
            std::vector< ublas::matrix<T> > l_prototypesGather;
            std::vector< ublas::vector<T> > l_normadaptGather;
            for(int l=0; l < p_MPI.size(); ++l) {
                
                // extract the prototypes for the process and create a copy for sending
                ublas::matrix_range< ublas::matrix<T> > l_range(l_localprototype, ublas::range(l_processinfo[l].second, l_processinfo[l].first+l_processinfo[l].second), ublas::range(0, l_localprototype.size2()));
                ublas::matrix<T> l_prototypesend      = l_range;
                
                // creates the normalize values for the prototypes
                ublas::vector<T> l_adeptsend(l_range.size1(), 0);
                for(std::size_t m=0; m < l_range.size1(); ++m)
                    l_adeptsend(m) = ublas::sum( ublas::row(l_adaptmatrix, l_processinfo[l].second+m) );
                
                // do the gathering
                mpi::gather(p_MPI, l_prototypesend, l_prototypesGather, l);
                mpi::gather(p_MPI, l_adeptsend, l_normadaptGather, l);
            }
            
            // create the prototypes of the gathering data as sum
                             m_prototypes = l_prototypesGather[0];
            ublas::vector<T> l_norm       = l_normadaptGather[0];
            for(std::size_t l=1; l < l_prototypesGather.size(); ++l) {
                m_prototypes += l_prototypesGather[l];
                l_norm       += l_normadaptGather[l];
            }

            // now we do the normalization 
            for(std::size_t n=0; n < m_prototypes.size1(); ++n)
                if (!tools::function::isNumericalZero(l_norm(n)))
                    ublas::row(m_prototypes, n) /= l_norm(n);
        }
    }
    
    
    /** return all prototypes of the cluster
     * @overload
     * @param p_MPI MPI object for communication
     * @return matrix (rows = number of prototypes)
     **/
    template<typename T> inline ublas::matrix<T> neuralgas<T>::getPrototypes( const mpi::communicator& p_MPI ) const
    {
        // we catch all prototypes on every process
		std::vector< ublas::matrix<T> > l_prototypesGather;
		
		std::size_t l_numprotos = 0;
		for(int l=0; l < p_MPI.size(); ++l) {
			std::size_t l_num = m_prototypes.size1();
            
			mpi::broadcast(p_MPI, l_num, l);
			mpi::gather(p_MPI, m_prototypes, l_prototypesGather, l);
			
			l_numprotos += l_num;
		}

		// conect the full row matrix of prototypes
		std::size_t l_idx = 0;
		ublas::matrix<T> l_prototypes(l_numprotos, m_prototypes.size2());
		for(std::size_t i=0; i < l_prototypesGather.size(); ++i) {
			ublas::matrix<T> l_processproto = l_prototypesGather[i];
			
			for(std::size_t n=0; n < l_processproto.size1(); ++n)
				ublas::row(l_prototypes, l_idx++) = ublas::row(l_processproto, n);
		}

        return l_prototypes;
    }
    
    #endif
    
};};};


#endif
