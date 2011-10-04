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
public class pca<T extends Number> extends machinelearning.object implements reduce<T> {
    
    private interface strategy<L> {
        
        public int getDimension();

        public L[][] map( L[][] p_data );
        
        public L[][] getProject();
        
        public void dispose();
        
    }
    
    
    private class delegate_float implements strategy<Float> {
        
        private final long cpp_ptr;
        
        public delegate_float( int p_dim ) { cpp_ptr = cpp_ctor(p_dim); }
        
        public native int getDimension();
        
        public native Float[][] map( Float[][] p_data );
        
        public native Float[][] getProject();
        
        public native void dispose();
        
        private native long cpp_ctor( int p_dim );
    }
    
    private class delegate_double implements strategy<Double> {
        
        private final long cpp_ptr;
        
        public delegate_double( int p_dim ) { cpp_ptr = cpp_ctor(p_dim); }
        
        public native int getDimension();
        
        public native Double[][] map( Double[][] p_data );
        
        public native Double[][] getProject();
        
        public native void dispose();
        
        private native long cpp_ctor( int p_dim );
    }
    
    
    
    
    private strategy<T> m_pca = null;
    
    
    /** constructor
     * @param p_dim number of target dimensions
     **/
    public pca( int p_dim ) {
    /*
        if (T instanceof Float)
            m_pca = new pca_float(p_dim);
        if (T instanceof Double)
            m_pca = new pca_double(p_dim);
        */
        if (m_pca == null)
            throw new machinelearning.exception.runtime("datatype can not use with PCA");
    }
    
    /** finalizer, that calls the disposer with exception handling **/
    protected void finalize() { try { dispose(); } catch(Exception e) {} }
    
    
    /** run the mapping structure
     * @param p_data input data / matrix
     * @return project data
     **/
    public T[][] map( T[][] p_data ) { return m_pca.map(p_data); }
    
    /** returns the project dimension
     * @return number of dimensions
     **/
    public int getDimension() { return m_pca.getDimension(); }
    
    /** returns the projection vectors
     * @return array with project vectors
     **/
    public T[][] getProject() { return m_pca.getProject(); };
    
    /** dispose for clearing memory **/
    public void dispose() { m_pca.dispose(); };
    
}
