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

#ifndef MACHINELEARNING_MULTILANGUAGE
#define _(string) string
#else

#ifndef __MACHINELEARNING_TOOLS_LANGUAGE_BINDINGS_H
#define __MACHINELEARNING_TOOLS_LANGUAGE_BINDINGS_H

extern "C" {
#include <libintl.h>
}
#include <string>

#define _(string) gettext(string)


namespace machinelearning { namespace tools { namespace language {
    
    
    /** class for create langugage bindings **/
    class bindings
    {
        
        public :
        
            static std::string getLanguage( void );
            static void bind( const std::string& = "machinelearning", const std::string& = "language", const std::string& = "" );
        
        
    };
    
    
    /** returns the environmental language
     * @return string with language
     **/
    inline std::string bindings::getLanguage( void )
    {
        return getenv("LANG");
    }
    
    
    /** bind the textdomain for different language
     * @param p_name name of language file (default: machinelearning)
     * @param p_path path to language file (default: language directory, relative to the executable / eg: language files locate under p_path / p_lang / p_name.mo)
     * @param p_lang language (empty for system value)
     **/
    inline void bindings::bind( const std::string& p_name, const std::string& p_path, const std::string& p_lang )
    {
        setlocale(LC_ALL, p_lang.c_str());
        bindtextdomain(p_name.c_str(), p_path.c_str());
        textdomain(p_name.c_str());
    }
    
    
}}}
#endif
#endif
