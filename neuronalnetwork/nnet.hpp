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
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "neuron.hpp"
#include "../neighborhood/neighborhood.h"


namespace machinelearning { namespace neuralnetwork {
    
    namespace ublas   = boost::numeric::ublas;
    
    

    template<typename T> class nnet {
        
        public :
        
            nnet( const std::size_t& );
        
            void setConnectionWeight( const std::size_t&, const std::size_t&, const T& );
            T getConnectionWeight( const std::size_t&, const std::size_t& ) const;
            void setConnectionWeight( const ublas::matrix<T>& );
            ublas::matrix<T> getConnectionWeight( void ) const;
            neuron<T> getNeuron( const std::size_t&, const std::size_t& ) const;
            void setNeuron( const std::size_t&, const std::size_t&, const neuron<T>& );
            
        
        private :
        
            std::map<std::size_t, neuron<T> > m_neurons;
            ublas::matrix<T> m_weights;
        
        
        
    };
    
    
    
};};


#endif