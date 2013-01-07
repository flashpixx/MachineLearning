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


#ifndef __MACHINELEARNING_CLASSIFIER_LAZYLEARNER_HPP
#define __MACHINELEARNING_CLASSIFIER_LAZYLEARNER_HPP

#include <algorithm>
#include <map>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "classifier.hpp"
#include "../errorhandling/exception.hpp"
#include "../tools/tools.h"
#include "../neighborhood/neighborhood.h"



namespace machinelearning { namespace classifier {
    
    namespace ublas   = boost::numeric::ublas;
    
    
    /** class for create a lazy learner
     * @todo optimize distance and label extraction with distribution (it should be faster)
     * @todo implementation of the logging structures
     **/
    template<typename T, typename L> class lazylearner : public classifier<T, L> 
    {
        
        public :
        
            enum weighttype
            {
                none            = 0, 
                distance        = 1,
                inversedistance = 2
            };
            
        
            lazylearner( const neighborhood::neighborhood<T>&, const weighttype& = inversedistance );
            void setDatabase( const ublas::matrix<T>&, const std::vector<L>& );
            ublas::matrix<T> getDatabasePoints( void ) const;
            std::vector<L> getDatabaseLabel( void ) const;
            void setLogging( const bool& );
            bool getLogging( void ) const;
            std::size_t getDatabaseSize( void ) const; 
            std::size_t getDatabaseCount( void ) const;
            std::vector<T> getLoggedQuantizationError( void ) const;
            std::vector<L> use( const ublas::matrix<T>& ) const;        
            void clearLogging( void );
        
        
        private :
        
            /** neighborhood object **/
            const neighborhood::neighborhood<T>* m_neighborhood;
            /** weightoption **/
            const weighttype m_weight;
            /** data basis **/
            ublas::matrix<T> m_basedata;
            /** vector with data label information **/
            std::vector<L> m_baselabels;
            /** bool for logging **/
            bool m_logging;
            /** std::vector with index of the nearest datapoints for each datapoint  **/
            std::vector< ublas::matrix<T> > m_logprototypes;
            /** std::vector with quantisation error for each datapoint **/
            std::vector<T> m_quantizationerror;
        
            std::vector<L> getLabelsWithoutWeight( const ublas::matrix<std::size_t>& ) const;
            std::vector<L> getLabelsWithWeight( const ublas::matrix<std::size_t>&, const ublas::matrix<T>& ) const;
        
            static bool labelMapCompair( const std::pair<L, std::size_t>&, const std::pair<L, std::size_t>& );
        
    };
    
    
    
    /** constructor
     * @param p_neighborhood neighbor object
     * @param p_weight weighttype
    **/
    template<typename T, typename L> inline lazylearner<T,L>::lazylearner( const neighborhood::neighborhood<T>& p_neighborhood, const weighttype& p_weight ) :
        m_neighborhood( &p_neighborhood ),
        m_weight( p_weight ),
        m_logging( false ),
        m_logprototypes( std::vector< ublas::matrix<T> >() ),
        m_quantizationerror( std::vector< T >() )
    {}
    
    
    /** returns the prototype / data matrix
     * @return matrix (rows = prototypes)
     **/
    template<typename T, typename L> inline ublas::matrix<T> lazylearner<T, L>::getDatabasePoints( void ) const
    {
        return m_basedata;
    }
    
    
    /** returns the prototypes / data labels
     * @return vector with label information
     **/
    template<typename T, typename L> inline std::vector<L> lazylearner<T, L>::getDatabaseLabel( void ) const
    {
        return m_baselabels;
    }
    
    
    /** returns the dimension of prototypes
     * @return dimension of the prototypes
     **/
    template<typename T, typename L> inline std::size_t lazylearner<T, L>::getDatabaseSize( void ) const 
    {
        return m_basedata.size2();
    }
    
    
    /** returns the number of prototypes
     * @return number of the prototypes / classes
     **/
    template<typename T, typename L> inline std::size_t lazylearner<T, L>::getDatabaseCount( void ) const 
    {
        return m_basedata.size1();
    }
    
    
    /** enabled / disable logging
     * @param p_log bool
     **/
    template<typename T, typename L> inline void lazylearner<T, L>::setLogging( const bool& p_log )
    {
        m_logging = p_log;
    }
    
    
    /** shows the logging status
     * @return bool
     **/
    template<typename T, typename L> inline bool lazylearner<T, L>::getLogging( void ) const
    {
        return m_logging;
    }
    
    
    /** returns the quantisation error 
     * @return error for each iteration
     **/
    template<typename T, typename L> inline std::vector<T> lazylearner<T, L>::getLoggedQuantizationError( void ) const
    {
        return m_quantizationerror;
    }

    
    /** cleares / initialize the log object **/
    template<typename T, typename L> inline void lazylearner<T, L>::clearLogging( void )
    {
        m_logprototypes     = std::vector< ublas::matrix<T> >();
        m_quantizationerror = std::vector< T >();
    }
    
    
    /** sets the datapoints as the fixed references 
     * @param p_data Matrix with data (rows are the vectors)
     * @param p_labels vector for labels
     **/
    template<typename T, typename L> inline void lazylearner<T, L>::setDatabase( const ublas::matrix<T>& p_data, const std::vector<L>& p_labels )
    {
        if (p_labels.size() != p_data.size1())
            throw exception::runtime(_("matrix rows and label size are not equal"), *this);
        
        clearLogging();
        m_basedata      = p_data;
        m_baselabels    = p_labels;
    }
    
    
    
