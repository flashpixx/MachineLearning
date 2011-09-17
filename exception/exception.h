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

#ifndef __MACHINELEARNING_EXCEPTION_H
#define __MACHINELEARNING_EXCEPTION_H

#include <string>
#include <stdexcept>

#include "../tools/typeinfo.h"


namespace machinelearning { 
    
    /** namespace for all exceptions of the framework
     * $LastChangedDate$
     **/
    namespace exception {
    
        /** exception class for throwing on not implementated method 
         * $LastChangedDate$
         **/
        class classmethod : public std::logic_error
        { 
            public : 
            
                explicit classmethod( const std::string& p_msg) :
                    std::logic_error( p_msg )
                {}
            
                template <typename T> explicit classmethod( const std::string& p_msg, const T* p_ptr ) :
                    std::logic_error( p_msg + ( !tools::typeinfo::getClassName(p_ptr).empty() ? " ["+tools::typeinfo::getClassName(p_ptr)+"]" : "") )
                {}  
            
                template <typename T> explicit classmethod( const std::string& p_msg, const T& p_obj ) :
                    std::logic_error( p_msg + ( !tools::typeinfo::getClassName(p_obj).empty() ? " ["+tools::typeinfo::getClassName(p_obj)+"]" : "") )
                {}  
        };
        
        
        
        /** exception class for throwing on runtime errors
         * $LastChangedDate$
         **/
        class runtime : public std::runtime_error
        { 
            public : 
                
                explicit runtime( const std::string& p_msg ) :
                    std::runtime_error( p_msg )
                {}  
            
                template <typename T> explicit runtime( const std::string& p_msg, const T* p_ptr ) :
                    std::runtime_error( p_msg + ( !tools::typeinfo::getClassName(p_ptr).empty() ? " ["+tools::typeinfo::getClassName(p_ptr)+"]" : "") )
                {}  
            
                template <typename T> explicit runtime( const std::string& p_msg, const T& p_obj ) :
                    std::runtime_error( p_msg + ( !tools::typeinfo::getClassName(p_obj).empty() ? " ["+tools::typeinfo::getClassName(p_obj)+"]" : "") )
                {}  
        };
             
        
    };
};

#endif
