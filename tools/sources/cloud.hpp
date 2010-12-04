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
#include "../vector.hpp"


namespace machinelearning { namespace tools { namespace sources {
    
    namespace ublas  = boost::numeric::ublas;
    
    
    /** class for creating cloudpoints. The class creates in the n-dimensional cube points
     *  with a normalized distribution
     **/
    template<typename T> class cloud {
        
        
        public :
            
            cloud( const std::size_t& );
        
            ublas::matrix<T> generate( void ) const;
        
            void setVariance( const std::size_t&, const T& );
            void setVarianceRandom( const std::size_t& );
            void setMean( const std::size_t&, const T& );
            void setMeanRandom( const std::size_t& );
            void setRange( const std::size_t&, const T&, const T&, const std::size_t& );
            void setPoints( const std::size_t&, const std::size_t&, const std::size_t& );
            void setPointsRandom( const std::size_t& );
            
        
        
        private :
            
            const std::size_t m_dimension;
        
            std::vector<bool> m_randomvariance;
            std::vector<bool> m_randommean;
            std::vector<bool> m_randompoints;
            ublas::vector<T> m_mean;    
            ublas::vector<T> m_variance;
            ublas::vector<std::size_t> m_sampling;
            std::vector< std::pair<std::size_t,std::size_t> > m_points;
            std::vector< std::pair<T,T> > m_range;
    };
    
    
    /** constructor with number of dimensions
     * @param p_dim number of dimension
     **/
    template<typename T> inline cloud<T>::cloud( const std::size_t& p_dim ) :
        m_dimension( p_dim ),
        m_mean( tools::vector::random<T>(p_dim) ),
        m_variance( tools::vector::random<T>(p_dim) ),
        m_sampling(p_dim, 10)
    {
        if (p_dim == 0)
            throw exception::runtime(_("number dimensions must be greater than zero"));
        
        for(std::size_t i=0; i < p_dim; ++i) {
            m_mean.push_back( true );
            m_randomvariance.push_back( true );
            
            m_randompoints.push_back( true );
            m_points.push_back( std::pair<std::size_t,std::size_t>(100, 1000) );
            
            m_range.push_back( std::pair<T,T>(0,1) );
        }
        
    }
    
    
    /** generates the clouds **/
    template<typename T> inline ublas::matrix<T> cloud<T>::generate( void ) const
    {
    }
    
    
};};};


#endif
#endif