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

#ifndef MACHINELEARNING_CLUSTERING_CLUSTERING_HPP
#define MACHINELEARNING_CLUSTERING_CLUSTERING_HPP


#include <boost/numeric/ublas/matrix.hpp>

#include "../exception/exception.h"
#include "../distances/distances.h"


namespace machinelearning { namespace clustering {
    
    namespace ublas = boost::numeric::ublas;
    
    
    
    /** abstract class for all supervised clustering classes
     * @note every data / prototype matrix must be row orientated.
     * data matrix NxM with n number of datapoints and M data dimension
     **/      
    template<typename T, typename L> class supervisedclustering {
        
        public :

            /** method for training prototypes **/
            virtual void train( const ublas::matrix<T>&, const std::vector<L>&, const std::size_t&  ) { throw exception::templatevirtual("train"); };
        
            /** method which returns prototypes **/
            virtual ublas::matrix<T> getPrototypes( void ) const { throw exception::templatevirtual("getPrototypes"); };
        
            /** return lables of prototypes **/
            virtual std::vector<L> getPrototypesLabel( void ) const { throw exception::templatevirtual("getPrototypesLabel"); };
        
            /** disable and enable logging **/
            virtual void setLogging( const bool& ) { throw exception::templatevirtual("setLogging"); };
        
            /** shows logging status **/
            virtual bool getLogging( void ) const { throw exception::templatevirtual("getLogging"); };
        
            /** return history of trained prototypes **/
            virtual std::vector< ublas::matrix<T> > getLoggedPrototypes( void ) const { throw exception::templatevirtual("getLoggedPrototypes"); };
        
            /** returns the dimension of prototypes **/
            virtual std::size_t getPrototypeSize( void ) const { throw exception::templatevirtual("getPrototypeSize"); };
        
            /** number of prototypes / classes **/
            virtual std::size_t getPrototypeNumber( void ) const { throw exception::templatevirtual("getPrototypeNumber"); };
        
            /** return the quantizationerror **/
            virtual std::vector<T> getLoggedQuantizationError( void ) const { throw exception::templatevirtual("getQuantizationError"); };
        
            /** index position for prototype or label **/
            virtual ublas::indirect_array< std::vector<std::size_t> > use( const ublas::matrix<T>& ) const { throw exception::templatevirtual("use"); };

        
        
        protected :
        
            /** destructor **/
            virtual ~supervisedclustering( void ) {}
        
    };

    
    
    
    /** abstract class for all non-supervised clustering classes
     * @note every data / prototype matrix must be row orientated.
     * data matrix NxM with n number of datapoints and M data dimension
     **/      
    template<typename T> class nonsupervisedclustering {
        
        public :

            /** method for training prototypes **/
            virtual void train( const ublas::matrix<T>&, const std::size_t& ) { throw exception::templatevirtual("train"); };
        
            /** method which returns prototypes **/
            virtual ublas::matrix<T> getPrototypes( void ) const { throw exception::templatevirtual("getPrototypes"); };
        
            /** disable and enable logging **/
            virtual void setLogging( const bool& ) { throw exception::templatevirtual("setLogging"); };
        
            /** shows logging status **/
            virtual bool getLogging( void ) const { throw exception::templatevirtual("getLogging"); };
        
            /** return history of trained prototypes **/
            virtual std::vector< ublas::matrix<T> > getLoggedPrototypes( void ) const { throw exception::templatevirtual("getLoggedPrototypes"); };
        
            /** returns the dimension of prototypes **/
            virtual std::size_t getPrototypeSize( void ) const { throw exception::templatevirtual("getPrototypeSize"); };
        
            /** number of prototypes / classes **/
            virtual std::size_t getPrototypeNumber( void ) const { throw exception::templatevirtual("getPrototypeNumber"); };
        
            /** return the quantizationerror **/
            virtual std::vector<T> getLoggedQuantizationError( void ) const { throw exception::templatevirtual("getQuantizationError"); };
        
            /** calculate prototype index for datapoints **/
            virtual ublas::indirect_array< std::vector<std::size_t> > use( const ublas::matrix<T>& ) const { throw exception::templatevirtual("use"); };


        
        protected :
        
            /** destructor **/
            virtual ~nonsupervisedclustering( void ) {}
        
    };
    
};};

#endif
