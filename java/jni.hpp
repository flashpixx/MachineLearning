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

#ifndef __MACHINELEARNING_JAVA_JNI_HPP
#define __MACHINELEARNING_JAVA_JNI_HPP

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <jni.h>


namespace machinelearning { namespace java {
    
        namespace ublas = boost::numeric::ublas;

        
        /** class for function that connect Java and C++ objects / reference
         * @note the JNIEnv pointer need not be a const reference, because
         * the environment can be changed
         * $LastChangedDate$
         **/
        class jni {
            
            public :
            
                template<typename T> static T* getObjectPointer(JNIEnv*, const jobject&, const jfieldID&);
                template<typename T> static jlong createObjectPointer(JNIEnv*, const jobject&, jfieldID&, T*);
                template<typename T> static void disposeObjectPointer(JNIEnv*, const jobject&, const jfieldID&);
                static jmethodID getMethodID(JNIEnv*, const jobject&, const char*, const char*);
                static jmethodID getMethodID(JNIEnv*, const char*, const char*, const char*);
                static void getCtor(JNIEnv*, const char*, const char*, jclass&, jmethodID&);
                static ublas::matrix<double> getDoubleMatrixFrom2DArray( JNIEnv*, const jobjectArray& );
                static ublas::matrix<float> getFloatMatrixFrom2DArray( JNIEnv*, const jobjectArray& );
                static jobjectArray getJObjectArrayFromMatrix( JNIEnv*, const ublas::matrix<double>& );
                static jobjectArray getJObjectArrayFromMatrix( JNIEnv*, const ublas::matrix<float>& );
                static jobjectArray getJObjectArrayFromVector( JNIEnv*, const ublas::vector<double>& );
                static jobjectArray getJObjectArrayFromVector( JNIEnv*, const ublas::vector<float>& );
                static std::size_t getEnumOrdinalValue( JNIEnv*, const jobject& );
            
        };
            
    
    
        /** checks the field id, that is used for determine the object pointer
         * @param p_env JNI environment
         * @param p_object JNI object
         * @param p_idx field index object
         **/
        template<typename T> inline T* jni::getObjectPointer(JNIEnv* p_env, const jobject& p_object, const jfieldID& p_idx)
        {
            // check the field index
            if (!p_idx)
                p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("pointer to object is empty") );  
            
            // read pointer reference on the object and cast it to the pointer of the object
            T* l_ptr = (T*)( p_env->GetLongField(p_object, p_idx) );
            if (!l_ptr) {
                p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("pointer to object is not empty") );
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
        template<typename T> inline jlong jni::createObjectPointer(JNIEnv* p_env, const jobject& p_object, jfieldID& p_idx, T* p_ptr)
        {
            // check pointer to C++ object
            if (!p_ptr)
                p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("pointer to object is empty") );

            // check parameter field is set
            if (!p_idx) {
                
                // get java class of the object
                jclass l_class = p_env->GetObjectClass( p_object );
                if (!l_class) {
                    //p_env->DeleteLocalRef(l_class);
                    delete(p_ptr);
                    p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("can not find associated java class") );
                    return NULL;
                }
                    
                // check if the object is in a thread-safe content and lock it
                if (p_env->MonitorEnter(p_object) != JNI_OK) {
                    //p_env->DeleteLocalRef(l_class);
                    delete(p_ptr);
                    p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("call is not within a thread-safe content") );
                    return NULL;
                }
                
                // set the field index to the parameter
                p_idx = p_env->GetFieldID( l_class, "cpp_ptr", "J" );
                
