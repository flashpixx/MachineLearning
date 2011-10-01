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

package machinelearning.math;


public class vector<T> extends machinelearning.object {
    
    static { System.loadLibrary("machinelearning"); }
    
    private final long cpp_ptr;
    
    
    
    public vector( int p_size ) { cpp_ptr = cpp_ctor(p_size); }
    
    public vector( int p_size, T p_value ) { cpp_ptr = cpp_ctor(p_size, p_value); }
    
    protected void finalize() { this.dispose(); }
    
    
    public native void dispose();
    
    public native int size();
    
    public native T get( int p_index );
    
    public native T[] get();
    
    public native void set( int p_index, T p_value );
    
    public native void set( T[] p_values );
    
                  
    private native long cpp_ctor( int p_size );
    
    private native long cpp_ctor( int p_size, T p_value );
    
    
}