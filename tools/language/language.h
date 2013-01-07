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

#ifndef __MACHINELEARNING_TOOLS_LANGUAGE_LANGUAGE_H
#define __MACHINELEARNING_TOOLS_LANGUAGE_LANGUAGE_H

#include <vector>
#include <boost/algorithm/string.hpp> 

#include "../../errorhandling/exception.hpp"
#include "bindings.h"


namespace machinelearning { namespace tools { 
    
    
    /** namespace with language structurs **/
    namespace language {
    
        /** enum with language codes on ISO 639-1 and ISO 639-3 **/
        enum code
        {
            #define LANGUAGE_CODE( iso6391, iso6393, description )    iso6391, iso6393 = iso6391,
            #include "iso639.h"
            #undef LANGUAGE_CODE
        };
        
        /** enum for creating the code list / returing type **/
        enum listcode {
            all,
            iso639_1,
            iso639_3
        };
        
        
   
        /** convert a string into the language code 
         * @param p_lang string value with language code
         * @return enum language code (upper-case)
         **/
        inline code fromString( const std::string& p_lang ) {
            if (p_lang.empty() || (p_lang.size() < 2) || (p_lang.size() > 3))
                throw exception::runtime(_("parameter is not a language code"));
            
            std::string l_lang = p_lang;
            boost::to_upper(l_lang);
            
            #define LANGUAGE_CODE( iso6391, iso6393, description )      if ((l_lang == #iso6391) || (l_lang == #iso6393)) return iso6391;
            #include "iso639.h"
            #undef LANGUAGE_CODE
            
            throw exception::runtime(_("language is not known"));
        }
        
        
        /** converts a enum type to the string representation
         * @param p_lang language code
         * @param p_return enum option for returning iso639-1 or iso639-3 representation
         * @return string
         **/
        inline std::string toString( const code& p_lang, const listcode& p_return = iso639_1 ) {
            
            switch (p_lang) {
                    
                #define LANGUAGE_CODE( iso6391, iso6393, description )      case iso6391 : return (p_return == iso639_3) ? #iso6393 : #iso6391;
                #include "iso639.h"
                #undef LANGUAGE_CODE
                    
                default :
                    throw exception::runtime(_("language is not known"));
            }
        }
        
        
        /** return a list with all language codes
         * @param p_list option for creating code list
         * @param p_lower set all return values to lower-case
         * @return vector with language codes
         **/
        inline std::vector<std::string> getCodeList( const listcode& p_list = all, const bool& p_lower = false ) {
            std::vector<std::string> l_list;
            
            #define LANGUAGE_CODE( iso6391, iso6393, description )      if ((p_list == all) || (p_list == iso639_1)) l_list.push_back(#iso6391); if ((p_list == all) || (p_list == iso639_3)) l_list.push_back(#iso6393);
            #include "iso639.h"
            #undef LANGUAGE_CODE
            
            if (p_lower)
                for(std::vector<std::string>::iterator it = l_list.begin(); it != l_list.end(); it++)
                    boost::to_lower( *it );
            
            return l_list;
        }
        
    }

}}
#endif
