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



#ifndef MACHINELEARNING_NEURONALNETWORK_NNET_HPP
#define MACHINELEARNING_NEURONALNETWORK_NNET_HPP

#include <map>
#include <algorithm>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_matrix.hpp>


#include "neuron.hpp"
#include "../exception/exception.h"


namespace machinelearning { namespace neuronalnetwork {
    
    namespace ublas   = boost::numeric::ublas;
    
    

    template<typename T> class nnet {
        
        public :
        
            nnet( const std::size_t& );
        
            neuron<T> getNeuron( const std::size_t&, const std::size_t& ) const;
            void setNeuron( const std::size_t&, const std::size_t&, const neuron<T>& );
            
        
        private :
        
            boost::adjacency_matrix<boost::undirectedS> m_net;
            std::multimap<std::size_t, std::size_t > m_layer;
        
        
    };
    
    
    
    template<typename T> inline nnet<T>::nnet( const std::size_t& p_vertex ) :
        m_net( p_vertex ),
        m_layer()
    {
        if (p_vertex == 0)
            throw exception::parameter(_("number of vertices must be greater than zero"));
    }

    
    
    
};};


#endif