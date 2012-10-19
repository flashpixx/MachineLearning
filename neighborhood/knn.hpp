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



#ifndef __MACHINELEARNING_NEIGHBORHOOD_KNN_HPP
#define __MACHINELEARNING_NEIGHBORHOOD_KNN_HPP


#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>


#include "neighborhood.hpp"
#include "../distances/distances.h"
#include "../tools/tools.h"


namespace machinelearning { namespace neighborhood {
    
    
    namespace ublas   = boost::numeric::ublas;
    
    
    /** implementation for the k-nearest-neighbor
     * @todo recreate the data structur with kdtree (http://en.wikipedia.org/wiki/Kd-tree) or r-tree (http://en.wikipedia.org/wiki/R-tree) for optimizing
     **/
    template<typename T> class knn : public neighborhood<T>
    {
        
        public :
        
            enum dataspace
            {
                kdtree  = 0,
                rtree   = 2
            };
        
        
            knn( const distances::distance<T>&, const std::size_t& );
            std::size_t getNeighborCount( void ) const;
            ublas::matrix<std::size_t> get( const ublas::matrix<T>& ) const;
            ublas::matrix<std::size_t> get( const ublas::matrix<T>&, const ublas::matrix<T>& ) const;
            T calculateDistance( const ublas::vector<T>&, const ublas::vector<T>& ) const;
            T invert( const T& p_val ) const;
        
            //methods for kd-tree use
            //knn( const distances::distance<T>&, const distances::metric<T>&, const ublas::matrix& );
            //ublas::indirect_array< std::vector<std::size_t> > get( const ublas::vector<T>&, const std::size_t& ) const;
            //std::vector< ublas::indirect_array< std::vector<std::size_t> > > get( const ublas::matrix<T>&, const std::size_t& ) const;
            //std::vector< ublas::indirect_array< std::vector<std::size_t> > > get( const ublas::matrix<T>&, const ublas::vector<T>& ) const;
        
        private :
        
            /** number of nearest **/
            const std::size_t m_knn;
            /** distance object **/
            const distances::distance<T>& m_distance;       
        
            ublas::symmetric_matrix<T, ublas::upper> calculate( const ublas::matrix<T>& ) const;
        
    };

    
    
    /** contructor for initialization the knn
     * @param p_distance distance object
     * @param p_knn number of neighborhood
     * @deprecated removed if class will be redesigned
     **/
    template<typename T> inline knn<T>::knn( const distances::distance<T>& p_distance, const std::size_t& p_knn ) :
        m_knn(p_knn),    
        m_distance( p_distance )
    {
        if (p_knn == 0)
            throw exception::runtime(_("knn must be greater than zero"), *this);
    }
    
    
    /** returns the number of neighbors
     * @return number
     * @deprecated removed if class will be redesigned
     **/
    template<typename T> inline std::size_t knn<T>::getNeighborCount( void ) const
    {
        return m_knn;
    }
    
    
    /** returns the k-nearest-index-points (row index) to every data point
     * @param p_data input data matrix
     * @return N x kNN matrix, with N rows (data points) and k index points
     * @deprecated removed if class will be redesigned
    **/
    template<typename T> inline ublas::matrix<std::size_t> knn<T>::get( const ublas::matrix<T>& p_data ) const
    {
        if (m_knn > p_data.size1())
            throw exception::runtime(_("knn is greater than datapoints"), *this);
        
        
        ublas::symmetric_matrix<T, ublas::upper> l_distance = calculate( p_data );
        ublas::matrix<std::size_t> l_index(l_distance.size1(), m_knn);
        
        // rank distances and extract index position
        for(std::size_t i=0; i < l_distance.size1(); ++i) {
            ublas::vector<T> l_vec = static_cast< ublas::vector<T> >(ublas::row(l_distance,i));
            
            ublas::vector<std::size_t> l_rank = tools::vector::rankIndexVector(l_vec);
            
            // index position in [1,knn+1] because we would like to have the neighbours of a data point
            // and rank[0] is index of the data point self (distance is 0) 
            const ublas::vector_range< ublas::vector<std::size_t> > l_range( l_rank, ublas::range(1, m_knn+1)  );
            
            ublas::row(l_index, i) = l_range;
        }
            
        return l_index;
    }
    
    
    /** returns the k-nearest-index-points (row index) to every data point
     * @param p_fix for every row row in the second parameter will be calculated the distance to this rows
     * @param p_data input data matrix
     * @return N x kNN matrix, with N rows (data points) and k index fix points
     * @deprecated removed if class will be redesigned
     **/
    template<typename T> inline ublas::matrix<std::size_t> knn<T>::get( const ublas::matrix<T>& p_fix, const ublas::matrix<T>& p_data  ) const
    {
        if (m_knn > p_data.size1())
            throw exception::runtime(_("knn is greater than datapoints"), *this);
        
        
        ublas::matrix<std::size_t> l_index(p_data.size1(), m_knn);
        
        for(std::size_t i=0; i < p_data.size1(); ++i) {
            const ublas::vector<T> l_vec = static_cast< ublas::vector<T> >(ublas::row(p_data, i));
            
            ublas::vector<T> l_distance(p_fix.size1());
            for(std::size_t j=0; j < p_fix.size1(); ++j)
                l_distance(j) = calculateDistance( l_vec, static_cast< ublas::vector<T> >(ublas::row(p_fix, j)) );
            
            ublas::vector<std::size_t> l_rank = tools::vector::rankIndexVector(l_distance);
            const ublas::vector_range< ublas::vector<std::size_t> > l_range( l_rank, ublas::range(0, m_knn)  );

            ublas::row(l_index, i) = l_range;
        }
        
        return l_index;
    }
    
    
    /** calculates the distances between two vectors
     * @param p_first first vector
     * @param p_second second vector
     * @return distance
     * @deprecated removed if class will be redesigned
     **/
    template<typename T> inline T knn<T>::calculateDistance( const ublas::vector<T>& p_first, const ublas::vector<T>& p_second ) const
    {
        return m_distance.getDistance( p_first, p_second );
    }
    
    
    /** invert a value with using the distance object
     * @param p_val value
     * @return inverted value
     * @deprecated removed if class will be redesigned
     **/
    template<typename T> inline T knn<T>::invert( const T& p_val ) const
    {
        return m_distance.getInvert( p_val );
    }
    
    
    /** calculate for every point the distance
     * @param p_data input data matrix (row orientated)
     * @return symmetric matrix with distance values
     * @deprecated removed if class will be redesigned
    **/
    template<typename T> inline ublas::symmetric_matrix<T, ublas::upper> knn<T>::calculate( const ublas::matrix<T>& p_data ) const
    {
        ublas::symmetric_matrix<T, ublas::upper> l_distance(p_data.size1(), p_data.size1());
        
        for(std::size_t i=0; i < p_data.size1(); ++i) {
            const ublas::vector<T> l_vec = static_cast< ublas::vector<T> >(ublas::row(p_data, i));
            
            for(std::size_t j=i+1; j < p_data.size1(); ++j)
                l_distance(i,j) = calculateDistance( l_vec, static_cast< ublas::vector<T> >(ublas::row(p_data, j)) );
        }
        
        return l_distance;
    }

}}
#endif
