/** 
 @cond
 #########################################################################
 # GPL License                                                           #
 #                                                                       #
 # This file is part of the Machine Learning Framework.                  #
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


#include "hdf.h"

namespace machinelearning { namespace exception { namespace hdf {

    sizecorrect::sizecorrect() : range_error("dataset size is incorrect") {}
    
    simpletype::simpletype()   : invalid_argument("hdf datatype is a simple datatype") {}

    equaltype::equaltype( const std::string& p_str1, const std::string& p_str2 ) : invalid_argument("type ["+p_str1+"] cannot read as type ["+p_str2+"]") {}
    
};};};
