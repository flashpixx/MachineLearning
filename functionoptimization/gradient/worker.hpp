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

#ifndef MACHINELEARNING_FUNCTIONALOPTIMIZATION_GRADIENT_WORKER_HPP
#define MACHINELEARNING_FUNCTIONALOPTIMIZATION_GRADIENT_WORKER_HPP

#include <map>
#include <ginac/ginac.h>
#include <boost/algorithm/string.hpp> 
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/multi_array.hpp>
#include <boost/static_assert.hpp>

#include "../../exception/exception.h"
#include "../../tools/tools.h"


namespace machinelearning { namespace functionaloptimization { namespace gradient {
    
    namespace ublas = boost::numeric::ublas;
    
    
    /** class for worker thread for calulating gradient values **/
    template<typename T, std::size_t D> class worker {
            BOOST_STATIC_ASSERT(D > 0);         // array dimension must be greater than 0
        
        
        public :
        
            worker( const std::size_t&, 
                    const T&,
                    const std::map<std::string, GiNaC::ex>&, 
                    const std::map<std::string, std::pair<T,T> >&, 
                    const std::map<std::string, boost::multi_array<T,D> >&,
                    const std::vector<std::string>&
                  );
        
            std::map<std::string, T> getResult( void ) const;
            void optimize( void );
        
        
        
        private :
            
            /** maximum iterations **/
            std::size_t m_iteration;
            /** init stepsize **/
            T m_stepsize;
            /** map with derivation **/
            std::map<std::string, GiNaC::ex> m_derivation;
            /** map with initialisation values **/
            std::map<std::string, std::pair<T,T> > m_initvalues;
            /** map with static values **/
            std::map<std::string, boost::multi_array<T,D> > m_staticvalues;
            /** vector with variables for batch optimize **/
            std::vector<std::string> m_batch;
            /** map with optimized values **/
            std::map<std::string, T> m_result;
        
        
    };
    
    
    /** constructor
     * @param p_iteration maximum iterations
     * @param p_stepsize initializsation stepsize
     * @param p_derivation map with all derivations
     * @param p_initvalues map with values and optimize range
     * @param p_static map with multidimensional array for static variables
     *
     **/
    template<typename T, std::size_t D> inline worker<T,D>::worker(  const std::size_t& p_iteration, const T& p_stepsize, const std::map<std::string, GiNaC::ex>& p_derivation, const std::map<std::string, std::pair<T,T> >& p_initvalues, const std::map<std::string, boost::multi_array<T,D> >& p_staticvalues, const std::vector<std::string>& p_batch  ) :
        m_iteration( p_iteration ),
        m_stepsize( p_stepsize ),
        m_derivation( p_derivation ),
        m_initvalues( p_initvalues ),
        m_staticvalues( p_staticvalues ),
        m_batch( p_batch )
    {
        if (m_iteration == 0)
            throw exception::parameter(_("iterations must be greater than zero"));
        if (p_derivation.size() == 0)
            throw exception::parameter(_("there are no variables for optimization"));
        if (p_derivation.size() != p_initvalues.size()) 
            throw exception::parameter(_("derivations and initialisation have not the same size"));
        if ( (m_stepsize < 0 ) || (tools::function::isNumericalZero(m_stepsize)) )
            throw exception::parameter(_("stepsize must be greater than zero"));
    }
    
    
    
    /** returns the calculated values 
     * @return std::map with value and variable name
     **/
    template<typename T, std::size_t D> inline std::map<std::string, T> worker<T,D>::getResult( void ) const
    {
        return m_result;
    }

    
    /** optimize value with gradient descent **/
    template<typename T, std::size_t D> inline void worker<T,D>::optimize( void ) 
    {
        // initialize values


        // run
        for(std::size_t i=0; i < m_iteration; ++i) {
            
            
            // iterate over every derivation
            
            
            
        }
    }


};};};
#endif
