/** 
 @cond
 #########################################################################
 # GPL License                                                           #
 #                                                                       #
 # This file is part of the Machine Learning Framework.                  #
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



#ifndef MACHINELEARNING_DISTANCES_RELATIONAL_EUCLID_H
#define MACHINELEARNING_DISTANCES_RELATIONAL_EUCLID_H

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "../../distance.hpp"
#include "../../../exception/exception.h"
#include "../../../tools/tools.h"


namespace machinelearning { namespace distances { namespace relational {
    
    namespace ublas  = boost::numeric::ublas;
    
    
    /** class for calculating relational euclid distance beween datapoints **/
    template<typename T> class euclid : public distance<T> {
        
        
    public:
        
        void normalize( ublas::vector<T>& ) const;
        void normalize( ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const;        
        ublas::vector<T> getNormalize( const ublas::vector<T>& ) const;
        ublas::matrix<T> getNormalize( const ublas::matrix<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const;

        ublas::vector<T> getDistance( const ublas::matrix<T>&, const ublas::vector<T>&, const tools::matrix::rowtype& = tools::matrix::row ) const;
        ublas::vector<T> getAbs( const ublas::vector<T>& ) const;
        
    };
    
    
    
    /** normalize a relational vector
     * @param p_vec vector which should be normalized
     **/
    template<typename T> inline void euclid<T>::normalize( ublas::vector<T>& p_vec ) const 
    {
        p_vec /= ublas::sum( p_vec );
    };
    
    
    
    /** normalize a matrix on their rows or columns vectors
     * @param p_matrix matrix for normalization
     * @param p_row option for row or column iteration (default row)
     **/
    template<typename T> inline void euclid<T>::normalize( ublas::matrix<T>& p_matrix, const tools::matrix::rowtype& p_row ) const
    { 
        switch (p_row) {                
            case tools::matrix::row :
                
                for(std::size_t i=0; i < p_matrix.size1(); ++i)
                    ublas::row(p_matrix, i) /= ublas::sum( ublas::row(p_matrix, i) );
                break;
                
                
                case tools::matrix::column :  
                
                for(std::size_t i=0; i < p_matrix.size2(); ++i)
                    ublas::column(p_matrix, i) /= ublas::sum( ublas::column(p_matrix, i) );
                break;
        }
        
    };
    
    
    
    /** return the normalized relational vector
     * @param p_vec vector which should be normalized
     * @return vector
     **/
    template<typename T> inline ublas::vector<T> euclid<T>::getNormalize( const ublas::vector<T>& p_vec ) const 
    {
        ublas::vector<T> l_vec = p_vec;
        normalize(l_vec);
        return l_vec;
    };
    
    
    
    /** normalize a matrix on their rows or columns vectors
     * @param p_matrix matrix for noamlization
     * @param p_row option for row or column iteration (default row)
     * @return normalized matrix
     **/
    template<typename T> inline ublas::matrix<T> euclid<T>::getNormalize( const ublas::matrix<T>& p_matrix, const tools::matrix::rowtype& p_row ) const
    {
        ublas::matrix<T> l_matrix = p_matrix;
        normalize(l_matrix, p_row);
        return l_matrix;
    }
    
    
    
    /** calculates the relational distance beween every row or column of the matrix and the vector 
     * @param p_data matrix
     * @param p_vec vector
     * @param p_row row / column option (default row)
     * @return vector with distance values
     **/    
    template<typename T> inline ublas::vector<T> euclid<T>::getDistance( const ublas::matrix<T>& p_data, const ublas::vector<T>& p_vec, const tools::matrix::rowtype& p_row ) const
    {
        ublas::vector<T> l_vec = ublas::prod(p_data, p_vec);
        return l_vec - ublas::scalar_vector<T>( l_vec.size(), 0.5 * ublas::inner_prod(l_vec, p_vec) );    
    }
    
    
    
    /** returns a vector with absolut elements
     * @param p_vec relational vector
     * @return the input vector, because a relational vector is positiv (>= 0)
     **/
    template<typename T> inline ublas::vector<T> euclid<T>::getAbs( const ublas::vector<T>& p_vec ) const
    {
        return p_vec;
    }

    
       
};};};

#endif