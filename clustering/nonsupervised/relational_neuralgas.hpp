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


#ifndef MACHINELEARNING_CLUSTERING_NONSUPERVISED_RNG_HPP
#define MACHINELEARNING_CLUSTERING_NONSUPERVISED_RNG_HPP

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
    
    /** class for calculate (batch) relational neural gas
     * $LastChangedDate$
     * @note The MPI methods do not check the correct ranges / dimension of the prototype
     * data, so it is the task of the developer to use the correct ranges. Also the MPI
     * methods must be called in the correct order, so the MPI calls must be run
     * on each process.
     **/
    template<typename T> class relational_neuralgas : public clustering<T>, public patch<T> {
        
        public:
        
            relational_neuralgas( const std::size_t&, const std::size_t&, const std::size_t& = 1 );
            void train( const ublas::matrix<T>&, const std::size_t& );
            void train( const ublas::matrix<T>&, const std::size_t&, const T& );
            ublas::matrix<T> getPrototypes( void ) const;
            void setLogging( const bool& );
            std::vector< ublas::matrix<T> > getLoggedPrototypes( void ) const;
            bool getLogging( void ) const;
            std::size_t getPrototypeSize( void ) const;
            std::size_t getPrototypeCount( void ) const;
            std::vector<T> getLoggedQuantizationError( void ) const;
        
        
        
        private:
        
            /** prototypes **/
            ublas::matrix<T> m_prototypes;
            /** k-approximation value **/
            const std::size_t m_kapprox;
            /** bool for logging prototypes **/
            bool m_logging;
            /** std::vector for prototypes for each iteration **/
            std::vector< ublas::matrix<T> > m_logprototypes;
            /** std::vector for quantisation error in each iteration **/
            std::vector<T> m_quantizationerror;
            /** bool for check initialized patch **/
            bool m_firstpatch;
        
        
    };

    
    /** contructor for initialization the neural gas
     * @param p_prototypes number of prototypes
     * @param p_prototypesize size of each prototype (data dimension)
     * @param p_kapprox k-approximation value for patch neural gas (default 1)
     **/
    template<typename T> inline relational_neuralgas<T>::relational_neuralgas( const std::size_t& p_prototypes, const std::size_t& p_prototypesize, const std::size_t& p_kapprox ) :
        m_prototypes( tools::matrix::random<T>(p_prototypes, p_prototypesize) ),
        m_kapprox(p_kapprox),
        m_logging( false ),
        m_logprototypes( std::vector< ublas::matrix<T> >() ),
        m_quantizationerror( std::vector<T>() ),
        m_firstpatch(true)
    {
        if (p_prototypesize == 0)
            throw exception::runtime(_("prototype size must be greater than zero"));
        
        // normalize the prototypes
        distances::euclid<T> l_dist;
        l_dist.normalize( m_prototypes );
    }
    
    /** returns the prototype matrix
     * @return matrix (rows = number of prototypes)
     **/
    template<typename T> inline ublas::matrix<T> relational_neuralgas<T>::getPrototypes( void ) const
    {
        return m_prototypes;
    }
    
    
    
    /** enabled logging for training
     * @param p bool
     **/
    template<typename T> inline void relational_neuralgas<T>::setLogging( const bool& p )
    {
        m_logging = p;
    }
    
    
    
    /** shows the logging status
     * @return bool
     **/
    template<typename T> inline bool relational_neuralgas<T>::getLogging( void ) const
    {
        return m_logging;
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
    };
    
    
    /** returns the number of prototypes
     * @return number of the prototypes / classes
     **/
    template<typename T> inline std::size_t relational_neuralgas<T>::getPrototypeCount( void ) const 
    {
        return m_prototypes.size1();
    };
    
    
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
            throw exception::runtime(_("number of prototypes must be greater than zero"));
        if (p_data.size1() < m_prototypes.size1())
            throw exception::runtime(_("number of datapoints are less than prototypes"));
        if (p_iterations == 0)
            throw exception::runtime(_("iterations must be greater than zero"));
        if (p_data.size2() != m_prototypes.size2())
            throw exception::runtime(_("data and prototype dimension are not equal"));
        if (p_lambda <= 0)
            throw exception::runtime(_("lambda must be greater than zero"));
        if (p_data.size1() != p_data.size2())
            throw exception::runtime(_("matrix must be square"));
    }

    //======= MPI ==================================================================================================================================
    #ifdef MACHINELEARNING_MPI
    #endif

};};};
#endif