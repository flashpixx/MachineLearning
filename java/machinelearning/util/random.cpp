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

#include "random.h"
#include "../../java.h"


namespace java  = machinelearning::java;
namespace tools = machinelearning::tools;

/* $LastChangedDate$ */

/** field id that stores the ID of the pointer **/
static jfieldID fidx_machinelearning_util_random    = NULL;



/** destructor call for the random object 
 * @param p_env JNI environment
 * @param p_object JNI object
 **/
JNIEXPORT void JNICALL Java_machinelearning_util_Random_dispose(JNIEnv* p_env, jobject p_object)
{
    java::jni::disposeObjectPointer< tools::random >(p_env, p_object, fidx_machinelearning_util_random);
}


/** constructor call
 * @param p_env JNI environment
 * @param p_object JNI object
 * @return object pointer
 **/
JNIEXPORT jlong JNICALL Java_machinelearning_util_Random_cppCtor(JNIEnv* p_env, jobject p_object)
{
    return java::jni::createObjectPointer(p_env, p_object, fidx_machinelearning_util_random, new tools::random());
}


/** creates a random float value
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_distribution distribution object
 * @return float random value
 **/
JNIEXPORT jobject JNICALL Java_machinelearning_util_Random_getFloat(JNIEnv* p_env, jobject p_object, jobject p_distribution)
{
    tools::random* l_ptr = java::jni::getObjectPointer< tools::random >(p_env, p_object, fidx_machinelearning_util_random);
    if (!l_ptr)
        return p_env->NewGlobalRef(NULL);
    
    // create random value
    float l_val = 0;
    switch (java::jni::getEnumOrdinalValue(p_env, p_distribution)) {
            
        case 0 :
            l_val = l_ptr->get<float>( tools::random::uniform );
            break;
            
        case 1 :
            l_val = l_ptr->get<float>( tools::random::bernoulli );
            break;
            
        case 2 :
            l_val = l_ptr->get<float>( tools::random::cauchy );
            break;
            
        case 3 :
            l_val = l_ptr->get<float>( tools::random::gamma );
            break;
            
        case 4 :
            l_val = l_ptr->get<float>( tools::random::poisson );
            break;
            
        case 5 :
            l_val = l_ptr->get<float>( tools::random::exponential );
            break;
            
        case 6 :
            l_val = l_ptr->get<float>( tools::random::normal );
            break;
            
        case 7 :
            l_val = l_ptr->get<float>( tools::random::student );
            break;
            
        case 8 :
            l_val = l_ptr->get<float>( tools::random::weibull );
            break;
            
        case 9 :
            l_val = l_ptr->get<float>( tools::random::rayleigh );
            break;
            
        case 10 :
            l_val = l_ptr->get<float>( tools::random::chisquared );
            break;
            
        case 11 :
            l_val = l_ptr->get<float>( tools::random::pareto );
            break;
            
        case 12 :
            l_val = l_ptr->get<float>( tools::random::triangular );
            break;
            
        case 13 :
            l_val = l_ptr->get<float>( tools::random::beta );
            break;
            
        default:
            p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("option value is unkown") );
            return p_env->NewGlobalRef(NULL);
    }
    
    // create java object
    jclass l_elementclass   = NULL;
    jmethodID l_elementctor = NULL;
    java::jni::getCtor(p_env, "java/lang/Float", "(F)V", l_elementclass, l_elementctor);
    
    return p_env->NewObject( l_elementclass, l_elementctor, l_val );
}


/** creates a random double value
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_distribution distribution object
 * @return double random value
 **/
