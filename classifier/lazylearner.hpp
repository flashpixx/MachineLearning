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


#ifndef MACHINELEARNING_CLASSIFIER_LAZYLEARNER_HPP
#define MACHINELEARNING_CLASSIFIER_LAZYLEARNER_HPP

#include <algorithm>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/foreach.hpp>

#include "classifier.hpp"
#include "../exception/exception.h"
#include "../tools/tools.h"
#include "../neighborhood/neighborhood.h"



namespace machinelearning { namespace classifier {
    
    namespace ublas   = boost::numeric::ublas;
    
    
    /** class for create a lazy learner
     * @todo Logging must be implementated
     **/
    template<typename T, typename L> class lazylearner : public classifier<T, L>  {
        
        public :
        
            enum weighttype
            {
                none            = 0, 
                distance        = 1,
                inversedistance = 2
            };
            
        
            lazylearner( const neighborhood::neighborhood<T>&, const weighttype& = 2 );
            void train( const ublas::matrix<T>&, const std::vector<L>&, const unsigned int& );
            ublas::matrix<T> getTrainedData( void ) const;
            std::vector<L> getTrainedLabel( void ) const;
            void setLogging( const bool& );
            bool getLogging( void ) const;
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
            /** vector with neuron label information **/
            const std::vector<L> m_baselabels;
            /** bool for logging prototypes **/
            bool m_logging;
            /** std::vector with index of the nearest datapoints for each datapoint  **/
            std::vector< ublas::matrix<T> > m_logprototypes;
            /** std::vector with quantisation error for each datapoint **/
            std::vector<T> m_quantizationerror;
        
            static bool labelMapCompair( std::pair<L, std::size_t> left, std::pair<L, std::size_t> right);
        
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
    template<typename T, typename L> inline ublas::matrix<T> lazylearner<T, L>::getTrainedData( void ) const
    {
        return m_basedata;
    }
    
    
    /** returns the prototypes / data labels
     * @return vector with label information
     **/
    template<typename T, typename L> inline std::vector<L> lazylearner<T, L>::getTrainedLabel( void ) const
    {
        return m_baselabels;
    }
    
    
    /** enabled / disable logging
     * @param p bool
     **/
    template<typename T, typename L> inline void lazylearner<T, L>::setLogging( const bool& p )
    {
        m_logging = p;
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
     * @param p_number number of rows which are used
     **/
    template<typename T, typename L> inline void lazylearner<T, L>::train( const ublas::matrix<T>& p_data, const std::vector<L>& p_labels, const unsigned int& p_number )
    {
        clearLogging;
        
        ublas::matrix_range< ublas::matrix<T> > l_rangedata( p_data, ublas::range(0, p_number), ublas::range(0,p_data.size2()-1)  );
        m_basedata   = l_rangedata;
        
        ublas::vector_range< std::vector<L> > l_rangelabel( p_labels, ublas::range(0, p_number) );
        m_baselabels = l_rangelabel;
    }
    
    
    
    /** compair call for std::map, so we get the maximum element (on the second entry)
     * @param p_left first element
     * @param p_right right element
     * @return bool if first pair (second component) is smaller
    **/
    template<typename T, typename L> inline bool lazylearner<T,L>::labelMapCompair( std::pair<L, std::size_t> p_left, std::pair<L, std::size_t> p_right)
    {
        return p_left.second < p_right.second;
    }
    
    
    /** label unkown data
     * @param p_data input data matrix (row orientated)
     * @return std::vector with label information
    **/
    template<typename T, typename L> inline std::vector<L> lazylearner<T, L>::use( const ublas::matrix<T>& p_data ) const
    {
        // determine nearest neighbour
        const ublas::matrix<std::size_t> l_neighbour = m_neighborhood->get(m_basedata, p_data);
        std::vector<L> l_label(p_data.size1()); 
        
           
        switch (m_weight) {
        
            // use only neighbourhood for determine label
            case none :

                for(std::size_t i=0; i < l_neighbour.size1(); ++i) {
                    ublas::vector<std::size_t> l_idx = ublas::row(l_neighbour,i);
                
                    // get label count
                    std::map<L, std::size_t> l_nnlabel;
                    for(std::size_t j=0; j < l_idx.size(); ++j)
                        if ( l_nnlabel.find(m_baselabels[l_idx[j]]) == l_nnlabel.end())
                            l_nnlabel.insert( std::pair<L, std::size_t>(m_baselabels[l_idx[j]], 1) );
                        else
                            l_nnlabel[ m_baselabels[l_idx[j]] ]++;
                
                    // set this label that is the biggest (most counts)
                    l_label[i] = std::max_element(l_nnlabel.begin(), l_nnlabel.end(), labelMapCompair)->first;
                }
                break;
                
                
                
            // get label from their distances
            case distance :
                
                for(std::size_t i=0; i < l_neighbour.size1(); ++i) {
                    ublas::vector<std::size_t> l_idx = ublas::row(l_neighbour,i);
                                        
                    // calculate the distance values of the neighbourhood points
                    // if data point exact over a prototype (distance == 0) set lable direct
                    ublas::vector<T> l_distance(l_idx.size());
                    bool l_exact = false;
                    for(std::size_t j=0; j < l_idx.size(); ++j) {
                        l_distance(j) = m_neighborhood->calculateDistance( static_cast< ublas::vector<T> >(ublas::row(p_data,i)), 
                                                                           static_cast< ublas::vector<T> >(ublas::row(m_basedata, l_idx(j)))
                                                                         );
                        
                        if (tools::function::isNumericalZero<T>(l_distance(j))) {
                            l_label[i] = m_baselabels[l_idx[j]];
                            l_exact    = true;
                            break;
                        }
                    }
                    
                    // data point is not over a prototype
                    if (!l_exact) {
                        // quadratic distance
                        BOOST_FOREACH( T& j, l_distance)
                            j *= j;
                        
                        // set this label that is the biggest (most counts)
                        l_label[i] = std::distance(l_distance.begin(), std::max_element(l_distance.begin(), l_distance.end()));
                    }
                    
                }
                break;
                
                
                
                
                
            // get label from their inverse distances    
            case inversedistance :
                
                for(std::size_t i=0; i < l_neighbour.size1(); ++i) {
                    ublas::vector<std::size_t> l_idx = ublas::row(l_neighbour,i);
                    
                    // calculate the distance values of the neighbourhood points
                    // if data point exact over a prototype (distance == 0) set lable direct
                    ublas::vector<T> l_distance(l_idx.size());
                    bool l_exact = false;
                    for(std::size_t j=0; j < l_idx.size(); ++j) {
                        l_distance(j) = m_neighborhood->calculateDistance( static_cast< ublas::vector<T> >(ublas::row(p_data,i)), 
                                                                           static_cast< ublas::vector<T> >(ublas::row(m_basedata, l_idx(j)))
                                                                         );
                        
                        if (tools::function::isNumericalZero<T>(l_distance(j))) {
                            l_label[i] = m_baselabels[l_idx[j]];
                            l_exact    = true;
                            break;
                        }
                    }
                    
                    // data point is not over a prototype
                    if (!l_exact) {
                        // quadratic inverse distance
                        BOOST_FOREACH( T& j, l_distance)
                            j = static_cast<T>(1.0)/(j*j);
                        
                        // set this label that is the biggest (most counts)
                        l_label[i] = std::distance(l_distance.begin(), std::max_element(l_distance.begin(), l_distance.end()));
                    }
                    
                }
                
                break;
                
                
                
            default :
                throw exception::initialization("weight");

        }
        
        m_logprototypes.push_back( l_neighbour );
        
        return l_label;
    }
    
    
};};
#endif

