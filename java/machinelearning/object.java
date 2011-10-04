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
import java.lang.reflect.*;


/** class that must derviated in each object in the package, that uses
 * the JNI interface, because each class must implement the methods for
 * clearing memory management and loading native library
 * $LastChangedDate$
 **/
public abstract class object {
    
    
    /** static call of the external library, each class that uses the native
     * interface calls must be derivated from this abstract base class, so
     * we create a own glue code of library binding
     **/
    static { 
        
        // first try to load the JNI library (first load)
        try {
            System.loadLibrary("machinelearning");
        } catch (UnsatisfiedLinkError e_link1) {
            
            try {
                // property value (like "java.library.path") are cached so we clear the cache manually to allow reloading
                Field l_syspath = ClassLoader.class.getDeclaredField("sys_paths");
                l_syspath.setAccessible( true );
                l_syspath.set( null, null );
            } catch(Exception e_prop) { e_prop.printStackTrace(); } finally {
                
                // create first a temp directory for setting the native libraries
                File l_temp = new File( System.getProperty("java.io.tmpdir") + "machinelearning" );
                if (!l_temp.isDirectory())
                    l_temp.mkdirs();
                
                // add temporary directory to the library path
                System.setProperty( "java.library.path", System.getProperty("java.library.path") + System.getProperty("path.separator") + l_temp );
            
                // try to load library with the temporary path (second load)
                try {
                    System.loadLibrary("machinelearning");
                } catch (UnsatisfiedLinkError e_link2) {
                    
                    try {
                        // extract from the classname the location of the JAR (remove URL prefix jar:file: and suffix after .jar)
                        String l_jarfile = Class.forName("machinelearning.object").getResource("").toString();
                        l_jarfile = l_jarfile.substring(9, l_jarfile.lastIndexOf(".jar!")) + ".jar";
                    
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
                            }
                        }
                    } catch(Exception e_file) { e_file.printStackTrace(); }
                    
                    // last try to load library (third load)
                    System.loadLibrary("machinelearning");
                }
                
            }
        }
    }
    
    
    /** dispose method for freeing memory of underlaying objects **/
    public abstract void dispose();
    
    
}