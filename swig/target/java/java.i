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


/** Java interface file for converting Java types and data into C++ 
 * datatypes and UBlas structurs
 * $LastChangedDate$
 * @todo add support for other datatypes
 **/



// ---------------------------------------------------------------------------------------------------------------------------------------------
// type converting from C++ types to Java types (both directions)
%typemap(jni)       ublas::matrix<double>,                          ublas::matrix<double>&                          "jobjectArray"
%typemap(jtype)     ublas::matrix<double>,                          ublas::matrix<double>&                          "Double[][]"
%typemap(jstype)    ublas::matrix<double>,                          ublas::matrix<double>&                          "Double[][]"

%typemap(jni)       ublas::symmetric_matrix<double, ublas::upper>,  ublas::symmetric_matrix<double, ublas::upper>&  "jobjectArray"
%typemap(jtype)     ublas::symmetric_matrix<double, ublas::upper>,  ublas::symmetric_matrix<double, ublas::upper>&  "Double[][]"
%typemap(jstype)    ublas::symmetric_matrix<double, ublas::upper>,  ublas::symmetric_matrix<double, ublas::upper>&  "Double[][]"

%typemap(jni)       std::vector<double>,                            std::vector<double>&                            "jobjectArray"
%typemap(jtype)     std::vector<double>,                            std::vector<double>&                            "Double[]"
%typemap(jstype)    std::vector<double>,                            std::vector<double>&                            "Double[]"

%typemap(jni)       std::vector< ublas::matrix<double> >,           std::vector< ublas::matrix<double> >&           "jobject"
%typemap(jtype)     std::vector< ublas::matrix<double> >,           std::vector< ublas::matrix<double> >&           "java.util.ArrayList<Double[][]>"
%typemap(jstype)    std::vector< ublas::matrix<double> >,           std::vector< ublas::matrix<double> >&           "java.util.ArrayList<Double[][]>"

%typemap(jni)       std::vector<std::string>,                       std::vector<std::string>&                       "jobjectArray"
%typemap(jtype)     std::vector<std::string>,                       std::vector<std::string>&                       "String[]"
%typemap(jstype)    std::vector<std::string>,                       std::vector<std::string>&                       "String[]"

%typemap(jni)       std::size_t,                                    std::size_t&                                    "jlong"
%typemap(jtype)     std::size_t,                                    std::size_t&                                    "long"
%typemap(jstype)    std::size_t,                                    std::size_t&                                    "long"

%typemap(jni)       ublas::indirect_array<>,                        ublas::indirect_array<>&                        "jobjectArray"
%typemap(jtype)     ublas::indirect_array<>,                        ublas::indirect_array<>&                        "Long[]"
%typemap(jstype)    ublas::indirect_array<>,                        ublas::indirect_array<>&                        "Long[]"

%typemap(jni)       std::string,                                    std::string&                                    "jstring"
%typemap(jtype)     std::string,                                    std::string&                                    "String"
%typemap(jstype)    std::string,                                    std::string&                                    "String"



// ---------------------------------------------------------------------------------------------------------------------------------------------
// input type, so that the value type will be passed through to the JNI (connection JNI to Java proxy class)
%typemap(javain)    ublas::matrix<double>,                          ublas::matrix<double>&                          "$javainput"
%typemap(javain)    ublas::symmetric_matrix<double, ublas::upper>,  ublas::symmetric_matrix<double, ublas::upper>&  "$javainput"
%typemap(javain)    std::size_t,                                    std::size_t&                                    "$javainput"
%typemap(javain)    std::string,                                    std::string&                                    "$javainput"
%typemap(javain)    std::vector<std::string>,                       std::vector<std::string>&                       "$javainput"


// swigtype will be removed by output types
%typemap(javaout) SWIGTYPE {
    return $jnicall;
}



// ---------------------------------------------------------------------------------------------------------------------------------------------
// main typemaps for "return types" with code converting
%typemap(out, optimal=1, noblock=1) ublas::matrix<double>, ublas::matrix<double>& {
    $result = swig::java::getArray(jenv, $1);
}

%typemap(out, optimal=1, noblock=1) std::vector<double>, std::vector<double>& {
    $result = swig::java::getArray(jenv, $1);
}

%typemap(out, optimal=1, noblock=1) ublas::indirect_array<>, ublas::indirect_array<>& {
    $result = swig::java::getArray(jenv, $1);
}

%typemap(out, optimal=1, noblock=1) std::vector< ublas::matrix<double> >, std::vector< ublas::matrix<double> >& {
    $result = swig::java::getArrayList(jenv, $1);
}

%typemap(out, optimal=1, noblock=1) ublas::symmetric_matrix<double, ublas::upper>, ublas::symmetric_matrix<double, ublas::upper>& {
    $result = swig::java::getArray(jenv, static_cast< ublas::matrix<double> >($1));
}

%typemap(out, optimal=1, noblock=1) std::size_t, std::size_t& {
    $result = $1;
}



// ---------------------------------------------------------------------------------------------------------------------------------------------
// main typemaps for "parameter types" with code converting
%typemap(in, optimal=1) ublas::matrix<double>&, ublas::symmetric_matrix<T, ublas::upper>& {
    ublas::matrix<double> l_arg = swig::java::getDoubleMatrixFrom2DArray(jenv, $input);
    $1                          = &l_arg;
}

