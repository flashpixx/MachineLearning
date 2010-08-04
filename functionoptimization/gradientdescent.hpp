/** 
 @cond
 #########################################################################
 # GPL License                                                           #
 #                                                                       #
 # This file is part of the Machine Learning Framework.                  #
 # Copyright (c) 2010, Philipp Kraus, <philipp.kraus@flashpixx.de>       #
 # This program is free software: you can redistribute it and/or modify  #
 # it under the terms of the GNU General Public License as published by  #
 # the Free Software Foundation, either version 3 of the License, or     #
 # (at your option) any later version.                                   #
 #                                                                       #
 # This program is distributed in the hope that it will be useful,       #
 # but WITHOUT ANY WARRANTY; without even the implied warranty of        #
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
 # GNU General Public License for more details.                          #
 #                                                                       #
 # You should have received a copy of the GNU General Public License     #
 # along with this program.  If not, see <http://www.gnu.org/licenses/>. #
 #########################################################################
 @endcond
 **/

#ifndef MACHINELEARNING_FUNCTIONALOPTIMIZATION_GRADIENTDESCENT_HPP
#define MACHINELEARNING_FUNCTIONALOPTIMIZATION_GRADIENTDESCENT_HPP

#include <string>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <ginac/ginac.h>


#include "../exception/exception.h"
#include "../tools/tools.h"



namespace machinelearning { namespace functionaloptimization {
    
    namespace ublas = boost::numeric::ublas;
    
    
    /** class for using a (stochastic) gradient descent **/
    template<typename T> class gradientdescent {
        
        public :
        
            gradientdescent( const std::string& );
        
        
        
    };

    
    
    template<typename T> inline gradientdescent<T>::gradientdescent( const std::string& p_func )
    {
        
        /*
         symbol x;
         const ex generator = pow(cosh(x),-1);
         return generator.diff(x,n).subs(x==0);

         */
    }
    
    
};};

#endif
