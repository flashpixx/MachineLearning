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

#ifdef MACHINELEARNING_SYMBOLICMATH

#ifndef __MACHINELEARNING_FUNCTIONOPTIMIZATION_GRADIENTDESCENT_HPP
#define __MACHINELEARNING_FUNCTIONOPTIMIZATION_GRADIENTDESCENT_HPP


#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <ginac/ginac.h>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp> 
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/multi_array.hpp>
#include <boost/static_assert.hpp>
#include <boost/thread.hpp>


#include "../errorhandling/exception.hpp"
#include "../tools/tools.h"



namespace machinelearning { namespace functionaloptimization { 
    
    namespace ublas = boost::numeric::ublas;
    
    
    /** class for using a (stochastic) gradient descent.
     * For symbolic numerical algorithms @see http://www.ginac.de .
     * GiNaC is not thread-safe, so for the thread we create only std::string vars
     * @bug uncomplete, so doesn't work
     * @todo adding detection of numerical instability eg x*exp(x) the optimization of the
     * multiplication x is uncomplicated that the exp(x) (in the exponent). One solution to
     * optimize this function is to optimize for the multiplication and next the exponent.
     * To detect the problem the optimization values are jumping between a range during
     * iteration, so if the values are jumping the optimization process must be switched
     * in two processes or all multiplication / exponents must be cut and each thread
     * must be create internal processes for the iteration (see batch parameter within the optimization method)
     * @see http://en.wikipedia.org/wiki/Frank%E2%80%93Wolfe_algorithm for optimazation
     **/
    template<typename T, std::size_t D=1> class gradientdescent
    {
        BOOST_STATIC_ASSERT(D > 0);         // array dimension must be greater than 0
        
    
        public :

            gradientdescent( const std::string& );
            void setErrorFunction( const std::string&, const std::string& = "0.5 * (target-(function))^2", const std::string& = "target", const std::string& = "function", const std::string& = " ,;\t\n-" );
            void setOptimizeVar( const std::string&, const T&, const T& );
            void setOptimizeVar( const std::string&, const T& );
            void setStaticVar( const std::string&, const boost::multi_array<T,D>& );
            std::map<std::string, T> optimize( const std::size_t&, const std::size_t&, const std::vector<std::string>& = std::vector<std::string>() ) const;
        
        
        private :
             
        
            /** expression for the function **/
            GiNaC::ex m_expression;
            /** symbols table for the function **/
            GiNaC::symtab m_exprtable;
            /** full error expression **/
            GiNaC::ex m_full;
            /** table with all symbols **/
            GiNaC::symtab m_fulltable;
            /** derivations **/
            std::vector<std::string> m_derivationvars;
            /** map with lower & upper value for parameter **/
            std::map<std::string, std::pair<T,T> > m_optimize;
            /** map for static parameter **/
            std::map<std::string, boost::multi_array<T,D> > m_static;
        
            static std::string expression2string( const GiNaC::ex& );
        
        
        
        
            /** class for worker thread for calulating gradient values **/
            class worker {
                
                public :
                
                    worker( const std::size_t&, 
                        const std::size_t&,
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
                    /** sampling value **/
                    std::size_t m_sampling;
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
        
    };

    
    
    /** converts a GiNaC expression to a string
     * @param p_ex expression
     * @return string with expression
     **/
    template<typename T, std::size_t D> inline std::string gradientdescent<T,D>::expression2string( const GiNaC::ex& p_ex )
    {
        std::string l_str;
        std::stringstream l_sstream;
        
        l_sstream << p_ex;
        l_sstream >> l_str;
        
        return l_str;
    }
    
    
    
    /** constructor
     * @param p_func arithmetic expression
     **/
    template<typename T, std::size_t D> inline gradientdescent<T,D>::gradientdescent( const std::string& p_func ) :
        m_expression(),
        m_exprtable(),
        m_derivationvars(),
        m_optimize(),
        m_static()
    {
        if (p_func.empty())
            throw exception::runtime(_("function need not be empty"), *this);
        
        
        // create expression parser, parse expresseion and add symbols to table
        GiNaC::parser l_parser;
        
        try {
            m_expression = l_parser( p_func );
            m_exprtable  = l_parser.get_syms();
        } catch (...) {
            throw exception::runtime(_("arithmetic expression could not be parsed"), *this);
        }
        
    
    }
    
    
    /** creates the gradient function (default sum-of-squared-error / SSE)
     * @param p_optimizevars is a list of variables in the original formula which will be optimized (vars musst be separated with spaces, comma, semicolon, return or tabulator, see separator)
     * @param p_errfunc error function in which must be set the name from p_funcname
     * @param p_targetname name of the variable name in which the dataset will be stored
     * @param p_funcname string name in which will be set the function
     * @param p_separator separator charaters (default space, comma and semicolon)
     * @todo check errorfunction if convex (2nd derivation must be >= 0 for all values)
     * @todo adding target value name
     **/
    template<typename T, std::size_t D> inline void gradientdescent<T,D>::setErrorFunction( const std::string& p_optimizevars, const std::string& p_errfunc, const std::string& p_targetname, const std::string& p_funcname, const std::string& p_separator )
    {
        if (p_errfunc.empty())
            throw exception::runtime(_("error function need not be empty"), *this);
        if (p_funcname.empty())
            throw exception::runtime(_("variable name for the function need not be empty"), *this);
        if (p_separator.empty())
            throw exception::runtime(_("separators need not be empty"), *this);
        
        // clear properties
        m_optimize.clear();
        m_static.clear();
        m_derivationvars.clear();
        m_fulltable.clear();
        
        // check if symbols for optimization are in the table
        std::vector<std::string> l_sep;
        boost::split( l_sep, p_optimizevars, boost::is_any_of(p_separator) );
               
        // during separation empty vector entries can be created, so we check if is not empty and in the symbolic table, than we copy
        for(std::size_t i=0; i < l_sep.size(); ++i) {
            if (l_sep[i].empty())
                continue;
        
             if (m_exprtable.find(l_sep[i]) == m_exprtable.end())
                 throw exception::runtime(_("variable for optimization is not found in the expression symbol table"), *this);
             
             m_derivationvars.push_back( l_sep[i] );
        }
        
        
        // create full expression, symboltable and parse the function
        GiNaC::symtab l_table(m_exprtable);
        
        // we copy the main expression into the definition variable name
        // than we parse the error function, GiNaC adds the main expression
        // into the error function
        l_table[p_funcname] = m_expression;
        GiNaC::parser l_parser(l_table);

        try {
            m_full      = l_parser( p_errfunc );
            
            // symbolic table for concat function and remove "functionname"
            m_fulltable = l_parser.get_syms();
            m_fulltable.erase(p_funcname);
        } catch (...) {
            throw exception::runtime(_("arithmetic expression could not be parsed"), *this);
        }
        
        // checks number of variables (target symbolic var, so increment +1)
        if (m_exprtable.size()+1 != m_fulltable.size())
            throw exception::runtime(_("only one variable for the data must be added"), *this);        
    }
    
    
    
    /** sets init values for optimizing values. Real start value will be taken with random between [lower, upper]
     * @param p_name variable name
     * @param p_lower lower value
     * @param p_upper upper vlaue
     **/
    template<typename T, std::size_t D> inline void gradientdescent<T,D>::setOptimizeVar( const std::string& p_name, const T& p_lower, const T& p_upper )
    {
        if (std::find(m_derivationvars.begin(), m_derivationvars.end(), p_name) == m_derivationvars.end())
            throw exception::runtime(_("variable for optimization is not found in the expression symbol table"), *this);
        
        m_optimize[p_name] = std::pair<T,T>(p_lower, p_upper);
    }
    
    
    /** sets init values for optimizing values. The value will be used exactly
     * @param p_name variable name
     * @param p_value lower value
     **/
    template<typename T, std::size_t D> inline void gradientdescent<T,D>::setOptimizeVar( const std::string& p_name, const T& p_value )
    {
        setOptimizeVar(p_name, p_value, p_value);
    }
    
    
    
    /** sets the static variables that are constant during optimization
     * @param p_name variable name
     * @param p_data data array (multidimensional)
     **/
    template<typename T, std::size_t D> inline void gradientdescent<T,D>::setStaticVar( const std::string& p_name, const boost::multi_array<T,D>& p_data )
    {
        if ( (std::find(m_derivationvars.begin(), m_derivationvars.end(), p_name) != m_derivationvars.end()) || (m_fulltable.find(p_name) == m_fulltable.end()) )
            throw exception::runtime(_("static variable is not in the symbol table or is an optimazation variable"), *this);
        
        m_static[p_name] = p_data;
    }
    
    
    /** optimization method
     * @param p_iteration number of iterations
     * @param p_sampling number of samples for each static range
     * @param p_batch batch call of fotting
     * @return map with name and value
     * @todo adding parameter for setting number of start points and creating the correct thread group
     **/
    template<typename T, std::size_t D> inline std::map<std::string, T> gradientdescent<T,D>::optimize( const std::size_t& p_iteration, const std::size_t& p_sampling, const std::vector<std::string>& p_batch ) const
    {
        if (p_iteration == 0)
            throw exception::runtime(_("iterations must be greater than zero"), *this);
        if (p_sampling == 0 )
            throw exception::runtime(_("stepsize must be greater than zero"), *this);
        
        // all variables must be set to a numerical value, so we check it
        if (m_static.size() + m_optimize.size() != m_fulltable.size())
            throw exception::runtime(_("there are unsed variables"), *this);
                
           
        // creating worker
        std::vector< worker > l_worker;
        
        // if only one thread is used, we create the worker object directly and run it
        if (boost::thread::hardware_concurrency() == 1) {
            l_worker.push_back(  worker(p_iteration, p_sampling, expression2string(m_expression), expression2string(m_full), m_derivationvars, m_optimize, m_static, p_batch)  );
            l_worker[0].optimize();
        } else { 
            boost::thread_group l_threadgroup;
            for(std::size_t i=0; i < boost::thread::hardware_concurrency(); ++i) {
                l_worker.push_back( worker(p_iteration, p_sampling, expression2string(m_expression), expression2string(m_full), m_derivationvars, m_optimize, m_static, p_batch)  );
                l_threadgroup.create_thread(  boost::bind( &worker::optimize, &l_worker[i] )  );
            }
        
            // run threads and wait during all finished
            l_threadgroup.join_all();
        }
        
        // get data and creates best values
        std::map<std::string, T> x;
        return x;
        
    }
     
    
    //======= Multithreading Workerclass ==================================================================================================================================
    
    /** The constructor is only called by the gradient class, so we must not check
     * the parameter
     * @param p_iteration maximum iterations
     * @param p_sampling initializsation stepsize
     * @param p_function function string
     * @param p_errorfunction error function
     * @param p_derivationvars map with all derivations
     * @param p_initvalues map with values and optimize range
     * @param p_staticvalues map with multidimensional array for static variables
     * @param p_batch vector with information wich variable can be optimize together
     **/
    template<typename T, std::size_t D> inline gradientdescent<T,D>::worker::worker(  
        const std::size_t& p_iteration, 
        const std::size_t& p_sampling, 
        const std::string& p_function,
        const std::string& p_errorfunction,
        const std::vector<std::string>& p_derivationvars, 
        const std::map<std::string, std::pair<T,T> >& p_initvalues, 
        const std::map<std::string, boost::multi_array<T,D> >& p_staticvalues, 
        const std::vector<std::string>& p_batch  
    ) :
    m_iteration( p_iteration ),
    m_sampling( p_stepsize ),
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
            throw exception::runtime(_("arithmetic expression could not be parsed"), *this);
        }
    }
    
    
    
    /** returns the calculated values 
     * @return std::map with value and variable name
     **/
    template<typename T, std::size_t D> inline std::map<std::string, T> gradientdescent<T,D>::worker::getResult( void ) const
    {
        return m_result;
    }
    
    
    /** optimize value with gradient descent **/
    template<typename T, std::size_t D> inline void gradientdescent<T,D>::worker::optimize( void ) 
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
    
}}
#endif
#endif
