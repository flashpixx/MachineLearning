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

#ifndef __MACHINELEARNING_CLASSIFIER_CLASSIFIER_HPP
#define __MACHINELEARNING_CLASSIFIER_CLASSIFIER_HPP


#include <boost/static_assert.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "../errorhandling/exception.hpp"
#include "../distances/distances.h"


namespace machinelearning { 
    
    /** namespace for all classifier algorithms **/
    namespace classifier {
        
        namespace ublas = boost::numeric::ublas;
        
        
        
        /** abstract class for classificator
         * @note every data matrix must be row orientated.
         * data matrix NxM with n number of datapoints and M data dimension
         * @todo checking method names and parameters (it's not optimal for using)
         **/      
        template<typename T, typename L> class classifier
        {
            BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
            
            
            public :
                
                /** method for training prototypes **/
                virtual void setDatabase( const ublas::matrix<T>&, const std::vector<L>& ) = 0;
                
                /** method which returns prototypes **/
                virtual ublas::matrix<T> getDatabasePoints( void ) const = 0;
                
                /** return lables of prototypes **/
                virtual std::vector<L> getDatabaseLabel( void ) const = 0;
                
                /** disable and enable logging **/
                virtual void setLogging( const bool& ) = 0; 
                
                /** returns dimension of data points **/
                virtual std::size_t getDatabaseSize( void ) const = 0;
                
                /** returns number of data points **/
                virtual std::size_t getDatabaseCount( void ) const = 0;
                
                /** shows logging status **/
                virtual bool getLogging( void ) const = 0;
               
                /** return the quantizationerror **/
                virtual std::vector<T> getLoggedQuantizationError( void ) const = 0;
                
                /** calculate label for unkown datapoints **/
                virtual std::vector<L> use( const ublas::matrix<T>& ) const = 0;
            
        };

    }
}

#endif
