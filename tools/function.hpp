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


#ifndef __MACHINELEARNING_TOOLS_FUNCTION_HPP
#define __MACHINELEARNING_TOOLS_FUNCTION_HPP

#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>
#include <iterator>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filtering_stream.hpp>


#include "iostreams/iostreams.h"


namespace machinelearning { namespace tools { 
    
    namespace ublas         = boost::numeric::ublas;
    namespace bio           = boost::iostreams; 
    
    
    /** class for functions **/
    class function
    {
        
        public :

            template<typename T> static bool isNumericalEqual( const T&, const T& );
            template<typename T> static bool isNumericalZero( const T& );
            static ublas::indirect_array<> unique( const ublas::indirect_array<>& );
            static bool fileExists( const std::string& );
            static std::string urlencode( const std::string& );
            template<typename T> static std::string toString( const T& );
    };

    
    
    /** check equality of floating point values 
     * @param p_val1 first value
     * @param p_val2 second value
     * @return equality
    **/
    template<typename T> inline bool function::isNumericalEqual( const T& p_val1, const T& p_val2 )
    {
        return !( std::fabs(p_val1-p_val2) > std::numeric_limits<T>::epsilon() );
    }
    
    
    /** check zero of floating point values
     * @param p_val value
     * @return equality to zero
    **/
    template<typename T> inline bool function::isNumericalZero( const T& p_val )
    {
        return std::fabs(p_val) <= std::numeric_limits<T>::epsilon();
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
        std::size_t* l_ptr = std::unique( &l_ar(0), &l_ar(l_ar.size()-1) );
        
        ublas::indirect_array<> l_unique( std::distance(&l_ar(0), l_ptr) );
        std::copy( &l_ar(0), l_ptr, &l_unique(0) );
        
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
    
    
    /** encode a string into the URL characters and returns a string
     * @param p_in input string
     * @return encoded string
     **/
    inline std::string function::urlencode( const std::string& p_in )
    {
        // create a a input stream stream and disable skipping whitespaces
        std::istringstream l_instream( p_in, std::stringstream::binary);
        l_instream >> std::noskipws;
        
        // create a output stream
        std::ostringstream l_outstream( std::stringstream::binary );
        
        // create filter chain
        bio::filtering_streambuf< bio::output > l_chain;
        l_chain.push( iostreams::urlencoder( std::numeric_limits<unsigned char>::max() ) );
        l_chain.push( l_outstream );
        
        // copy data (with std::copy the stream must be closed)
        //std::copy( std::istream_iterator<char>(l_instream), std::istream_iterator<char>(), std::ostreambuf_iterator<char>(&l_chain) );
        bio::copy( l_instream, l_chain );
        
        return l_outstream.str();
    }
    
    
    /** converts a value to a string
     * @param p_val value
     * @return string
     **/
    template<typename T> inline std::string function::toString( const T& p_val )
    {
        std::ostringstream l_stream;
        l_stream << p_val;
        return l_stream.str();
    }
    
}}
#endif