JNIEXPORT jobject JNICALL Java_machinelearning_util_Random_getDouble(JNIEnv* p_env, jobject p_object, jobject p_distribution)
{
    tools::random* l_ptr = java::jni::getObjectPointer< tools::random >(p_env, p_object, fidx_machinelearning_util_random);
    if (!l_ptr)
        return p_env->NewGlobalRef(NULL);
    
    // create random value
    double l_val = 0;
    switch (java::jni::getEnumOrdinalValue(p_env, p_distribution)) {
            
        case 0 :
            l_val = l_ptr->get<double>( tools::random::uniform );
            break;
            
        case 1 :
            l_val = l_ptr->get<double>( tools::random::bernoulli );
            break;
            
        case 2 :
            l_val = l_ptr->get<double>( tools::random::cauchy );
            break;
            
        case 3 :
            l_val = l_ptr->get<double>( tools::random::gamma );
            break;
            
        case 4 :
            l_val = l_ptr->get<double>( tools::random::poisson );
            break;
            
        case 5 :
            l_val = l_ptr->get<double>( tools::random::exponential );
            break;
            
        case 6 :
            l_val = l_ptr->get<double>( tools::random::normal );
            break;
            
        case 7 :
            l_val = l_ptr->get<double>( tools::random::student );
            break;
            
        case 8 :
            l_val = l_ptr->get<double>( tools::random::weibull );
            break;
            
        case 9 :
            l_val = l_ptr->get<double>( tools::random::rayleigh );
            break;
            
        case 10 :
            l_val = l_ptr->get<double>( tools::random::chisquared );
            break;
            
        case 11 :
            l_val = l_ptr->get<double>( tools::random::pareto );
            break;
            
        case 12 :
            l_val = l_ptr->get<double>( tools::random::triangular );
            break;
            
        case 13 :
            l_val = l_ptr->get<double>( tools::random::beta );
            break;
            
        default:
            p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("option value is unkown") );
            return p_env->NewGlobalRef(NULL);
    }
    
    // create java object
    jclass l_elementclass   = NULL;
    jmethodID l_elementctor = NULL;
    java::jni::getCtor(p_env, "java/lang/Double", "(D)V", l_elementclass, l_elementctor);
    
    return p_env->NewObject( l_elementclass, l_elementctor, l_val );
}


/** generates a random value
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_distribution distribution object
 * @param p_first first value for the distribution
 * @return random value
 **/
JNIEXPORT jobject JNICALL Java_machinelearning_util_Random_get__Lmachinelearning_util_Random_Distribution_2Ljava_lang_Float_2(JNIEnv* p_env, jobject p_object, jobject p_distribution, jobject p_first)
{
    tools::random* l_ptr = java::jni::getObjectPointer< tools::random >(p_env, p_object, fidx_machinelearning_util_random);
    if (!l_ptr)
        return p_env->NewGlobalRef(NULL);
    
    const jmethodID l_valueof = java::jni::getMethodID(p_env, "java/lang/Float", "floatValue", "()F");
    
    // create random value
    float l_val = 0;
    switch (java::jni::getEnumOrdinalValue(p_env, p_distribution)) {
            
        case 0 :
            l_val = l_ptr->get<float>( tools::random::uniform, p_env->CallFloatMethod( p_first, l_valueof ) );
            break;
            
        case 1 :
            l_val = l_ptr->get<float>( tools::random::bernoulli, p_env->CallFloatMethod( p_first, l_valueof ) );
            break;
            
        case 2 :
            l_val = l_ptr->get<float>( tools::random::cauchy, p_env->CallFloatMethod( p_first, l_valueof ) );
            break;
            
        case 3 :
            l_val = l_ptr->get<float>( tools::random::gamma, p_env->CallFloatMethod( p_first, l_valueof ) );
            break;
            
        case 4 :
            l_val = l_ptr->get<float>( tools::random::poisson, p_env->CallFloatMethod( p_first, l_valueof ) );
            break;
            
        case 5 :
            l_val = l_ptr->get<float>( tools::random::exponential, p_env->CallFloatMethod( p_first, l_valueof ) );
            break;
            
        case 6 :
            l_val = l_ptr->get<float>( tools::random::normal, p_env->CallFloatMethod( p_first, l_valueof ) );
            break;
            
        case 7 :
            l_val = l_ptr->get<float>( tools::random::student, p_env->CallFloatMethod( p_first, l_valueof ) );
            break;
            
        case 8 :
            l_val = l_ptr->get<float>( tools::random::weibull, p_env->CallFloatMethod( p_first, l_valueof ) );
            break;
            
        case 9 :
            l_val = l_ptr->get<float>( tools::random::rayleigh, p_env->CallFloatMethod( p_first, l_valueof ) );
            break;
            
        case 10 :
            l_val = l_ptr->get<float>( tools::random::chisquared, p_env->CallFloatMethod( p_first, l_valueof ) );
            break;
            
        case 11 :
            l_val = l_ptr->get<float>( tools::random::pareto, p_env->CallFloatMethod( p_first, l_valueof ) );
            break;
            
        case 12 :
            l_val = l_ptr->get<float>( tools::random::triangular, p_env->CallFloatMethod( p_first, l_valueof ) );
            break;
            
        case 13 :
            l_val = l_ptr->get<float>( tools::random::beta, p_env->CallFloatMethod( p_first, l_valueof ) );
            break;
            
        default:
            p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("option value is unkown") );
            return p_env->NewGlobalRef(NULL);
    }
    
    // create java object
    jclass l_elementclass   = NULL;
    jmethodID l_elementctor = NULL;
    java::jni::getCtor(p_env, "java/lang/Float", "(F)V", l_elementclass, l_elementctor);
    
    return p_env->NewObject( l_elementclass, l_elementctor, l_val );   
}


