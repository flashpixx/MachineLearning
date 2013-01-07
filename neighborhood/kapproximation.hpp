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



#ifndef __MACHINELEARNING_NEIGHBORHOOD_KAPPROXIMATION_HPP
#define __MACHINELEARNING_NEIGHBORHOOD_KAPPROXIMATION_HPP

#include <boost/variant.hpp> 
#include <boost/static_assert.hpp>  
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/bindings/blas.hpp>

#include "../errorhandling/exception.hpp"
#include "../tools/tools.h"


namespace machinelearning { namespace neighborhood {  
    
    namespace ublas = boost::numeric::ublas;
    
    
    /** class for k-approximation
     * @todo switch code to using distance classes / objects
     **/
    template<typename T> class kapproximation
    {
        BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
        
        public:
        
            enum approximation
            {
                knn         = 0,
                spread      = 1,
                random      = 2,
                purerandom  = 3
            };
        
            typedef boost::variant< std::vector< ublas::indirect_array<> >, ublas::vector<T> > approximateddata;
        
        
            kapproximation( const approximation&, const std::size_t& );
            approximateddata approximate( const ublas::matrix<T>&, const ublas::vector<T>&, const ublas::matrix<T>& ) const;
        
        
        private:
            
            /** type of approximation **/
            const kapproximation m_approx;
            /** number of approximate datasets **/
            const std::size_t m_number;
        
            approximateddata approx_knn( const ublas::matrix<T>&, const ublas::vector<T>&, const ublas::matrix<T>& ) const;
        
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
            throw exception::runtime(_("number of approximate data must be greater than zero"), *this);
    }
    
    
    /** run approximation
     * @param p_prototypes prototype matrix
     * @param p_multiplier multiplier vector
     * @param p_distance distance values
     * @return index array
     **/
    template<typename T> inline typename kapproximation<T>::approximateddata kapproximation<T>::approximate( const ublas::matrix<T>& p_prototypes, const ublas::vector<T>& p_multiplier, const ublas::matrix<T>& p_distance ) const
    {
        switch (m_approx) {
                
            case knn :
                return approx_knn( p_prototypes, p_multiplier, p_distance );
        
            default :
                throw exception::runtime(_("project option is unkown"), *this);
        }
    }
    
    
    /** create k-nearest-neighbour approximation
     * @param p_prototypes prototype matrix
     * @param p_multiplier multiplier vector
     * @param p_distance distance values
     * @return index array
     **/
    template<typename T> inline typename kapproximation<T>::approximateddata kapproximation<T>::approx_knn( const ublas::matrix<T>& p_prototypes, const ublas::vector<T>& p_multiplier, const ublas::matrix<T>& p_distance ) const
    {       
        // the dimension of the prototype is interpretated as probability
        const T l_boundery      = static_cast<T>(1) / p_prototypes.size2();
        
        // iterate over each prototyp so the we create a array with dimensions
        // that are greater and equal than the probalisitc value
        std::vector< ublas::indirect_array<> > l_idx;
        for(std::size_t i=0; i < p_prototypes.size1(); ++i) {
            
            std::vector<std::size_t> l_tmp;
            for(std::size_t j=0; j < p_prototypes.size2(); ++j)
                if (p_prototypes(i,j) >= l_boundery)
                    l_tmp.push_back(j);

            // add an empty data set if no boundary data is found
            if (l_tmp.size() == 0) {
                l_idx.push_back( ublas::indirect_array<>(0) );
                continue;   
            }
            
            // we read the distance data and sort them to get the nearest data vectors
            const ublas::vector<T> l_row      = p_distance(i, tools::vector::toIndirectArray(l_tmp));
            ublas::indirect_array<> l_sortidx = tools::vector::rankIndex(l_row);
            
            // create a index array with <= k-max number of indices and fill it with the smallest indices
            // (we can't use a std::copy, because we need the original index values, so we use the numerical index)
            ublas::indirect_array<> l_tmpidx( ((l_sortidx.size() <= m_number) ? l_sortidx.size() : m_number) );
            for(std::size_t j=0; j < l_tmpidx.size(); ++j)
                l_tmpidx[j] = l_tmp[ l_sortidx(j) ];
            
            l_idx.push_back(l_tmpidx);
        }

        // determine multiplier for each approximate dataset and determine / remove "lonely neurons"
        ublas::vector<T> l_multiplier(l_idx.size(), 0);
        for(std::size_t i=0; i < l_multiplier.size(); ++i)
            if (l_idx[i].size() > 0)
                l_multiplier(i) = static_cast<T>(p_multiplier(i)) / l_idx[i].size();
            else
                l_multiplier.erase_element(i);
            
        // create return dataset       
        approximateddata l_ret;
        l_ret = l_idx;
        l_ret = l_multiplier;
        
        return l_ret;
    }
    
    
    
}}
#endif
