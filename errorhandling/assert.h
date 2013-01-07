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

/** preprocessor command for framework asserts, which can be added to the logger
 * if the logger exists
 **/

#ifndef __MACHINELEARNING_ERRORHANDLING_ASSERT_H
#define __MACHINELEARNING_ERRORHANDLING_ASSERT_H

#include <cstdlib>
#include <sstream>
#include <iostream>

#include "../tools/logger.hpp"



#ifdef MACHINELEARNING_NDEBUG

    #define MACHINELEARNING_ASSERT( p_expression, p_message )

#else

    #ifdef MACHINELEARNING_LOGGER 

        #define MACHINELEARNING_ASSERT( p_expression, p_message ) \
            { \
                if (!p_expression) { \
                    if (machinelearning::tools::logger::exists()) {   \
                        std::stringstream l_msg; \
                        l_msg << p_message << ", " << __FILE__ << ", line " << __LINE__; \
                        machinelearning::tools::logger::getInstance()->write( machinelearning::tools::logger::assert, l_msg.str() ); \
                    } \
                    std::cerr << "Machinlearning Assertion failed: " << p_message << ", " << __FILE__ << ", line " << __LINE__ << std::endl; \
                    exit(EXIT_FAILURE); \
                } \
            }

    #else
    
        #define MACHINELEARNING_ASSERT( p_expression, p_message ) \
            { \
                if (!p_expression) { \
                    std::cerr << "Machinlearning Assertion failed: " << p_message << ", " << __FILE__ << ", line " << __LINE__ << std::endl; \
                    exit(EXIT_FAILURE); \
                } \
            }

    #endif
        
#endif



#endif