/** generates a random value
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_distribution distribution object
 * @param p_first first value for the distribution
 * @param p_second second value for the distribution
 * @return random value
 **/
JNIEXPORT jobject JNICALL Java_machinelearning_util_Random_get__Lmachinelearning_util_Random_Distribution_2Ljava_lang_Float_2Ljava_lang_Float_2(JNIEnv* p_env, jobject p_object, jobject p_distribution, jobject p_first, jobject p_second)
{
    tools::random* l_ptr = java::jni::getObjectPointer< tools::random >(p_env, p_object, fidx_machinelearning_util_random);
    if (!l_ptr)
        return p_env->NewGlobalRef(NULL);
    
    const jmethodID l_valueof = java::jni::getMethodID(p_env, "java/lang/Float", "floatValue", "()F");
    
    // create random value
    float l_val = 0;
    switch (java::jni::getEnumOrdinalValue(p_env, p_distribution)) {
            
        case 0 :
            l_val = l_ptr->get<float>( tools::random::uniform, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ) );
            break;
            
        case 1 :
            l_val = l_ptr->get<float>( tools::random::bernoulli, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ) );
            break;
            
        case 2 :
            l_val = l_ptr->get<float>( tools::random::cauchy, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ) );
            break;
            
        case 3 :
            l_val = l_ptr->get<float>( tools::random::gamma, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ) );
            break;
            
        case 4 :
            l_val = l_ptr->get<float>( tools::random::poisson, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ) );
            break;
            
        case 5 :
            l_val = l_ptr->get<float>( tools::random::exponential, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ) );
            break;
            
        case 6 :
            l_val = l_ptr->get<float>( tools::random::normal, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ) );
            break;
            
        case 7 :
            l_val = l_ptr->get<float>( tools::random::student, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ) );
            break;
            
        case 8 :
            l_val = l_ptr->get<float>( tools::random::weibull, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ) );
            break;
            
        case 9 :
            l_val = l_ptr->get<float>( tools::random::rayleigh, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ) );
            break;
            
        case 10 :
            l_val = l_ptr->get<float>( tools::random::chisquared, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ) );
            break;
            
        case 11 :
            l_val = l_ptr->get<float>( tools::random::pareto, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ) );
            break;
            
        case 12 :
            l_val = l_ptr->get<float>( tools::random::triangular, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ) );
            break;
            
        case 13 :
            l_val = l_ptr->get<float>( tools::random::beta, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ) );
            break;
            
        default:
            p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("option value is unkown") );
            return p_env->NewGlobalRef(NULL);
    }
    
    // create java object
    jclass l_elementclass   = NULL;
    jmethodID l_elementctor = NULL;
    java::jni::getCtor(p_env, "java/lang/Float", "(F)V", l_elementclass, l_elementctor);
    
    return p_env->NewObject( l_elementclass, l_elementctor, l_val );   
}


/** generates a random value
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_distribution distribution object
 * @param p_first first value for the distribution
 * @param p_second second value for the distribution
 * @param p_third third value for the distribution
 * @return random value
 **/
