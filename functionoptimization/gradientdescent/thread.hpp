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

#ifndef MACHINELEARNING_FUNCTIONALOPTIMIZATION_GRADIENTDESCENT_THREAD_HPP
#define MACHINELEARNING_FUNCTIONALOPTIMIZATION_GRADIENTDESCENT_THREAD_HPP

#include <map>
#include <ginac/ginac.h>
#include <boost/algorithm/string.hpp> 
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/multi_array.hpp>
#include <boost/static_assert.hpp>
#include <boost/thread.hpp>



namespace machinelearning { namespace functionaloptimization { namespace thread {
    
    namespace ublas = boost::numeric::ublas;
    
    
    /** class for worker thread for calulating gradient values **/
    template<typename T, std::size_t D> class gradientdescent {
            BOOST_STATIC_ASSERT(D > 0);         // array dimension must be greater than 0
        
        
        public :
        
            gradientdescent(    const std::map<std::string, GiNaC::ex>&, 
                                const std::map<std::string, std::pair<T,T> >&, 
                                const std::map<std::string, boost::multi_array<T,D> >&,
                                const std::vector<std::string>&
                           );
        
            std::map<std::string, T> getResult( void ) const;
        
        
        private :
        
            /** map with derivation **/
            const std::map<std::string, GiNaC::ex> m_derivation;
            /** map with initialisation values **/
            const std::map<std::string, std::pair<T,T> > m_initvalues;
            /** map with static values **/
            const std::map<std::string, boost::multi_array<T,D> > m_staticvalues;
            /** vector with variables for batch optimize **/
            const std::vector<std::string> m_batch;
            /** map with optimized values **/
            std::map<std::string, T> m_result;
        
        
    };
    
    
    
    template<typename T, std::size_t D> inline gradientdescent<T,D>::gradientdescent(  const std::map<std::string, GiNaC::ex>& p_derivation, const std::map<std::string, std::pair<T,T> >& p_initvalues, const std::map<std::string, boost::multi_array<T,D> >& p_staticvalues, const std::vector<std::string>& p_batch  ) :
        m_derivation( p_derivation ),
        m_initvalues( p_initvalues ),
        m_staticvalues( p_staticvalues ),
        m_batch( p_batch )
    {}
    
    
    
    
    template<typename T, std::size_t D> inline std::map<std::string, T> gradientdescent<T,D>::getResult( void ) const
    {
        return m_result;
    }



};};};
#endif
