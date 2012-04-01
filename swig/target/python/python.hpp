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

#ifndef __MACHINELEARNING_SWIG_PYTHON_HPP
#define __MACHINELEARNING_SWIG_PYTHON_HPP

extern "C" {
#include <Python.h>
}
    
#include "machinelearning/machinelearning.h"


namespace machinelearning { namespace swig {
    
    namespace ublas = boost::numeric::ublas;
    
    
    /** class for generate fragment code, that is used to convert
     * numericial structurs of Python to C++ UBlas (both ways), it is called by
     * the SWIG fragment calls
     * $LastChangedDate$
     * @todo catching exception and pipe them to Java (use Swig calls)
     * @todo add copy process with OpenMP
     **/
    class python {
        
        public :
        
            /** row type **/
            enum rowtype
            {
                row     = 0,
                column  = 1
            };

    }

}}
#endif