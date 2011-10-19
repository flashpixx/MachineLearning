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


/** generate a random value
 * @param p_env JNI environment
 * @param p_object JNI object
 * @param p_distribution java distribution object
 * @param p_first first random option value
 * @param p_second second random option value
 * @param p_third third random option value
 * @return random value
 **/
template<typename T> T getRandomValue( JNIEnv* p_env, const jobject& p_object, const jobject& p_distribution,  const T& p_first, const T& p_second, const T& p_third )
{
    tools::random* l_ptr = java::jni::getObjectPointer< tools::random >(p_env, p_object, fidx_machinelearning_util_random);
    T l_val              = static_cast<T>(0);
    
    try {
        switch (java::jni::getEnumOrdinalValue(p_env, p_distribution)) {
                
            case 0  : l_val = l_ptr->get<T>( tools::random::uniform, p_first, p_second, p_third ); break;
            case 1  : l_val = l_ptr->get<T>( tools::random::bernoulli, p_first, p_second, p_third ); break;
            case 2  : l_val = l_ptr->get<T>( tools::random::cauchy, p_first, p_second, p_third ); break;
            case 3  : l_val = l_ptr->get<T>( tools::random::gamma, p_first, p_second, p_third ); break;
            case 4  : l_val = l_ptr->get<T>( tools::random::poisson, p_first, p_second, p_third ); break;
            case 5  : l_val = l_ptr->get<T>( tools::random::exponential, p_first, p_second, p_third ); break;
            case 6  : l_val = l_ptr->get<T>( tools::random::normal, p_first, p_second, p_third ); break;
            case 7  : l_val = l_ptr->get<T>( tools::random::student, p_first, p_second, p_third ); break;
            case 8  : l_val = l_ptr->get<T>( tools::random::weibull, p_first, p_second, p_third ); break;
            case 9  : l_val = l_ptr->get<T>( tools::random::rayleigh, p_first, p_second, p_third ); break;
            case 10 : l_val = l_ptr->get<T>( tools::random::chisquared, p_first, p_second, p_third ); break;
            case 11 : l_val = l_ptr->get<T>( tools::random::pareto, p_first, p_second, p_third ); break;
            case 12 : l_val = l_ptr->get<T>( tools::random::triangular, p_first, p_second, p_third ); break;
            case 13 : l_val = l_ptr->get<T>( tools::random::beta, p_first, p_second, p_third ); break;
                
            default :
                p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("option value is unkown") );
        }
    } catch (const std::exception& e) {
        p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), e.what() );
    }
        
    return l_val;
}


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
    // create random value
    const float l_val = getRandomValue( p_env, p_object, p_distribution, std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::epsilon() );
        
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
    // create random value
    const double l_val = getRandomValue( p_env, p_object, p_distribution, std::numeric_limits<double>::epsilon(), std::numeric_limits<double>::epsilon(), std::numeric_limits<double>::epsilon() );
    
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
    // create random value
    const float l_val = getRandomValue( p_env, p_object, p_distribution,  p_env->CallFloatMethod( p_first, java::jni::getMethodID(p_env, "java/lang/Float", "floatValue", "()F") ), std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::epsilon() );
    
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
    // create random value
    const jmethodID l_valueof = java::jni::getMethodID(p_env, "java/lang/Float", "floatValue", "()F");
    const float l_val         = getRandomValue( p_env, p_object, p_distribution,  p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ), std::numeric_limits<float>::epsilon() );
    
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
    // create random value
    const jmethodID l_valueof = java::jni::getMethodID(p_env, "java/lang/Float", "floatValue", "()F");
    const float l_val         = getRandomValue( p_env, p_object, p_distribution,  p_env->CallFloatMethod( p_first, l_valueof ), p_env->CallFloatMethod( p_second, l_valueof ), p_env->CallFloatMethod( p_third, l_valueof ) );
    
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
 * @return random value
 **/
JNIEXPORT jobject JNICALL Java_machinelearning_util_Random_get__Lmachinelearning_util_Random_Distribution_2Ljava_lang_Double_2(JNIEnv* p_env, jobject p_object, jobject p_distribution, jobject p_first)
{
    // create random value
    const double l_val = getRandomValue( p_env, p_object, p_distribution, p_env->CallDoubleMethod( p_first, java::jni::getMethodID(p_env, "java/lang/Double", "doubleValue", "()D") ), std::numeric_limits<double>::epsilon(), std::numeric_limits<double>::epsilon() );

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
 * @param p_second second value for the distribution
 * @return random value
 **/
JNIEXPORT jobject JNICALL Java_machinelearning_util_Random_get__Lmachinelearning_util_Random_Distribution_2Ljava_lang_Double_2Ljava_lang_Double_2(JNIEnv* p_env, jobject p_object, jobject p_distribution, jobject p_first, jobject p_second)
{
    // create random value
    const jmethodID l_valueof = java::jni::getMethodID(p_env, "java/lang/Double", "doubleValue", "()D");
    const double l_val        = getRandomValue( p_env, p_object, p_distribution, p_env->CallDoubleMethod( p_first, l_valueof ), p_env->CallDoubleMethod( p_second, l_valueof ), std::numeric_limits<double>::epsilon() );
    
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
 * @param p_second second value for the distribution
 * @param p_third third value for the distribution
 * @return random value
 **/
JNIEXPORT jobject JNICALL Java_machinelearning_util_Random_get__Lmachinelearning_util_Random_Distribution_2Ljava_lang_Double_2Ljava_lang_Double_2Ljava_lang_Double_2(JNIEnv* p_env, jobject p_object, jobject p_distribution, jobject p_first, jobject p_second, jobject p_third)
{
    // create random value
    const jmethodID l_valueof = java::jni::getMethodID(p_env, "java/lang/Double", "doubleValue", "()D");
    const double l_val        = getRandomValue( p_env, p_object, p_distribution, p_env->CallDoubleMethod( p_first, l_valueof ), p_env->CallDoubleMethod( p_second, l_valueof ), p_env->CallDoubleMethod( p_third, l_valueof ) );
    
    // create java object
    jclass l_elementclass   = NULL;
    jmethodID l_elementctor = NULL;
    java::jni::getCtor(p_env, "java/lang/Double", "(D)V", l_elementclass, l_elementctor);
    
    return p_env->NewObject( l_elementclass, l_elementctor, l_val );  
}