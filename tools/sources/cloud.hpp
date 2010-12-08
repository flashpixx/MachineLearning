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

#ifdef ML_SOURCES

#ifndef MACHINELEARNING_TOOLS_SOURCES_CLOUD_HPP
#define MACHINELEARNING_TOOLS_SOURCES_CLOUD_HPP


#include <boost/numeric/ublas/matrix.hpp>


#include "../../exception/exception.h"
#include "../language/language.h"
#include "../matrix.hpp"
#include "../random.hpp"
#include "../function.hpp"


namespace machinelearning { namespace tools { namespace sources {
    
    namespace ublas  = boost::numeric::ublas;
    
    
    /** class for creating cloudpoints. The class creates in the n-dimensional cube points
     *  with a normalized distribution
     **/
    template<typename T> class cloud {
        
        
        public :
        
            enum cloudecreate {
                all         = 0,
                alternate   = 1,
                random      = 2
            };
        
        
            
            cloud( const std::size_t& );
        
            ublas::matrix<T> generate( const bool& = true, const cloudecreate& = alternate, const T& = 0 ) const;
        
            void setScale( const std::size_t&, const T& );
            void setScaleRandom( const std::size_t&, const bool& );
            void setPoints( const std::size_t&, const std::size_t&, const std::size_t& );
            void setPointsRandom( const std::size_t&, const bool& );
            void setRange( const std::size_t&, const T&, const T&, const std::size_t& );
        
        
        private :
            
            /** number of dimensions **/
            const std::size_t m_dimension;
        
            std::vector<bool> m_randomscale;
            std::vector<bool> m_randompoints;
            ublas::vector<T> m_scale;
            ublas::vector<std::size_t> m_sampling;
            std::vector< std::pair<std::size_t,std::size_t> > m_points;
            std::vector< std::pair<T,T> > m_range;
        
            void createCenter( const std::vector< ublas::vector<T> >&, const std::size_t&, ublas::vector<T>&, ublas::matrix<T>& ) const;
    };
    
    
    /** constructor with number of dimensions
     * @param p_dim number of dimension
     **/
    template<typename T> inline cloud<T>::cloud( const std::size_t& p_dim ) :
        m_dimension( p_dim ),
        m_scale( p_dim, 1 ),
        m_sampling(p_dim, 5)
    {
        if (p_dim < 2)
            throw exception::runtime(_("number dimensions must be greater than one"));
        
        for(std::size_t i=0; i < p_dim; ++i) {
            m_randomscale.push_back( false );
            
            m_randompoints.push_back( true );
            m_points.push_back( std::pair<std::size_t,std::size_t>(0, 500) );
            
            m_range.push_back( std::pair<T,T>(0,1) );
        }
        
    }
    
    
    /** scales the values in the dimension
     * @param p_dim dimension value (between [0, max. dimension)
     * @param p_var scale
     **/
    template<typename T> inline void cloud<T>::setScale( const std::size_t& p_dim, const T& p_var )
    {
        if ( p_dim >= m_dimension )
            throw exception::runtime(_("dimension must be smaller than saved dimension"));
        
        if (tools::function::isNumericalZero(p_var))
            throw exception::runtime(_("scale need not be zero"));
        
        m_scale(p_dim)       = p_var;
        m_randomscale[p_dim] = false;
    }
    
    
    /** enable / disable random value for scaling
     * @param p_dim dimension value (between [0, max. dimension)
     * @param p_bool bool for enable / disable
     **/
    template<typename T> inline void cloud<T>::setScaleRandom( const std::size_t& p_dim, const bool& p_bool )
    {
        if ( p_dim >= m_dimension )
            throw exception::runtime(_("dimension must be smaller than saved dimension"));
        
        m_randomscale[p_dim] = p_bool;
    }
    
    
    /** sets the minimal and maximal number of points
     * @param p_dim dimension value (between [0, max. dimension)
     * @param p_min minimal number of points
     * @param p_max maximal number of points
     **/
    template<typename T> inline void cloud<T>::setPoints( const std::size_t& p_dim, const std::size_t& p_min, const std::size_t& p_max )
    {
        if ( p_dim >= m_dimension )
            throw exception::runtime(_("dimension must be smaller than saved dimension"));
        
        if (p_min > p_max)
            throw exception::runtime(_("minimal value is greater than maximal value"));
        
        m_points[p_dim]       = std::pair<std::size_t,std::size_t>(p_min, p_max);
        m_randompoints[p_dim] = false;
    }
    
    
    /** enable / disable random value for number of points
     * @param p_dim dimension value (between [0, max. dimension)
     * @param p_bool bool for enable / disable
     **/     
    template<typename T> inline void cloud<T>::setPointsRandom( const std::size_t& p_dim, const bool& p_bool )
    {
        if ( p_dim >= m_dimension )
            throw exception::runtime(_("dimension must be smaller than saved dimension"));
        
        m_randompoints[p_dim] = p_bool;
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
            throw exception::runtime(_("dimension must be smaller than saved dimension"));
        
        if (p_min > p_max)
            throw exception::runtime(_("minimal value is greater than maximal value"));
        
        if (p_sample == 0)
            throw exception::runtime(_("sampling value must be greater than zero"));
    
        m_range[p_dim]      = std::pair<T,T>(p_min, p_max);
        m_sampling(p_dim)   = p_sample;
    }
    
    
    
    /** generates the clouds
     * @param p_shuffle shuffle the generated data points (if not the cloud will be generated along the sampling values)
     * @param p_build type of cloud generation
     * @param p_random random value for random-cloud-generation
     **/
    template<typename T> inline ublas::matrix<T> cloud<T>::generate( const bool& p_shuffle, const cloudecreate& p_build, const T& p_random ) const
    {
        if ( (p_build == random) && ((p_random < 0) || (p_random > 1)) )
            throw exception::runtime(_("random value must be between [0,1]"));
        
        // create sampling adaption
        ublas::vector<T> l_sampleadaption(m_dimension, 1);
        for(std::size_t i=0; i < l_sampleadaption.size(); ++i)
            l_sampleadaption(i) = (m_range[i].second - m_range[i].first) / m_sampling(i);
        
        // samples for each dimension (begin / end values are not used)
        std::vector< ublas::vector<T> > l_samples;
        for(std::size_t i=0; i < m_dimension; ++i) {
            ublas::vector<T> l_val( m_sampling(i)-1 );
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
        
            std::size_t l_numpoints;
            /*if (m_randompoints[i] && (m_range[i].first != m_range[i].second))
                l_numpoints = static_cast<std::size_t>(l_rand.get<T>( tools::random::uniform, m_range[i].first, m_range[i].second ));
            else*/
            //    l_numpoints = 0.5 * (m_range[i].second + m_range[i].first) ;
            
            //std::cout << l_numpoints << std::endl;

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
    
};};};


#endif
#endif