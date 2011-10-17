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

/** class for storing SVD information
 * $LastChangedDate$
 **/
public class SVD<T extends Number>
{
    
    /** array with SVD values **/
    private T[] m_values    = null;
    /** array with SVD vectors (MxN) **/
    private T[][] m_vectors1 = null;
    /** array with SVD vectors (NxM) **/
    private T[][] m_vectors2 = null;
    
    
    /** constructor
     * @param p_values array with SVD values
     * @param p_vectors1 array with SVD vectors (MxN)
     * @param p_vectors2 array with SVD vectors (NxM)
     **/
    public SVD( T[] p_values, T[][] p_vectors1, T[][] p_vectors2 )
    {
        if ( (p_values == null) || (p_vectors1 == null) || (p_vectors1 == null) )
            throw new NullPointerException("array object must not be null");
        
        m_values   = p_values;
        m_vectors1 = p_vectors1;
        m_vectors2 = p_vectors2;
    }
    
    /** returns array with SVD values
     * @return array with SVD values
     **/
    public T[] getValues() { return m_values; }
    
    /** returns array with SVD vectors (MxN)
     * @returns 2D matrix with SVD vectors (i-th row = i-th SVD vector)
     **/
    public T[][] getVectors1() { return m_vectors1; }
    
    /** returns array with SVD vectors (MxN)
     * @returns 2D matrix with SVD vectors (i-th row = i-th SVD vector)
     **/
    public T[][] getVectors2() { return m_vectors2; }
}