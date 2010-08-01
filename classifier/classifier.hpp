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

#ifndef MACHINELEARNING_CLASSIFIER_HPP
#define MACHINELEARNING_CLASSIFIER_HPP


#include <boost/numeric/ublas/matrix.hpp>

#include "../exception/exception.h"
#include "../distances/distances.h"


namespace machinelearning { namespace classifier {
    
    namespace ublas = boost::numeric::ublas;
    
    
    
    /** abstract class for classificator
     * @note every data matrix must be row orientated.
     * data matrix NxM with n number of datapoints and M data dimension
     **/      
    template<typename T, typename L> class classifier {
        
        public :
            
            /** method for training prototypes **/
            virtual void setDatabase( const ublas::matrix<T>&, const std::vector<L>& ) { throw exception::templatevirtual("train"); };
            
            /** method which returns prototypes **/
            virtual ublas::matrix<T> getDatabasePoints( void ) const { throw exception::templatevirtual("getTrainedData"); };
            
            /** return lables of prototypes **/
            virtual std::vector<L> getDatabaseLabel( void ) const { throw exception::templatevirtual("getTrainedLabel"); };
            
            /** disable and enable logging **/
            virtual void setLogging( const bool& ) { throw exception::templatevirtual("setLogging"); };
            
            /** returns dimension of data points **/
            virtual std::size_t getDatabaseSize( void ) const { throw exception::templatevirtual("getTrainedDataSize"); };
            
            /** returns number of data points **/
            virtual std::size_t getDatabaseCount( void ) const { throw exception::templatevirtual("getTrainedDataCount"); };
            
            /** shows logging status **/
            virtual bool getLogging( void ) const { throw exception::templatevirtual("getLogging"); };
           
            /** return the quantizationerror **/
            virtual std::vector<T> getLoggedQuantizationError( void ) const { throw exception::templatevirtual("getQuantizationError"); };
            
            /** calculate label for unkown datapoints **/
            virtual std::vector<L> use( const ublas::matrix<T>& ) const { throw exception::templatevirtual("use"); };
        
        
        
        protected :
        
            /** destructor **/
            virtual ~classifier( void ) {}
        
    };

};};

#endif
