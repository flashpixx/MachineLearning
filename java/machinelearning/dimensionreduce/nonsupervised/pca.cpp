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


// ======= delegate for double =============================================================================================================================

/** field id that stores the ID that stores the pointer **/
jfieldID fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_double    = NULL;

/** constructor call for delegate double
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_dimension dimension value
 * @return pointer
 **/
JNIEXPORT jlong JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024delegate_1double_cpp_1ctor(JNIEnv* p_env, jobject p_object, jint p_dim)
{
    jlong l_ptr = 0;
    
    try {
        l_ptr = java::jni::createObjectPointer(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_double, new dim::pca<double>(p_dim));
    } catch (const std::exception& e) {
        p_env->ThrowNew( p_env->FindClass("machinelearning/exception/runtime"), e.what() );
    }
    
    return l_ptr;
}

/** destructor call for delegate double
 * @param p_env JNI environment
 * @param p_object JNI object
 **/
JNIEXPORT void JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024delegate_1double_dispose(JNIEnv* p_env, jobject p_object)
{
    java::jni::disposeObjectPointer< dim::pca<double> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_double);
}

/** returns the number of project dimension
 * @param p_env JNI environment
 * @param p_object JNI object
 * @return dimension number
 **/
JNIEXPORT jint JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024delegate_1double_getDimension(JNIEnv* p_env, jobject p_object)
{
    dim::pca<double>* l_ptr = java::jni::getObjectPointer< dim::pca<double> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_double);
    return l_ptr->getDimension();
}

/** maps data to the target dimension
 * @param p_env JNI environment
 * @param p_object JNI object
 * @retunr mapped data
 **/
JNIEXPORT jobjectArray JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024delegate_1double_map(JNIEnv* p_env, jobject p_object, jobjectArray p_data)
{
    
}

/** get the project vectors
 * @param p_env JNI environment
 * @param p_object JNI object
 * @return vector array
 **/
JNIEXPORT jobjectArray JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024delegate_1double_getProject(JNIEnv* p_env, jobject p_object)
{
    
}



// ======= delegate for float ==============================================================================================================================

/** field id that stores the ID that stores the pointer **/
jfieldID fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_float     = NULL;

/** constructor call for delegate double
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_dimension dimension value
 * @return pointer
 **/
JNIEXPORT jlong JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024delegate_1float_cpp_1ctor(JNIEnv* p_env, jobject p_object, jint p_dim)
{
    jlong l_ptr = 0;
    
    try {
        l_ptr = java::jni::createObjectPointer(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_float, new dim::pca<float>(p_dim));
    } catch (const std::exception& e) {
        p_env->ThrowNew( p_env->FindClass("machinelearning/exception/runtime"), e.what() );
    }

    return l_ptr;
}

/** destructor call for delegate double
 * @param p_env JNI environment
 * @param p_object JNI object
 **/
JNIEXPORT void JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024delegate_1float_dispose(JNIEnv* p_env, jobject p_object)
{
    java::jni::disposeObjectPointer< dim::pca<float> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_float);
}

/** returns the number of project dimension
 * @param p_env JNI environment
 * @param p_object JNI object
 * @return dimension number
 **/
JNIEXPORT jint JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024delegate_1float_getDimension(JNIEnv* p_env, jobject p_object)
{
    dim::pca<float>* l_ptr = java::jni::getObjectPointer< dim::pca<float> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_pca_delegate_float);
    return l_ptr->getDimension();
}

/** maps data to the target dimension
 * @param p_env JNI environment
 * @param p_object JNI object
 * @retunr mapped data
 **/
JNIEXPORT jobjectArray JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024delegate_1float_map(JNIEnv* p_env, jobject p_object, jobjectArray p_data)
{
    
}

/** get the project vectors
 * @param p_env JNI environment
 * @param p_object JNI object
 * @return vector array
 **/
JNIEXPORT jobjectArray JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_00024delegate_1float_getProject(JNIEnv* p_env, jobject p_object)
{
    
}









