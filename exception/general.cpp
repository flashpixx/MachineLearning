/** 
 \verbatim
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
 \endverbatim
 **/


#include "general.h"

namespace machinelearning { namespace exception {

    greaterthanzero::greaterthanzero( const std::string& p_str ) : std::invalid_argument("argument ["+p_str+"] must be greater than zero") {}
    
    greaterthan::greaterthan( const std::string& p_str1, const std::string& p_str2 ) : std::invalid_argument("argument ["+p_str1+"] must be greater than ["+p_str2+"]") {}

    initialization::initialization( const std::string& p_str ) : std::range_error("value ["+p_str+"] is not initialisized") {}
    
    iostreamopen::iostreamopen( const std::string& p_str ) : std::ios_base::failure("fail to open stream ["+p_str+"]") {}  
    
    iostreamempty::iostreamempty( const std::string& p_str ) : std::ios_base::failure("["+p_str+"] stream is empty") {} 
    
    
    
    
    
    templatevirtual::templatevirtual( const std::string& p_str ) : std::runtime_error("virtual method ["+p_str+"] must not be called") {}
    
    matrix::matrix( const std::string& p_str ) : std::runtime_error("matrix error"+( (!p_str.empty()) ? " ["+p_str+"]" : "" )) {}  
    
    
};};