// the return parameter must be copy into a local variable (l_param)
// because Swig uses pointers to reference the parameter
%typemap(in, optimal=1, noblock=1) std::string, std::string& (std::string l_param) {
    l_param = swig::java::getString(jenv, $input);
    $1      = &l_param;
}

%typemap(in, optimal=1, noblock=1) std::vector<std::string>, std::vector<std::string>& (std::vector<std::string> l_param) {
    l_param = swig::java::getStringVectorFromArray(jenv, $input);
    $1      = &l_param;
}

%typemap(in, optimal=1, noblock=1) std::size_t, std::size_t& {
    $1 = ($1_ltype)& $input;
}



// ---------------------------------------------------------------------------------------------------------------------------------------------
// structure that is included in each cpp file
%{
#include "machinelearning/swig/target/java/java.hpp"
namespace swig  = machinelearning::swig;
namespace ublas = boost::numeric::ublas;
%}



// ---------------------------------------------------------------------------------------------------------------------------------------------
// Java code for loading the dynamic library
%pragma(java) jniclasscode=%{
    
    /** static call of the external library, each class that uses the native
     * interface calls must be derivated from this abstract base class, so
     * we create a own glue code of library binding
     * @bug does not work if system path(es) not set. Depended libraries eg boost_system
     * are not loaded from the machinelearning-library.
     * @todo the memory management can be worked with addShutDownHook(), so on this
     * event objects can be destroyed ( http://download.oracle.com/javase/1.4.2/docs/api/java/lang/Runtime.html#addShutdownHook%28java.lang.Thread%29 )
     **/    
    static {
        
        // first try to load the JNI library directly
        try {
            System.loadLibrary("machinelearning");
        } catch (UnsatisfiedLinkError e_link1) {
            
            // create first a temp directory for setting the native libraries
            java.io.File l_temp = new java.io.File(  System.getProperty("java.io.tmpdir") + System.getProperty("file.separator") + "machinelearning" );
            if (!l_temp.isDirectory())
                l_temp.mkdirs();
            
            String l_lib = l_temp + System.getProperty("file.separator") + System.mapLibraryName("machinelearning");
            // OSX adds *.jnilib but switch to *.dylib
            if (System.getProperty("os.name").toLowerCase().indexOf( "mac" ) >= 0)
                l_lib = l_lib.substring(0, l_lib.indexOf(".jnilib")) + ".dylib";
            
            // try to load the libraries manually from the temporary directory
            try {
                System.load(l_lib);
            } catch (UnsatisfiedLinkError e_link2) {
                
                // try to read class name
                if (Thread.currentThread().getStackTrace().length < 2)
                    throw new RuntimeException("can not determine class name");
                                
                // extract files from the Jar
                try {
                    // extract from the classname the location of the JAR (remove URL prefix jar:file: and suffix after .jar)
                    String l_jarfile = java.net.URLDecoder.decode(Class.forName(Thread.currentThread().getStackTrace()[1].getClassName()).getResource("").toString(),"UTF-8");
                    l_jarfile        = l_jarfile.substring(9, l_jarfile.lastIndexOf(".jar!")) + ".jar";
                    
                    // open the Jar file to get all Jar entries and extract the "native" subdirectory
                    java.util.jar.JarFile l_jar = new java.util.jar.JarFile( l_jarfile, true );
                    java.util.Enumeration<java.util.jar.JarEntry> l_list = l_jar.entries();
                    
                    while (l_list.hasMoreElements()) {
                        
                        String l_fileentry = l_list.nextElement().getName();

                        if ( (l_fileentry.startsWith("native/")) && (l_fileentry.charAt(l_fileentry.length()-1) != '/') ) {
                            
                            // copy stream with buffered stream because it's faster
                            java.io.InputStream l_instream            = l_jar.getInputStream(l_jar.getEntry(l_fileentry));
                            java.io.BufferedInputStream l_binstream   = new java.io.BufferedInputStream(l_instream);
                            
                            java.io.FileOutputStream l_outstream      = new java.io.FileOutputStream(l_temp.toString() + System.getProperty("file.separator") + l_fileentry.substring(7, l_fileentry.length()) );
                            java.io.BufferedOutputStream l_boutstream = new java.io.BufferedOutputStream(l_outstream);
                            
                            int l_data;
                            while ((l_data = l_binstream.read ()) != -1)
                                l_boutstream.write(l_data);
                            
                            l_binstream.close();
                            l_instream.close();
                            
                            l_boutstream.close();
                            l_outstream.close();
                            
                            l_binstream  = null;
                            l_instream   = null;
                            l_boutstream = null;
                            l_outstream  = null;
                        }
                        
                        l_fileentry = null;
                    }
                    
                    l_list    = null;
                    l_jar     = null;
                    l_jarfile = null;
                } catch(Exception e_file) { e_file.printStackTrace(); } finally {
                    System.load(l_lib);
                }
            }
            l_lib = null;
            l_temp = null;
        }
    }    
    
%}
