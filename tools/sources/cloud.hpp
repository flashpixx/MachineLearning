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

#ifdef MACHINELEARNING_SOURCES

#ifndef __MACHINELEARNING_TOOLS_SOURCES_CLOUD_HPP
#define __MACHINELEARNING_TOOLS_SOURCES_CLOUD_HPP

#include <boost/numeric/ublas/matrix.hpp>


#include "../../errorhandling/exception.hpp"
#include "../language/language.h"
#include "../matrix.hpp"
#include "../random.hpp"
#include "../function.hpp"


namespace machinelearning { namespace tools { namespace sources {
    
    namespace ublas  = boost::numeric::ublas;
    
    
    /** class for creating multimodal cloudpoints. The class creates in the n-dimensional
     * cube points with a normalized distribution
     **/
    template<typename T> class cloud 
    {
        
        
        public :
        
            enum cloudcreate {
                all         = 0,
                alternate   = 1,
                random      = 2
            };
        
        
            
            cloud( const std::size_t& );
            ublas::matrix<T> generate( const cloudcreate& = all, const T& = 0.5, const bool& = false ) const;
            void setVariance( const T&, const T& );
            void setVarianceRandom( const bool& );
            void setPoints( const std::size_t&, const std::size_t& );
            void setPointsRandom( const bool& );
            void setRange( const std::size_t&, const T&, const T&, const std::size_t& );
        
        
        private :
            
            /** number of dimensions **/
            const std::size_t m_dimension;
            /** bool for creating random number of points **/
            bool m_randompoints;
            /** minimum and maximum value for number of points **/
            std::pair<std::size_t,std::size_t> m_points;
            /** minimum and maximum variance value **/
            std::pair<T,T> m_variance;
            /** bool for creating variance randomly **/
            bool m_randomvariance;
            /** sampling values for each dimension **/
            ublas::vector<std::size_t> m_sampling;
            /** ranges of the each dimension **/
            std::vector< std::pair<T,T> > m_range;
        
            void createCenter( const std::vector< ublas::vector<T> >&, const std::size_t&, ublas::vector<T>&, ublas::matrix<T>& ) const;
    };
    
    
    /** constructor with number of dimensions
     * @param p_dim number of dimension
     **/
    template<typename T> inline cloud<T>::cloud( const std::size_t& p_dim ) :
        m_dimension( p_dim ),
        m_randompoints( false ),
        m_points( std::pair<std::size_t,std::size_t>(500, 500) ),
        m_variance( std::pair<T,T>(1,1) ),
        m_randomvariance( false ),
        m_sampling(p_dim, 5)
    {
        if (p_dim < 2)
            throw exception::runtime(_("number of dimensions must be greater than one"), *this);
        
        for(std::size_t i=0; i < p_dim; ++i)
            m_range.push_back( std::pair<T,T>(0,1) );
    }
    
    
    /** sets the variance for each cloud
     * @param p_min minimum value
     * @param p_max maximum value
     **/
    template<typename T> inline void cloud<T>::setVariance( const T& p_min, const T& p_max )
    {
        if (tools::function::isNumericalZero(p_min))
            throw exception::runtime(_("minimum need not be zero"), *this);
        
        if (tools::function::isNumericalZero(p_max))
            throw exception::runtime(_("maximum need not be zero"), *this);       
        
        if (p_min > p_max)
            throw exception::runtime(_("minimal value is greater than maximal value"), *this);
        
        if ((p_min < 0) || (p_max < 0))
            throw exception::runtime(_("minimum and maximum must be greater than zero"), *this);
        
        m_variance       = std::pair<T,T>(p_min, p_max);
        m_randomvariance = true;
    }
    
    
    /** enable / disable random value for variance
     * @param p_bool bool for enable / disable
     **/
    template<typename T> inline void cloud<T>::setVarianceRandom( const bool& p_bool )
    {
        m_randomvariance = p_bool;
    }
    
    
    /** sets the minimal and maximal number of points
     * @param p_min minimal number of points
     * @param p_max maximal number of points
     **/
    template<typename T> inline void cloud<T>::setPoints( const std::size_t& p_min, const std::size_t& p_max )
    {
        if (p_min > p_max)
            throw exception::runtime(_("minimal value is greater than maximal value"), *this);
        
        m_points       = std::pair<std::size_t,std::size_t>(p_min, p_max);
        m_randompoints = true;
    }
    
    
    /** enable / disable random value for number of points
     * @param p_bool bool for enable / disable
     **/     
    template<typename T> inline void cloud<T>::setPointsRandom( const bool& p_bool )
    {
        m_randompoints = p_bool;
    }
    
    
    /** sets the value range and sampling number in each dimension
     * @param p_dim dimension value (between [0, max. dimension)
     * @param p_min minimal value
     * @param p_max maximal value
     * @param p_sample number of samples
     **/
    template<typename T> inline void cloud<T>::setRange( const std::size_t& p_dim, const T& p_min, const T& p_max, const std::size_t& p_sample )
    {
        if ( p_dim >= m_dimension )
            throw exception::runtime(_("dimension must be smaller than saved dimension"), *this);
        
        if (p_min > p_max)
            throw exception::runtime(_("minimal value is greater than maximal value"), *this);
        
        if (p_sample == 0)
            throw exception::runtime(_("sampling value must be greater than zero"), *this);
    
        m_range[p_dim]      = std::pair<T,T>(p_min, p_max);
        m_sampling(p_dim)   = p_sample;
    }
    
    
    
