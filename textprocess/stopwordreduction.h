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



#ifndef MACHINELEARNING_TEXTPROCESS_STOPWORDREDUCTION_H
#define MACHINELEARNING_TEXTPROCESS_STOPWORDREDUCTION_H

#include <string>
#include <sstream>
#include <iostream>
#include <boost/regex.hpp> 

#include "../exception/exception.h"



namespace machinelearning { namespace textprocess {
    
    
    /** class for stop-word-reduction **/
    class stopwordreduction {
        
        public:
        
            stopwordreduction( const std::vector<std::string>&, const bool& = true );
        
            std::string remove( const std::string& ) const;
            bool iscaseinsensitivity( void ) const;
        
        
        private:
        
            /** vector with stop word **/
            boost::regex m_stopwordsexpr;
            /** bool for case-sensitive / case-insensitive wordlist **/
            const bool m_caseinsensitive;
        
    };
    
    
    
    /** constructor
     * @param p_list list with stopwords
     * @param p_caseinsensitive words should be case-insensitive
     **/
    inline stopwordreduction::stopwordreduction( const std::vector<std::string>& p_list, const bool& p_caseinsensitive ) :
        m_stopwordsexpr(),
        m_caseinsensitive( p_caseinsensitive )
    {
        if (p_list.size() == 0)
            throw exception::runtime(_("stopwordlist can not be empty"));
        
        // create regular expression  for removing
        std::string l_stopwordsexpr = "\\<(?:";
        for(std::size_t i=0; i < p_list.size()-1; ++i)
            l_stopwordsexpr += p_list[i]+"|";
        l_stopwordsexpr += p_list[p_list.size()-1]+")\\>";
        
        m_stopwordsexpr.assign( l_stopwordsexpr );
        
        std::cout << l_stopwordsexpr << std::endl << std::endl;
    }
    
    
    
    /** returns the value for case-sensitive words
     * @return bool for case-insensitive
     **/
    inline bool stopwordreduction::iscaseinsensitivity( void ) const
    {
        return m_caseinsensitive;
    }
    
        
    /** removes the stopword on the text
     * @param p_text input text
     * @return text with removed words
     **/
    inline std::string stopwordreduction::remove( const std::string& p_text ) const
    {
        std::stringstream l_out(std::ios::out | std::ios::binary);
        std::ostream_iterator<char, char> it(l_out);

        boost::regex_replace(it, p_text.begin(), p_text.end(), m_stopwordsexpr, "", boost::match_default | boost::format_all);
        
        return l_out.str();
    }

};};

#endif