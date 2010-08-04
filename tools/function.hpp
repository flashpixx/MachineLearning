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


#ifndef MACHINELEARNING_TOOLS_FUNCTION_HPP
#define MACHINELEARNING_TOOLS_FUNCTION_HPP

#include <cmath>
#include <limits>


namespace machinelearning { namespace tools { 
    
    
    /** class for functions **/
    class function {
        
        public :

            template<typename T> static bool isNumericalEqual( const T& p1, const T& p2 );
            template<typename T> static bool isNumericalZero( const T& p );
    
    };

    
    
	/** check equality of floating point values 
     * @param p1 first value
     * @param p2 second value
     * @return equality
    **/
    template<typename T> inline bool function::isNumericalEqual( const T& p1, const T& p2 )
    {
		return !( std::fabs(p1-p2) > std::numeric_limits<T>::epsilon() );
	}
	
	/** check zero of floating point values
     * @param p value
     * @return equality to zero
    **/
    template<typename T> inline bool function::isNumericalZero( const T& p )
    {
		return std::fabs(p) <= std::numeric_limits<T>::epsilon();
	}
    
    
};};


#endif
