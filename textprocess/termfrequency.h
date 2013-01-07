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



#ifndef __MACHINELEARNING_TEXTPROCESS_TERMFREQUENCY_H
#define __MACHINELEARNING_TEXTPROCESS_TERMFREQUENCY_H

#include <map>
#include <string>
#include <boost/algorithm/string.hpp> 
#include <boost/algorithm/string/erase.hpp>

#include "../errorhandling/exception.hpp"



namespace machinelearning { namespace textprocess {

    
    /** class for term frequency. The class analyses texts and create a map with word counts,
     * that can be used for stop-word-reduction
     **/
    class termfrequency
    {
        
        public:
        
            enum comparison {
                lessequal       = 0,
                greaterequal    = 1,
                less            = 2,
                greater         = 3
            };
        
        
            termfrequency( const std::string& = ",;.:!?- \n\t|=", const std::string& = "#_()[]{}%$*/\\\"=|<>\r", const bool& = true );
            void add( const std::string&, const std::size_t& = 0 );
            void add( const std::vector<std::string>&, const std::size_t& = 2 );
            bool iscaseinsensitivity( void ) const;
            std::size_t getTermCount( void ) const;
            std::string getTermSeparator( void ) const;
            std::vector<std::string> getTerms( const float&, const float&, const comparison& = lessequal, const comparison& = greaterequal );
            std::vector<std::string> getTerms( const float&, const comparison& = lessequal );
            void clear( void );
            void erase( const std::string& );
            std::map<std::string, std::size_t> getMap( void ) const;
        
        
        private:
            
            /** seperators **/
            const std::string m_seperators;   
            /** chars that will be removed **/
            const std::string m_remove;
            /** bool for case-sensitive / case-insensitive wordlist **/
            const bool m_caseinsensitive;
            /** map with words and their counts **/
            std::map<std::string, std::size_t> m_map;
            /** sum over all words **/
            std::size_t m_wordcount;
        
            bool compare( const float&, const float&, const comparison& ) const;
    };
    
    
    
