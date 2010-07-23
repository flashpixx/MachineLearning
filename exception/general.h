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

#ifndef MACHINELEARNING_EXCEPTION_GENERAL_H
#define MACHINELEARNING_EXCEPTION_GENERAL_H

#include <string>
#include <iostream>
#include <stdexcept>
#include <exception>

namespace machinelearning { namespace exception {

    class greaterthanzero  : public std::invalid_argument   { public : greaterthanzero( const std::string& ); };
    class greaterthan      : public std::invalid_argument   { public : greaterthan( const std::string&, const std::string& ); };
    class matrixsymmetric  : public std::invalid_argument   { public : matrixsymmetric( void ); };
    class samesize         : public std::invalid_argument   { public : samesize( const std::string&, const std::string& ); };
    class initialization   : public std::range_error        { public : initialization( const std::string& ); };
    class iostreamopen     : public std::ios_base::failure  { public : iostreamopen( const std::string& ); };
    class iostreamempty    : public std::ios_base::failure  { public : iostreamempty( const std::string& ); };
    class templatevirtual  : public std::runtime_error      { public : templatevirtual( const std::string& ); };
    
    
    
    class lapack           : public std::runtime_error      { public : lapack( const std::string& ); };
    class numeric          : public std::runtime_error      { public : numeric( const std::string& ); };
    
    
};};

#endif
