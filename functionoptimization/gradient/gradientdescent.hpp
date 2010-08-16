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

#ifndef MACHINELEARNING_FUNCTIONALOPTIMIZATION_GRADIENT_GRADIENTDESCENT_HPP
#define MACHINELEARNING_FUNCTIONALOPTIMIZATION_GRADIENT_GRADIENTDESCENT_HPP


#include <map>
#include <string>
#include <algorithm>
#include <ginac/ginac.h>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp> 
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/multi_array.hpp>
#include <boost/static_assert.hpp>
#include <boost/thread.hpp>


#include "worker.hpp"
#include "../../exception/exception.h"
#include "../../tools/tools.h"



namespace machinelearning { namespace functionaloptimization { 
    
    namespace ublas = boost::numeric::ublas;
    
    
    /** class for using a (stochastic) gradient descent
     * symbolic numerical algorithms @see http://www.ginac.de
     **/
    template<typename T, std::size_t D=1> class gradientdescent {
        BOOST_STATIC_ASSERT(D > 0);         // array dimension must be greater than 0
        
    
        public :

            gradientdescent( const std::string& );
            void setErrorFunction( const std::string&, const std::string& = "0.5 * (target-(function))^2", const std::string& = "function", const std::string& = " ,;" );
            void setOptimizeVar( const std::string&, const T&, const T& );
            void setStaticVar( const std::string&, const boost::multi_array<T,D>& );
            std::map<std::string, T> optimize( const std::size_t&, const std::size_t&, const T&, const std::vector<std::string>& = std::vector<std::string>() ) const;
        
        
        private :
             
        
            /** expression for the function **/
            GiNaC::ex m_expression;
            /** symbols table for the function **/
            GiNaC::symtab m_exprtable;
            /** symbols table for all variables **/
            GiNaC::symtab m_fulltable;
            /** derivations **/
            std::map<std::string, GiNaC::ex> m_derivation;
            /** map with lower & upper value for parameter **/
            std::map<std::string, std::pair<T,T> > m_optimize;
            /** map for static parameter **/
            std::map<std::string, boost::multi_array<T,D> > m_static;
        
    };

    
    
    /** constructor
     * @param p_func arithmetic expression
     **/
    template<typename T, std::size_t D> inline gradientdescent<T,D>::gradientdescent( const std::string& p_func ) :
        m_expression(),
        m_exprtable(),
        m_fulltable(),
        m_derivation(),
        m_optimize(),
        m_static()
    {
        if (p_func.empty())
            throw exception::parameter(_("function need not be empty"));
        
        
        // create expression parser, parse expresseion and add symbols to table
        GiNaC::parser l_parser;
        
        try {
            m_expression = l_parser( p_func );
            m_exprtable  = l_parser.get_syms();
        } catch (...) {
            throw exception::parameter(_("arithmetic expression could not be parsed"));
        }
        
    
    }
    
    
    /** creates the gradient function (default sum-of-squared-error / SSE)
     * @param p_optimizevars is a list of variables in the original formula which will be optimized (vars musst be separated with spaces, comma or semicolon, see separator)
     * @param p_errfunc error function in wich must be set the name from p_funcname
     * @param p_funcname string name in which will be set the function
     * @param p_separator separator charaters (default space, comma and semicolon)
     * @todo check errorfunction if convex (2nd derivation must be >= 0 for all values)
     **/
    template<typename T, std::size_t D> inline void gradientdescent<T,D>::setErrorFunction( const std::string& p_optimizevars, const std::string& p_errfunc, const std::string& p_funcname, const std::string& p_separator )
    {
        if (p_errfunc.empty())
            throw exception::parameter(_("error function need not be empty"));
        if (p_funcname.empty())
            throw exception::parameter(_("variable name for the function need not be empty"));
        if (p_separator.empty())
            throw exception::parameter(_("separators need not be empty"));
        
        // clear properties
        m_optimize.clear();
        m_static.clear();
        m_derivation.clear();
        
        // check if symbols for optimization are in the table
        std::vector<std::string> l_sep;
        std::vector<std::string> l_vars;
        boost::split( l_sep, p_optimizevars, boost::is_any_of(p_separator) );
               
        // during separation empty vector entries can be created, so we check if is not empty and in the symbolic table, than we copy
        for(std::size_t i=0; i < l_sep.size(); ++i) {
            if (l_sep[i].empty())
                continue;
        
             if (m_exprtable.find(l_sep[i]) == m_exprtable.end())
                 throw exception::parameter(_("variable for optimization is not found in the expression symbol table"));
             
             l_vars.push_back( l_sep[i] );
        }
        
        
        // create full expression, symboltable and parse the function
        GiNaC::ex l_full;
        GiNaC::symtab l_table(m_exprtable);
        
        // we copy the main expression into the definition variable name
        // than we parse the error function, GiNaC adds the main expression
        // into the error function
        l_table[p_funcname] = m_expression;
        GiNaC::parser l_parser(l_table);
        

        try {
            l_full      = l_parser( p_errfunc );
            
            // symbolic table for concat function and remove "functionname"
            m_fulltable = l_parser.get_syms();
            m_fulltable.erase(p_funcname);
        } catch (...) {
            throw exception::parameter(_("arithmetic expression could not be parsed"));
        }
        
        // checks number of variables (target symbolic var, so increment +1)
        if (m_exprtable.size()+1 != m_fulltable.size())
            throw exception::parameter(_("only one variable for the data must be added"));
        
        
        // create first derivation for each variable, which should be fitted
        for(std::size_t i=0; i < l_vars.size(); ++i)
            m_derivation[l_vars[i]] =  l_full.diff( GiNaC::ex_to<GiNaC::symbol>(m_fulltable[ l_vars[i] ]), 1);
    }
    
    
    
