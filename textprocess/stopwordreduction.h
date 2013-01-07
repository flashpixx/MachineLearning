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



#ifndef __MACHINELEARNING_TEXTPROCESS_STOPWORDREDUCTION_H
#define __MACHINELEARNING_TEXTPROCESS_STOPWORDREDUCTION_H

#include <string>
#include <sstream>
#include <iostream>
#include <boost/regex.hpp> 
#include <boost/xpressive/xpressive.hpp>

#include "../errorhandling/exception.hpp"



namespace machinelearning { namespace textprocess {
    
    namespace xps = boost::xpressive;
    
    
    /** class for stop-word-reduction
     * @todo try to optimize with http://en.wikipedia.org/wiki/Ukkonen%27s_algorithm / http://en.wikipedia.org/wiki/Radix_tree
     **/
    class stopwordreduction
    {
        
        public:
        
            stopwordreduction( const std::vector<std::string>&, const bool& = true );
        
            std::string remove( const std::string& ) const;
            bool iscaseinsensitivity( void ) const;
        
        
        private:
        
            /** expression stop word - use xpressive interface for big expressions
             * because the default interface does not work with the expression 
             **/
            xps::sregex m_stopwordsexpr;
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
            throw exception::runtime(_("stopwordlist can not be empty"), *this);
        
        // we create a regular expression with perl syntax (default) for masking chars within the words,
        // that are also used for regular expressions
        const boost::regex l_mask("\\.|\\[|\\]|\\{|\\}|\\(|\\)|\\\\|\\*|\\+|\\?|\\||\\^|\\$|<|>");
        const std::string l_replace("\\\\$&");
        
        
        // create regular expression part with words
        std::string l_stopwordsexpr;
        for(std::size_t i=0; i < p_list.size()-1; ++i)
            l_stopwordsexpr += boost::regex_replace(p_list[i], l_mask, l_replace)+"|";
        l_stopwordsexpr += boost::regex_replace(p_list[p_list.size()-1], l_mask, l_replace);
        
        if (l_stopwordsexpr.empty())
            throw exception::runtime(_("stopwordlist can not be empty"), *this);
        
        
        // create regular expression case-sensitive / case-insensitive with perl syntax (default)
        l_stopwordsexpr = "\\<(?:" + l_stopwordsexpr + ")\\>";
        if (p_caseinsensitive)
            m_stopwordsexpr = xps::sregex::compile( l_stopwordsexpr, xps::regex_constants::icase );
        else
            m_stopwordsexpr = xps::sregex::compile( l_stopwordsexpr );
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
        return xps::regex_replace(p_text, m_stopwordsexpr, ""); //, boost::match_default | boost::format_all);
    }

}}
#endif
