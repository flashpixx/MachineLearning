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



#ifndef MACHINELEARNING_NEIGHBORHOOD_KAPPROXIMATION_HPP
#define MACHINELEARNING_NEIGHBORHOOD_KAPPROXIMATION_HPP

#include <boost/static_assert.hpp>  
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/bindings/blas.hpp>

#include "../exception/exception.h"
#include "../tools/tools.h"


namespace machinelearning { namespace neighborhood {  
    
    namespace ublas = boost::numeric::ublas;
    
    
    /** class for k-approximation
     * $LastChangedDate$
     * @todo switch code to using distance classes / objects
     **/
    template<typename T> class kapproximation {
        BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
        
        public:
        
            enum approximation
            {
                knn         = 0,
                spread      = 1,
                random      = 2,
                purerandom  = 3
            };
        
        
            kapproximation( const approximation&, const std::size_t& );
            ublas::indirect_array<> approximate( const ublas::matrix<T>, const ublas::vector<T> ) const;
        
        
        private:
            
            /** type of approximation **/
            const kapproximation m_approx;
            /** number of approximate datasets **/
            const std::size_t m_number;
        
        
            ublas::indirect_array<> approx_knn( const ublas::matrix<T>, const ublas::vector<T> ) const;
        
        
    };
    
    
    /** constructor
     * @param p_approx type of approximation
     * @param p_num number of datasets
    **/
    template<typename T> inline kapproximation<T>::kapproximation( const approximation& p_approx, const std::size_t& p_num ) :
        m_approx(p_approx),
        m_number(p_num)
    {
        if (p_num == 0)
            throw exception::runtime(_("number of approximate data must be greater than zero"));
    }
    
    
    /** run approximation
     * @param p_prototypes prototype matrix
     * @param p_multiplier multiplier vector
     * @return index array
     **/
    template<typename T> inline ublas::indirect_array<> kapproximation<T>::approximate( const ublas::matrix<T> p_prototypes, const ublas::vector<T> p_multiplier ) const
    {
        
        switch (m_approx) {
                
            case knn :
                return approx_knn( p_prototypes, p_multiplier );
        
            default :
                throw exception::runtime(_("project option is unkown"));
        }
    }
    
    
    /** create k-nearest-neighbour approximation
     * @param p_prototypes prototype matrix
     * @param p_multiplier multiplier vector
     * @return index array
     **/
    template<typename T> inline ublas::indirect_array<> kapproximation<T>::approx_knn( const ublas::matrix<T> p_prototypes, const ublas::vector<T> p_multiplier ) const
    {
        // first we create a structure in which the prototype values interpretated as probalistic values
        const T l_boundery = static_cast<T>(1) / p_prototypes.size2();
        
        std::vector< ublas::indirect_array<> > l_idx;
        for(std::size_t i=0; i < p_prototypes.size1(); ++i) {
            
            std::vector<std::size_t> l_tmp;
            for(std::size_t j=0; j < p_prototypes.size2(); ++j)
                if (p_prototypes(i,j) >= l_boundery)
                    l_tmp.push_back(j);
            
            l_idx.push_back( tools::vector::toIndirectArray(l_tmp) );
        }
        
        
        
        return ublas::indirect_array<>();
    }
    
    
    
};};
#endif