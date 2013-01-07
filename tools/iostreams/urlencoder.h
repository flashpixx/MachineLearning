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

#ifndef __MACHINELEARNING_TOOLS_IOSTREAMS_URLENCODER_H
#define __MACHINELEARNING_TOOLS_IOSTREAMS_URLENCODER_H

#include <locale>
#include <sstream>
#include <string>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/operations.hpp>

namespace machinelearning { namespace tools { namespace iostreams {

    /** filter class for creating a stream with "percent encoding"
     * @see http://en.wikipedia.org/wiki/Percent-encoding
     * @todo test with different encodings of the input stream and |values| > 255
     **/
    class urlencoder : public boost::iostreams::output_filter
    {
    
        public :
        
            urlencoder( const unsigned int& );
            template<typename S> bool put(S&, int);
        
        private :
        
            /** maximal unsigned value for the input character **/
            const unsigned int m_maxinputsize;
            /** number of prefill values **/
            unsigned int m_prefill;
        
            std::string toHex( const unsigned int& ) const;
            unsigned int prefill( const unsigned int& ) const;
        
    };
    
    
    
    
    /** constructor
     * @param p_max maximum value for hex encoding
    **/
    inline urlencoder::urlencoder( const unsigned int& p_max ) :
        m_maxinputsize( p_max ),
        m_prefill( prefill(p_max) )
    {}

    
    /** determine prefill number of hex codes
     * @param p_max maximal number
     * @return number of prefixes
     **/
    inline unsigned int urlencoder::prefill( const unsigned int& p_max ) const
    {
        unsigned int l_prefill = 0;
        
        for(unsigned int l_max = p_max; l_max != 0; ++l_prefill)
            l_max >>= 4;
        
        return l_prefill;
    }
    
    
    /** create a hexdecimal value of a input char
     * @param p_char input value
     * @return string with char value
     **/
    inline std::string urlencoder::toHex( const unsigned int& p_char ) const
    {
        std::stringstream l_stream;
        l_stream << std::setfill('0') << std::setw(m_prefill) << std::hex << p_char;
        return l_stream.str();
    }
    
    
    /** create representation
     * @param p_dest destination stream
     * @param p_char input character
     * @return bool for writing
     **/
    template<typename S> inline bool urlencoder::put(S& p_dest, int p_char)
    {
        // unreserved characters
        if ((isalnum(p_char)) || (p_char == '_') || (p_char == '~') || (p_char == '.') || (p_char == '-'))
            return boost::iostreams::put(p_dest, p_char);
        
        // reserved characters
        bool l_res = boost::iostreams::put(p_dest, '%');
        
        // for the correct hex encoding, we need a uint value, the char value are in the range
        // -maxvalue/2 till maxvalue/2 and we change it to 0 to maxvalue-1
        const std::string l_hex= toHex( static_cast<unsigned int>(p_char < 0 ? m_maxinputsize + p_char + 1 : p_char) % m_maxinputsize );
        
        // change char to hex number and add the values to the stream
        for(std::size_t i=0; (i < l_hex.size()) && l_res; ++i)
            l_res = l_res && boost::iostreams::put(p_dest, l_hex[i]);
                
        return l_res;
    }

}}}
#endif
