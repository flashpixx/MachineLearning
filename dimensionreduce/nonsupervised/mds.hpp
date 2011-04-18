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

#ifndef MACHINELEARNING_DIMENSIONREDUCE_NONSUPERVISED_MDS_HPP
#define MACHINELEARNING_DIMENSIONREDUCE_NONSUPERVISED_MDS_HPP


#include <limits>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>

#include "../dimensionreduce.hpp"
#include "../../exception/exception.h"
#include "../../tools/tools.h"


namespace machinelearning { namespace dimensionreduce { namespace nonsupervised {
    
    namespace ublas  = boost::numeric::ublas;
    
    
    /** create the multidimensional scaling (MDS) with different algorithms
     * $LastChangedDate$
     **/
    template<typename T> class mds : public reduce<T> {
        
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
            
            
            ublas::matrix<T> project_metric( const ublas::matrix<T>& );
            ublas::matrix<T> project_sammon( const ublas::matrix<T>& );
            ublas::matrix<T> project_hit( const ublas::matrix<T>& );
            
            ublas::matrix<T> sammon_distance( const ublas::matrix<T>& ) const;
            T sammon_calculateQuantizationError( const ublas::matrix<T>&, const ublas::matrix<T>& ) const;
            void hit_setZeros(const std::vector< std::pair<std::size_t, std::size_t> >&, ublas::matrix<T>& ) const;
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
            throw exception::runtime(_("dimension must be greater than zero"));

        // set default values for centering
        switch (m_type) {
                
            case metric :
                m_centering = doublecenter;
                break;
                
            case sammon :
                m_centering = singlecenter;
                break;
                
            default : break;
        }
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
     * @param p_en bool
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
            throw exception::runtime( _("matrix must be square") );
        if (p_data.size2() <= m_dim)
            throw exception::runtime(_("datapoint dimension are less than target dimension"));
                
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
                return project_sammon(p_data);
                
            case hit :
                return project_hit(p_data);
                       
            default :
                throw exception::runtime(_("project option is unkown"));

        };
    }
    
    
    
