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


#ifndef MACHINELEARNING_GENETICALGORITHM_IMPLEMENTATION_BINARYINDIVIDUAL_HPP
#define MACHINELEARNING_GENETICALGORITHM_IMPLEMENTATION_BINARYINDIVIDUAL_HPP

#include <cmath>

#include <boost/static_assert.hpp>
#include <boost/shared_ptr.hpp>

#include "../individual.hpp"
#include "../../tools/tools.h"
#include "../../exception/exception.h"


namespace machinelearning { namespace geneticalgorithm {
    
    /** class of a binary indivdual (template type must be an unsigned integral type)
     * $LastChangedDate$
     **/
    template<typename T> class binaryindividual : virtual public individual<T> {
        BOOST_STATIC_ASSERT( boost::is_integral<T>::value && boost::is_unsigned<T>::value );
        
        
        public :
        
            binaryindividual( const std::size_t& );
    
            T getData( void ) const;
            void clone( boost::shared_ptr< binaryindividual<T> >& ) const;
            void mutate( void );
            std::size_t size( void ) const;
        
        
        private :
        
            /** number generator **/
            tools::random m_rand;
            /** number of bit position are used **/
            const std::size_t m_size;
            /** value of the object **/
            T m_value;
    };

    
    
    /** contructor of the binary individual
     * @param p_size number of bits that should be used
     **/
    template<typename T> inline binaryindividual<T>::binaryindividual( const std::size_t& p_size ) :
        m_rand(),
        m_size( p_size ),
        m_value(0) 
    {
        if (p_size == 0)
            throw exception::runtime(_("size number need not to be zero"));
        if (m_size > sizeof(T)*8)
            throw exception::runtime(_("size number is lager than datatype"));
        
        m_value = static_cast<T>(m_rand.get<double>(tools::random::uniform, 0, std::pow(2.0,static_cast<double>(m_size))-1));
    }
    
    
    /** mutates the object **/
    template<typename T> inline void binaryindividual<T>::mutate( void )
    {
        m_value ^= 0x01 << static_cast<T>(m_rand.get<double>(tools::random::uniform, 0, m_size));
    }
    
    
    /** clones the object / create a new object on the heap
     * @param p_ptr return reference of the new smart-pointer object
     **/
    template<typename T> inline void binaryindividual<T>::clone( boost::shared_ptr< binaryindividual<T> >& p_ptr) const
    {
        p_ptr = boost::shared_ptr< binaryindividual<T> >( new binaryindividual<T>(m_size) );
    }
    
    
    /** returns the data of the object **/
    template<typename T> inline T binaryindividual<T>::getData( void ) const
    {
        return m_value;
    }
    
    
    /** returns the number of bits that are used
     * @return length
     **/
    template<typename T> inline std::size_t binaryindividual<T>::size( void ) const
    {
        return m_size;
    }
    
    
};};

#endif