    /** generates the clouds
     * @param p_build type of cloud generation
     * @param p_random random value for random-cloud-generation
     * @param p_shuffle shuffel the datapoints
     **/
    template<typename T> inline ublas::matrix<T> cloud<T>::generate( const cloudcreate& p_build, const T& p_random, const bool& p_shuffle ) const
    {
        if ( (p_build == random) && ((p_random < 0) || (p_random > 1)) )
            throw exception::runtime(_("random value must be between [0,1]"), *this);
        
        // create sampling adaption
        ublas::vector<T> l_sampleadaption(m_dimension, 1);
        for(std::size_t i=0; i < l_sampleadaption.size(); ++i)
            l_sampleadaption(i) = (m_range[i].second - m_range[i].first) / (m_sampling(i)+1);
        
        // samples for each dimension (begin / end values are not used)
        std::vector< ublas::vector<T> > l_samples;
        for(std::size_t i=0; i < m_dimension; ++i) {
            ublas::vector<T> l_val( m_sampling(i) );
            for(std::size_t j=0; j < l_val.size(); ++j)
                l_val(j) = m_range[i].first + (j+1)*l_sampleadaption(i);
            
            l_samples.push_back(l_val);
        }
        
        // create center values (each row is a center of the normal disribution)
        ublas::vector<T> l_vec(l_samples.size(), 0);
        ublas::matrix<T> l_center;
        createCenter( l_samples, 0, l_vec, l_center );
        
        
        // create the cloud values
        ublas::matrix<T> l_cloud;
        tools::random l_rand;
        
        for(std::size_t i=0; i < l_center.size1(); ++i) {
            
            if ((p_build == alternate) && (i%2 != 0))
                continue;
            if ((p_build == random) && (l_rand.get<T>(tools::random::uniform, 0, 1) >= p_random))
                continue;
            
            
            // sets number of points
            std::size_t l_numpoints = 0;
            if (m_randompoints && (m_points.first != m_points.second))
                l_numpoints = static_cast<std::size_t>(l_rand.get<T>( tools::random::uniform, m_points.first, m_points.second ));
            else
                l_numpoints = (m_points.second + m_points.first) / 2;
            
            
            // sets the variance
            T l_variance;
            if ((m_randomvariance) && (!function::isNumericalEqual(m_variance.first, m_variance.second)) )
                l_variance = l_rand.get<T>( tools::random::uniform, m_variance.first, m_variance.second );
            else
                l_variance = 0.5 * (m_variance.first + m_variance.second);
            
            
            // create one cloud
            ublas::matrix<T> l_points = tools::matrix::random<T>( l_numpoints, l_center.size2(), tools::random::normal, 0, l_variance );
            
            
            // translation to center
            for(std::size_t j=0; j < l_points.size1(); ++j)
                ublas::row(l_points, j) += ublas::row(l_center, i);
                
            
            // add to dataset
            l_cloud.resize( l_cloud.size1()+l_points.size1(), l_points.size2());
            ublas::matrix_range< ublas::matrix<T> > l_range(l_cloud, 
                                                            ublas::range( l_cloud.size1()-l_points.size1(), l_cloud.size1() ), 
                                                            ublas::range( 0, l_cloud.size2() )
                                                            );
            l_range.assign(l_points);
            
        }
        
        // we shuffel all rows
        if (p_shuffle) {
            const ublas::vector<std::size_t> l_idx = static_cast< ublas::vector<std::size_t> >(tools::vector::random<T>(l_cloud.size1(), tools::random::uniform, 0, l_cloud.size1()));
            for(std::size_t i=0; i < l_idx.size(); ++i) {
                ublas::vector<T> l_tmp        = ublas::row(l_cloud, i);
                ublas::row(l_cloud, i)        = ublas::row(l_cloud, l_idx(i));
                ublas::row(l_cloud, l_idx(i)) = l_tmp;
            }
        }
            
        
        return l_cloud;
    }
    
    
    /** create recursive a matrix with position of the centers
     * @param p_allvec vector with vector of sampling points
     * @param p_pos position with sampling vector is used
     * @param p_vec row vector which will be added to the matrix (size must be allocated first [size of p_allvec] )
     * @param p_mat matrix
     **/
    template<typename T> inline void cloud<T>::createCenter( const std::vector< ublas::vector<T> >& p_allvec, const std::size_t& p_pos, ublas::vector<T>& p_vec, ublas::matrix<T>& p_mat ) const
    {
        if (p_pos < p_allvec.size())
            for(std::size_t i=0; i < p_allvec[p_pos].size(); ++i) {
                p_vec(p_pos) = p_allvec[p_pos](i);
                createCenter( p_allvec, p_pos+1, p_vec, p_mat );
            }
        else {
            p_mat.resize( p_mat.size1()+1, p_vec.size() );
            ublas::row(p_mat, p_mat.size1()-1) = p_vec;
        }

    }    
    
}}}
#endif
#endif