    /** caluate the metric MDS (for metric we use eigenvalues)
     * @param p_data input datamatrix (dissimilarity matrix)
     * @return mapped data
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::project_metric( const ublas::matrix<T>& p_data )
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
     * @note uses code idea of http://ticc.uvt.nl/~lvdrmaaten (in the Matlab code there are duplicated variables)
     * @param p_data input datamatrix (dissimilarity matrix)
     * @return mapped data
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::project_sammon( const ublas::matrix<T>& p_data )
    {
        if (m_iteration == 0)
            throw exception::runtime(_("iterations must be greater than zero"));
        if (m_step == 0)
            throw exception::runtime(_("steps must be greater than zero"));
        
        
        // create the distance for each row/colum (create distance matrix) of the matrix and sets the diagonal elements to one
        const ublas::mapped_matrix<T> l_DataOnes    = tools::matrix::eye<T>( p_data.size1() );   
        const ublas::matrix<T> l_data               = p_data + l_DataOnes;
        const ublas::matrix<T> l_dataInv            = tools::matrix::invert(l_data);
        
        // target point matrix und one matrix
        ublas::matrix<T> l_target                   = tools::matrix::random( l_data.size1(), m_dim, tools::random::uniform, static_cast<T>(-1), static_cast<T>(1) );
        const ublas::mapped_matrix<T> l_TargetOnes  = ublas::scalar_matrix<T>( l_target.size1(), l_target.size2(), static_cast<T>(1) );
        
        
        // optimize
        for(std::size_t i=0; i < m_iteration; ++i) {
            const ublas::matrix<T> l_Distance        = sammon_distance(l_target) + l_DataOnes;           
            const ublas::matrix<T> l_DistanceInv     = tools::matrix::invert(l_Distance);
            const ublas::matrix<T> l_DistanceInv3    = tools::matrix::pow(l_DistanceInv, static_cast<T>(3));
            const ublas::matrix<T> l_target2         = tools::matrix::pow(l_target, static_cast<T>(2));
            
            const ublas::matrix<T> l_delta           = l_data - l_Distance;
            const ublas::matrix<T> l_deltaInv        = l_DistanceInv - l_dataInv;
            const ublas::matrix<T> l_deltaOne        = ublas::prod( l_deltaInv, l_TargetOnes );
            
            // calculating gradient & hesse-matrix values
            const ublas::matrix<T> l_gradient        = ublas::prod( l_deltaInv, l_target ) - ublas::element_prod( l_target, l_deltaOne );
            const ublas::matrix<T> l_hesse           = ublas::prod( l_DistanceInv3, l_target2 ) -  l_deltaOne - 2 * ublas::element_prod(l_target, ublas::prod(l_DistanceInv3, l_target)) + ublas::element_prod(l_target2, ublas::prod(l_DistanceInv3, l_TargetOnes)); 
            
            // create adaption
            ublas::matrix<T> l_adapt(l_target.size1(), l_target.size2(), 0);
            for(std::size_t n=0; n < l_adapt.size1(); ++n)
                for(std::size_t j=0; j < l_adapt.size2(); ++j)
                    if (!tools::function::isNumericalZero(l_hesse(n,j)))
                        l_adapt(n,j) = -l_gradient(n,j) / std::fabs( l_hesse(n,j) );
            
            
            // get quantization error & try to optimize in half-steps
            const T l_error                          = sammon_calculateQuantizationError( l_delta, l_dataInv );
            T l_errornew                             = l_error;
            const ublas::matrix<T> l_targetTmp       = l_target;

            
            for(std::size_t n=1; n <= m_step; ++n) {
                l_target                     = l_targetTmp + l_adapt;
                l_errornew                   = sammon_calculateQuantizationError( l_data - (sammon_distance(l_target) + l_DataOnes), l_dataInv );
                
                if (l_errornew < l_error)
                    break;
                
                if (n == m_step)
                    throw exception::runtime(_("sammon mapping may not converge"));
                
                l_adapt                     *= 0.5;
            }
            
            // if the error "numerical zero" we stop
            if (tools::function::isNumericalZero( (l_error - l_errornew) / l_error ) )
                break;
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
            for(std::size_t i=0; i < l_sse.size1(); ++i)
                for(std::size_t j=0; j < l_sse.size2(); ++j)
                    l_sse(i,j) = std::pow(ublas::sum( tools::vector::pow( static_cast< ublas::vector<T> >(ublas::row(p_matrix, j)-ublas::row(p_matrix, i)), static_cast<T>(2)) ), static_cast<T>(0.5));
        
        return l_sse;
    }
    

    /** caluate the High-Throughput Dimensional Scaling (HIT-MDS)
     * @bug numerical problems
     * @todo optimize matrix with temporary assignment
     * @see http://dig.ipk-gatersleben.de/hitmds/hitmds.html
     * @param p_data input datamatrix (dissimilarity matrix)
     * @return mapped data
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::project_hit( const ublas::matrix<T>& p_data )
    {
        ublas::matrix<T> l_target(p_data.size1(), m_dim); //                   = tools::matrix::random<T>( p_data.size1(), m_dim );
        std::size_t n = 1;
        for(std::size_t i=0; i < l_target.size1(); ++i) {
            l_target(i,0) = n++;
            l_target(i,1) = n++;
        }
        
        // count zero elements
        std::vector< std::pair<std::size_t, std::size_t> > l_zeros;
        for(std::size_t i=0; i < p_data.size1(); ++i)
            for(std::size_t j=0; j < p_data.size2(); ++j)
                if (tools::function::isNumericalZero(p_data(i,j)))
                    l_zeros.push_back( std::pair<std::size_t, std::size_t>(i,j) );
        
        // create init matrix and values
        if (l_zeros.size() == p_data.size1() * p_data.size2())
            throw exception::runtime(_("data matrix has only zero entries"));
        
        ublas::matrix<T> l_data = p_data;
        const T l_datainv   = 1.0 / (l_data.size1() * l_data.size2() - l_zeros.size());
        const T l_mnD       = l_datainv * ublas::sum( tools::matrix::sum(l_data) );
        
        for(std::size_t i=0; i < l_data.size1(); ++i)
            for(std::size_t j=0; j < l_data.size2(); ++j)
                if (i != j)
                    l_data(i,j) -= l_mnD;
        
        
        // optimize
        for(std::size_t i=0; i < m_iteration; ++i) {
            
            std::cout << l_target << std::endl << std::endl;
            
            // create pairs of differences between optimized points and data
            ublas::matrix<T> l_tmp;
            for(std::size_t j=0; j < m_dim; ++j) {
                
                // create a matrix with rows of the j-th column
                ublas::matrix<T> l_row = tools::matrix::repeat( static_cast< ublas::vector<T> >(ublas::column(l_target, j)), tools::matrix::row );
                // create a matrix with columns of the j-th column
                ublas::matrix<T> l_col = tools::matrix::repeat( static_cast< ublas::vector<T> >(ublas::column(l_target, j)), tools::matrix::column );
              
                l_col -= l_row;
                l_col  = ublas::element_prod(l_col, l_col);
                
                if (j == 0)
                    l_tmp = l_col;
                else
                    l_tmp += l_col;
            }
            
            // optimize cost function
            hit_setZeros(l_zeros, l_tmp);
            for(std::size_t j=0; j < l_tmp.size1(); ++j)
                for(std::size_t n=0; n < l_tmp.size2(); ++n)
                    l_tmp(j,n) = std::sqrt( l_tmp(j,n) );
            
            const T l_mnT = l_datainv * ublas::sum( tools::matrix::sum(l_tmp) );
            
            for(std::size_t j=0; j < l_tmp.size1(); ++j)
                for(std::size_t n=0; n < l_tmp.size2(); ++n)
                    l_tmp(j,n) -= l_mnT;
            hit_setZeros(l_zeros, l_tmp);
            
            
            // create adaption values
            const ublas::matrix<T> l_el1 = ublas::element_prod(l_tmp, l_data);
            const ublas::matrix<T> l_el2 = ublas::element_prod(l_tmp, l_tmp);
            
            T l_miT = ublas::sum( tools::matrix::sum( l_el1 ) ); 
            T l_moT = ublas::sum( tools::matrix::sum( l_el2 ) );
            
            const T l_F   = 2.0 / (std::fabs(l_miT) +  std::fabs(l_moT));
            l_miT *= l_F;
            l_moT *= l_F;
            
            // calculate update strength parts
            ublas::matrix<T> l_strength = l_tmp * l_miT - l_data * l_moT;
            
            for(std::size_t j=0; j < l_tmp.size1(); ++j)
                for(std::size_t n=0; n < l_tmp.size2(); ++n)
                    l_tmp(j,n) += 0.1 + l_mnT;
            
            l_strength = ublas::element_div(l_strength, l_tmp);
            

            // calculate update strength of the points
            ublas::matrix<T> l_adapt = tools::matrix::repeat( static_cast< ublas::vector<T> >(ublas::column(l_target, m_dim-1)), tools::matrix::column ) - tools::matrix::repeat( static_cast< ublas::vector<T> >(ublas::column(l_target, m_dim-1)), tools::matrix::row );;
            l_adapt = ublas::element_prod(l_adapt, l_strength);

            ublas::matrix<T> l_update(l_target.size1(), l_target.size2());
            ublas::column(l_update, m_dim-1) = tools::matrix::sum(l_adapt, tools::matrix::column);
            
            for(std::size_t j=0; j < m_dim-1; ++j) {
                // create a matrix with rows of the j-th column
                ublas::matrix<T> l_row = tools::matrix::repeat( static_cast< ublas::vector<T> >(ublas::column(l_target, j)), tools::matrix::row );
                // create a matrix with columns of the j-th column
                ublas::matrix<T> l_col = tools::matrix::repeat( static_cast< ublas::vector<T> >(ublas::column(l_target, j)), tools::matrix::column );
                
                l_col -= l_row;
                l_col  = ublas::element_prod(l_col, l_strength);
                
                ublas::column(l_update, j) = tools::matrix::sum(l_col, tools::matrix::column);
            }
            
            // create new target points
            const T l_rate = m_rate * (m_iteration-i) * 0.25 * (1.0 + (m_iteration-i)%2) / m_iteration;
            
            for(std::size_t j=0; j < l_target.size1(); ++j)
                for(std::size_t n=0; n < l_target.size2(); ++n)
                    l_target(j,n) += l_rate * l_update(j,n) / std::sqrt(std::fabs(l_update(j,n))+0.001);
        }
        
        return l_target;
    }
    
    /** sets all elements which are in the vector to zero values
     * @param pair vector with indices
     * @param referenz of a matrix
    **/
    template<typename T> inline void mds<T>::hit_setZeros(const std::vector< std::pair<std::size_t, std::size_t> >& p_zeros, ublas::matrix<T>& p_matrix ) const
    {
        for(std::size_t i=0; i < p_zeros.size(); ++i)
            p_matrix( p_zeros[i].first, p_zeros[i].second ) = 0;
    }
    
    
};};};
#endif
