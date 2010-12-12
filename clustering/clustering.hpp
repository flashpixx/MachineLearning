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

#ifndef MACHINELEARNING_CLUSTERING_CLUSTERING_HPP
#define MACHINELEARNING_CLUSTERING_CLUSTERING_HPP


#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#ifdef ML_CLUSTER
#include <boost/mpi.hpp>
#endif

#include "../exception/exception.h"
#include "../distances/distances.h"
#include "../tools/tools.h"


namespace machinelearning { 
    
    /** namespace for all clustering algorithms
     * $LastChangedDate$
     **/
    namespace clustering {
        
        /** namespace for all non-supervised cluster algorithms
         * $LastChangedDate$
         **/
        namespace nonsupervised {};
        
        /** namespace for all supervised cluster algorithms
         * $LastChangedDate$
         **/
        namespace supervised {};
        
        
        
        
        namespace ublas = boost::numeric::ublas;
        #ifdef ML_CLUSTER
        namespace mpi   = boost::mpi;
        #endif
        
        
        /** abstract class for all supervised clustering classes
         * $LastChangedDate$
         * @note every data / prototype matrix must be row orientated.
         * data matrix NxM with n number of datapoints and M data dimension
         **/      
        template<typename T, typename L> class supervisedclustering {
            
            public :

                /** method for training prototypes **/
                virtual void train( const ublas::matrix<T>&, const std::vector<L>&, const std::size_t&  ) { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** method which returns prototypes **/
                virtual ublas::matrix<T> getPrototypes( void ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** return lables of prototypes **/
                virtual std::vector<L> getPrototypesLabel( void ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** disable and enable logging **/
                virtual void setLogging( const bool& ) { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** shows logging status **/
                virtual bool getLogging( void ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** return history of trained prototypes **/
                virtual std::vector< ublas::matrix<T> > getLoggedPrototypes( void ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** returns the dimension of prototypes **/
                virtual std::size_t getPrototypeSize( void ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** number of prototypes / classes **/
                virtual std::size_t getPrototypeNumber( void ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** return the quantizationerror **/
                virtual std::vector<T> getLoggedQuantizationError( void ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** index position for prototype or label **/
                virtual ublas::indirect_array< std::vector<std::size_t> > use( const ublas::matrix<T>& ) const { throw exception::classmethod(_("method is not implementated in the base class")); };

            
            
            protected :
            
                /** destructor **/
                virtual ~supervisedclustering( void ) {}
            
        };

        
        
        
        /** abstract class for all non-supervised clustering classes
         * $LastChangedDate$
         * @note every data / prototype matrix must be row orientated.
         * data matrix NxM with n number of datapoints and M data dimension
         **/      
        template<typename T> class nonsupervisedclustering {
            
            public :

                /** method for training prototypes **/
                virtual void train( const ublas::matrix<T>&, const std::size_t& ) { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** method which returns prototypes **/
                virtual ublas::matrix<T> getPrototypes( void ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** disable and enable logging **/
                virtual void setLogging( const bool& ) { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** shows logging status **/
                virtual bool getLogging( void ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** return history of trained prototypes **/
                virtual std::vector< ublas::matrix<T> > getLoggedPrototypes( void ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** returns the dimension of prototypes **/
                virtual std::size_t getPrototypeSize( void ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** number of prototypes / classes **/
                virtual std::size_t getPrototypeNumber( void ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** return the quantizationerror **/
                virtual std::vector<T> getLoggedQuantizationError( void ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** calculate prototype index for datapoints **/
                virtual ublas::indirect_array< std::vector<std::size_t> > use( const ublas::matrix<T>& ) const { throw exception::classmethod(_("method is not implementated in the base class")); };

            
            
                #ifdef ML_CLUSTER
            
                /** MPI method for training prototypes **/
                virtual void train( const mpi::communicator&, const ublas::matrix<T>&, const std::size_t& ) { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** MPI method which returns prototypes **/
                virtual ublas::matrix<T> getPrototypes( const mpi::communicator& ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** MPI method for returning history of trained prototypes **/
                virtual std::vector< ublas::matrix<T> > getLoggedPrototypes( const mpi::communicator& ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** MPI method for returning the quantizationerror **/
                virtual std::vector<T> getLoggedQuantizationError( const mpi::communicator& ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** MPI calculate prototype index for datapoints **/
                virtual ublas::indirect_array< std::vector<std::size_t> > use( const mpi::communicator&, const ublas::matrix<T>& ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                /** MPI call for determine the distance without returning and input values, only for MPI connection **/
                virtual void use( const mpi::communicator& ) const { throw exception::classmethod(_("method is not implementated in the base class")); };
            
                #endif
            
            
            protected :
            
                /** destructor **/
                virtual ~nonsupervisedclustering( void ) {}
            
        };
    
    };
};

#endif
