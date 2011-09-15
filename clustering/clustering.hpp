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


#include <boost/static_assert.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#ifdef MACHINELEARNING_MPI
#include <boost/mpi.hpp>
#endif

#include "../exception/exception.h"
#include "../distances/distances.h"
#include "../tools/tools.h"


namespace machinelearning { 
    
    /** namespace for all clustering algorithms
     * $LastChangedDate$
     * @todo adding SOM http://www.cis.hut.fi/research/som-research/
     **/
    namespace clustering {
        
        namespace ublas = boost::numeric::ublas;
        #ifdef MACHINELEARNING_MPI
        namespace mpi   = boost::mpi;
        #endif
        
        
        /** namespace for all supervised cluster algorithms
         * $LastChangedDate$
         **/
        namespace supervised {
        
            /** abstract class for all supervised clustering classes
             * $LastChangedDate$
             * @note every data / prototype matrix must be row orientated.
             * data matrix NxM with n number of datapoints and M data dimension
             * @note The template type of this class need not be a countable datatype like (eg. int, long)
             * @todo add method for setting prototypes
             * @todo add supervised neural gas (see bng_supervised/bng_supervised.m)
             **/      
            template<typename T, typename L> class clustering
            {
                BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
                
                
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
                    virtual ublas::indirect_array<> use( const ublas::matrix<T>& ) const { throw exception::classmethod(_("method is not implementated in the base class")); };

                
                protected :
                
                    /** destructor **/
                    virtual ~clustering( void ) {}
                
            };
        
        };

        
        
        
        /** namespace for all non-supervised cluster algorithms
         * $LastChangedDate$
         **/
        namespace nonsupervised {
        
            /** abstract class for all non-supervised clustering classes
             * $LastChangedDate$
             * @todo add method for setting prototypes
             * @todo implement serializable interface
             **/      
            template<typename T> class clustering
            {
                BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
                
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
             * $LastChangedDate$
             * @todo add method for setting weights
             * @todo implement serializable interface
             **/
            template<typename T> class patch
            {
                BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
                
                public :
                
                    /** method for using patch clustering with one patch **/
                    virtual void trainpatch( const ublas::matrix<T>&, const std::size_t& ) = 0;
                    
                    /** method for getting prototype weights **/
                    virtual ublas::vector<T> getPrototypeWeights( void ) const = 0;
                    
                    /** method for returning changes of the weights **/
                    virtual std::vector< ublas::vector<T> > getLoggedPrototypeWeights( void ) const = 0;
            };            

            
            
            #ifdef MACHINELEARNING_MPI
            
            /** abstract class for clustering with MPI interface
             * $LastChangedDate$
             **/           
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
             * $LastChangedDate$
             * @todo add method for setting weights
             **/ 
            template<typename T> class mpipatch
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
            
        };
    
    };
};

#endif
