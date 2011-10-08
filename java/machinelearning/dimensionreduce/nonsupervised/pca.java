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

package machinelearning.dimensionreduce.nonsupervised;



/** create the principal component analysis (PCA)
 * $LastChangedDate$
 **/
public class pca<T extends Number> extends machinelearning.Object implements Reduce<T> {
    
    /** inner class interface for using the delegate pattern
     * with the conrect type binding of the native class
     **/
    private interface Strategy<L extends Number> extends Reduce<L> {
        
        /** returning dimensions **/
        public int getDimension();

        /** map data
         * @param p_data 2D input array (matrix)
         * @return projected data
         **/
        public L[][] map( L[][] p_data );
        
        /** returns the eigenvectors of projection
         * @return 2D array with eigenvectors
         **/
        public L[][] getProject();
        
        /** freeing memory **/
        public void dispose();
        
    }
    
    
    /** delegate class for float datatype **/
    private class delegate_float implements Strategy<Float> {
        
        /** private pointer member **/
        private final long cpp_ptr;
        
        /** constructor
         * @param p_dim number of dimension
         **/
        public delegate_float( int p_dim ) { cpp_ptr = cpp_ctor(p_dim); }
        
        /** returns the number of dimension
         * @return dimension
         **/
        public native int getDimension();
        
        /** maps the data
         * @param p_data 2D input array (matrix)
         * @return projected data
         **/
        public native Float[][] map( Float[][] p_data );
        
        /** returns the eigenvectors of projection
         * @return 2D array with eigenvectors
         **/
        public native Float[][] getProject();
        
        /** release objects **/
        public native void dispose();
        
        /** JNI constructor call
         * @param p_dim nuber of dimension
         **/
        private native long cpp_ctor( int p_dim );
        
        /** finalizer **/
        protected void finalize() { try { dispose(); } catch(Exception e) {} }
    }
    
    
    /** delegate class for double datatype **/
    private class delegate_double implements Strategy<Double> {
        
        /** private pointer member **/
        private final long cpp_ptr;
        
        /** constructor
         * @param p_dim number of dimension
         **/
        public delegate_double( int p_dim ) { cpp_ptr = cpp_ctor(p_dim); }
        
        /** returns the number of dimension
         * @return dimension
         **/
        public native int getDimension();
        
        /** maps the data
         * @param p_data 2D input array (matrix)
         * @return projected data
         **/
        public native Double[][] map( Double[][] p_data );
        
        /** returns the eigenvectors of projection
         * @return 2D array with eigenvectors
         **/
        public native Double[][] getProject();
        
        /** release objects **/
        public native void dispose();
        
        /** JNI constructor call
         * @param p_dim nuber of dimension
         **/
        private native long cpp_ctor( int p_dim );
        
        /** finalizer **/
        protected void finalize() { try { dispose(); } catch(Exception e) {} }
    }
    
    
    
    /** member for storing the delegated object **/
    private Strategy<T> m_delegate = null;
    
    
    /** constructor
     * @param p_type reference to the generic type of the class
     * @param p_dim number of target dimensions
     **/
    public pca( Class p_type, int p_dim ) {
        if (p_type == Float.class)
            m_delegate = (Strategy<T>)(new delegate_float(p_dim));
        else
            if (p_type == Double.class)
                m_delegate = (Strategy<T>)(new delegate_double(p_dim));
            else
               throw new machinelearning.exception.Unknowntype("datatype can not use with PCA");
    }
    
    /** finalizer, that calls the disposer with exception handling **/
    protected void finalize() { dispose(); }
        
    /** run the mapping structure
     * @param p_data input data / matrix
     * @return project data
     **/
    public T[][] map( T[][] p_data ) { return m_delegate.map(p_data); }
    
    /** returns the project dimension
     * @return number of dimensions
     **/
    public int getDimension() { return m_delegate.getDimension(); }
    
    /** returns the projection vectors
     * @return array with project vectors
     **/
    public T[][] getProject() { return m_delegate.getProject(); };
    
    /** dispose for clearing memory **/
    public void dispose() { m_delegate.dispose(); };
    
}
