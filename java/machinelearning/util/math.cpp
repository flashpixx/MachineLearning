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

#include <stdexcept>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "math.h"
#include "../../java.h"


namespace java  = machinelearning::java;
namespace tools = machinelearning::tools;
namespace ublas = boost::numeric::ublas;


/* $LastChangedDate$ */


/** returns eigenvalues and -vectors for a float matrix
 * @param p_env JNI environment
 * @param p_matrix input matrix
 * @param p_eigenvalues returning eigenvalues
 * @param p_eigenvectors returning eigenvectors
 **/
JNIEXPORT void JNICALL Java_machinelearning_util_Math_eigen___3_3Ljava_lang_Float_2_3Ljava_lang_Float_2_3_3Ljava_lang_Float_2(JNIEnv* p_env, jclass, jobjectArray p_matrix, jobjectArray p_eigenvalues, jobjectArray p_eigenvectors)
{
    
}


/** returns eigenvalues and -vectors for a double matrix
 * @param p_env JNI environment
 * @param p_matrix input matrix
 * @param p_eigenvalues returning eigenvalues
 * @param p_eigenvectors returning eigenvectors
 **/
JNIEXPORT void JNICALL Java_machinelearning_util_Math_eigen___3_3Ljava_lang_Double_2_3Ljava_lang_Double_2_3_3Ljava_lang_Double_2(JNIEnv* p_env, jclass, jobjectArray p_matrix, jobjectArray p_eigenvalues, jobjectArray p_eigenvectors)
{
    
}


/** returns the singular-value-decomposition for float values
 * @param p_env JNI environment
 * @param p_matrix input matrix
 * @param p_eigenvalues returning eigenvalues
 * @param p_eigenvec1 returning eigenvectors
 * @param p_eigenvec2 returning eigenvectors
 **/
JNIEXPORT void JNICALL Java_machinelearning_util_Math_svd___3_3Ljava_lang_Float_2_3Ljava_lang_Float_2_3_3Ljava_lang_Float_2_3_3Ljava_lang_Float_2(JNIEnv* p_env, jclass, jobjectArray p_matrix, jobjectArray p_eigenvalues, jobjectArray p_eigenvec1, jobjectArray p_eigenvec2)
{
    
}


/** returns the singular-value-decomposition for double values
 * @param p_env JNI environment
 * @param p_matrix input matrix
 * @param p_eigenvalues returning eigenvalues
 * @param p_eigenvec1 returning eigenvectors
 * @param p_eigenvec2 returning eigenvectors
 **/
JNIEXPORT void JNICALL Java_machinelearning_util_Math_svd___3_3Ljava_lang_Double_2_3Ljava_lang_Double_2_3_3Ljava_lang_Double_2_3_3Ljava_lang_Double_2(JNIEnv* p_env, jclass, jobjectArray p_matrix, jobjectArray p_eigenvalues, jobjectArray p_eigenvec1, jobjectArray p_eigenvec2)
{
    
}


/** creates the largest eigenvector for float type with perron-frobenius-algorithm
 * @param p_env JNI environment
 * @param p_matrix input matrix
 * @param p_iteration number of iterations
 * @return eigenvector
 **/
JNIEXPORT jobjectArray JNICALL Java_machinelearning_util_Math_perronfrobenius___3_3Ljava_lang_Float_2I(JNIEnv* p_env, jclass, jobjectArray p_matrix, jint p_iteration)
{
    
}


/** creates the largest eigenvector for double type with perron-frobenius-algorithm
 * @param p_env JNI environment
 * @param p_matrix input matrix
 * @param p_iteration number of iterations
 * @return eigenvector
 **/
JNIEXPORT jobjectArray JNICALL Java_machinelearning_util_Math_perronfrobenius___3_3Ljava_lang_Double_2I(JNIEnv* p_env, jclass, jobjectArray p_matrix, jint p_iteration)
{
    
}