    /** constructor
     * @param p_separator characters for seperate words within the text
     * @param p_remove string with characters that will be removed
     * @param p_caseinsensitive words should be case-insensitive
     **/
    inline termfrequency::termfrequency( const std::string& p_separator, const std::string& p_remove, const bool& p_caseinsensitive ) :
        m_seperators( p_separator ),
        m_remove( p_remove ),
        m_caseinsensitive( p_caseinsensitive ),
        m_map(),
        m_wordcount( 0 )
    {
        if (m_seperators.empty())
            throw exception::runtime(_("separator can not be empty"), *this);
    }
    
    
    /** returns the value for case-sensitive words
     * @return bool for case-insensitive
     **/
    inline bool termfrequency::iscaseinsensitivity( void ) const
    {
        return m_caseinsensitive;
    }
    
    
    /** returns the number of words
     * @return number of words
     **/
    inline std::size_t termfrequency::getTermCount( void ) const
    {
        return m_wordcount;
    }
    
    
    /** returns a string with character that are used for seperating the words
     * @return string with separators
     **/
    inline std::string termfrequency::getTermSeparator( void ) const
    {
        return m_seperators;
    }

    
    /** returns the internal map. Each key is the word and the value the number of counts
     * @return word map
     **/
    inline std::map<std::string, std::size_t> termfrequency::getMap( void ) const
    {
        return m_map;
    }
    
    
    /** adds a string vector to the map
     * @param p_vec string vector with data
     * @param p_minlen only word equal or greater will be added
     **/
    inline void termfrequency::add( const std::vector<std::string>& p_vec, const std::size_t& p_minlen )
    {
        for(std::size_t i=0; i < p_vec.size(); ++i)
            add( p_vec[i], p_minlen );
    }
    
    
    /** adds textdata to the term frequency
     * @param p_text input text
     * @param p_minlen only word equal or greater will be added
     **/
    inline void termfrequency::add( const std::string& p_text, const std::size_t& p_minlen )
    {
        std::vector<std::string> l_data;
        boost::split( l_data, p_text, boost::is_any_of(m_seperators) );
        
        #pragma omp parallel for
        for(std::size_t i=0; i < l_data.size(); ++i) {
            std::string l_item = l_data[i];
            if (!m_remove.empty())
                boost::erase_all(l_item, m_remove);
            boost::trim(l_item);
            
            if ( (l_item.length() < p_minlen) || (l_item.empty()) )
                continue;
                
            if (m_caseinsensitive)
                boost::to_lower(l_item);
            
            std::map<std::string, std::size_t>::iterator it = m_map.find(l_item);
            #pragma omp critical
            {
                m_wordcount++;
                if (it == m_map.end())
                    m_map[l_item] = 1;
                else
                    it->second++;
            }
        }
    }
    
    
    /** returns a list of words, that between the ranges
     * @param p_val1 first value in range [0,1]
     * @param p_val2 second value in range [0,1]
     * @param p_comp1 comparasion operator of the first value
     * @param p_comp2 comparasion operator of the second value
     **/
    inline std::vector<std::string> termfrequency::getTerms( const float& p_val1, const float& p_val2, const comparison& p_comp1, const comparison& p_comp2 )
    {
        if ( (p_val1 < 0) || (p_val1 > 1) || (p_val2 < 0) || (p_val2 > 1) )
            throw exception::runtime(_("ranges must be between [0,1]"), *this);
        if (m_wordcount == 0)
            throw exception::runtime(_("no words within the map"), *this);
        
        
        std::vector<std::string> l_list;
        
        for(std::map<std::string, std::size_t>::iterator it = m_map.begin(); it != m_map.end(); it++) {
            const float l_val = static_cast<float>(it->second) / m_wordcount;
            
            if ( (compare(l_val, p_val1, p_comp1)) || (compare(l_val, p_val2, p_comp2)) )
                  l_list.push_back( it->first );
        }
        
        return l_list;
    }
    
    
    /** returns a list of words, that in the range
     * @param p_val value in range [0,1]
     * @param p_comp comparasion operator of the value
     **/
    inline std::vector<std::string> termfrequency::getTerms( const float& p_val, const comparison& p_comp )
    {
        if ( (p_val < 0) || (p_val > 1) )
            throw exception::runtime(_("ranges must be between [0,1]"), *this);
        if (m_wordcount == 0)
            throw exception::runtime(_("no words within the map"), *this);
        
        std::vector<std::string> l_list;
        
        for(std::map<std::string, std::size_t>::iterator it = m_map.begin(); it != m_map.end(); it++) {
            const float l_val = static_cast<float>(it->second) / m_wordcount;
            
            if (compare(l_val, p_val, p_comp))
                l_list.push_back( it->first );
        }
        
        return l_list;
    }
    
    
    /** compares two values with the operator
     * @param p_val1 value
     * @param p_val2 value
     * @param p_comp comparasion
     * @return bool
     **/
    inline bool termfrequency::compare( const float& p_val1, const float& p_val2, const comparison& p_comp ) const
    {
        bool l_comp = false;
        
        switch (p_comp) {
            case lessequal      : l_comp = p_val1 <= p_val2; break;
            case greaterequal   : l_comp = p_val1 >= p_val2; break;
            case less           : l_comp = p_val1 < p_val2;  break;
            case greater        : l_comp = p_val1 > p_val2;  break;
        }
        
        return l_comp;
    }

    
    /** removes all elements within the map **/
    inline void termfrequency::clear( void ) 
    {
        m_map.clear();
        m_wordcount = 0;
    }
    
    
    /** remove a word on the map
     * @param p_word word
     **/
    inline void termfrequency::erase( const std::string& p_word )
    {
        std::string l_word = p_word;
        if (m_caseinsensitive)
            boost::to_lower(l_word);
        
        std::map<std::string, std::size_t>::iterator it = m_map.find(l_word);
        if (it != m_map.end()) {
            m_wordcount -= it->second;
            m_map.erase( it );
        }
    }
    
}}
#endif