    /** compair call for std::map, so we get the maximum element (on the second entry)
     * @param p_left first element
     * @param p_right right element
     * @return bool if first pair (second component) is smaller
    **/
    template<typename T, typename L> inline bool lazylearner<T,L>::labelMapCompair( const std::pair<L, std::size_t>& p_left, const std::pair<L, std::size_t>& p_right)
    {
        return p_left.second < p_right.second;
    }
    
    
    /** create labels for every data point (data point label with maximum label counts)
     * @param p_neighbour neighbourhood matrix
     * @return vector with labels
    **/
    template<typename T, typename L> inline std::vector<L> lazylearner<T, L>::getLabelsWithoutWeight( const ublas::matrix<std::size_t>& p_neighbour ) const
    {
        std::vector<L> l_label; 
        std::map<L, std::size_t> l_nnlabel;
        
        for(std::size_t i=0; i < p_neighbour.size1(); ++i) {            

            // get label count
            l_nnlabel.clear();
            for(std::size_t j=0; j < p_neighbour.size2(); ++j)
                
                if ( l_nnlabel.find(m_baselabels[p_neighbour(i,j)]) == l_nnlabel.end())
                    l_nnlabel.insert( std::pair<L, std::size_t>(m_baselabels[p_neighbour(i,j)], 1) );
                else
                    l_nnlabel[ m_baselabels[p_neighbour(i,j)] ]++;
            
            // set this label that is the biggest (most counts)
            l_label.push_back(  std::max_element(l_nnlabel.begin(), l_nnlabel.end(), labelMapCompair)->first );
        }
        return l_label;
    }
    
    
    /** create weighted labels for every data point (data point label with maximum weight)
     * @param p_neighbour neighbourhood matrix
     * @param p_data data values for calculate distance
     * @return vector with labels
     **/
    template<typename T, typename L> inline std::vector<L> lazylearner<T, L>::getLabelsWithWeight( const ublas::matrix<std::size_t>& p_neighbour, const ublas::matrix<T>& p_data ) const
    {
        std::vector<L> l_label; 
        std::map<L,T> l_nnlabel;
        
        for(std::size_t i=0; i < p_neighbour.size1(); ++i) {
            
            // calculate the distance values of the neighbourhood points
            // if data point exact over a prototype (distance == 0) set lable direct
            l_nnlabel.clear();
            for(std::size_t j=0; j < p_neighbour.size2(); ++j) {
                T l_distance = m_neighborhood->calculateDistance( static_cast< ublas::vector<T> >(ublas::row(p_data,i)), 
                                                                  static_cast< ublas::vector<T> >(ublas::row(m_basedata, p_neighbour(i,j)))
                                                                );
                
                if (tools::function::isNumericalZero<T>(l_distance)) {
                    l_label.push_back( m_baselabels[p_neighbour(i,j)] );
                    break;
                }
                
                
                if (m_weight == inversedistance)
                    l_distance = m_neighborhood->invert(l_distance);
                
                // add distance to bucket
                if ( l_nnlabel.find(m_baselabels[p_neighbour(i,j)]) == l_nnlabel.end())
                    l_nnlabel.insert( std::pair<L,T>(m_baselabels[p_neighbour(i,j) ], l_distance) );
                else
                    l_nnlabel[ m_baselabels[p_neighbour(i,j)] ] += l_distance;
            }
            
            // data point is not over a prototype (normalize buckets and get max element label)
            if ( i == l_label.size())
                l_label.push_back(  std::max_element(l_nnlabel.begin(), l_nnlabel.end(), labelMapCompair)->first );
        
        }
        
        return l_label;
    }
        
        
    
    /** label unkown data
     * @param p_data input data matrix (row orientated)
     * @return std::vector with label information
    **/
    template<typename T, typename L> inline std::vector<L> lazylearner<T, L>::use( const ublas::matrix<T>& p_data ) const
    {
        // determine nearest neighbour
        const ublas::matrix<std::size_t> l_neighbour = m_neighborhood->get(m_basedata, p_data);
        std::vector<L> l_label; 
        
           
        switch (m_weight) {
        
            // use only neighbourhood for determine label
            case none :     
                l_label = getLabelsWithoutWeight( l_neighbour );            break;
                
            // get label from their distances
            case distance : 
                l_label = getLabelsWithWeight( l_neighbour, p_data );       break;
                
            // get label from their inverse distances    
            case inversedistance : 
                l_label = getLabelsWithWeight( l_neighbour, p_data );       break;

        }
        
        //if (m_logging)
        //    m_logprototypes.push_back( l_neighbour );
        
        return l_label;
    }
    
    
}}
#endif

