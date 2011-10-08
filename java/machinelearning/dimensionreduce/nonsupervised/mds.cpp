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

#include "mds.h"
#include "../../../java.h"


namespace java  = machinelearning::java;
namespace tools = machinelearning::tools;
namespace dim   = machinelearning::dimensionreduce::nonsupervised;
namespace ublas = boost::numeric::ublas;


/* $LastChangedDate$ */

// ======= delegate for double =============================================================================================================================

/** field id that stores the ID of the double pointer **/
static jfieldID fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatedouble    = NULL;

/** constructor call for delegate double
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_dimension dimension value
 * @param p_type enum object with type option
 * @return pointer
 **/
JNIEXPORT jlong JNICALL Java_machinelearning_dimensionreduce_nonsupervised_MDS_00024DelegateDouble_cppCtor(JNIEnv* p_env, jobject p_object, jint p_dimension, jobject p_type)
{
    jlong l_ptr = 0;
    
    try {
        switch (java::jni::getEnumOrdinalValue(p_env, p_type)) {
        
            case 0 :
                l_ptr = java::jni::createObjectPointer(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatedouble, new dim::mds<double>(p_dimension, dim::mds<double>::metric));
                break;
                
            case 1 :
                l_ptr = java::jni::createObjectPointer(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatedouble, new dim::mds<double>(p_dimension, dim::mds<double>::sammon));
                break;
                
            case 2 :
                l_ptr = java::jni::createObjectPointer(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatedouble, new dim::mds<double>(p_dimension, dim::mds<double>::hit));
                break;
                
            default :
                p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("enum value is not known") );
        }
    } catch (const std::exception& e) {
        p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), e.what() );
    }
    
    return l_ptr;
}

/** destructor call for delegate double
 * @param p_env JNI environment
 * @param p_object JNI object
 **/
JNIEXPORT void JNICALL Java_machinelearning_dimensionreduce_nonsupervised_MDS_00024DelegateDouble_dispose(JNIEnv* p_env, jobject p_object)
{
    java::jni::disposeObjectPointer< dim::mds<double> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatedouble);
}

/** returns the number of project dimension
 * @param p_env JNI environment
 * @param p_object JNI object
 * @return dimension number
 **/
JNIEXPORT jint JNICALL Java_machinelearning_dimensionreduce_nonsupervised_MDS_00024DelegateDouble_getDimension(JNIEnv* p_env, jobject p_object)
{
    dim::mds<double>* l_ptr = java::jni::getObjectPointer< dim::mds<double> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatedouble);
    return l_ptr->getDimension();
}

/** sets the number of iteration
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_iteration number of iterations
 **/
JNIEXPORT void JNICALL Java_machinelearning_dimensionreduce_nonsupervised_MDS_00024DelegateDouble_setIteration(JNIEnv* p_env, jobject p_object, jint p_iteration)
{
    dim::mds<double>* l_ptr = java::jni::getObjectPointer< dim::mds<double> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatedouble);
    l_ptr->setIteration( static_cast<std::size_t>(p_iteration) );
}

/** sets the steps
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_step number of steps
 **/
JNIEXPORT void JNICALL Java_machinelearning_dimensionreduce_nonsupervised_MDS_00024DelegateDouble_setStep(JNIEnv* p_env, jobject p_object, jint p_step)
{
    dim::mds<double>* l_ptr = java::jni::getObjectPointer< dim::mds<double> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatedouble);
    l_ptr->setStep( static_cast<std::size_t>(p_step) );
}

/** sets the rate value
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_rate rate value
 **/
JNIEXPORT void JNICALL Java_machinelearning_dimensionreduce_nonsupervised_MDS_00024DelegateDouble_setRate(JNIEnv* p_env, jobject p_object, jobject p_rate)
{
    dim::mds<double>* l_ptr = java::jni::getObjectPointer< dim::mds<double> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatedouble);
    l_ptr->setRate( p_env->CallDoubleMethod(p_rate, java::jni::getMethodID(p_env, "java/lang/Double", "doubleValue", "()D")) );
}

/** sets centering option
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_center enum value
 **/
JNIEXPORT void JNICALL Java_machinelearning_dimensionreduce_nonsupervised_MDS_00024DelegateDouble_setCentering(JNIEnv* p_env, jobject p_object, jobject p_center)
{
    dim::mds<double>* l_ptr = java::jni::getObjectPointer< dim::mds<double> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatedouble);
    
    switch (java::jni::getEnumOrdinalValue(p_env, p_center)) {
            
        case 0 :
            l_ptr->setCentering( dim::mds<double>::none );
            break;
            
        case 1 :
            l_ptr->setCentering( dim::mds<double>::singlecenter );
            break;
            
        case 2 :
            l_ptr->setCentering( dim::mds<double>::doublecenter );
            break;
            
        default :
            p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("enum value is not known") );
    }
}

/** maps data to the target dimension
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_data input data
 * @return mapped data
 **/
JNIEXPORT jobjectArray JNICALL Java_machinelearning_dimensionreduce_nonsupervised_MDS_00024DelegateDouble_map(JNIEnv* p_env, jobject p_object, jobjectArray p_data)
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
    
    // run MDS with the data and catches exception and sets around zero values to exactly zero for java programmers
    dim::mds<double>* l_ptr = java::jni::getObjectPointer< dim::mds<double> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatedouble);
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



