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

#ifndef MACHINELEARNING_EXCEPTION_H
#define MACHINELEARNING_EXCEPTION_H

#include <string>
#include <stdexcept>


namespace machinelearning { 
    
    /** namespace for all exceptions of the framework
     * $LastChangedDate$
     **/
    namespace exception {
    
        /** exception class for throwing on not implementated method 
         * $LastChangedDate$
         **/
        class classmethod      : public std::runtime_error{ public : classmethod( const std::string& ); };
        
        /** exception class for throwing on runtime errors
         * $LastChangedDate$
         **/
        class runtime          : public std::runtime_error      { public : runtime( const std::string& ); };
        
        
        
        
        /** constructor
         * @param p_str exception message
        **/
        inline classmethod::classmethod( const std::string& p_str ) : std::runtime_error(p_str) {};
        
        /** constructor
         * @param p_str exception message
         **/
        inline runtime::runtime( const std::string& p_str ) : std::runtime_error(p_str) {};
    
    };
};

#endif
