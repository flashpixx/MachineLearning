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

package machinelearning.exception;



/** runtime exception class equal to the C++ exception
 * $LastChangedDate$
 **/
public class Runtime extends RuntimeException {

    
    /** constructor **/
    public Runtime() { super(); }
    
    /** constructor 
     * @param p_msg message
     **/
    public Runtime(String p_msg) { super(p_msg); }
    
    /** constructor
     * @param p_msg message
     * @param p_cause cause
     **/
    public Runtime(String p_msg, Throwable p_cause) { super(p_msg, p_cause); }
    
    /** constructor
     * @param p_cause
     **/
    public Runtime(Throwable p_cause) { super(p_cause); }


}