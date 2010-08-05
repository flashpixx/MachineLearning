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

#ifndef MACHINELEARNING_FUNCTIONALOPTIMIZATION_GRADIENTDESCENT_HPP
#define MACHINELEARNING_FUNCTIONALOPTIMIZATION_GRADIENTDESCENT_HPP

#include <string>
#include <boost/algorithm/string.hpp> 
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <ginac/ginac.h>


#include "../exception/exception.h"
#include "../tools/tools.h"



namespace machinelearning { namespace functionaloptimization {
    
    namespace ublas = boost::numeric::ublas;
    
    
    /** class for using a (stochastic) gradient descent
     * symbolic numerical algorithms @see http://www.ginac.de
     **/
    class gradientdescent {
    
        public :

            gradientdescent( const std::string& );
        void set( const std::string&, const std::string& = "0.5 * (target+function)^2", const std::string& = "function", const std::string& = " ,;" );
        
        template<typename T> void blub();
        
        
        private :
            
            /** expression for the function **/
            GiNaC::ex m_expression;
            /** symbols table for the function **/
            GiNaC::symtab m_exprtable;
            /** symbols table for gradient function **/
            GiNaC::symtab m_difftable;
            /** derivations **/
            std::vector<GiNaC::ex> m_derivation;

        
    };

    
    
    /** constructor
     * @param p_func arithmetic expression
     **/
    inline gradientdescent::gradientdescent( const std::string& p_func ) :
        m_expression(),
        m_exprtable(),
        m_difftable(),
        m_derivation()
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
     **/
    inline void gradientdescent::set( const std::string& p_optimizevars, const std::string& p_errfunc, const std::string& p_funcname, const std::string& p_separator )
    {
        if (p_errfunc.empty())
            throw exception::parameter(_("error function need not be empty"));
        if (p_funcname.empty())
            throw exception::parameter(_("variable name for the function need not be empty"));
        if (p_separator.empty())
            throw exception::parameter(_("separators need not be empty"));
        
        // check if symbols for optimization are in the table
        std::vector<std::string> l_sep;
        std::vector<std::string> l_vars;
        boost::split( l_sep, p_optimizevars, boost::is_any_of(p_separator) );
               
        // during spearation empty vector entries can be created, so we check if is not empty and in the symbolic table, than we copy
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
            m_difftable = l_parser.get_syms();
        } catch (...) {
            throw exception::parameter(_("arithmetic expression could not be parsed"));
        }

        
        // checks number of variables (target and function are symbolic vars, so increment +2)
        //if (m_exprtable.size()+2 != m_difftable.size())
        //    throw exception::parameter(_("only one variable for the data must be added"));
        
        
        // create first derivation for each variable, which should be fitted
        m_derivation.clear();
        for(std::size_t i=0; i < l_vars.size(); ++i)
            m_derivation.push_back(  l_full.diff( GiNaC::ex_to<GiNaC::symbol>(m_difftable[l_vars[i]]), 1)  );
            

        
        
        std::cout << l_full << std::endl << std::endl;
        for(std::size_t i=0; i < m_derivation.size(); ++i)
            std::cout << m_derivation[i]  << std::endl;
                
    }
    
    

    
    
};};

#endif
