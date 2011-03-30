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


#ifndef MACHINELEARNING_TOOLS_FUNCTION_HPP
#define MACHINELEARNING_TOOLS_FUNCTION_HPP

#include <fstream>
#include <cmath>
#include <limits>
#include <iterator>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>


namespace machinelearning { namespace tools { 
    
    namespace ublas         = boost::numeric::ublas;
    
    
    /** class for functions
     * $LastChangedDate$
     **/
    class function {
        
        public :

            template<typename T> static bool isNumericalEqual( const T& p1, const T& p2 );
            template<typename T> static bool isNumericalZero( const T& p );
            static ublas::indirect_array<> unique( const ublas::indirect_array<>& );
            static bool fileExists( const std::string& );
    
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
    
    
    /** returns a ublas::indirect array that elements are unique
     * @param p_ar input indirect array
     * @return unique indirect array
     **/
    inline ublas::indirect_array<> function::unique( const ublas::indirect_array<>& p_ar )
    {
        ublas::indirect_array<> l_ar(p_ar);
         
        // we can't use l_ar.begin() on the third argument, because there is e design error within
        // the indirect_array: begin() returns a const reference iterator
        // @see http://answerpot.com/showthread.php?726979-submatrix+with+indexvector
        std::sort( &l_ar(0), &l_ar(l_ar.size()-1) );
        std::size_t* it = std::unique( &l_ar(0), &l_ar(l_ar.size()-1) );
        
        ublas::indirect_array<> l_unique( std::distance(&l_ar(0), it) );
        std::copy( &l_ar(0), it, &l_unique(0) );
        
        return l_unique;
    }
    
    
    /** check if file exists
     * @param p_filename filename
     * @return bool for existing
     **/
    inline bool function::fileExists( const std::string& p_filename )
    {
        std::ifstream l_file(p_filename.c_str());
        return l_file;
    }
    
    
    
};};


#endif
