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



#ifndef MACHINELEARNING_NEIGHBORHOOD_HPP
#define MACHINELEARNING_NEIGHBORHOOD_HPP


#include <boost/numeric/ublas/matrix.hpp>

#include "../tools/tools.h"


namespace machinelearning { namespace neighborhood {
    
    
    namespace ublas   = boost::numeric::ublas;
    
    
    
    /** abstract class for neighborhood classes. **/      
    template<typename T> class neighborhood {
        
        
        public :
        
            /** function for calculating the neighborhoods **/
            virtual ublas::matrix<std::size_t> get( const ublas::matrix<T>& ) const { throw exception::classmethod(_("method is not implementated in the base class")); }
        
            /** function for calculating the neighborhoods between different datapoints **/
            virtual ublas::matrix<std::size_t> get( const ublas::matrix<T>&, const ublas::matrix<T>& ) const { throw exception::classmethod(_("method is not implementated in the base class")); }
        
            /** calculates the distance between two vectors **/
            virtual T calculateDistance( const ublas::vector<T>&, const ublas::vector<T>& ) const { throw exception::classmethod(_("method is not implementated in the base class")); }
        
            /** invert a value **/
            virtual T invert( const T& ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
        
            /** returns the number of neighbors **/
            virtual std::size_t getNeighborCount( void ) const { throw exception::classmethod(_("method is not implementated in the base class")); }
        
        
        protected :
            
            /** destructor **/
            virtual ~neighborhood( void ) {};
        
        
    };
    

};};
#endif
