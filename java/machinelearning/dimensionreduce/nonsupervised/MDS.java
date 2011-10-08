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



/** create the multidimensional scaling (MDS) with different algorithms
 * $LastChangedDate$
 **/
public class MDS<T extends Number> extends machinelearning.Object implements Reduce<T> {

    /** inner class with enum options of projection method
     * @note order of the enum values must be equal to the order of the C++ class
     **/
    public enum Project
    {
        METRIC, SAMMON, HIT
    }
    
    /** inner class with enum options of centering methods
     * @note order of the enum values must be equal to the order of the C++ class
     **/
    public enum Centeroption
    {
        NONE, SINGLECENTER, DOUBLECENTER
    }
            
    
    
    /** inner class interface for using the delegate pattern
     * with the conrect type binding of the native class
     **/
    private interface Strategy<L extends Number> extends Reduce<L> {
        
        /** returning dimensions **/
        public int getDimension();
        
        /** set the number of iteration
         * @param p_iteration iterations
         **/
        public void setIteration( int p_iteration );
        
        /* set number of step
         * @param p_step setps
         **/
        public void setStep( int p_step );
        
        /** set rate
         * @param p_rate rate
         **/
        public void setRate( L p_rate );
        
        /** set centering option **/
        public void setCentering( Centeroption p_center );
        
        /** map data **/
        public L[][] map( L[][] p_data );
        
        /** freeing memory **/
        public void dispose();
        
    }
    
    
    /** delegate class for float datatype **/
    private class DelegateFloat implements Strategy<Float> {
        
        /** private pointer member **/
        private final long cpp_ptr;
        
        /** constructor
         * @param p_dim number of dimension
         * @param p_project project option
         **/
        public DelegateFloat( int p_dim, Project p_project ) { cpp_ptr = cppCtor(p_dim, p_project); }
        
        /** returns the number of dimension
         * @return dimension
         **/
        public native int getDimension();
        
        /** set the number of iteration
         * @param p_iteration iterations
         **/
        public native void setIteration( int p_iteration );
        
        /* set number of step
         * @param p_step setps
         **/
        public native void setStep( int p_step );
        
        /** set rate
         * @param p_rate rate
         **/
        public native void setRate( Float p_rate );
        
        /** set centering option **/
        public native void setCentering( Centeroption p_center );
        
        /** maps the data
         * @param p_data 2D input array (matrix)
         * @return projected data
         **/
        public native Float[][] map( Float[][] p_data );
        
        /** release objects **/
        public native void dispose();
        
        /** JNI constructor call
         * @param p_dim nuber of dimension
         * @param p_project project option
         **/
        private native long cppCtor( int p_dim, Project p_project );
        
        /** finalizer **/
        protected void finalize() { try { dispose(); } catch(Exception e) {} }
    }
    
    
    /** delegate class for double datatype **/
    private class DelegateDouble implements Strategy<Double> {
        
        /** private pointer member **/
        private final long cpp_ptr;
        
        /** constructor
         * @param p_dim number of dimension
         * @param p_project project option
         **/
        public DelegateDouble( int p_dim, Project p_project ) { cpp_ptr = cppCtor(p_dim, p_project); }
        
        /** returns the number of dimension
         * @return dimension
         **/
        public native int getDimension();
        
        /** set the number of iteration
         * @param p_iteration iterations
         **/
        public native void setIteration( int p_iteration );
        
        /* set number of step
         * @param p_step setps
         **/
        public native void setStep( int p_step );
        
        /** set rate
         * @param p_rate rate
         **/
        public native void setRate( Double p_rate );
        
        /** set centering option **/
        public native void setCentering( Centeroption p_center );
        
        /** maps the data
         * @param p_data 2D input array (matrix)
         * @return projected data
         **/
        public native Double[][] map( Double[][] p_data );
        
        /** release objects **/
        public native void dispose();
        
        /** JNI constructor call
         * @param p_dim nuber of dimension
         * @param p_project project option
         **/
        private native long cppCtor( int p_dim, Project p_project );
        
        /** finalizer **/
        protected void finalize() { try { dispose(); } catch(Exception e) {} }
     }
    
    
    
    /** member for storing the delegated object **/
    private Strategy<T> m_delegate = null;
    
    
    /** constructor
     * @param p_type reference to the generic type of the class
     * @param p_dim number of target dimensions
     * @param p_project projection option
     **/
    public MDS( Class p_type, int p_dim, Project p_project ) {
        if (p_type == Float.class)
            m_delegate = (Strategy<T>)(new DelegateFloat(p_dim, p_project));
        else
            if (p_type == Double.class)
                m_delegate = (Strategy<T>)(new DelegateDouble(p_dim, p_project));
            else
                throw new machinelearning.exception.Unknowntype("datatype can not use with MDS");
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
    
    /** set the number of iteration
     * @param p_iteration iterations
     **/
    public void setIteration( int p_iteration ) { m_delegate.setIteration(p_iteration); }
    
    /* set number of step
     * @param p_step setps
     **/
    public void setStep( int p_step ) { m_delegate.setStep(p_step); }
    
    /** set rate
     * @param p_rate rate
     **/
    public void setRate( T p_rate ) { m_delegate.setRate( p_rate ); }
    
    /** set centering option **/
    public void setCentering( Centeroption p_center ) { m_delegate.setCentering( p_center ); }
    
    /** dispose for clearing memory **/
    public void dispose() { m_delegate.dispose(); };
    
}
