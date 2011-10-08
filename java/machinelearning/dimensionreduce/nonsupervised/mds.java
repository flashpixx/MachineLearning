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
public class mds<T extends Number> extends machinelearning.object implements reduce<T> {

    /** inner class with enum options of projection method **/
    public enum project
    {
        metric, sammon, hit
    }
    
    /** inner class with enum options of centering methods **/
    public enum centeroption
    {
        none, singlecenter, doublecenter
    }
            
    
    
    /** inner class interface for using the delegate pattern
     * with the conrect type binding of the native class
     **/
    private interface strategy<L extends Number> extends reduce<L> {
        
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
        public void setCentering( centeroption p_center );
        
        /** map data **/
        public L[][] map( L[][] p_data );
        
        /** freeing memory **/
        public void dispose();
        
    }
    
    
    /** delegate class for float datatype **/
    private class delegate_float implements strategy<Float> {
        
        /** private pointer member **/
        private final long cpp_ptr;
        
        /** constructor
         * @param p_dim number of dimension
         * @param p_project project option
         **/
        public delegate_float( int p_dim, project p_project ) { cpp_ptr = cpp_ctor(p_dim, p_project); }
        
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
        public native void setCentering( centeroption p_center );
        
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
        private native long cpp_ctor( int p_dim, project p_project );
        
        /** finalizer **/
        protected void finalize() { try { dispose(); } catch(Exception e) {} }
    }
    
    
    /** delegate class for double datatype **/
    private class delegate_double implements strategy<Double> {
        
        /** private pointer member **/
        private final long cpp_ptr;
        
        /** constructor
         * @param p_dim number of dimension
         * @param p_project project option
         **/
        public delegate_double( int p_dim, project p_project ) { cpp_ptr = cpp_ctor(p_dim, p_project); }
        
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
        public native void setCentering( centeroption p_center );
        
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
        private native long cpp_ctor( int p_dim, project p_project );
        
        /** finalizer **/
        protected void finalize() { try { dispose(); } catch(Exception e) {} }
     }
    
    
    
    /** member for storing the delegated object **/
    private strategy<T> m_delegate = null;
    
    
    /** constructor
     * @param p_type reference to the generic type of the class
     * @param p_dim number of target dimensions
     * @param p_project projection option
     **/
    public mds( Class p_type, int p_dim, project p_project ) {
        if (p_type == Float.class)
            m_delegate = (strategy<T>)(new delegate_float(p_dim, p_project));
        else
            if (p_type == Double.class)
                m_delegate = (strategy<T>)(new delegate_double(p_dim, p_project));
            else
                throw new machinelearning.exception.unknowntype("datatype can not use with MDS");
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
    public void setCentering( centeroption p_center ) { m_delegate.setCentering( p_center ); }
    
    /** dispose for clearing memory **/
    public void dispose() { m_delegate.dispose(); };
    
}