                // release thread content
                if (p_env->MonitorExit(p_object) != JNI_OK) {
                    //p_env->DeleteLocalRef(l_class);
                    delete(p_ptr);
                    p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("release thread can not be determine correctly") );
                    return NULL;
                }
            }

            // check field index
            if (!p_idx) {
                delete(p_ptr);
                p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("pointer field can not detected") );
                return NULL;
            }
            
            // return casted pointer to the call
            return (jlong)p_ptr;
        }

        
        /** dispose the object pointer within the Java object
         * @param p_env JNI environment
         * @param p_object JNI object
         * @param p_idx field index object
         **/         
        template<typename T> inline void jni::disposeObjectPointer(JNIEnv* p_env, const jobject& p_object, const jfieldID& p_idx)
        {
            // dispose must be thread-safe so do this
            if (p_env->MonitorEnter(p_object) != JNI_OK) {
                p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("call is not within a thread-safe content") );
                return;
            }
        
            // destroy the object and sets the pointer field to null (the field is stored as a "final" field, but it can change by JNI call)
            delete( jni::getObjectPointer<T>(p_env, p_object, p_idx) );
            p_env->SetLongField(p_object, p_idx, 0); 
            
            // release the synchronize content
            if (p_env->MonitorExit(p_object) != JNI_OK)
                p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("release thread can not be determine correctly") );
        }
    
    
        /** returns the method id of an object
         * @param p_env JNI environment
         * @param p_object JNI object
         * @param p_name method name
         * @param p_signatur method signature
         * @return method id
         * @todo check jmethodID use for memory leak
         **/
        inline jmethodID jni::getMethodID(JNIEnv* p_env, const jobject& p_object, const char* p_name, const char* p_signatur)
        {
            jclass l_class = p_env->GetObjectClass( p_object );
            if (!l_class) {
                //p_env->DeleteLocalRef(l_class);
                p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("can not find associated java class") );
                return NULL;
            }
            
            jmethodID l_id = p_env->GetMethodID(l_class, p_name, p_signatur);
            if (!l_id) {
                //p_env->DeleteLocalRef(l_class);
                p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("can not find method with signature") );
                return NULL;
            }

            p_env->DeleteLocalRef(l_class);
            return l_id;
        }
    
    
        /** returns the method id of a class
         * @param p_env JNI environment
         * @param p_classname class name
         * @param p_methodname method name
         * @param p_signatur method signature
         * @return method id
         * @todo check jmethodID use for memory leak
         **/
        inline jmethodID jni::getMethodID(JNIEnv* p_env, const char* p_classname, const char* p_methodname, const char* p_signatur)
        {
            jclass l_class = p_env->FindClass(p_classname);
            if (!l_class) {
                //p_env->DeleteLocalRef(l_class);
                p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("can not find associated java class") );
                return NULL;
            }
            
            jmethodID l_id = p_env->GetMethodID(l_class, p_methodname, p_signatur);
            if (!l_id) {
                //p_env->DeleteLocalRef(l_class);
                p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("can not find method with signature") );
                return NULL;
            }
            
            //p_env->DeleteLocalRef(l_class);
            return l_id;
        }


        /** create resources to the constructor call
         * @param p_env JNI environment
         * @param p_name name of the class
         * @param p_signatur signatur for the constructor
         * @param p_classid reference to the class id
         * @param p_ctorid reference to the ctor method id
         **/
        inline void jni::getCtor(JNIEnv* p_env, const char* p_name, const char* p_signatur, jclass& p_classid, jmethodID& p_ctorid)
        {
            p_ctorid  = 0;
            p_classid = p_env->FindClass(p_name);
            if (!p_classid)
                p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("can not find associated java class") );
            else {
                p_ctorid = p_env->GetMethodID(p_classid, "<init>", p_signatur);
                if (!p_ctorid)
                    p_env->ThrowNew( p_env->FindClass("machinelearning/exception/Runtime"), _("can not find constructor call") );
            }
        }
    
    
        /** creates a ublas double matrix from a java 2D array
         * @param p_env JNI environment
         * @param p_data java array
         * @return ublas matrix if matrix have zero columns and/or rows the array can not be read
         * @todo check jmethodID use for memory leak
         **/
        inline ublas::matrix<double> jni::getDoubleMatrixFrom2DArray( JNIEnv* p_env, const jobjectArray& p_data )
        {
            ublas::matrix<double> l_data(0,0);
            
            // convert the java array to a ublas matrix (first read the row dimension and than read the first array element, cast it to jobjectArray and get the length)
            const std::size_t l_rows = p_env->GetArrayLength(p_data);
            if (l_rows == 0)
                return l_data;
            
            const std::size_t l_cols = p_env->GetArrayLength( (jobjectArray)p_env->GetObjectArrayElement(p_data, 0) );
            if (l_cols == 0)
                return l_data;
            
            // each element in the array is a "java.lang.Double" value, for reading the value the method "double doubleValue()" must be called, so get the ID
            const jmethodID l_valueof = java::jni::getMethodID(p_env, "java/lang/Double", "doubleValue", "()D"); 
            
            // read array data
            l_data = ublas::matrix<double>(l_rows, l_cols);
            for(std::size_t i=0; i < l_rows; ++i) {
                jobjectArray l_coldata = (jobjectArray)p_env->GetObjectArrayElement(p_data, i);
                
                for(std::size_t j=0; j < std::min(l_cols, static_cast<std::size_t>(p_env->GetArrayLength(l_coldata))); ++j)
                    l_data(i,j) = p_env->CallDoubleMethod( p_env->GetObjectArrayElement(l_coldata, j), l_valueof );
            }
            
            return l_data;
        }
    

        /** creates a ublas float matrix from a java 2D array
         * @param p_env JNI environment
         * @param p_data java array
         * @return ublas matrix if matrix have zero columns and/or rows the array can not be read
         * @todo check jmethodID use for memory leak
         **/
        inline ublas::matrix<float> jni::getFloatMatrixFrom2DArray( JNIEnv* p_env, const jobjectArray& p_data )
        {
            ublas::matrix<float> l_data(0,0);
            
            // convert the java array to a ublas matrix (first read the row dimension and than read the first array element, cast it to jobjectArray and get the length)
            const std::size_t l_rows = p_env->GetArrayLength(p_data);
            if (l_rows == 0)
                return l_data;
            
            const std::size_t l_cols = p_env->GetArrayLength( (jobjectArray)p_env->GetObjectArrayElement(p_data, 0) );
            if (l_cols == 0)
                return l_data;
            
            // each element in the array is a "java.lang.Double" value, for reading the value the method "double doubleValue()" must be called, so get the ID
            const jmethodID l_valueof = java::jni::getMethodID(p_env, "java/lang/Float", "floatValue", "()F"); 
            
            // read array data
            l_data = ublas::matrix<float>(l_rows, l_cols);
            for(std::size_t i=0; i < l_rows; ++i) {
                jobjectArray l_coldata = (jobjectArray)p_env->GetObjectArrayElement(p_data, i);
                
                for(std::size_t j=0; j < std::min(l_cols, static_cast<std::size_t>(p_env->GetArrayLength(l_coldata))); ++j)
                    l_data(i,j) = p_env->CallFloatMethod( p_env->GetObjectArrayElement(l_coldata, j), l_valueof );
            }

            return l_data;
        }
    
        
        /** creates a 2D java array of an ublas double matrix
         * @param p_env JNI environment
         * @param p_data input data matrix
         * @return java array / or a null object if the matrix is empty
         **/
        inline jobjectArray jni::getJObjectArrayFromMatrix( JNIEnv* p_env, const ublas::matrix<double>& p_data )
        {
            if ( (p_data.size1() == 0) || (p_data.size2() == 0) )
                return (jobjectArray)p_env->NewGlobalRef(NULL);
            
            jclass l_elementclass   = NULL;
            jmethodID l_elementctor = NULL;
            java::jni::getCtor(p_env, "java/lang/Double", "(D)V", l_elementclass, l_elementctor);
            
            // create the row array
            jobjectArray l_row = p_env->NewObjectArray( static_cast<jint>(p_data.size1()), p_env->FindClass("[Ljava/lang/Double;"), NULL );
            for(std::size_t i=0; i < p_data.size1(); ++i) {
                
                // create column array and fill data into the double java object
                jobjectArray l_col = p_env->NewObjectArray( static_cast<jint>(p_data.size2()), l_elementclass, NULL );
                for(std::size_t j=0; j < p_data.size2(); ++j)
                    p_env->SetObjectArrayElement(l_col, j, p_env->NewObject(l_elementclass, l_elementctor, p_data(i,j)) );
                
                p_env->SetObjectArrayElement(l_row, i, l_col);
            }
            
            return l_row;
        }
    
    
        /** creates a 2D java array of an ublas float matrix
         * @param p_env JNI environment
         * @param p_data input data matrix
         * @return java array / or a null object if the matrix is empty
         **/
        inline jobjectArray jni::getJObjectArrayFromMatrix( JNIEnv* p_env, const ublas::matrix<float>& p_data )
        {
            if ( (p_data.size1() == 0) || (p_data.size2() == 0) )
                return (jobjectArray)p_env->NewGlobalRef(NULL);
            
            jclass l_elementclass   = NULL;
            jmethodID l_elementctor = NULL;
            java::jni::getCtor(p_env, "java/lang/Float", "(F)V", l_elementclass, l_elementctor);
            
            // create the row array
            jobjectArray l_row = p_env->NewObjectArray( static_cast<jint>(p_data.size1()), p_env->FindClass("[Ljava/lang/Float;"), NULL );
            for(std::size_t i=0; i < p_data.size1(); ++i) {
                
                // create column array and fill data into the double java object
                jobjectArray l_col = p_env->NewObjectArray( static_cast<jint>(p_data.size2()), l_elementclass, NULL );
                for(std::size_t j=0; j < p_data.size2(); ++j)
                    p_env->SetObjectArrayElement(l_col, j, p_env->NewObject(l_elementclass, l_elementctor, p_data(i,j)) );
                
                p_env->SetObjectArrayElement(l_row, i, l_col);
            }
            
            return l_row;
        }
    
        
        /** get the ordinal [0,1,..] value representation if the java enum value
         * @param p_env JNI environment
         * @param p_object java enum object
         * @return ordinal value
         **/
        inline std::size_t jni::getEnumOrdinalValue( JNIEnv* p_env, const jobject& p_object )
        {
            return static_cast<std::size_t>(p_env->CallIntMethod(p_object, jni::getMethodID(p_env, p_object, "ordinal", "()I")));
        }
    
    
        /** converts a ublas::vector to a java array
         * @param p_env JNI environment
         * @param p_data vector
         * @return java array
         **/
        inline jobjectArray jni::getJObjectArrayFromVector( JNIEnv* p_env, const ublas::vector<double>& p_data )
        {
            if (p_data.size() == 0)
                return (jobjectArray)p_env->NewGlobalRef(NULL);
            
            jclass l_elementclass   = NULL;
            jmethodID l_elementctor = NULL;
            java::jni::getCtor(p_env, "java/lang/Double", "(D)V", l_elementclass, l_elementctor);
            
            jobjectArray l_vec = p_env->NewObjectArray( static_cast<jint>(p_data.size()), l_elementclass, NULL );
            for(std::size_t i=0; i < p_data.size(); ++i)
                p_env->SetObjectArrayElement(l_vec, i, p_env->NewObject(l_elementclass, l_elementctor, p_data(i)) );
            
            return l_vec;
        }
    
    
        /** converts a ublas::vector to a java array
         * @param p_env JNI environment
         * @param p_data vector
         * @return java array
         **/
        inline jobjectArray jni::getJObjectArrayFromVector( JNIEnv* p_env, const ublas::vector<float>& p_data )
        {
            if (p_data.size() == 0)
                return (jobjectArray)p_env->NewGlobalRef(NULL);
            
            jclass l_elementclass   = NULL;
            jmethodID l_elementctor = NULL;
            java::jni::getCtor(p_env, "java/lang/Float", "(F)V", l_elementclass, l_elementctor);
            
            jobjectArray l_vec = p_env->NewObjectArray( static_cast<jint>(p_data.size()), l_elementclass, NULL );
            for(std::size_t i=0; i < p_data.size(); ++i)
                p_env->SetObjectArrayElement(l_vec, i, p_env->NewObject(l_elementclass, l_elementctor, p_data(i)) );
            
            return l_vec;
        }
    
}}
#endif
