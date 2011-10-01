#include "pca.h"
#include <machinelearning.h>

namespace ml = machinelearning::dimensionreduce::nonsupervised;

jFieldID machinelearning_dimensionreduce_nonsupervised_pca_fieldidx = NULL;



JNIEXPORT jobjectArray JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_map(JNIEnv* p_env, jobject p_object, jobjectArray j_data)
{
	if (!machinelearning_dimensionreduce_nonsupervised_pca_fieldidx)
        p_env->ThrowNew( p_env->FindClass("java/lang/Exception"), _("pointer to object is empty") );    
}


JNIEXPORT jint JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_getDimension(JNIEnv* p_env, jobject p_object)
{
	if (!machinelearning_dimensionreduce_nonsupervised_pca_fieldidx)
        p_env->ThrowNew( p_env->FindClass("java/lang/Exception"), _("pointer to object is empty") );  
    
    ml::pca<>* l_ptr = (jnitest*) p_env->GetLongField(p_object, machinelearning_dimensionreduce_nonsupervised_pca_fieldidx);
    if (!l_ptr)
        p_env->ThrowNew( p_env->FindClass("java/lang/Exception"), _("pointer to object is not empty") );
    
    return l_ptr->getDimension();
}


JNIEXPORT void JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_dispose(JNIEnv* p_env, jobject p_object)
{
	if (machinelearning_dimensionreduce_nonsupervised_pca_fieldidx)
        p_env->ThrowNew( p_env->FindClass("java/lang/Exception"), _("pointer to object is empty") );        
}


JNIEXPORT jlong JNICALL Java_machinelearning_dimensionreduce_nonsupervised_pca_cpp_1ctor(JNIEnv* p_env, jobject p_object, jint j_size)
{
	if (!machinelearning_dimensionreduce_nonsupervised_pca_fieldidx) {
		jclass l_class                                             = p_env->GetObjectClass( p_object );
		machinelearning_dimensionreduce_nonsupervised_pca_fieldidx = p_env->GetFieldID( l_class, "cpp_ptr", "J" );
	}
    
	if (!machinelearning_dimensionreduce_nonsupervised_pca_fieldidx)
        p_env->ThrowNew( p_env->FindClass("java/lang/Exception"), _("pointer to object is not empty") );    
}