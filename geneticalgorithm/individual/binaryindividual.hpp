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


#ifndef __MACHINELEARNING_GENETICALGORITHM_INDIVIDUAL_BINARYINDIVIDUAL_HPP
#define __MACHINELEARNING_GENETICALGORITHM_INDIVIDUAL_BINARYINDIVIDUAL_HPP

#include <iostream>
#include <boost/static_assert.hpp>
#include <boost/shared_ptr.hpp>

#include "individual.hpp"
#include "../../tools/tools.h"
#include "../../errorhandling/exception.hpp"


namespace machinelearning { namespace geneticalgorithm { namespace individual {
    
    /** class of a binary indivdual (template type must be an unsigned integral type) **/
    template<typename T> class binaryindividual : public individual<T>
    {
        BOOST_STATIC_ASSERT( boost::is_integral<T>::value && boost::is_unsigned<T>::value );
        
        
        public :
        
            binaryindividual( const std::size_t& );
            ~binaryindividual( void );
    
            T& operator[]( const std::size_t& );
            T operator[]( const std::size_t& ) const;
            void show( void ) const;
            void clone( boost::shared_ptr< individual<T> >& ) const;
            void mutate( void );
            std::size_t size( void ) const;
        
        private :
        
            /** number generator **/
            tools::random m_rand;
            /** number of bit position are used **/
            const std::size_t m_size;
            /** value of the object **/
            T* m_value;

    };
    
    
    
    
    /** contructor of the binary individual
     * @param p_size number of bits that should be used
     **/
    template<typename T> inline binaryindividual<T>::binaryindividual( const std::size_t& p_size ) :
        m_rand(),
        m_size( p_size ),
        m_value( NULL ) 
    {
        if (p_size == 0)
            throw exception::runtime(_("size number need not to be zero"), *this);
        
        m_value = (T*)calloc( m_size, sizeof(T) );
        if (!m_value)
            throw exception::runtime(_("can not allocate memory"), *this);
        
        for(std::size_t i=0; i < m_size; ++i)
            m_value[i] = static_cast<T>(m_rand.get<double>(tools::random::uniform, 0, 2));
    }

    
    /** destructor **/
    template<typename T> inline binaryindividual<T>::~binaryindividual( void )
    {
        free(m_value);
    }

    
    /** return reference on index position
     * @param p_index index position
     * @return reference
     **/
    template<typename T> inline T& binaryindividual<T>::operator[]( const std::size_t& p_index )
    {
        if (p_index >= m_size)
            throw exception::runtime(_("index out of range"), *this);

        // only at this point we can create different values [0,1],
        // so we reorganize here values
        for( std::size_t i=0; i < m_size; ++i)
            m_value[i] = m_value[i] != 0 ? 1 : 0;
        
        return  m_value[p_index];
    }
    
    
    /** read value on index position
     * @param p_index index position
     * @return value
     **/
    template<typename T> inline T binaryindividual<T>::operator[]( const std::size_t& p_index ) const
    {
        if (p_index >= m_size)
            throw exception::runtime(_("index out of range"), *this);
    
        return  m_value[p_index] != 0 ? 1 : 0;
    }
   
    
    /** mutates the object **/
    template<typename T> inline void binaryindividual<T>::mutate( void )
    {
        const std::size_t l_pos = static_cast<std::size_t>(m_rand.get<double>(tools::random::uniform, 0, m_size));
        m_value[l_pos] = m_value[l_pos] == 0 ? 1 : 0;
    }
    
    
    /** clones the object / create a new object on the heap
     * @param p_ptr return reference of the new smart-pointer object
     **/
    template<typename T> inline void binaryindividual<T>::clone( boost::shared_ptr< individual<T> >& p_ptr) const
    {
        p_ptr = boost::shared_ptr< individual<T> >( new binaryindividual<T>(m_size) );
    }
    
    
    /** returns the number of bits that are used
     * @return length
     **/
    template<typename T> inline std::size_t binaryindividual<T>::size( void ) const
    {
        return m_size;
    }
    
    
}}}
#endif

