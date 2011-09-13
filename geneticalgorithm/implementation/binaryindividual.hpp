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

#include <boost/shared_ptr.hpp>

#include "../individual.hpp"
#include "../../tools/tools.h"
#include "../../exception/exception.h"


namespace machinelearning { namespace geneticalgorithm {
    
    /** class of a binary indivdual
     * $LastChangedDate: 2011-09-10 20:05:39 +0200 (Sa, 10 Sep 2011) $
     **/
    template<typename T> class binaryindividual : public individual<T> {
        
        public :
        
            binaryindividual( const std::size_t& );
    
            void clone( boost::shared_ptr<binaryindividual>& ) const;
            void mutate( void );
        
        
        private :
        
            /** number of bit position are used **/
            const std::size_t m_size;
            /** value of the object **/
            T m_value;
    };

    
    
    template<typename T> inline binaryindividual<T>::binaryindividual( const std::size_t& p_size ) :
        m_size( sizeof(std::size_t)*8 ),
        m_value(0) // static_cast<std::size_t>(m_rand.get<double>(tools::random::uniform, 0, m_size))
    {
        if (p_size == 0)
            throw exception::runtime(_("size number need not to be zero"));
        if (p_size > m_size)
            throw exception::runtime(_("size number is lager than datatype"));
    }
    
    
    
    template<typename T> inline void binaryindividual<T>::mutate( void )
    {
        tools::random l_rand;
        const std::size_t l_mask( 0x01 << static_cast<std::size_t>(l_rand.get<double>(tools::random::uniform, 0, m_size)) );
        m_value = (!(m_value & l_mask)) | (!l_mask);
    }
    
    
    template<typename T> inline void binaryindividual<T>::clone( boost::shared_ptr< binaryindividual<T> >& p_ptr) const
    {
        p_ptr = boost::shared_ptr< binaryindividual<T> >( new binaryindividual<T>(m_size) );
    }
    
};};

#endif