    /** sets init values for optimizing values. Real start value will be taken with random between [lower, upper]
     * @param p_name variable name
     * @param p_lower lower value
     * @param p_upper upper vlaue
     **/
    template<typename T, std::size_t D> inline void gradientdescent<T,D>::setOptimizeVar( const std::string& p_name, const T& p_lower, const T& p_upper )
    {
        if (m_derivation.find(p_name) == m_derivation.end())
            throw exception::parameter(_("variable for optimization is not found in the expression symbol table"));
        
        m_optimize[p_name] = std::pair<T,T>(p_lower, p_upper);
    }
    
    
    
    /** sets the static variables that are constant during optimization
     * @param p_name variable name
     * @param p_data data array (multidimensional)
     **/
    template<typename T, std::size_t D> inline void gradientdescent<T,D>::setStaticVar( const std::string& p_name, const boost::multi_array<T,D>& p_data )
    {
        if ( (m_derivation.find(p_name) != m_derivation.end()) || (m_fulltable.find(p_name) == m_fulltable.end()) )
            throw exception::parameter(_("static variable is not in the symbol table or is an optimazation variable"));
        
        m_static[p_name] = p_data;
    }
    
    
    
    
    template<typename T, std::size_t D> inline std::map<std::string, T> gradientdescent<T,D>::optimize( const std::size_t& p_threads, const std::size_t& p_iteration, const T& p_stepsize, const std::vector<std::string>& p_batch ) const
    {
        if (p_iteration == 0)
            throw exception::parameter(_("iterations must be greater than zero"));
        if ( (p_stepsize < 0 ) || (tools::function::isNumericalZero(p_stepsize)) )
            throw exception::parameter(_("stepsize must be greater than zero"));
        if (p_threads == 0)
            throw exception::parameter(_("number of threads must be greater than zero"));
        
        // all variables must be set to a numerical value, so we check it
        if (m_static.size() + m_optimize.size() != m_fulltable.size())
            throw exception::parameter(_("there are unsed variables"));
                
        
        // creating worker and thread objects
        std::vector< gradient::worker<T,D> > l_worker;
        boost::thread_group l_threadgroup;
        for(std::size_t i=0; i < p_threads; ++i) {
           l_worker.push_back(  gradient::worker<T,D>(p_iteration, p_stepsize, m_fulltable, m_derivation, m_optimize, m_static, p_batch)  );
            l_threadgroup.create_thread(  boost::bind( &gradient::worker<T,D>::optimize, l_worker[i] )  );
        }
        
        // run threads and wait during all finished
        // remove all threads bindings
        l_threadgroup.join_all();

        
        // get data and creates best values
        std::map<std::string, T> x;
        return x;
        
    }
     
    
};};

#endif
