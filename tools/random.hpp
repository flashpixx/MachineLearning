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


#ifndef MACHINELEARNING_TOOLS_RANDOM_HPP
#define MACHINELEARNING_TOOLS_RANDOM_HPP


#include <ctime>
#include <limits>
#include <boost/thread/thread.hpp>
#include <boost/random.hpp>

#ifdef RANDOMDEVICE
#include <boost/nondet_random.hpp>
#endif

#include <boost/math/distributions/beta.hpp>
#include <boost/math/distributions/students_t.hpp>
#include <boost/math/distributions/weibull.hpp>
#include <boost/math/distributions/rayleigh.hpp>
#include <boost/math/distributions/pareto.hpp>
#include <boost/math/distributions/chi_squared.hpp>
#include <boost/math/distributions/triangular.hpp>


#include "../exception/exception.h"
#include "function.hpp"
#include "language/language.h"


namespace machinelearning { namespace tools {
    
    
    /** class for using some random structures. Pseudo generator (Mersenne Twister) and
     * system-random-generator can be used. The class holds different distribution that
     * can be used. The system-random-generator musst be set with the compileflag 
     * "DEVICERANDOM"
     **/
    class random {
        
        public :
        
            enum distribution
            {
                uniform     = 0,
                binomial    = 1,
                bernoulli   = 2,
                cauchy      = 3,
                gamma       = 4,
                poisson     = 5,
                exponential = 6,
                normal      = 7,
                student     = 9,
                weibull     = 10,
                rayleigh    = 11,
                chisquared  = 12,
                pareto      = 13,
                triangular  = 14
            };
            
            
     
            template<typename T> static T get( const distribution&, const T& = std::numeric_limits<T>::epsilon(), const T& = std::numeric_limits<T>::epsilon(), const T& = std::numeric_limits<T>::epsilon() );
            
        
        private :
        
            #ifdef RANDOMDEVICE
            /** static random device pbject **/
            static boost::random_device m_random;
            #else
            /** mersenne twister object **/
            static boost::mt19937 m_random;
            #endif
            
            template<typename T> static T getUniform( const T&, const T& );
            template<typename T> static T getBinomial( const T&, const T& );
            template<typename T> static T getBernoulli( const T& );
            template<typename T> static T getCauchy( const T&, const T& );
            template<typename T> static T getGamma( const T& );
            template<typename T> static T getPoisson( const std::size_t& );
            template<typename T> static T getExponential( const T& );
            template<typename T> static T getNormal( const T&, const T& );
            template<typename T> static T getStudent( const T& );
            template<typename T> static T getWeibull( const T&, const T& );
            template<typename T> static T getRayleigh( const T& );
            template<typename T> static T getBeta( const T&, const T& );
            template<typename T> static T getPareto( const T&, const T& );
            template<typename T> static T getChiSquared( const T& );
            template<typename T> static T getTriangular( const T&, const T&, const T& );
        
    };
    
    
    
