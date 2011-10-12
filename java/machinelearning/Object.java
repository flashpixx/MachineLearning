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

package machinelearning;

import java.io.*;
import java.util.*;
import java.util.jar.*;


/** class that must derviated in each object in the package, that uses
 * the JNI interface, because each class must implement the methods for
 * clearing memory management and loading native library
 * $LastChangedDate$
 **/
public abstract class Object {


    /** static call of the external library, each class that uses the native
     * interface calls must be derivated from this abstract base class, so
     * we create a own glue code of library binding
     * @bug does not work if system path(es) not set. Depended libraries eg boost_system
     * are not loaded from the machinelearning-library.
     * @todo switch to a own classload for loading dynamic libraries with the depend
     * libraries
     * @todo add with the own classload a memory management so that the classload stores
     * references to each object that use native calls, so a main classloader call
     * runs each dispose-method on the native objects for cleaning memory
     * @todo the memory management can be worked with addShutDownHook(), so on this
     * event objects can be destroyed
     **/
    static {

        // first try to load the JNI library directly
        try {
            System.loadLibrary("machinelearning");
        } catch (UnsatisfiedLinkError e_link1) {
            
            // library can not load, so define all used libraries
            // in the order that are loaded and mask them with the
            // c++ style preprocessor commands. The Scons script
            // sets the correct source line like
            // final String[] l_libraries = {...};
            
            //#loadLibrary#
            
            final String[] l_libraries = {"xxx"};
            
            // create first a temp directory for setting the native libraries
            File l_temp = new File(  System.getProperty("java.io.tmpdir") + System.getProperty("file.separator") + "machinelearning" );
            if (!l_temp.isDirectory())
                l_temp.mkdirs();
            
                
            // try to load the libraries manually from the temporary directory
            try {
                for(String i : l_libraries) {
                    // create manually path
                    String l_lib = l_temp + System.getProperty("file.separator") + System.mapLibraryName(i);
                    
                    // OSX adds *.jnilib but switch to *.dylib
                    if (System.getProperty("os.name").toLowerCase().indexOf( "mac" ) >= 0)
                        l_lib = l_lib.substring(0, l_lib.indexOf(".jnilib")) + ".dylib";
        
                    // load library
                    System.load(l_lib);
                    l_lib = null;
                }
            } catch (UnsatisfiedLinkError e_link2) {
                
                // extract the machinelearning library first and try to load them
                // on error extract all other libraries
                
            
                // extract files from the Jar
                try {
                    // extract from the classname the location of the JAR (remove URL prefix jar:file: and suffix after .jar)
                    String l_jarfile = Class.forName("machinelearning.Object").getResource("").toString();
                    l_jarfile        = l_jarfile.substring(9, l_jarfile.lastIndexOf(".jar!")) + ".jar";
                    
                    // open the Jar file to get all Jar entries and extract the "native" subdirectory
                    JarFile l_jar = new JarFile( l_jarfile, true );
                    Enumeration<JarEntry> l_list = l_jar.entries();
                    
                    while (l_list.hasMoreElements()) {
                        
                        String l_fileentry = l_list.nextElement().getName();
                        if ( (l_fileentry.startsWith("native/")) && (l_fileentry.charAt(l_fileentry.length()-1) != '/') ) {
                            
                            // copy stream with buffered stream because it's faster
                            InputStream l_instream            = l_jar.getInputStream(l_jar.getEntry(l_fileentry));
                            BufferedInputStream l_binstream   = new BufferedInputStream(l_instream);
                            
                            FileOutputStream l_outstream      = new FileOutputStream(l_temp.toString() + System.getProperty("file.separator") + l_fileentry.substring(7, l_fileentry.length()) );
                            BufferedOutputStream l_boutstream = new BufferedOutputStream(l_outstream);
                            
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
                
                    // try to load libraries manually
                    for(String i : l_libraries) {
                        // create manually path
                        String l_lib = l_temp + System.getProperty("file.separator") + System.mapLibraryName(i);
                    
                        // OSX adds *.jnilib but switch to *.dylib
                        if (System.getProperty("os.name").toLowerCase().indexOf( "mac" ) >= 0)
                            l_lib = l_lib.substring(0, l_lib.indexOf(".jnilib")) + ".dylib";
                          
                        // load library
                        System.load(l_lib);
                        l_lib = null;
                    }
                }
            }
                
            l_temp = null;
            
        }
    }


    /** dispose method for freeing memory of underlaying objects **/
    public abstract void dispose();


}