// ======= delegate for float ==============================================================================================================================

/** field id that stores the ID of the float pointer **/
static jfieldID fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatefloat    = NULL;

/** constructor call for delegate double
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_dimension dimension value
 * @param p_type enum object with type option
 * @return pointer
 **/
JNIEXPORT jlong JNICALL Java_machinelearning_dimensionreduce_nonsupervised_MDS_00024DelegateFloat_cppCtor(JNIEnv* p_env, jobject p_object, jint p_dimension, jobject p_type)
{
    jlong l_ptr = 0;
    
    try {
        switch (java::jni::getEnumOrdinalValue(p_env, p_type)) {
                
            case 0 :
                l_ptr = java::jni::createObjectPointer(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatefloat, new dim::mds<float>(p_dimension, dim::mds<float>::metric));
                break;
                
            case 1 :
                l_ptr = java::jni::createObjectPointer(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatefloat, new dim::mds<float>(p_dimension, dim::mds<float>::sammon));
                break;
                
            case 2 :
                l_ptr = java::jni::createObjectPointer(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatefloat, new dim::mds<float>(p_dimension, dim::mds<float>::hit));
                break;
                
            default :
                p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("enum value is not known") );
        }
    } catch (const std::exception& e) {
        p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), e.what() );
    }
    
    return l_ptr;
}

/** destructor call for delegate double
 * @param p_env JNI environment
 * @param p_object JNI object
 **/
JNIEXPORT void JNICALL Java_machinelearning_dimensionreduce_nonsupervised_MDS_00024DelegateFloat_dispose(JNIEnv* p_env, jobject p_object)
{
    java::jni::disposeObjectPointer< dim::mds<float> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatefloat);
}

/** returns the number of project dimension
 * @param p_env JNI environment
 * @param p_object JNI object
 * @return dimension number
 **/
JNIEXPORT jint JNICALL Java_machinelearning_dimensionreduce_nonsupervised_MDS_00024DelegateFloat_getDimension(JNIEnv* p_env, jobject p_object)
{
    dim::mds<float>* l_ptr = java::jni::getObjectPointer< dim::mds<float> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatefloat);
    return l_ptr->getDimension();
}

/** sets the number of iteration
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_iteration number of iterations
 **/
JNIEXPORT void JNICALL Java_machinelearning_dimensionreduce_nonsupervised_MDS_00024DelegateFloat_setIteration(JNIEnv* p_env, jobject p_object, jint p_iteration)
{
    dim::mds<float>* l_ptr = java::jni::getObjectPointer< dim::mds<float> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatefloat);
    l_ptr->setIteration( static_cast<std::size_t>(p_iteration) );
}

/** sets the steps
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_step number of steps
 **/
JNIEXPORT void JNICALL Java_machinelearning_dimensionreduce_nonsupervised_MDS_00024DelegateFloat_setStep(JNIEnv* p_env, jobject p_object, jint p_step)
{
    dim::mds<float>* l_ptr = java::jni::getObjectPointer< dim::mds<float> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatefloat);
    l_ptr->setStep( static_cast<std::size_t>(p_step) );
}

/** sets the rate value
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_rate rate value
 **/
JNIEXPORT void JNICALL Java_machinelearning_dimensionreduce_nonsupervised_MDS_00024DelegateFloat_setRate(JNIEnv* p_env, jobject p_object, jobject p_rate)
{
    dim::mds<float>* l_ptr = java::jni::getObjectPointer< dim::mds<float> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatefloat);
    l_ptr->setRate( p_env->CallFloatMethod(p_rate, java::jni::getMethodID(p_env, "java/lang/Float", "floatValue", "()F")) );
}

/** sets centering option
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_center enum value
 **/
JNIEXPORT void JNICALL Java_machinelearning_dimensionreduce_nonsupervised_MDS_00024DelegateFloat_setCentering(JNIEnv* p_env, jobject p_object, jobject p_center)
{
    dim::mds<float>* l_ptr = java::jni::getObjectPointer< dim::mds<float> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatefloat);
    
    switch (java::jni::getEnumOrdinalValue(p_env, p_center)) {
            
        case 0 :
            l_ptr->setCentering( dim::mds<float>::none );
            break;
            
        case 1 :
            l_ptr->setCentering( dim::mds<float>::singlecenter );
            break;
            
        case 2 :
            l_ptr->setCentering( dim::mds<float>::doublecenter );
            break;
            
        default :
            p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("enum value is not known") );
    }
}

/** maps data to the target dimension
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_data input data
 * @return mapped data
 **/
JNIEXPORT jobjectArray JNICALL Java_machinelearning_dimensionreduce_nonsupervised_MDS_00024DelegateFloat_map(JNIEnv* p_env, jobject p_object, jobjectArray p_data)
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
    
    // run MDS with the data and catches exception and sets around zero values to exactly zero for java programmers
    dim::mds<float>* l_ptr = java::jni::getObjectPointer< dim::mds<float> >(p_env, p_object, fidx_machinelearning_dimensionreduce_nonsupervised_mds_delegatefloat);
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

