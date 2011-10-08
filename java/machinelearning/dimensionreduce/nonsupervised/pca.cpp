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

#include <boost/numeric/ublas/matrix.hpp>

#include "pca.h"
#include "../../../java.h"


namespace java  = machinelearning::java;
namespace tools = machinelearning::tools;
namespace dim   = machinelearning::dimensionreduce::nonsupervised;
namespace ublas = boost::numeric::ublas;


/* $LastChangedDate$ */

// ======= delegate for double =============================================================================================================================

/** field id that stores the ID that stores the pointer **/
static jfieldID fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_double    = NULL;

/** constructor call for delegate double
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_dim dimension value
 * @return pointer
 **/
JNIEXPORT jlong JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024DelegateDouble_cppCtor(JNIEnv* p_env, jobject p_object, jint p_dim)
{
    jlong l_ptr = 0;
    
    try {
        l_ptr = java::jni::createObjectPointer(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_double, new dim::pca<double>(p_dim));
    } catch (const std::exception& e) {
        p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), e.what() );
    }
    
    return l_ptr;
}

/** destructor call for delegate double
 * @param p_env JNI environment
 * @param p_object JNI object
 **/
JNIEXPORT void JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024DelegateDouble_dispose(JNIEnv* p_env, jobject p_object)
{
    java::jni::disposeObjectPointer< dim::pca<double> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_double);
}

/** returns the number of project dimension
 * @param p_env JNI environment
 * @param p_object JNI object
 * @return dimension number
 **/
JNIEXPORT jint JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024DelegateDouble_getDimension(JNIEnv* p_env, jobject p_object)
{
    dim::pca<double>* l_ptr = java::jni::getObjectPointer< dim::pca<double> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_double);
    return l_ptr->getDimension();
}

/** maps data to the target dimension
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_data input data
 * @return mapped data
 **/
JNIEXPORT jobjectArray JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024DelegateDouble_map(JNIEnv* p_env, jobject p_object, jobjectArray p_data)
{
    // check if the input array is a NULL object, than break with a default NullPointerException
    if (!p_data) {
        p_env->ThrowNew( p_env->FindClass("java/lang/NullPointerException"), _("data matrix is null") );
        return (jobjectArray)p_env->NewGlobalRef(NULL);
    }
    
    // convert input data and returning a null-object if an error occurs
    const ublas::matrix<double> l_data = java::jni::getDoubleMatrixFrom2DArray( p_env, p_data );    
    if ((l_data.size1() == 0) || (l_data.size2() == 0)) {
        p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("data matrix is empty") );
        return (jobjectArray)p_env->NewGlobalRef(NULL);
    }
    
    // run PCA with the data and catches exception and sets around zero values to exactly zero for java programmers
    dim::pca<double>* l_ptr = java::jni::getObjectPointer< dim::pca<double> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_double);
    ublas::matrix<double> l_result;
    try {
        l_result = tools::matrix::setNumericalZero(l_ptr->map(l_data));
    } catch (const std::exception& e) {
        p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), e.what() );
        return (jobjectArray)p_env->NewGlobalRef(NULL);
    }
    
    // returns the data within a java 2D array
    return java::jni::getJObjectArrayFromMatrix(p_env, l_result);
}

/** get the project vectors
 * @param p_env JNI environment
 * @param p_object JNI object
 * @return vector array
 **/
JNIEXPORT jobjectArray JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024DelegateDouble_getProject(JNIEnv* p_env, jobject p_object)
{
    dim::pca<double>* l_ptr = java::jni::getObjectPointer< dim::pca<double> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_double);
    return java::jni::getJObjectArrayFromMatrix(p_env, tools::matrix::setNumericalZero(l_ptr->getProject()));
}



// ======= delegate for float ==============================================================================================================================

/** field id that stores the ID that stores the pointer **/
static jfieldID fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_float     = NULL;

/** constructor call for delegate double
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_dim dimension value
 * @return pointer
 **/
JNIEXPORT jlong JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024DelegateFloat_cppCtor(JNIEnv* p_env, jobject p_object, jint p_dim)
{
    jlong l_ptr = 0;
    
    try {
        l_ptr = java::jni::createObjectPointer(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_float, new dim::pca<float>(p_dim));
    } catch (const std::exception& e) {
        p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), e.what() );
    }

    return l_ptr;
}

/** destructor call for delegate double
 * @param p_env JNI environment
 * @param p_object JNI object
 **/
JNIEXPORT void JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024DelegateFloat_dispose(JNIEnv* p_env, jobject p_object)
{
    java::jni::disposeObjectPointer< dim::pca<float> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_float);
}

/** returns the number of project dimension
 * @param p_env JNI environment
 * @param p_object JNI object
 * @return dimension number
 **/
JNIEXPORT jint JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024DelegateFloat_getDimension(JNIEnv* p_env, jobject p_object)
{
    dim::pca<float>* l_ptr = java::jni::getObjectPointer< dim::pca<float> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_float);
    return l_ptr->getDimension();
}

/** maps data to the target dimension
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_data input data
 * @return mapped data
 **/
JNIEXPORT jobjectArray JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024DelegateFloat_map(JNIEnv* p_env, jobject p_object, jobjectArray p_data)
{
    // check if the input array is a NULL object, than break with a default NullPointerException
    if (!p_data) {
        p_env->ThrowNew( p_env->FindClass("java/lang/NullPointerException"), _("data matrix is null") );
        return (jobjectArray)p_env->NewGlobalRef(NULL);
    }
    
    // convert input data and returning a null-object if an error occurs
    const ublas::matrix<float> l_data = java::jni::getFloatMatrixFrom2DArray( p_env, p_data );    
    if ((l_data.size1() == 0) || (l_data.size2() == 0)) {
        p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("data matrix is empty") );
        return (jobjectArray)p_env->NewGlobalRef(NULL);
    }
    
    // run PCA with the data and catches exception and sets around zero values to exactly zero for java programmers
    dim::pca<float>* l_ptr = java::jni::getObjectPointer< dim::pca<float> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_float);
    ublas::matrix<float> l_result;
    try {
        l_result = tools::matrix::setNumericalZero(l_ptr->map(l_data));
    } catch (const std::exception& e) {
        p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), e.what() );
        return (jobjectArray)p_env->NewGlobalRef(NULL);
    }
    
    // returns the data within a java 2D array
    return java::jni::getJObjectArrayFromMatrix(p_env, l_result);
    
}

/** get the project vectors
 * @param p_env JNI environment
 * @param p_object JNI object
 * @return vector array
 **/
JNIEXPORT jobjectArray JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024DelegateFloat_getProject(JNIEnv* p_env, jobject p_object)
{
    dim::pca<float>* l_ptr = java::jni::getObjectPointer< dim::pca<float> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_float);
    return java::jni::getJObjectArrayFromMatrix(p_env, tools::matrix::setNumericalZero(l_ptr->getProject()));
}
