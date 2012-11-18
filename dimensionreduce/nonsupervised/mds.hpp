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

#ifndef __MACHINELEARNING_DIMENSIONREDUCE_NONSUPERVISED_MDS_HPP
#define __MACHINELEARNING_DIMENSIONREDUCE_NONSUPERVISED_MDS_HPP

#include <omp.h>

#include <limits>
#include <boost/static_assert.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#ifdef MACHINELEARNING_MPI
#include <boost/mpi.hpp>
#endif

#include "reduce.hpp"
#include "../../errorhandling/exception.hpp"
#include "../../tools/tools.h"


namespace machinelearning { namespace dimensionreduce { namespace nonsupervised {
    
    #ifndef SWIG
    namespace ublas  = boost::numeric::ublas;
    #ifdef MACHINELEARNING_MPI
    namespace mpi   = boost::mpi;
    #endif
    #endif
    
    
    /** create the multidimensional scaling (MDS) with different algorithms **/
    template<typename T> class mds : public reduce<T>
        #ifdef MACHINELEARNING_MPI
        , public reducempi<T>
        #endif
    {
        #ifndef SWIG
        BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
        #endif
        
        
        public :
        
            enum project {
                metric              = 0,
                sammon              = 1,
                hit                 = 2
            };
            
            enum centeroption {
                none             = 0,
                singlecenter     = 1,
                doublecenter     = 2
            };
        
        
            mds( const std::size_t&, const project& = metric );
            ublas::matrix<T> map( const ublas::matrix<T>& );
            std::size_t getDimension( void ) const;
            void setIteration( const std::size_t& );
            void setStep( const std::size_t& );
            void setRate( const T& );
            void setCentering( const centeroption& );
        
            #ifdef MACHINELEARNING_MPI
            ublas::matrix<T> map( const mpi::communicator&, const ublas::matrix<T>& );
            #endif
        
        
        private :
        
            /** number of iterations for sammon and hit **/
            std::size_t m_iteration;
            /** stepsize for sammon **/
            std::size_t m_step;
            /** rate value for hit **/
            T m_rate;
            /** target dimension **/
            const std::size_t m_dim;
            /** project type **/
            const project m_type;
            /** centering **/
            centeroption m_centering;
            
            
            ublas::matrix<T> project_metric( const ublas::matrix<T>& ) const;
            ublas::matrix<T> project_sammon( const ublas::matrix<T>& ) const;
            ublas::matrix<T> project_hit( const ublas::matrix<T>& ) const;
        
            ublas::matrix<T> sammon_distance( const ublas::matrix<T>& ) const;
            T sammon_calculateQuantizationError( const ublas::matrix<T>&, const ublas::matrix<T>& ) const;
            void hit_setZeros(const std::vector< std::pair<std::size_t, std::size_t> >&, ublas::matrix<T>& ) const;
        
            #ifdef MACHINELEARNING_MPI
            ublas::matrix<T> project_hit( const mpi::communicator&, const ublas::matrix<T>& ) const;
            ublas::vector<T> hit_connectVector( const mpi::communicator&, const ublas::vector<T>& ) const;
            std::size_t hit_matrixPosition( const mpi::communicator&, const std::size_t& ) const;
            #endif
        
    };
    
    
    /** constructor
     * @param p_dim target dimension
     * @param p_type project type
     **/
    template<typename T> inline mds<T>::mds( const std::size_t& p_dim, const project& p_type ) :
        m_iteration( 500 ),
        m_step( 20 ),
        m_rate( 1 ),
        m_dim( p_dim ),
        m_type( p_type ),
        m_centering( none )
    {
        if (p_dim == 0)
            throw exception::runtime(_("dimension must be greater than zero"), *this);
    }
    
    
    /** returns the target dimensione size
     * @return number of dimension
     **/
    template<typename T> inline std::size_t mds<T>::getDimension( void ) const
    {
        return m_dim;
    }
    
    
    /** sets the rate value for hit mds
     * @param p_rate
    **/
    template<typename T> inline void mds<T>::setRate( const T& p_rate ) {
        m_rate = p_rate;
    }
    
    
    /** sets the iterations for iterative algorithms like Sammons mapping
     * @param p_iteration iterations
     **/
    template<typename T> inline void mds<T>::setIteration( const std::size_t& p_iteration )
    {
        m_iteration = p_iteration;
    }
    
    
    /** sets the steps for iterative algorithms
     * @param p_step number of steps
     **/
    template<typename T> inline void mds<T>::setStep( const std::size_t& p_step )
    {
        m_step = p_step;
    }
    
    
    /** enables / disables centering before mapping
     * @param p_center centering option
     **/
    template<typename T> inline void mds<T>::setCentering( const centeroption& p_center )
    {
        m_centering = p_center;
    }
    
    
    /** caluate and project the input data
     * @param p_data input datamatrix (dissimilarity matrix)
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::map( const ublas::matrix<T>& p_data )
    {
        if (p_data.size1() != p_data.size2())
            throw exception::runtime( _("matrix must be square"), *this );
        if (p_data.size2() <= m_dim)
            throw exception::runtime(_("datapoint dimension are less than target dimension"), *this);
                
        // do centering
        ublas::matrix<T> l_data = p_data;
        switch (m_centering) {
                
            case singlecenter :
                l_data = tools::matrix::centering(l_data);
                break;
                
            case doublecenter :
                l_data = tools::matrix::doublecentering(l_data);
                break;
                
            default : break;
        };
        
        
        // do project
        switch (m_type) {
                
            case metric:
                // we use a stable matrix for eigendecompsition
                return project_metric( 1.0/l_data.size1() * ublas::prod(l_data, ublas::trans(l_data)) );
            
            case sammon:
                return project_sammon(l_data);
                
            case hit :
                return project_hit(l_data);
                       
            default :
                throw exception::runtime(_("project option is unkown"), *this);

        };
    }
    
    
    
    /** caluate the metric MDS (for metric we use eigenvalues)
     * @param p_data input datamatrix (dissimilarity matrix)
     * @return mapped data
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::project_metric( const ublas::matrix<T>& p_data ) const
    {
        // calculate the eigenvalues & -vectors
        ublas::vector<T> l_eigenvalues;
        ublas::matrix<T> l_eigenvectors;
        tools::lapack::eigen<T>(p_data, l_eigenvalues, l_eigenvectors);
        
        // rank the eigenvalues
        const ublas::indirect_array<> l_rank = tools::vector::rankIndex( l_eigenvalues );
        
        // create projection (largest eigenvectors correspondends with the largest eigenvalues -> last values in rank)
        ublas::matrix<T> l_project( l_eigenvectors.size2(), m_dim );
        ublas::matrix<T> l_values(m_dim, m_dim, 0);
        for(std::size_t i=0; i < m_dim; ++i) {
            ublas::column(l_project, m_dim-i-1) = ublas::column(l_eigenvectors, l_rank(l_rank.size()-i-1));
            l_values(m_dim-i-1, m_dim-i-1) = std::pow(l_eigenvalues(l_rank(l_rank.size()-i-1)), static_cast<T>(0.5));
        }
        
        return ublas::prod(l_project, l_values);
    }
    
    
    /** calculate the sammon mapping on MDS (with pseudo-newton method for optimization)
     * @note uses code idea of http://ticc.uvt.nl/~lvdrmaaten (in the Matlab code there are duplicated variables). The Matlab code does
     * not return the same points like the function code. The break during the optimization is set by the numerical limits of the data types.
     * @param p_data input datamatrix (dissimilarity matrix)
     * @return mapped data
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::project_sammon( const ublas::matrix<T>& p_data ) const
    {
        if (m_iteration == 0)
            throw exception::runtime(_("iterations must be greater than zero"), *this);
        if (m_step == 0)
            throw exception::runtime(_("steps must be greater than zero"), *this);
        
        
        // create the distance for each row/colum (create distance matrix) of the matrix and sets the diagonal elements to one
        const ublas::mapped_matrix<T> l_DataEye     = tools::matrix::eye<T>( p_data.size1() );   
        const ublas::matrix<T> l_data               = p_data + l_DataEye;
        const ublas::matrix<T> l_dataInv            = tools::matrix::invert(l_data);
        
        // target point matrix und one matrix
        ublas::matrix<T> l_target                   = tools::matrix::random( l_data.size1(), m_dim, tools::random::uniform, static_cast<T>(-1), static_cast<T>(1) );
        const ublas::mapped_matrix<T> l_TargetOnes  = ublas::scalar_matrix<T>( l_target.size1(), l_target.size2(), static_cast<T>(1) );
        T l_error                                   = sammon_calculateQuantizationError( l_data - sammon_distance(l_target) + l_DataEye, l_dataInv );
        
        // optimize
        for(std::size_t i=0; i < m_iteration; ++i) {
            const ublas::matrix<T> l_Distance        = sammon_distance(l_target) + l_DataEye;           
            const ublas::matrix<T> l_DistanceInv     = tools::matrix::invert(l_Distance);
            const ublas::matrix<T> l_DistanceInv3    = tools::matrix::pow(l_DistanceInv, static_cast<T>(3));
            const ublas::matrix<T> l_target2         = tools::matrix::pow(l_target, static_cast<T>(2));
            
            const ublas::matrix<T> l_delta           = l_data - l_Distance;
            const ublas::matrix<T> l_deltaInv        = l_DistanceInv - l_dataInv;
            const ublas::matrix<T> l_deltaOne        = ublas::prod( l_deltaInv, l_TargetOnes );
            
            // calculating gradient & hesse-matrix values
            const ublas::matrix<T> l_gradient        = ublas::prod( l_deltaInv, l_target ) - ublas::element_prod( l_target, l_deltaOne );
            const ublas::matrix<T> l_hesse           = ublas::prod( l_DistanceInv3, l_target2 ) -  l_deltaOne - static_cast<T>(2) * ublas::element_prod(l_target, ublas::prod(l_DistanceInv3, l_target)) + ublas::element_prod(l_target2, ublas::prod(l_DistanceInv3, l_TargetOnes)); 
            
            // create adaption
            ublas::matrix<T> l_adapt(l_target.size1(), l_target.size2(), static_cast<T>(0));
            #pragma omp parallel for shared(l_adapt)
            for(std::size_t n=0; n < l_adapt.size1(); ++n)
                for(std::size_t j=0; j < l_adapt.size2(); ++j)
                    if (!tools::function::isNumericalZero(l_hesse(n,j)))
                        l_adapt(n,j) = -l_gradient(n,j) / std::fabs( l_hesse(n,j) );
            
            // get quantization error & try to optimize in half-steps
            T l_errornew                         = 0;
            const ublas::matrix<T> l_targetTmp   = l_target;
            
            for(std::size_t n=1; n <= m_step; ++n) {
                l_target             = l_targetTmp + l_adapt;
                l_errornew           = sammon_calculateQuantizationError( l_data - (sammon_distance(l_target) + l_DataEye), l_dataInv );
                
                if (l_errornew < l_error)
                    break;
                
                if (n == m_step)
                    throw exception::runtime(_("sammon mapping may not converge"), *this);
                
                l_adapt                     *= static_cast<T>(0.5);
            }
            
            // if the error "numerical zero" we stop
            if (tools::function::isNumericalZero( (l_error - l_errornew) / l_error ) )
                break;

            l_error = l_errornew;
        }
        
        return l_target;
    }

    
    /** creates the error value of sammon optimizing
     * @param p_delta delta values (differnce between original and target points)
     * @param p_invert inverted data values
     * @return error value
     **/
    template<typename T> inline T mds<T>::sammon_calculateQuantizationError( const ublas::matrix<T>& p_delta, const ublas::matrix<T>& p_invert ) const
    {
        ublas::matrix<T> l_mat = ublas::element_prod( tools::matrix::pow(p_delta, static_cast<T>(2)), p_invert);
        return ublas::sum( tools::matrix::sum( l_mat ) );
    }
    
    
    /** calculate the distance between every row of the matrix and the other rows
     * @param p_matrix input matrix
     * @return distance matrix
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::sammon_distance( const ublas::matrix<T>& p_matrix ) const
    {
        ublas::matrix<T> l_sse( p_matrix.size1(), p_matrix.size1(), 0 );
        if ( p_matrix.size1() == p_matrix.size2() ) {
            l_sse = tools::matrix::pow(p_matrix, static_cast<T>(2));
            const ublas::matrix<T> l_mat = ublas::outer_prod(tools::matrix::sum(l_sse), ublas::scalar_vector<T>(p_matrix.size1(),1));
            l_sse = l_mat + ublas::trans(l_mat) - 2*ublas::prod(p_matrix, ublas::trans(p_matrix));
            l_sse = tools::matrix::pow(l_sse, static_cast<T>(0.5));                       
        } else 
            #pragma omp parallel for shared(l_sse)
            for(std::size_t i=0; i < l_sse.size1(); ++i)
                for(std::size_t j=0; j < l_sse.size2(); ++j)
                    l_sse(i,j) = std::pow(ublas::sum( tools::vector::pow( static_cast< ublas::vector<T> >(ublas::row(p_matrix, j)-ublas::row(p_matrix, i)), static_cast<T>(2)) ), static_cast<T>(0.5));
        
        return l_sse;
    }
    

    /** caluate the High-Throughput Dimensional Scaling (HIT-MDS)
     * @todo optimize matrix with temporary assignment
     * @note the actual position of data points is dependent on the template type of the class, because the accuracy of the type of influence on the optimization
     * @see http://dig.ipk-gatersleben.de/hitmds/hitmds.html
     * @param p_data input datamatrix (dissimilarity matrix)
     * @return mapped data
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::project_hit( const ublas::matrix<T>& p_data ) const
    {
        ublas::matrix<T> l_target = tools::matrix::random( p_data.size1(), m_dim, tools::random::uniform, static_cast<T>(-1), static_cast<T>(1) );
  
        // count zero elements
        std::vector< std::pair<std::size_t, std::size_t> > l_zeros;
        #pragma omp parallel for shared(l_zeros)
        for(std::size_t i=0; i < p_data.size1(); ++i)
            for(std::size_t j=0; j < p_data.size2(); ++j)
                if (tools::function::isNumericalZero(p_data(i,j)))
                    #pragma omp critical
                    l_zeros.push_back( std::pair<std::size_t, std::size_t>(i,j) );
        
        // create init matrix and values
        if (l_zeros.size() == p_data.size1() * p_data.size2())
            throw exception::runtime(_("data matrix has only zero entries"), *this);
        
        ublas::matrix<T> l_data = p_data;
        const T l_datainv       = static_cast<T>(1) / (l_data.size1() * l_data.size2() - l_zeros.size());
        const T l_mnD           = l_datainv * ublas::sum( tools::matrix::sum(l_data) );
        
        #pragma omp parallel for shared(l_data)
        for(std::size_t i=0; i < l_data.size1(); ++i)
            for(std::size_t j=0; j < l_data.size2(); ++j)
                if (i != j)
                    l_data(i,j) -= l_mnD;
        hit_setZeros(l_zeros, l_data);

        
        // optimize
        for(std::size_t i=0; i < m_iteration; ++i) {
            
            // create pairs of differences between optimized points and data
            ublas::matrix<T> l_tmp(l_data.size1(), l_data.size2(), static_cast<T>(0));
            for(std::size_t j=0; j < m_dim; ++j) {
                // create a matrix with columns of the j-th column
                ublas::matrix<T> l_col = tools::matrix::repeat( static_cast< ublas::vector<T> >(ublas::column(l_target, j)), tools::matrix::column );
                
                l_col -= ublas::trans(l_col);
                l_tmp += ublas::element_prod(l_col, l_col);
            }
            
            // optimize cost function
            hit_setZeros(l_zeros, l_tmp);
            for(std::size_t j=0; j < l_tmp.size1(); ++j)
                for(std::size_t n=0; n < l_tmp.size2(); ++n)
                    l_tmp(j,n) = std::sqrt( l_tmp(j,n) );
            
            const T l_mnT = l_datainv * ublas::sum( tools::matrix::sum(l_tmp) );
            
            #pragma omp parallel for shared(l_tmp)
            for(std::size_t j=0; j < l_tmp.size1(); ++j)
                for(std::size_t n=0; n < l_tmp.size2(); ++n)
                    l_tmp(j,n) -= l_mnT;
            hit_setZeros(l_zeros, l_tmp);
            
            // create adaption values
            const ublas::matrix<T> l_el1 = ublas::element_prod(l_tmp, l_data);
            const ublas::matrix<T> l_el2 = ublas::element_prod(l_tmp, l_tmp);
            
            T l_miT = ublas::sum( tools::matrix::sum( l_el1 ) ); 
            T l_moT = ublas::sum( tools::matrix::sum( l_el2 ) );
 
            const T l_F  = static_cast<T>(2) / (std::fabs(l_miT) + std::fabs(l_moT));
            l_miT       *= l_F;
            l_moT       *= l_F;
            
            
            // calculate update strength parts
            ublas::matrix<T> l_strength = l_tmp * l_miT - l_data * l_moT;
            
            #pragma omp parallel for shared(l_tmp)
            for(std::size_t j=0; j < l_tmp.size1(); ++j)
                for(std::size_t n=0; n < l_tmp.size2(); ++n)
                    l_tmp(j,n) += static_cast<T>(0.1) + l_mnT;
            
            l_strength = ublas::element_div(l_strength, l_tmp);
          

            // calculate update strength of the points
            ublas::matrix<T> l_adapt = tools::matrix::repeat( static_cast< ublas::vector<T> >(ublas::column(l_target, m_dim-1)), tools::matrix::column );
            l_adapt = ublas::element_prod( l_adapt-ublas::trans(l_adapt), l_strength);
          
            // create update matrix
            ublas::matrix<T> l_update(l_target.size1(), l_target.size2(), static_cast<T>(0));
            ublas::column(l_update, m_dim-1) = tools::matrix::sum(l_adapt, tools::matrix::column);

            for(std::size_t j=0; j < m_dim-1; ++j) {
                // create a matrix with columns of the j-th column
                ublas::matrix<T> l_col = tools::matrix::repeat( static_cast< ublas::vector<T> >(ublas::column(l_target, j)), tools::matrix::column );
                
                l_col  = ublas::element_prod(l_col-ublas::trans(l_col), l_strength);
                ublas::column(l_update, j) = tools::matrix::sum(l_col, tools::matrix::column);
            }
            
            // create new target points
            const T l_rate = m_rate * (m_iteration-i) * static_cast<T>(0.25) * (static_cast<T>(1) + (m_iteration-i)%2) / m_iteration;
            
            #pragma omp parallel for shared(l_target)
            for(std::size_t j=0; j < l_target.size1(); ++j)
                for(std::size_t n=0; n < l_target.size2(); ++n)
                    l_target(j,n) += l_rate * l_update(j,n) / std::sqrt(std::fabs(l_update(j,n))+static_cast<T>(0.001));
        }
        
        return l_target;
    }
    
    
    /** sets all elements which are in the vector to zero values
     * @param p_zeros pair vector with indices
     * @param p_matrix referenz of a matrix
    **/
    template<typename T> inline void mds<T>::hit_setZeros(const std::vector< std::pair<std::size_t, std::size_t> >& p_zeros, ublas::matrix<T>& p_matrix ) const
    {
        for(std::size_t i=0; i < p_zeros.size(); ++i)
            p_matrix( p_zeros[i].first, p_zeros[i].second ) = static_cast<T>(0);
    }
    
    
    //======= MPI ==================================================================================================================================
    #ifdef MACHINELEARNING_MPI
    
    
    /** caluate and project the input data
     * @param p_mpi MPI object for communication
     * @param p_data input datamatrix (dissimilarity matrix), over all cores the matrix must be square (the matrix is cutted in column parts)
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::map( const mpi::communicator& p_mpi, const ublas::matrix<T>& p_data )
    {
        // centering works only on non-mpi-use because we 
        if ( (m_centering == singlecenter) || (m_centering == doublecenter) )
            throw exception::runtime(_("centering can not be used with MPI"), *this);
        
        // we check data dimension (data matrix over all CPUs must be squared and the column size of
        // the prototype matrix must be equal to the data size)
        std::size_t l_col      = 0;
        mpi::all_reduce(p_mpi, p_data.size2(), l_col, std::plus<std::size_t>());
        
        if (l_col != p_data.size1())
            throw exception::runtime(_("matrix must be square"), *this);

        
        // do project
        switch (m_type) {
                
            case hit :
                return project_hit(p_mpi, p_data);
                
            default :
                throw exception::runtime(_("MPI project option is unkown"), *this);
                
        };
    }
    
    
    /** caluate the High-Throughput Dimensional Scaling (HIT-MDS) with MPI
     * @todo optimize matrix with temporary assignment
     * @note the actual position of data points is dependent on the template type of the class, because the accuracy of the type of influence on the optimization
     * @see http://dig.ipk-gatersleben.de/hitmds/hitmds.html
     * @param p_mpi MPI object for communication
     * @param p_data input datamatrix (dissimilarity matrix)
     * @return mapped data
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::project_hit( const mpi::communicator& p_mpi, const ublas::matrix<T>& p_data ) const
    {
        // sync global data
        const std::size_t l_iterationsMPI = mpi::all_reduce(p_mpi, m_iteration, mpi::maximum<std::size_t>());
        const std::size_t l_dimensionMPI  = mpi::all_reduce(p_mpi, m_dim, mpi::maximum<std::size_t>());
        const T l_rateMPI                 = mpi::all_reduce(p_mpi, m_rate, mpi::maximum<T>());
        
        // detect start position within the full matrix, for setting diagonal values
        const std::size_t l_columnstart = hit_matrixPosition( p_mpi, p_data.size2() );
        
        ublas::matrix<T> l_target = tools::matrix::random( p_data.size2(), l_dimensionMPI, tools::random::uniform, static_cast<T>(-1), static_cast<T>(1) );
        
        // count zero elements
        std::vector< std::pair<std::size_t, std::size_t> > l_zeros;
        #pragma omp parallel for shared(l_zeros)
        for(std::size_t i=0; i < p_data.size1(); ++i)
            for(std::size_t j=0; j < p_data.size2(); ++j)
                if (tools::function::isNumericalZero(p_data(i,j)))
                    #pragma omp critical
                    l_zeros.push_back( std::pair<std::size_t, std::size_t>(i,j) );
        
        // create init matrix and values
        std::size_t l_numzeros = 0;
        mpi::all_reduce(p_mpi, l_zeros.size(), l_numzeros, std::plus<std::size_t>());
        if (l_numzeros == p_data.size1() * p_data.size1())
            throw exception::runtime(_("data matrix has only zero entries"), *this);
    
        ublas::matrix<T> l_data = p_data;
        const T l_datainv       = static_cast<T>(1) / (l_data.size1() * l_data.size1() - l_numzeros);
    
        T l_help                = static_cast<T>(0);
        mpi::all_reduce(p_mpi, ublas::sum( tools::matrix::sum(l_data) ), l_help, std::plus<T>());
        const T l_mnD           = l_datainv * l_help;
        
        #pragma omp parallel for shared(l_data)
        for(std::size_t i=0; i < l_data.size1(); ++i)
            for(std::size_t j=0; j < l_data.size2(); ++j)
                if (i != j+l_columnstart)
                    l_data(i,j) -= l_mnD;
        hit_setZeros(l_zeros, l_data);
        
        
        // optimize
        for(std::size_t i=0; i < l_iterationsMPI; ++i) {
            
            // create pairs of differences between optimized points and data (the temp matrix has the size of the input matrix columns)
            ublas::matrix<T> l_tmp(l_data.size2(), l_data.size1(), static_cast<T>(0));
            for(std::size_t j=0; j < l_dimensionMPI; ++j) {
                
                // reads over all processes the column of the target matrix and extract the relevant elements
                const ublas::vector<T> l_fullrow = hit_connectVector( p_mpi, static_cast< ublas::vector<T> >(ublas::column(l_target, j)) );
                
                // create a matrix with columns of the j-th column
                ublas::matrix<T> l_col      = tools::matrix::repeat( static_cast< ublas::vector<T> >(ublas::column(l_target, j)), l_data.size1(), tools::matrix::column );
                
                // do subtract (equiv the subtract with transpose)
                for(std::size_t n=0; n < l_col.size1(); ++n)
                    ublas::row(l_col, n) -= l_fullrow;

                l_tmp += ublas::element_prod(l_col, l_col);
            }
            
            // transpose l_temp because we need the same oriantation like l_data (input matrix)
            l_tmp = ublas::trans(l_tmp);
            
            // optimize cost function
            hit_setZeros(l_zeros, l_tmp);
            for(std::size_t j=0; j < l_tmp.size1(); ++j)
                for(std::size_t n=0; n < l_tmp.size2(); ++n)
                    l_tmp(j,n) = std::sqrt( l_tmp(j,n) );
            
            // the sum must be weighted with the full data size, because the square matrix l_tmp is only the part of the matrix.
            l_help = static_cast<T>(0);
            mpi::all_reduce(p_mpi, ublas::sum( tools::matrix::sum(l_tmp) ), l_help, std::plus<T>());
            const T l_mnT = l_datainv * l_help;
            
            #pragma omp parallel for shared(l_tmp)
            for(std::size_t j=0; j < l_tmp.size1(); ++j)
                for(std::size_t n=0; n < l_tmp.size2(); ++n)
                    l_tmp(j,n) -= l_mnT;
            hit_setZeros(l_zeros, l_tmp);
            
            
            // create adaption values
            const ublas::matrix<T> l_el1 = ublas::element_prod(l_tmp, l_data);
            const ublas::matrix<T> l_el2 = ublas::element_prod(l_tmp, l_tmp);
            
            T l_miT = static_cast<T>(0);
            T l_moT = static_cast<T>(0);
            mpi::all_reduce(p_mpi, ublas::sum( tools::matrix::sum( l_el1 ) ), l_miT, std::plus<T>());
            mpi::all_reduce(p_mpi, ublas::sum( tools::matrix::sum( l_el2 ) ), l_moT, std::plus<T>());
            
            const T l_F  = static_cast<T>(2) / (std::fabs(l_miT) + std::fabs(l_moT));
            l_miT       *= l_F;
            l_moT       *= l_F;
            
            // calculate update strength parts
            ublas::matrix<T> l_strength = l_tmp * l_miT - l_data * l_moT;
            
            #pragma omp parallel for shared(l_tmp)
            for(std::size_t j=0; j < l_tmp.size1(); ++j)
                for(std::size_t n=0; n < l_tmp.size2(); ++n)
                    l_tmp(j,n) += static_cast<T>(0.1) + l_mnT;
            
            l_strength = ublas::element_div(l_strength, l_tmp);

    
            // calculate update strength of the points - reads over all processes the column of the target matrix
            const ublas::vector<T> l_fulllastrow = hit_connectVector( p_mpi, static_cast< ublas::vector<T> >(ublas::column(l_target, l_dimensionMPI-1)) );
            
            // create a matrix with columns of the j-th column
            ublas::matrix<T> l_adapt         = tools::matrix::repeat( static_cast< ublas::vector<T> >(ublas::column(l_target, l_dimensionMPI-1)), l_data.size1(), tools::matrix::column );
            
            // do subtract (equiv the subtract with transpose)
            for(std::size_t n=0; n < l_adapt.size1(); ++n)
                ublas::row(l_adapt, n) -= l_fulllastrow;
            
            // strength matrix must be rotate
            l_strength = ublas::trans(l_strength);
            
            // transpose l_temp because we need the same oriantation like l_adapt
            l_adapt = ublas::element_prod(l_adapt, l_strength);
            
            
            // we sum over all columns of the adapt matrix and sum over each element
            // and cut the elements, which are needed for the local matrix
            ublas::vector<T> l_lastcolumn( l_data.size1(), static_cast<T>(0) );
            mpi::all_reduce(p_mpi, tools::matrix::sum(l_adapt, tools::matrix::column), l_lastcolumn, std::plus< ublas::vector<T> >());
            ublas::vector_range< ublas::vector<T> > l_updatelastcolumn( l_lastcolumn, ublas::range( l_columnstart, l_columnstart+l_data.size2() ) );
            
            // create update matrix
            ublas::matrix<T> l_update(l_target.size1(), l_target.size2(), static_cast<T>(0));
            ublas::column(l_update, l_dimensionMPI-1) =  l_updatelastcolumn;

            for(std::size_t j=0; j < l_dimensionMPI-1; ++j) {
                const ublas::vector<T> l_fullrow = hit_connectVector( p_mpi, static_cast< ublas::vector<T> >(ublas::column(l_target, j)) );
                
                // create a matrix with columns of the j-th column
                ublas::matrix<T> l_col = tools::matrix::repeat( static_cast< ublas::vector<T> >(ublas::column(l_target, j)), l_data.size1(), tools::matrix::column );
                
                // do subtract (equiv the subtract with transpose)
                for(std::size_t n=0; n < l_col.size1(); ++n)
                    ublas::row(l_col, n) -= l_fullrow;
    
                l_col  = ublas::element_prod(l_col, l_strength);
                
                // we create the column data of the update matrix over all processes
                ublas::vector<T> l_columnvector( l_data.size1(), static_cast<T>(0) );
                mpi::all_reduce(p_mpi, tools::matrix::sum(l_col, tools::matrix::column), l_columnvector, std::plus< ublas::vector<T> >());
                ublas::vector_range< ublas::vector<T> > l_updatecol( l_columnvector, ublas::range( l_columnstart, l_columnstart+l_data.size2() ) );
                
                ublas::column(l_update, j) = l_updatecol;
            }
            
            // create new target points
            const T l_rate = l_rateMPI * (l_iterationsMPI-i) * static_cast<T>(0.25) * (static_cast<T>(1) + (l_iterationsMPI-i)%2) / l_iterationsMPI;
            
            #pragma omp parallel for shared(l_target)
            for(std::size_t j=0; j < l_target.size1(); ++j)
                for(std::size_t n=0; n < l_target.size2(); ++n)
                    l_target(j,n) += l_rate * l_update(j,n) / std::sqrt(std::fabs(l_update(j,n))+static_cast<T>(0.001));
        }
        
        return l_target;
    }
    
    
    /** connects a vector over all processes and return the full vector
     * @param p_mpi MPI object for communication
     * @param p_vec input vector
     * @return full vecotr
     **/
    template<typename T> inline ublas::vector<T> mds<T>::hit_connectVector( const mpi::communicator& p_mpi, const ublas::vector<T>& p_vec ) const
    {
        // gathering vector data
        std::vector< ublas::vector<T> > l_data;
        mpi::all_gather(p_mpi, p_vec, l_data);
        
        ublas::vector<T> l_vec = l_data[0];
        for(std::size_t i=1; i < l_data.size(); ++i) {
            l_vec.resize( l_vec.size() + l_data[i].size() );
            
            ublas::vector_range< ublas::vector<T> > l_range( l_vec, ublas::range( l_vec.size()-l_data[i].size(), l_vec.size() ) );
            l_range.assign( l_data[i] );
        }    
        
        return l_vec;
    }
    
    
    /** returns the position within the matrix (over all processes)
     * @param p_mpi MPI object for communication
     * @param p_col column position if the matrix part on the local process
     * @return colum position of the local matrix
     **/
    template<typename T> inline std::size_t mds<T>::hit_matrixPosition( const mpi::communicator& p_mpi, const std::size_t& p_col ) const
    {
        std::size_t l_col = 0;
        std::vector<std::size_t> l_columndata;
        mpi::all_gather(p_mpi, p_col, l_columndata);

        for(std::size_t i=0; i < static_cast<std::size_t>(p_mpi.rank()); ++i)
            l_col += l_columndata[i];
        
        return l_col;
    }
    
    #endif
    
    
}}}
#endif
