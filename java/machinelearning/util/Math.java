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



/** mathematic algorithms
 * $LastChangedDate$
 **/
public class Math extends machinelearning.Object
{
    
    /** creates eigenvalues and -vectors of a squared matrix for float types
     * @param p_matrix input matrix
     * @return eigenobject
     **/
    public static native Eigen<Float> eigen( Float[][] p_matrix );
    
    /** creates eigenvalues and -vectors of a squared matrix for double types
     * @param p_matrix input matrix
     * @return eigenobject
     **/
    public static native Eigen<Double> eigen( Double[][] p_matrix );
    
    /** creates the singular value decomposition for float types
     * @param p_matrix input matrix
     * @return SVD object
     **/
    public static native SVD<Float> svd( Float[][] p_matrix );
    
    /** creates the singular value decomposition for double types
     * @param p_matrix input matrix
     * @return SVD object
     **/
    public static native SVD<Double> svd( Double[][] p_matrix );
    
    /** returns the largest eigenvector with perron-frobenius algorithm for float types
     * @param p_matrix input matrix
     * @param p_iteration number of iterations
     * @return eigenvector 
     **/
    public static native Float[] perronfrobenius( Float[][] p_matrix, int p_iteration );
    
    /** returns the largest eigenvector with perron-frobenius algorithm for double types
     * @param p_matrix input matrix
     * @param p_iteration number of iterations
     * @return eigenvector 
     **/
    public static native Double[] perronfrobenius( Double[][] p_matrix, int p_iteration );
    
    /** dispose must exists, because of derivating object, but all calls are static, so it does nothing **/
    public void dispose() {}
    
}