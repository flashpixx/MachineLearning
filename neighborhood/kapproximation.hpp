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
            ublas::indirect_array<> approximate( const ublas::vector<T>, const ublas::matrix<T> ) const;
        
        
        private:
            
            /** type of approximation **/
            const kapproximation m_approx;
            /** number of approximate datasets **/
            const std::size_t m_number;
        
        
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
    
    
    template<typename T> inline ublas::indirect_array<> kapproximation<T>::approximate( const ublas::vector<T>, const ublas::matrix<T> ) const
    {
        return ublas::indirect_array<>();
    }
    
    
    
};};
#endif