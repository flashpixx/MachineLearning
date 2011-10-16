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
     * @param p_eigenvalues return eigenvalues
     * @param p_eigenvectors return 2D array with eigenvectors (array elements p_eigenvector[0..n-1][i] is the i-th eigenvector)
     **/
    public static native void eigen( Float[][] p_matrix, Float[] p_eigenvalues, Float[][] p_eigenvectors );
    
    /** creates eigenvalues and -vectors of a squared matrix for double types
     * @param p_matrix input matrix
     * @param p_eigenvalues return eigenvalues
     * @param p_eigenvectors return 2D array with eigenvectors (array elements p_eigenvector[0..n-1][i] is the i-th eigenvector)
     **/
    public static native void eigen( Double[][] p_matrix, Double[] p_eigenvalues, Double[][] p_eigenvectors );
    
    /** creates the singular value decomposition for float types
     * @param p_matrix input matrix
     * @param p_svdval eigenvalues
     * @param p_svdvec1 eigenvectors (equal to eigen)
     * @param p_svdvec2 eigenvectors (equal to eigen)
     **/
    public static native void svd( Float[][] p_matrix, Float[] p_svdval, Float[][] p_svdvec1, Float[][] p_svdvec2 );
    
    /** creates the singular value decomposition for double types
     * @param p_matrix input matrix
     * @param p_svdval eigenvalues
     * @param p_svdvec1 eigenvectors (equal to eigen)
     * @param p_svdvec2 eigenvectors (equal to eigen)
     **/
    public static native void svd( Double[][] p_matrix, Double[] p_svdval, Double[][] p_svdvec1, Double[][] p_svdvec2 );
    
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