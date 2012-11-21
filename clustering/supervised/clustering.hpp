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

#ifndef __MACHINELEARNING_CLUSTERING_SUPERVISED_CLUSTERING_HPP
#define __MACHINELEARNING_CLUSTERING_SUPERVISED_CLUSTERING_HPP


#include <boost/static_assert.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "../../errorhandling/exception.hpp"
#include "../../distances/distances.h"
#include "../../tools/tools.h"

namespace machinelearning {  namespace clustering {
        
        /** namespace for all supervised cluster algorithms **/
        namespace supervised {
            
            #ifndef SWIG
            namespace ublas = boost::numeric::ublas;
            #endif
            
            
            /** abstract class for all supervised clustering classes
             * @note every data / prototype matrix must be row orientated.
             * data matrix NxM with n number of datapoints and M data dimension
             * @note The template type of this class need not be a countable datatype like (eg. int, long)
             * @todo add method for setting prototypes
             * @todo add supervised neural gas (see bng_supervised/bng_supervised.m)
             **/      
            template<typename T, typename L> class clustering
            {
                #ifndef SWIG
                BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
                #endif
                
                
                public :
                
                    /** method for training prototypes **/
                    virtual void train( const ublas::matrix<T>&, const std::vector<L>&, const std::size_t&  ) = 0;
                    
                    /** method which returns prototypes **/
                    virtual ublas::matrix<T> getPrototypes( void ) const = 0;
                    
                    /** return lables of prototypes **/
                    virtual std::vector<L> getPrototypesLabel( void ) const = 0;
                    
                    /** disable and enable logging **/
                    virtual void setLogging( const bool& ) = 0;
                    
                    /** shows logging status **/
                    virtual bool getLogging( void ) const = 0;
                    
                    /** return history of trained prototypes **/
                    virtual std::vector< ublas::matrix<T> > getLoggedPrototypes( void ) const = 0;
                    
                    /** returns the dimension of prototypes **/
                    virtual std::size_t getPrototypeSize( void ) const = 0;
                    
                    /** number of prototypes / classes **/
                    virtual std::size_t getPrototypeCount( void ) const = 0;
                    
                    /** return the quantizationerror **/
                    virtual std::vector<T> getLoggedQuantizationError( void ) const = 0;
                    
                    /** index position for prototype or label **/
                    virtual ublas::indirect_array<> use( const ublas::matrix<T>& ) const = 0;
                
            };
            
        }
    
    }
}
#endif
