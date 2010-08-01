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


#ifndef MACHINELEARNING_DIMENSIONREDUCE_HPP
#define MACHINELEARNING_DIMENSIONREDUCE_HPP


#include <boost/numeric/ublas/matrix.hpp>

#include "../exception/exception.h"


namespace machinelearning { namespace dimensionreduce {

    namespace ublas = boost::numeric::ublas;
    
    
    
    /** abstract class for nonsupervised dimension reducing classes **/      
    template<typename T> class nonsupervisedreduce {
        
        public :

            /** maps data to target dimension **/
            virtual ublas::matrix<T> map( const ublas::matrix<T>& ) { throw exception::templatevirtual("map"); };
        
            /** returns the direction **/
            virtual ublas::matrix<T> getMapping( void ) const { throw exception::templatevirtual("getMapping"); };
      
            /** returns the mapped dimension **/
            virtual std::size_t getDimension( void ) const { throw exception::templatevirtual("getDimension"); };
        
        protected :
        
            /** destructor **/
            virtual ~nonsupervisedreduce( void ) {}

    };

    
    
    /** abstract class for supervised dimension reducing classes **/      
    template<typename T, typename L> class supervisedreduce {
        
        public :
        
            /** maps data to target dimension **/
            virtual ublas::matrix<T> map( const ublas::matrix<T>&, const std::vector<L>& ) { throw exception::templatevirtual("map"); };
        
            /** returns the direction **/
            virtual ublas::matrix<T> getMapping( void ) const { throw exception::templatevirtual("getMapping"); };
        
            /** returns the mapped dimension **/
            virtual std::size_t getDimension( void ) const { throw exception::templatevirtual("getDimension"); };
        
        protected :
        
            /** destructor **/
            virtual ~supervisedreduce( void ) {}
        
    };
    
    
};};
#endif