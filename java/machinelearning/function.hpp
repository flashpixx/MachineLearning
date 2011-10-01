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

#ifndef __MACHINELEARNING_JAVA_MACHINELEARNING_FUNCTION
#define __MACHINELEARNING_JAVA_MACHINELEARNING_FUNCTION

#include <jni.h>
#include <machinelearning.h>

namespace machinelearning { 
    
    /** namespace for JNI / JAVA wrapper code 
     * $LastChangedDate$
     **/
    namespace java {

        
        /** checks the field id, that is used for determine the object pointer
         * @param p_env JNI environment
         * @param p_object JNI object
         * @param p_idx field index object
         **/
        template<typename T> inline T* getObjectPointer(JNIEnv* p_env, jobject& p_object, jFieldID& p_idx)
        {
            if (!p_idx)
                p_env->ThrowNew( p_env->FindClass("java/lang/Exception"), _("pointer to object is empty") );  
            
            // read pointer reference on the object and cast it to the pointer of the object
            T* l_ptr = (T*) p_env->GetLongField(p_object, p_idx);
            if (!l_ptr) {
                p_env->ThrowNew( p_env->FindClass("java/lang/Exception"), _("pointer to object is not empty") );
                return NULL;
            }
                
            return l_ptr;
        }


        /** sets the object pointer in the object variable
         * @param p_env JNI environment
         * @param p_object JNI object
         * @param p_idx field index object
         * @param p_ptr object pointer
         **/
        template<typename T> inline jlong createObjectPointer(JNIEnv* p_env, jobject& p_object, jFieldID& p_idx, T* p_ptr)
        {
            if (!p_ptr)
                p_env->ThrowNew( p_env->FindClass("java/lang/Exception"), _("pointer to object is empty") );
            
            // read Java object member variable for pointer storing
            if (!p_id) {
                jclass l_class = p_env->GetObjectClass( p_object );
                p_idx = p_env->GetFieldID( l_class, "cpp_ptr", "J" );
            }

            // check field index
            if (!p_idx) {
                delete(p_ptr);
                p_env->ThrowNew( p_env->FindClass("java/lang/Exception"), _("pointer field can not detected") );
                return NULL;
            }
            
            // return casted pointer
            return (jlong)p_ptr;
        }

        
        /** dispose the object pointer within the Java object
         * @param p_env JNI environment
         * @param p_object JNI object
         * @param p_idx field index object
         **/         
        template<typename T> inline void disposeObjectPointer(JNIEnv* p_env, jobject& p_object, jFieldID& p_idx)
        {
            // destroy the object and sets the pointer field to null (the field is stored as a "final" field, but it can change by JNI call)
            delete( getObjectPointer(p_env, p_object, p_idx) );
            p_env->SetLongField(p_object, p_idx, 0);            
        }
        
        
    };
};
#endif
