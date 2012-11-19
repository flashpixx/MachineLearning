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

#ifndef __MACHINELEARNING_CLUSTERING_NONSUPERVISED_CLUSTERING_HPP
#define __MACHINELEARNING_CLUSTERING_NONSUPERVISED_CLUSTERING_HPP


#include <boost/static_assert.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#ifdef MACHINELEARNING_MPI
#include <boost/mpi.hpp>
#endif

#include "../../errorhandling/exception.hpp"
#include "../../distances/distances.h"
#include "../../tools/tools.h"

namespace machinelearning {  namespace clustering {

    /** namespace for all non-supervised cluster algorithms **/
    namespace nonsupervised {
        
        #ifndef SWIG
        namespace ublas = boost::numeric::ublas;
        #ifdef MACHINELEARNING_MPI
        namespace mpi   = boost::mpi;
        #endif
        #endif
        
        
        /** abstract class for all non-supervised clustering classes
         * @todo add method for setting prototypes
         * @todo remove set logging and add to cluster method
         **/      
        template<typename T> class clustering
        {
            #ifndef SWIG
            BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
            #endif
            
            
            public :
            
                /** method for training prototypes **/
                virtual void train( const ublas::matrix<T>&, const std::size_t& ) = 0;
                
                /** method which returns prototypes **/
                virtual ublas::matrix<T> getPrototypes( void ) const = 0;
                
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
                
                /** calculate prototype index for datapoints **/
                virtual ublas::indirect_array<> use( const ublas::matrix<T>& ) const = 0;
            
        };
        
        
        /** abstract class for patch clustering
         * @todo add method for setting weights
         **/
        template<typename T> class patchclustering
        {
            #ifndef SWIG
            BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
            #endif
            
            
            public :
            
                /** method for using patch clustering with one patch **/
                virtual void trainpatch( const ublas::matrix<T>&, const std::size_t& ) = 0;
                
                /** method for getting prototype weights **/
                virtual ublas::vector<T> getPrototypeWeights( void ) const = 0;
                
                /** method for returning changes of the weights **/
                virtual std::vector< ublas::vector<T> > getLoggedPrototypeWeights( void ) const = 0;
        };            
        
        
        
        #ifdef MACHINELEARNING_MPI
        
        /** abstract class for clustering with MPI interface **/           
        template<typename T> class mpiclustering
        {
            BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
            
            public :
            
                /** MPI method for training prototypes **/
                virtual void train( const mpi::communicator&, const ublas::matrix<T>&, const std::size_t& ) = 0;
                
                /** MPI method which returns prototypes **/
                virtual ublas::matrix<T> getPrototypes( const mpi::communicator& ) const = 0;
                
                /** MPI method for returning history of trained prototypes **/
                virtual std::vector< ublas::matrix<T> > getLoggedPrototypes( const mpi::communicator& ) const = 0;
                
                /** MPI method for returning the quantizationerror **/
                virtual std::vector<T> getLoggedQuantizationError( const mpi::communicator& ) const = 0;
                
                /** MPI calculate prototype index for datapoints **/
                virtual ublas::indirect_array<> use( const mpi::communicator&, const ublas::matrix<T>& ) const = 0;
                
                /** MPI call for determine the distance without returning and input values, only for MPI connection **/
                virtual void use( const mpi::communicator& ) const = 0;
            
        };
        
        
        /** abstract class for clustering with MPI patch interface
         * @todo add method for setting weights
         **/ 
        template<typename T> class mpipatchclustering
        {
            BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
            
            public :
            
                /** MPI method for training patch prototypes **/
                virtual void trainpatch( const mpi::communicator&, const ublas::matrix<T>&, const std::size_t& ) = 0;
                
                /** MPI method for getting prototype weights **/
                virtual ublas::vector<T> getPrototypeWeights( const mpi::communicator& ) const = 0;
                
                /** MPI method for returning changes of the weights **/
                virtual std::vector< ublas::vector<T> > getLoggedPrototypeWeights( const mpi::communicator& ) const = 0;
        };
        
        #endif
        
    }
    
} }
#endif
