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
#include <sstream>
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
        void create( const std::string&, const std::string& = "function", const std::string& = "0.5 * (target - (function))^2", const std::string& = " ,;" );
        
        template<typename T> void blub();
        
        
        private :
            
            /** expression for the function **/
            GiNaC::ex m_expression;

        
    };

    
    
    /** constructor
     * @param p_func arithmetic expression
     **/
    inline gradientdescent::gradientdescent( const std::string& p_func ) :
        m_expression()
    {
        if (p_func.empty())
            throw exception::parameter(_("function need not be empty"));
        
        
        // create symbolic extression witch strict parsing
        GiNaC::parser l_parser;
        
        try {
            m_expression = l_parser( p_func );
        } catch (...) {
            throw exception::parameter(_("arithmetic expression could not be parsed"));
        }
    
    }
    
    
    /** creates the gradient function (default sum-of-squared-error / SSE)
     * @param p_optimizevars is a list of variables in the original formula which will be optimized (vars musst be separated with spaces, comma or semicolon, see separator)
     * @param p_funcname string name in which will be set the function
     * @param p_errfunc error function in wich must be set the name from p_funcname
     * @param p_separator separator charaters (default space, comma and semicolon)
     **/
    inline void gradientdescent::create( const std::string& p_optimizevars, const std::string& p_funcname, const std::string& p_errfunc, const std::string& p_separator )
    {
        if (p_errfunc.empty())
            throw exception::parameter(_("error function need not be empty"));
        if (p_funcname.empty())
            throw exception::parameter(_("variable name for the function need not be empty"));
        if (p_separator.empty())
            throw exception::parameter(_("separators need not be empty"));
        
        // create parser and other structures
        GiNaC::symtab l_table;
        GiNaC::ex l_full;   
        
        // set name for function and insert the expression
        l_table[p_funcname] = m_expression;
        GiNaC::parser l_parser(l_table);
        
        // create full function
        try {
            l_full = l_parser( p_errfunc );
        } catch (...) {
            throw exception::parameter(_("arithmetic expression could not be parsed"));
        }
        
        // seperates variables and create derivations
        std::vector<std::string> l_vars;
        boost::split( l_vars, p_optimizevars, boost::is_any_of(p_separator) );
        
        std::cout << l_full << std::endl;
                
        /*for(std::size_t i=0; i < l_vars.size(); ++i) {
            GiNaC::symbol x(l_vars[i]);
            
            std::cout << l_full.diff(x, 1) << std::endl;
        }*/
                
    }
    
    

    
    
};};

#endif