    /** returns a number from a pseudo random generator. Default values are set with the numerical limits for checking
     * because every distribution has other default values
     * @param p_distribution enum with distribution
     * @param p_first first parameter for distribution
     * @param p_second second parameter for distribution
     * @param p_third third parameter for distribution
     * @return value of the distribution
     **/
    template<typename T> inline T random::get( const distribution& p_distribution, const T& p_first, const T& p_second, const T& p_third )
    {
        switch (p_distribution)
        {
            case uniform     :       return getUniform(    (function::isNumericalZero<T>(p_first) ? 0 : p_first),   (function::isNumericalZero<T>(p_second) ? 1 : p_second)  );
            case bernoulli   :       return getBernoulli(  (function::isNumericalZero<T>(p_first) ? static_cast<T>(0.5) : p_first)  );
            case binomial    :       return getBinomial(   (function::isNumericalZero<T>(p_first) ? 1 : p_first),   (function::isNumericalZero<T>(p_second) ? static_cast<T>(0.5) : p_second)  );
            case cauchy      :       return getCauchy(     (function::isNumericalZero<T>(p_first) ? 0 : p_first),   (function::isNumericalZero<T>(p_second) ? 1 : p_second));
            case gamma       :       return getGamma(      (function::isNumericalZero<T>(p_first) ? 1 : p_first)  );
            case poisson     :       return getPoisson<T>( (function::isNumericalZero<T>(p_first) ? 1 : static_cast<std::size_t>(p_first))  );
            case exponential :       return getExponential((function::isNumericalZero<T>(p_first) ? 1 : p_first)  );
            case normal      :       return getNormal(     (function::isNumericalZero<T>(p_first) ? 0 : p_first),   (function::isNumericalZero<T>(p_second) ? 1 : p_second));
            case student     :       return getStudent(    (function::isNumericalZero<T>(p_first) ? 1 : p_first)  );
            case weibull     :       return getWeibull(    (function::isNumericalZero<T>(p_first) ? 0 : p_first),   (function::isNumericalZero<T>(p_second) ? 1 : p_second)  );
            case rayleigh    :       return getRayleigh(   (function::isNumericalZero<T>(p_first) ? 1 : p_first)  );
            case chisquared  :       return getChiSquared( (function::isNumericalZero<T>(p_first) ? 1 : p_first)  );
            case pareto      :       return getPareto(     (function::isNumericalZero<T>(p_first) ? 0 : p_first),   (function::isNumericalZero<T>(p_second) ? 1 : p_second)  );
            case triangular  :       return getTriangular( (function::isNumericalZero<T>(p_first) ? 0 : p_first),   (function::isNumericalZero<T>(p_second) ? static_cast<T>(0.5) : p_second),  (function::isNumericalZero<T>(p_third) ? 1 : p_third) );
        }
        
        throw exception::parameter(_("distribution is not kwon"));
    }
    
    
    /** get a pseudo uniform random number 
     * @param p_min min value
     * @param p_max max value
     * @return uniform random number in [min,max)
     **/
    template<typename T> inline T random::getUniform( const T& p_min, const T& p_max )
    {
        boost::uniform_real<T> l_range(p_min, p_max);
        
        #ifdef RANDOMDEVICE
        boost::variate_generator<boost::random_device&, boost::uniform_real<T> > l_noise( m_random, l_range );
        #else
        boost::variate_generator<boost::mt19937&, boost::uniform_real<T> > l_noise(  m_random, l_range );
        #endif
        
        return l_noise();
    }
    
    
    /** returns a bernoulli distribution value 
     * @param p_prop probability in [0,1] for seperate the two classes
     * @return 0 or 1 with probability 
     **/
    template<typename T> inline T random::getBernoulli( const T& p_prop )
    {
        boost::bernoulli_distribution<T> l_range(p_prop);
        
        #ifdef RANDOMDEVICE
        boost::variate_generator<boost::random_device&, boost::bernoulli_distribution<T> > l_noise( m_random, l_range );
        #else
        boost::variate_generator<boost::mt19937&, boost::bernoulli_distribution<T> > l_noise(  m_random, l_range );
        #endif
        
        return l_noise();
    }
    
    
    /** returns a binomial distribution value 
     * @param p_n n value
     * @param p_p p value
     * @return binomial random number
     **/
    template<typename T> inline T random::getBinomial( const T& p_n, const T& p_p )
    {
        boost::binomial_distribution<T> l_range(p_n, p_p);
        
        #ifdef RANDOMDEVICE
        boost::variate_generator<boost::random_device&, boost::binomial_distribution<T> > l_noise( m_random, l_range );
        #else
        boost::variate_generator<boost::mt19937&, boost::binomial_distribution<T> > l_noise(  m_random, l_range );
        #endif
        
        return l_noise();
    }
    
    
    /** returns a cauchy distribution value 
     * @param p_loc localisation
     * @param p_scale scale
     * @return cauchy random number
     **/
    template<typename T> inline T random::getCauchy( const T& p_loc, const T& p_scale )
    {
        boost::cauchy_distribution<T> l_range(p_loc, p_scale);
        
        #ifdef RANDOMDEVICE
        boost::variate_generator<boost::random_device&, boost::cauchy_distribution<T> > l_noise( m_random, l_range );
        #else
        boost::variate_generator<boost::mt19937&, boost::cauchy_distribution<T> > l_noise(  m_random, l_range );
        #endif
        
        return l_noise();
    }
    
    
    /** returns a gamma/erlang distribution value 
     * @param p_shape parameter for dense function
     * @return gamma/erlang distribution value
     **/
    template<typename T> inline T random::getGamma( const T& p_shape )
    {
        boost::gamma_distribution<T> l_range(p_shape);
        
        #ifdef RANDOMDEVICE
        boost::variate_generator<boost::random_device&, boost::gamma_distribution<T> > l_noise( m_random, l_range );
        #else
        boost::variate_generator<boost::mt19937&, boost::gamma_distribution<T> > l_noise(  m_random, l_range );
        #endif
        
        return l_noise();
    }    
    
    
    /** returns a poisson distribution value 
     * @param p_lambda lambda parameter
     * @return poisson distribution value
     **/
    template<typename T> inline T random::getPoisson( const std::size_t& p_lambda )
    {
        boost::poisson_distribution<std::size_t> l_range(p_lambda);
        
        #ifdef RANDOMDEVICE
        boost::variate_generator<boost::random_device&, boost::poisson_distribution<std::size_t> > l_noise( m_random, l_range );
        #else
        boost::variate_generator<boost::mt19937&, boost::poisson_distribution<std::size_t> > l_noise(  m_random, l_range );
        #endif
        
        return l_noise();
    }    
    
    
    /** returns a exponential distribution value 
     * @param p_lambda lambda parameter
     * @return exponential distribution value
     **/
    template<typename T> inline T random::getExponential( const T& p_lambda )
    {
        boost::exponential_distribution<T> l_range(p_lambda);
        
        #ifdef RANDOMDEVICE
        boost::variate_generator<boost::random_device&, boost::exponential_distribution<T> > l_noise( m_random, l_range );
        #else
        boost::variate_generator<boost::mt19937&, boost::exponential_distribution<T> > l_noise(  m_random, l_range );
        #endif
        
        return l_noise();
    }  
    
    
    /** returns a normal distribution value 
     * @param p_mean mean value
     * @param p_sd 2nd derevation (variance)
     * @return normal random number
     **/
    template<typename T> inline T random::getNormal( const T& p_mean, const T& p_sd )
    {
        boost::normal_distribution<T> l_range(p_mean, p_sd);
        
        #ifdef RANDOMDEVICE
        boost::variate_generator<boost::random_device&, boost::normal_distribution<T> > l_noise( m_random, l_range );
        #else
        boost::variate_generator<boost::mt19937&, boost::normal_distribution<T> > l_noise(  m_random, l_range );
        #endif
        
        return l_noise();
    }
    
    
    /** returns student distribution value 
     * @param p_v v value
     * @return student random number
     **/
    template<typename T> inline T random::getStudent( const T& p_v )
    {
        boost::math::students_t_distribution<T> l_range(p_v);
        
        #ifdef RANDOMDEVICE
        boost::variate_generator<boost::random_device&, boost::uniform_01<T> > l_uniform(  m_random, boost::uniform_01<T>() );
        #else
        boost::variate_generator<boost::mt19937&, boost::uniform_01<T> > l_uniform(  m_random, boost::uniform_01<T>() );
        #endif
        
        return quantile(l_range, l_uniform());
    }
    
    
    /** returns a weibull distribution value 
     * @param p_shape shape value
     * @param p_scale scale value
     * @return weibull random number
     **/
    template<typename T> inline T random::getWeibull( const T& p_shape, const T& p_scale )
    {
        boost::math::weibull_distribution<T> l_range(p_shape, p_scale);
        
        #ifdef RANDOMDEVICE
        boost::variate_generator<boost::random_device&, boost::uniform_01<T> > l_uniform(  m_random, boost::uniform_01<T>() );
        #else
        boost::variate_generator<boost::mt19937&, boost::uniform_01<T> > l_uniform(  m_random, boost::uniform_01<T>() );
        #endif
        
        return quantile(l_range, l_uniform());
    }
    
    
    /** returns a rayleigh distribution value 
     * @param p_sigma sigma value
     * @return rayleigh random number
     **/
    template<typename T> inline T random::getRayleigh( const T& p_sigma )
    {
        boost::math::rayleigh_distribution<T> l_range(p_sigma);
        
        #ifdef RANDOMDEVICE
        boost::variate_generator<boost::random_device&, boost::uniform_01<T> > l_uniform(  m_random, boost::uniform_01<T>() );
        #else
        boost::variate_generator<boost::mt19937&, boost::uniform_01<T> > l_uniform(  m_random, boost::uniform_01<T>() );
        #endif
        
        return quantile(l_range, l_uniform());
    }
    
    
    /** returns a beta distribution value
     * @param p_alpha alpha value
     * @param p_beta beta value
     * @return beta random number
     **/
    template<typename T> inline T random::getBeta( const T& p_alpha, const T& p_beta )
    {
        boost::math::beta_distribution<T> l_range(p_alpha, p_beta);
        
        #ifdef RANDOMDEVICE
        boost::variate_generator<boost::random_device&, boost::uniform_01<T> > l_uniform(  m_random, boost::uniform_01<T>() );
        #else
        boost::variate_generator<boost::mt19937&, boost::uniform_01<T> > l_uniform(  m_random, boost::uniform_01<T>() );
        #endif
        
        return quantile(l_range, l_uniform());
    }
    
    
    /** returns a pareto distribution value
     * @param p_loc location
     * @param p_scale scale
     * @return pareto random number
     **/
    template<typename T> inline T random::getPareto( const T& p_loc, const T& p_scale )
    {
        boost::math::pareto_distribution<T> l_range(p_loc, p_scale);
        
        #ifdef RANDOMDEVICE
        boost::variate_generator<boost::random_device&, boost::uniform_01<T> > l_uniform(  m_random, boost::uniform_01<T>() );
        #else
        boost::variate_generator<boost::mt19937&, boost::uniform_01<T> > l_uniform(  m_random, boost::uniform_01<T>() );
        #endif
        
        return quantile(l_range, l_uniform());
    }
    
    
    /** returns a chi^2 distribution value
     * @param p_v freedom deegre
     * @return chi^2 random number
     **/
    template<typename T> inline T random::getChiSquared( const T& p_v )
    {
        boost::math::chi_squared_distribution<T> l_range(p_v);
        
        #ifdef RANDOMDEVICE
        boost::variate_generator<boost::random_device&, boost::uniform_01<T> > l_uniform(  m_random, boost::uniform_01<T>() );
        #else
        boost::variate_generator<boost::mt19937&, boost::uniform_01<T> > l_uniform(  m_random, boost::uniform_01<T>() );
        #endif
        
        return quantile(l_range, l_uniform());
    }
    
    
    /** returns a triangular distribution value
     * @param p_min min value
     * @param p_center peak / center value
     * @param p_max max value
     * @return triangular random number
     **/
    template<typename T> inline T random::getTriangular( const T& p_min, const T& p_center, const T& p_max )
    {
        boost::math::triangular_distribution<T> l_range(p_min, p_center, p_max);
        
        #ifdef RANDOMDEVICE
        boost::variate_generator<boost::random_device&, boost::uniform_01<T> > l_uniform(  m_random, boost::uniform_01<T>() );
        #else
        boost::variate_generator<boost::mt19937&, boost::uniform_01<T> > l_uniform(  m_random, boost::uniform_01<T>() );
        #endif
        
        return quantile(l_range, l_uniform());
    }
    
    
    
};};


#endif