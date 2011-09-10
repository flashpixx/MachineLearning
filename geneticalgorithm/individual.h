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


#ifndef MACHINELEARNING_GENETICALGORITHM_INDIVIDUAL_H
#define MACHINELEARNING_GENETICALGORITHM_INDIVIDUAL_H

#include <boost/static_assert.hpp>


namespace machinelearning { namespace geneticalgorithm {
    
    /** abstract class of an indivdual of the population
     * $LastChangedDate: 2011-09-09 21:26:20 +0200 (Fr, 09 Sep 2011) $
     **/
    class individual {

        public :
        
            /** method for cloning the object. The method should be create a new individual for the population initialization **/
            virtual individual* clone( void ) const;
        
            /** combines the individual with another individual and returns the new one **/
            //virtual individual<T> combine( const individual<T>& ) const;
        
            /** combines the individual with another individual and constructs the new one on the pointer address **/
            virtual void combine( const individual&, const individual* ) const;
        

            /** mutates the individual **/
            virtual void mutate( void );
        
    };
    
};};

#endif

