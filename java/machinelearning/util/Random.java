/** 
 @cond
 ############################################################################
 # LGPL License                                                             #
 #                                                                          #
 # This file is part of the Machine Learning Framework.                     #
 # Copyright (c) 2010, Philipp Kraus, <philipp.kraus@flashpixx.de>          #
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

package machinelearning.util;



/** random / distribution generator
 * $LastChangedDate$
 **/
public class Random extends machinelearning.Object
{
    public enum Distribution
    {
        UNIFORM, BERNOULLI, CAUCHY, GAMMA, POISSON, EXPONENTIAL, NORMAL, STUDENT, WEIBULL, RAYLEIGH, CHISQUARED, PARETO, TRIANGULAR, BETA
    }
    

    /** private pointer member **/
    private final long cpp_ptr;
    
    
    public Random() { cpp_ptr = cppCtor(); }
    
    
    /** release objects **/
    public native void dispose();
    
    
    /** creates a float value of the distribution with default parameter
     * @param p_distribution distribution
     * @return random value
     **/
    public native Float getFloat( Distribution p_distribution );
    
    
    /** creates a float value of the distribution
     * @param p_distribution distribution
     * @param p_first first configuration value of the distribution
     * @return random value
     **/
    public native Float get( Distribution p_distribution, Float p_first );
    
    
    /** creates a float value of the distribution
     * @param p_distribution distribution
     * @param p_first first configuration value of the distribution
     * @param p_second first configuration value of the distribution
     * @return random value
     **/
    public native Float get( Distribution p_distribution, Float p_first, Float p_second );
    
    
    /** creates a float value of the distribution
     * @param p_distribution distribution
     * @param p_first first configuration value of the distribution
     * @param p_second first configuration value of the distribution
     * @param p_third first configuration value of the distribution
     * @return random value
     **/
    public native Float get( Distribution p_distribution, Float p_first, Float p_second, Float p_third );
    
    
    /** creates a double value of the distribution with default parameter
     * @param p_distribution distribution
     * @return random value
     **/
    public native Double getDouble( Distribution p_distribution );
    
    
    /** creates a double value of the distribution
     * @param p_distribution distribution
     * @param p_first first configuration value of the distribution
     * @return random value
     **/
    public native Double get( Distribution p_distribution, Double p_first );
    
    
    /** creates a double value of the distribution
     * @param p_distribution distribution
     * @param p_first first configuration value of the distribution
     * @param p_second first configuration value of the distribution
     * @return random value
     **/
    public native Double get( Distribution p_distribution, Double p_first, Double p_second );
    
    
    /** creates a double value of the distribution
     * @param p_distribution distribution
     * @param p_first first configuration value of the distribution
     * @param p_second first configuration value of the distribution
     * @param p_third first configuration value of the distribution
     * @return random value
     **/
    public native Double get( Distribution p_distribution, Double p_first, Double p_second, Double p_third );
    
    
    
    /** JNI constructor call **/
    private native long cppCtor();
    
    /** finalizer **/
    protected void finalize() { try { dispose(); } catch(Exception e) {} }
}