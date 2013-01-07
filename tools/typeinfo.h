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



#ifndef __MACHINELEARNING_TOOLS_TYPEINFO_H
#define __MACHINELEARNING_TOOLS_TYPEINFO_H

#include <string>
#include <typeinfo>

#ifdef __GNUC__
#include <cxxabi.h>
#endif

namespace machinelearning { namespace tools {
    
    
    /** class that creates a typeinfo interface **/
    class typeinfo
    {

        public :
        
            template <typename T> static std::string getClassName( const T* );
            template <typename T> static std::string getClassName( const T& );
    };

    
    /** returns the class name of a pointer
     * @param p_ptr pointer
     * @return string with class name and namespaces
     **/
    template <typename T> inline std::string typeinfo::getClassName( const T* p_ptr )
    {
        try {

            #ifdef __GNUC__
            return std::string(abi::__cxa_demangle( typeid(*p_ptr).name(), NULL, 0, NULL ));
            #endif
        
        
        } catch (...) {}

        return std::string();
    }
    
    
    /** returns the class name of a reference
     * @param p_obj reference
     * @return string with class name and namespaces
     **/
    template <typename T> inline std::string typeinfo::getClassName( const T& p_obj )
    {
        try {
            
            #ifdef __GNUC__
            return std::string(abi::__cxa_demangle( typeid(p_obj).name(), NULL, 0, NULL ));
            #endif
            
            
        } catch (...) {}
        
        return std::string();
    }
    
    
}}
#endif
