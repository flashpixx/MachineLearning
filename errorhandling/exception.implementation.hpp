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

#ifndef __MACHINELEARNING_ERRORHANDLING_EXCEPTION_HPP
#error never include this file directly
#elif !defined __MACHINELEARNING_ERRORHANDLING_EXCEPTION_IMPLEMENTATION_HPP
#define __MACHINELEARNING_ERRORHANDLING_EXCEPTION_IMPLEMENTATION_HPP

#include <string>
#include <stdexcept>

#include "../tools/language/language.h"
#include "../tools/typeinfo.h"
#include "../tools/logger.hpp"



namespace machinelearning { namespace exception {
    
    /** creates the exception with a message
     * @param p_msg error message
     **/
    inline runtime::runtime( const std::string& p_msg ) :
        std::runtime_error( p_msg )
    {
        #ifdef MACHINELEARNING_LOGGER
        if (tools::logger::exists())
            tools::logger::getInstance()->write( tools::logger::exception, _("runtime exception is thrown: ") + p_msg);
        #endif
    }  


    /** creates the exception with a message and a class name
     * @param p_msg error message
     * @param p_ptr pointer to an class object
     **/
    template <typename T> inline runtime::runtime( const std::string& p_msg, const T* p_ptr ) :
        std::runtime_error( p_msg + ( !tools::typeinfo::getClassName(p_ptr).empty() ? " ["+tools::typeinfo::getClassName(p_ptr)+"]" : "") )
    {
        #ifdef MACHINELEARNING_LOGGER
        if (tools::logger::exists())
            tools::logger::getInstance()->write( tools::logger::exception, _("runtime exception is thrown: ") + p_msg + ( !tools::typeinfo::getClassName(p_ptr).empty() ? " ["+tools::typeinfo::getClassName(p_ptr)+"]" : ""));
        #endif
    }  


    /** creates the exception with a message and a class name
     * @param p_msg error message
     * @param p_obj reference to a classobject
     **/
    template <typename T> inline runtime::runtime( const std::string& p_msg, const T& p_obj ) :
        std::runtime_error( p_msg + ( !tools::typeinfo::getClassName(p_obj).empty() ? " ["+tools::typeinfo::getClassName(p_obj)+"]" : "") )
    {
        #ifdef MACHINELEARNING_LOGGER
        if (tools::logger::exists())
            tools::logger::getInstance()->write( tools::logger::exception, _("runtime exception is thrown: ") + p_msg + ( !tools::typeinfo::getClassName(p_obj).empty() ? " ["+tools::typeinfo::getClassName(p_obj)+"]" : ""));
        #endif
    }  
  
    
}}

#endif
