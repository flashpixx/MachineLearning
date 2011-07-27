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

#ifndef MACHINELEARNING_TOOLS_IOSTREAMS_URLENCODER_H
#define MACHINELEARNING_TOOLS_IOSTREAMS_URLENCODER_H

#include <locale>
#include <sstream>
#include <string>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/operations.hpp>

namespace machinelearning { namespace tools { namespace iostreams {

    /** filter class for creating a stream with "percent encoding"
     * @see http://en.wikipedia.org/wiki/Percent-encoding
     **/
    class urlencoder : public boost::iostreams::output_filter
    {
    
        public :
        
            template<typename sink> bool put(sink&, int);
        
        private :
            std::string toHex( const int& ) const;
    };
    
    
    /** create a hexdecimal value of a input char
     * @param p_char input value
     * @return string with char value
     **/
    inline std::string urlencoder::toHex( const int& p_char ) const
    {
        std::stringstream l_stream;
        l_stream << std::setfill('0') << std::setw(2) << std::hex << p_char;
        return l_stream.str();
    }
    
    
    /** create representation
     * @param p_dest destination stream
     * @param p_char input character
     * @return bool for writing
     **/
    template<typename sink> inline bool urlencoder::put(sink& p_dest, int p_char)
    {
        // unreserved characters
        if ((isalnum(p_char)) || (p_char == '_') || (p_char == '~') || (p_char == '.') || (p_char == '-'))
            return boost::iostreams::put(p_dest, p_char);
        
        // reserved characters
        bool l_res = boost::iostreams::put(p_dest, '%');
        
        // change char to hex number and add the values to the stream
        const std::string l_hex = toHex( p_char );
        for(std::size_t i=0; (i < l_hex.size()) && l_res; ++i)
            l_res = l_res && boost::iostreams::put(p_dest, l_hex[i]);
                
        return l_res;
    }

};};};


#endif