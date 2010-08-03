/** 
 @cond
 #########################################################################
 # GPL License                                                           #
 #                                                                       #
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

#ifndef MULTILANGUAGE
    #define _(string) string
#else

#ifndef MACHINELEARNING_TOOLS_LANGUAGE_H
#define MACHINELEARNING_TOOLS_LANGUAGE_H

#include <libintl.h>
#include <locale.h>
#include <string>

#define _(string) gettext(string)


namespace machinelearning { namespace tools {
    
    
    /** class for create langugage bindings **/
    class language {
      
        public :
            
            static std::string getLanguage( void );
            static void bind( const std::string&, const std::string&, const std::string& = "" );

        
    };
    
    
    /** returns the environmental language
     * @return string with language
     **/
    inline std::string language::getLanguage( void )
    {
         return getenv("LANG");
    }
    
    
    /** bind the textdomain for different language
     * @param p_name name of language file
     * @param p_path path to language file (eg: ./language files locate under p_path / p_lang / p_name.mo)
     * @param p_lang language (empty for system value)
     **/
    inline void language::bind( const std::string& p_name, const std::string& p_path, const std::string& p_lang )
    {
        setlocale(LC_ALL, p_lang.c_str());
        bindtextdomain(p_name.c_str(), p_path.c_str());
        textdomain(p_name.c_str());
    }
    
    
};};

#endif
#endif
