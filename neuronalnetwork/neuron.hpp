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



#ifndef MACHINELEARNING_NEURONALNETWORK_NEURON_HPP
#define MACHINELEARNING_NEURONALNETWORK_NEURON_HPP

#include <string>
#include <ginac/ginac.h>

#include "../distances/distances.h"
#include "../exception/exception.h"



namespace machinelearning { namespace neuronalnetwork {
    
    namespace ublas   = boost::numeric::ublas;
    
    
    

    template<typename T> class neuron {
        
        public :
        
            neuron( const std::string&, const std::size_t& );
            neuron( const distances::distance<T>&, const std::string&, const std::size_t& );
            ublas::vector<T> getWeights( void ) const;
            T evaluate( const ublas::vector<T>& );
        
        //operator() 
        //operator= 
        
        private :
        
            const GiNaC::ex m_function;
            const distances::distance<T> m_distance;
            ublas::vector<T> m_weights;

        
    };
    
    
    
    
    
};};


#endif