JNIEXPORT jobject JNICALL Java_machinelearning_util_Random_get__Lmachinelearning_util_Random_Distribution_2Ljava_lang_Float_2Ljava_lang_Float_2Ljava_lang_Float_2(JNIEnv* p_env, jobject p_object, jobject p_distribution, jobject p_first, jobject p_second, jobject p_third)
{
    tools::random* l_ptr = java::jni::getObjectPointer< tools::random >(p_env, p_object, fidx_machinelearning_util_random);
    if (!l_ptr)
        return p_env->NewGlobalRef(NULL);
    
    const jmethodID l_valueof = java::jni::getMethodID(p_env, "java/lang/Float", "floatValue", "()F");
    
    // create random value
    float l_val = 0;
    switch (java::jni::getEnumOrdinalValue(p_env, p_distribution)) {
            
        case 0 :
            l_val = l_ptr->get<float>( tools::random::uniform, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ), p_env->CallFloatMethod( p_third, l_valueof ) );
            break;
            
        case 1 :
            l_val = l_ptr->get<float>( tools::random::bernoulli, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ), p_env->CallFloatMethod( p_third, l_valueof ) );
            break;
            
        case 2 :
            l_val = l_ptr->get<float>( tools::random::cauchy, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ), p_env->CallFloatMethod( p_third, l_valueof ) );
            break;
            
        case 3 :
            l_val = l_ptr->get<float>( tools::random::gamma, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ), p_env->CallFloatMethod( p_third, l_valueof ) );
            break;
            
        case 4 :
            l_val = l_ptr->get<float>( tools::random::poisson, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ), p_env->CallFloatMethod( p_third, l_valueof ) );
            break;
            
        case 5 :
            l_val = l_ptr->get<float>( tools::random::exponential, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ), p_env->CallFloatMethod( p_third, l_valueof ) );
            break;
            
        case 6 :
            l_val = l_ptr->get<float>( tools::random::normal, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ), p_env->CallFloatMethod( p_third, l_valueof ) );
            break;
            
        case 7 :
            l_val = l_ptr->get<float>( tools::random::student, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ), p_env->CallFloatMethod( p_third, l_valueof ) );
            break;
            
        case 8 :
            l_val = l_ptr->get<float>( tools::random::weibull, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ), p_env->CallFloatMethod( p_third, l_valueof ) );
            break;
            
        case 9 :
            l_val = l_ptr->get<float>( tools::random::rayleigh, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ), p_env->CallFloatMethod( p_third, l_valueof ) );
            break;
            
        case 10 :
            l_val = l_ptr->get<float>( tools::random::chisquared, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ), p_env->CallFloatMethod( p_third, l_valueof ) );
            break;
            
        case 11 :
            l_val = l_ptr->get<float>( tools::random::pareto, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ), p_env->CallFloatMethod( p_third, l_valueof ) );
            break;
            
        case 12 :
            l_val = l_ptr->get<float>( tools::random::triangular, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ), p_env->CallFloatMethod( p_third, l_valueof ) );
            break;
            
        case 13 :
            l_val = l_ptr->get<float>( tools::random::beta, p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ), p_env->CallFloatMethod( p_third, l_valueof ) );
            break;
            
        default:
            p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("option value is unkown") );
            return p_env->NewGlobalRef(NULL);
    }
    
    // create java object
    jclass l_elementclass   = NULL;
    jmethodID l_elementctor = NULL;
    java::jni::getCtor(p_env, "java/lang/Float", "(F)V", l_elementclass, l_elementctor);
    
    return p_env->NewObject( l_elementclass, l_elementctor, l_val );   
}





JNIEXPORT jobject JNICALL Java_machinelearning_util_Random_get__Lmachinelearning_util_Random_Distribution_2Ljava_lang_Double_2(JNIEnv* p_env, jobject p_object, jobject p_distribution, jobject p_first)
{
    tools::random* l_ptr = java::jni::getObjectPointer< tools::random >(p_env, p_object, fidx_machinelearning_util_random);
    if (!l_ptr)
        return p_env->NewGlobalRef(NULL);
}

JNIEXPORT jobject JNICALL Java_machinelearning_util_Random_get__Lmachinelearning_util_Random_Distribution_2Ljava_lang_Double_2Ljava_lang_Double_2(JNIEnv* p_env, jobject p_object, jobject p_distribution, jobject p_first, jobject p_second)
{
    tools::random* l_ptr = java::jni::getObjectPointer< tools::random >(p_env, p_object, fidx_machinelearning_util_random);
    if (!l_ptr)
        return p_env->NewGlobalRef(NULL);
}

JNIEXPORT jobject JNICALL Java_machinelearning_util_Random_get__Lmachinelearning_util_Random_Distribution_2Ljava_lang_Double_2Ljava_lang_Double_2Ljava_lang_Double_2(JNIEnv* p_env, jobject p_object, jobject p_distribution, jobject p_first, jobject p_second, jobject p_third)
{
    tools::random* l_ptr = java::jni::getObjectPointer< tools::random >(p_env, p_object, fidx_machinelearning_util_random);
    if (!l_ptr)
        return p_env->NewGlobalRef(NULL);
}