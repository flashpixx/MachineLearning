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

#ifndef MACHINELEARNING_DISTANCES_DISTANCE_HPP
#define MACHINELEARNING_DISTANCES_DISTANCE_HPP

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "../exception/exception.h"
#include "../tools/tools.h"
#include "../tools/matrix.hpp"



namespace machinelearning { namespace distances {
    
    namespace ublas = boost::numeric::ublas;
    
    
    /** abstract class for distance classes. Calculates 
     * between matrix-, vectordata and weighted data
     **/      
    template<typename T> class distance {
        
        
        public :
         
            /** method for calculating distances between row / column vectors of matrix **/
            virtual ublas::vector<T> calculate( const ublas::matrix<T>&, const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const { throw exception::templatevirtual("calculate"); };
        
            /** method for calculating weightes distances of row / column vector of matrix **/
            virtual ublas::vector<T> calculate( const ublas::matrix<T>&, const ublas::matrix<T>&, const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const { throw exception::templatevirtual("calculate"); };
        
            /** calculate absolut values of the vector **/
            virtual ublas::vector<T> abs( const ublas::vector<T>& p_vec ) const { throw exception::templatevirtual("abs"); };
        
            /** calculate distances between two vectors **/
            virtual T calculate( const ublas::vector<T>&, const ublas::vector<T>& ) const { throw exception::templatevirtual("calculate"); };
        
            /** calculate weight distance between two vectors **/
            virtual T calculate( const ublas::vector<T>&, const ublas::vector<T>&, const ublas::vector<T>& ) const { throw exception::templatevirtual("calculate"); };
        
            /** normalize a vector */
            virtual void norm( ublas::vector<T>& ) const { throw exception::templatevirtual("normalize"); };
        
            /** normalize a matrix for rows or columns **/
            virtual void norm( ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const { throw exception::templatevirtual("normalize"); };

            /** return the length of the vector **/
            virtual T length( const ublas::vector<T>& ) const { throw exception::templatevirtual("length"); };
        
        
        protected :
        
            /** destructor **/
            virtual ~distance( void ) {}; 
        
    };

    
};};

#endif
