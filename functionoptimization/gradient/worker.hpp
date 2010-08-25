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
                    const std::string&,
                    const std::string&,
                    const std::vector<std::string>&, 
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
            /** map with lower & upper initialisation values **/
            std::map<std::string, std::pair<T,T> > m_initvalues;
            /** map with static values **/
            std::map<std::string, boost::multi_array<T,D> > m_staticvalues;
            /** vector with variables for batch optimize **/
            std::vector<std::string> m_batch;
            /** map with optimized values **/
            std::map<std::string, T> m_result;
        
            GiNaC::ex m_function;
            GiNaC::symtab m_functionsymbols;
            GiNaC::ex m_errorfunction;
            GiNaC::symtab m_errorfunctionsymbols;
        std::vector<std::string> m_derivationvars;
        
    };
    
    
    /** The constructor is only called by the gradient class, so we must not check
     * the parameter
     * @param p_iteration maximum iterations
     * @param p_stepsize initializsation stepsize
     * @param p_syms symbolic value map
     * @param p_derivation map with all derivations
     * @param p_initvalues map with values and optimize range
     * @param p_staticvalues map with multidimensional array for static variables
     * @param p_batch vector with information wich variable can be optimize together
     **/
    template<typename T, std::size_t D> inline worker<T,D>::worker(  
                        const std::size_t& p_iteration, 
                        const T& p_stepsize, 
                        const std::string& p_function,
                        const std::string& p_errorfunction,
                        const std::vector<std::string>& p_derivationvars, 
                        const std::map<std::string, std::pair<T,T> >& p_initvalues, 
                        const std::map<std::string, boost::multi_array<T,D> >& p_staticvalues, 
                        const std::vector<std::string>& p_batch  
    ) :
        m_iteration( p_iteration ),
        m_stepsize( p_stepsize ),
        m_initvalues( p_initvalues ),
        m_staticvalues( p_staticvalues ),
        m_batch( p_batch ),
        m_result(),
        m_function(),
        m_functionsymbols(),
        m_errorfunction(),
        m_derivationvars(p_derivationvars)
    {
        // parsing the expression are not thread-safe, so we do this here
        GiNaC::parser l_parser;
        
        try {
            m_function              = l_parser( p_function );
            m_functionsymbols       = l_parser.get_syms();
            
            m_errorfunction         = l_parser( p_errorfunction );
            m_errorfunctionsymbols  = l_parser.get_syms();
        } catch (...) {
            throw exception::parameter(_("arithmetic expression could not be parsed"));
        }
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
        
        //create derivations of the errorfunction
        std::vector<GiNaC::ex> l_derivation;
        
        for(std::size_t i=0; i < m_derivationvars.size(); ++i)
            m_errorfunction.diff( GiNaC::ex_to<GiNaC::symbol>(m_errorfunctionsymbols[ m_derivationvars[i] ]), 1);
  

        
        // init dynamic values
        /*GiNaC::exmap l_dynamic;
        tools::random l_rand;
        for(typename std::map<std::string, std::pair<T,T> >::iterator it = m_initvalues.begin(); it != m_initvalues.end(); ++it)
            if (tools::function::isNumericalEqual(it->second.first, it->second.second))
                l_dynamic[ m_symbols[it->first] ] = it->second.first;
            else
                l_dynamic[ m_symbols[it->first] ] = l_rand.get<T>( tools::random::uniform, it->second.first, it->second.second );

          */
        // run
        //for(std::size_t i=0; i < m_iteration; ++i) {
            
            
            // iterate over every derivation
            //for(std::map<std::string, GiNaC::ex>::iterator it = m_derivation.begin(); it != m_derivation.end(); ++it) {
                //std::cout << it->second << "\t\t" << it->second.subs(l_dynamic) << std::endl;
            //}
        //}
    }


};};};
#endif
