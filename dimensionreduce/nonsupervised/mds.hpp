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

#ifndef MACHINELEARNING_DIMENSIONREDUCE_NONSUPERVISED_MDS_HPP
#define MACHINELEARNING_DIMENSIONREDUCE_NONSUPERVISED_MDS_HPP


#include <boost/numeric/ublas/matrix.hpp>

#include "../dimensionreduce.hpp"
#include "../../exception/exception.h"
#include "../../tools/tools.h"


namespace machinelearning { namespace dimensionreduce { namespace nonsupervised {
    
    namespace ublas  = boost::numeric::ublas;
    
    
    /** create the Multidimensional scaling (MDS) **/
    template<typename T> class mds : public nonsupervisedreduce<T> {
        
        public :
        
            enum project {
                metric      = 0,
                general     = 1,
                sammon      = 2
            };
        
        
            mds( const std::size_t&, const project& = metric );
            ublas::matrix<T> map( const ublas::matrix<T>& );
            ublas::matrix<T> getMapping( void ) const;
            std::size_t getDimension( void ) const;
        
        
        private :
        
            /** target dimension **/
            const std::size_t m_dim;
            /** project type **/
            const project m_type;
            /** matrix with projection vectors (row orientated) **/
            ublas::matrix<T> m_project;

    };


    /** constructor
     * @param p_dim target dimension
     * @param p_type project type
     **/
    template<typename T> inline mds<T>::mds( const std::size_t& p_dim, const project& p_type ) :
        m_dim( p_dim ),
        m_type( p_type ),
        m_project()
    {
        if (p_dim == 0)
            throw exception::runtime(_("dimension must be greater than zero"));
    }
    
    
    /** returns the target dimensione size
     * @return number of dimension
     **/
    template<typename T> inline std::size_t mds<T>::getDimension( void ) const
    {
        return m_dim;
    }
    
    
    /** returns the project vectors (largest eigenvectors)
     * @return matrix with eigenvector
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::getMapping( void ) const
    {
        return m_project;
    }
    
    
    /** caluate and project the input data
     * @param p_data input datamatrix
     **/
    template<typename T> inline ublas::matrix<T> mds<T>::map( const ublas::matrix<T>& p_data )
    {
    }
    

};};};
