/** 
 @cond
 #########################################################################
 # GPL License                                                           #
 #                                                                       #
 # This file is part of the Machine Learning Framework.                  #
 # Copyright (c) 2010, Philipp Kraus, <philipp.kraus@flashpixx.de>       #
 # This program is free software: you can redistribute it and/or modify  #
 # it under the terms of the GNU General Public License as published by  #
 # the Free Software Foundation, either version 3 of the License, or     #
 # (at your option) any later version.                                   #
 #                                                                       #
 # This program is distributed in the hope that it will be useful,       #
 # but WITHOUT ANY WARRANTY; without even the implied warranty of        #
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
 # GNU General Public License for more details.                          #
 #                                                                       #
 # You should have received a copy of the GNU General Public License     #
 # along with this program.  If not, see <http://www.gnu.org/licenses/>. #
 #########################################################################
 @endcond
 **/


#ifndef MACHINELEARNING_TOOLS_FILES_HDF_HPP
#define MACHINELEARNING_TOOLS_FILES_HDF_HPP

#include <string>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/algorithm/string.hpp> 
#include <H5Cpp.h>

#include "../../exception/exception.h"

//http://www.hdfgroup.uiuc.edu/UserSupport/code-examples/stratt.cpp

namespace machinelearning { namespace tools { namespace files {
    
    namespace ublas  = boost::numeric::ublas;
    

    
    /** class for reading and writing the HDF data
     * @see http://www.hdfgroup.org
     * @note hdf uses their own datatypes http://www.hdfgroup.org/HDF5/doc/cpplus_RM/classH5_1_1PredType.html 
     * @todo implement array functions
     **/
    class hdf {
        
        public :
        
            hdf( const std::string& );
            hdf( const std::string&, const bool& );
            ~hdf( void );
        
            void flush( void ) const;
            std::string getFilename( void ) const;
            std::size_t getFilesize( void ) const;
            void remove( const std::string& ) const;
            bool pathexists( const std::string& ) const;

		
			template<typename T> ublas::matrix<T> readMatrix( const std::string&, const H5::PredType& ) const;
			template<typename T> ublas::vector<T> readVector( const std::string&, const H5::PredType& ) const;
		
            std::string readString( const std::string& ) const;
			std::vector<std::string> readStringVector( const std::string& ) const;

            
            template<typename T> void write( const std::string&, ublas::matrix<T>, const H5::PredType& ) const;
            template<typename T> void write( const std::string&, ublas::vector<T>, const H5::PredType& ) const;
			
			void write( const std::string&, const std::string&, const H5::PredType& ) const;

        
        private :
            void createPathWithDataset( const std::string&, const H5::PredType&, const H5::DataSpace&, std::vector<H5::Group>&, H5::DataSet& ) const;

            /** file handler **/
            H5::H5File m_file;
        
    };
    
    
    
    
    /** constructor
     * @overload
     * @param p_file filename
     **/
    inline hdf::hdf( const std::string& p_file ) :
        m_file( p_file.c_str(), H5F_ACC_RDWR )
    {
        #ifdef NDEBUG
            H5::Exception::dontPrint();
        #endif
    }
    
    
    /** constructor
     * @overload
     * @param p_file filename
     * @param p_write bool for clear/create file
     **/
    inline hdf::hdf( const std::string& p_file, const bool& p_write ) :
        m_file( p_file.c_str(), (p_write ? H5F_ACC_TRUNC : H5F_ACC_RDWR) )
    {
        #ifdef NDEBUG
            H5::Exception::dontPrint();
        #endif
    }
    
    
    /** destructor for closing file **/
    inline hdf::~hdf( void )
    {
        flush();
        m_file.close();
    }
    

    /** flushs all data for the file **/
    inline void hdf::flush( void ) const
    {
        m_file.flush( H5F_SCOPE_GLOBAL );
    }
    
    
    /** returns the used filename
     * @return string with filename
     **/
    inline std::string hdf::getFilename( void ) const
    {
        return m_file.getFileName();
    }
    
    /** returns the filesize in bytes
     * @return filesize in bytes
     **/
    inline std::size_t hdf::getFilesize( void ) const
    {
        return m_file.getFileSize();
    }
    
    
    /** deletes an entry in the HDF file
     * @param p_path path to element
     **/
    inline void hdf::remove( const std::string& p_path ) const
    {
        m_file.unlink( p_path );
    }
    
    
	/** reads a string array from HDF file into a std::vector
	 * @param p_path path to element
	 * @return std::vector with std::string elements
     **/
	inline std::vector<std::string> hdf::readStringVector( const std::string& p_path ) const
    {
		H5::DataSet   l_dataset   = m_file.openDataSet( p_path.c_str() );
        H5::DataSpace l_dataspace = l_dataset.getSpace();
        
        // check datasetdimension
        if (l_dataspace.getSimpleExtentNdims() != 1)
            throw exception::hdf::sizecorrect();
        if (!l_dataspace.isSimple())
            throw exception::hdf::simpletype();
		
 		// get array size
		hsize_t l_size[1];
        l_dataspace.getSimpleExtentDims( l_size );		
        
		// get data
		std::vector<std::string> l_vec( l_size[0] );
        //l_dataset.read(
        
        l_dataspace.close();
        l_dataset.close();
        
        return l_vec;
    }
    
    
    /** returns the bool if path exists
     * @note we use the C function, because this is not portet to C++
     * @param p_path path
     * @return boolean
     **/
    inline bool hdf::pathexists( const std::string& p_path ) const
    {
        return H5Lexists( m_file.getId(), p_path.c_str(), H5P_DEFAULT );
    }
    
    
    /** read a simple string from file
     * @param p_path path to dataset
     * @return string with data
     **/
    inline std::string hdf::readString( const std::string& p_path ) const
    {
        H5::DataSet   l_dataset   = m_file.openDataSet( p_path.c_str() );
        H5::DataSpace l_dataspace = l_dataset.getSpace();
        
        // check datasetdimension
        if (l_dataspace.getSimpleExtentNdims() != 1)
            throw exception::hdf::sizecorrect();
        if (!l_dataspace.isSimple())
            throw exception::hdf::simpletype();       
        
        // read size of chars
        hsize_t l_strlength = l_dataset.getStorageSize(); 
        
        // creates strcutures and read data
        H5::StrType l_temp(0, l_strlength);
        char l_chars[l_strlength];
        l_dataset.read(l_chars, l_temp );
        
        l_dataspace.close();
        l_dataset.close();
        
        return std::string(l_chars);
    }
    
    
    
    /** writes a simple string to hdf
     * @param p_path path to dataset
     * @param p_value string value
     * @param p_datatype HDF% datatype
	 * @warning incomplete
     **/
    inline void hdf::write( const std::string& p_path, const std::string& p_value, const H5::PredType& p_datatype ) const
    {
        hsize_t l_size[1];
        l_size[0] = p_value.size();
        H5::DataSpace l_dataspace( 1, l_size );
        std::vector<H5::Group> l_groups;        
        
        // create Dataspace in path        
        H5::DataSet l_dataset;
        createPathWithDataset( p_path,  p_datatype, l_dataspace, l_groups, l_dataset ); 
        
        // close groups
        for(std::vector<H5::Group>::reverse_iterator it = l_groups.rbegin(); it != l_groups.rend(); ++it)
            (*it).close();
        
        l_dataspace.close();
        l_dataset.close();
    }
    
    
    
    /** create a path with dataset (path separator is "/")
     * @param p_path full path (last element dataset name)
     * @param p_datatype HDF datatype for dataset
     * @param p_dataspace space for dataset
     * @param p_groups std::vector with opened/created groups for closing
     * @param p_dataset data
     * @return HDF dataset
     **/
    inline void hdf::createPathWithDataset( const std::string& p_path, const H5::PredType& p_datatype, const H5::DataSpace& p_dataspace, std::vector<H5::Group>& p_groups, H5::DataSet& p_dataset ) const
    {
        // split string and remove first element if empty
        std::vector<std::string> l_path;
        boost::split( l_path, p_path, boost::is_any_of("/") );
        
        // clear empty elements
        for(std::vector<std::string>::iterator it = l_path.begin(); it != l_path.end(); ++it)
            if ((*it).empty())
                l_path.erase( it );
        
        
        // path must have more than zero elements
        if (l_path.size() == 0)
            throw exception::parameter(_("empty path is forbidden"));
        
        // if only one element then create dataset directly
        if (l_path.size() == 1) {
            p_dataset = m_file.createDataSet( l_path[0].c_str(), p_datatype, p_dataspace );
        } else {
            
            // more elements exists, check every group and clear emtpy elements
            H5::Group l_group;
            try {
                l_group = m_file.openGroup( l_path[0].c_str() );
            } catch ( H5::FileIException e ) {
                l_group = m_file.createGroup( l_path[0].c_str() );
            }        
            p_groups.push_back( l_group );
            
            // create path structure
            for(std::vector<std::string>::iterator it = l_path.begin()+1; it != l_path.end()-1; ++it) {
                try {
                    l_group = l_group.openGroup( (*it).c_str() );
                } catch ( H5::GroupIException e ) {
                    l_group = l_group.createGroup( (*it).c_str() );
                }
                p_groups.push_back( l_group );
            }
            
            p_dataset = l_group.createDataSet( (*l_path.end()).c_str(), p_datatype, p_dataspace );
        }
    }
    
    
    /** reads a matrix with convert to blas matrix
     * @param p_path dataset name
     * @param p_datatype datatype for reading data (see http://www.hdfgroup.org/HDF5/doc/cpplus_RM/classH5_1_1PredType.html )
     * @return ublas matrix
    **/ 
    template<typename T> inline ublas::matrix<T> hdf::readMatrix( const std::string& p_path, const H5::PredType& p_datatype ) const
    {
        H5::DataSet   l_dataset   = m_file.openDataSet( p_path.c_str() );
        H5::DataSpace l_dataspace = l_dataset.getSpace();

        // check datasetdimension
        if (l_dataspace.getSimpleExtentNdims() != 2)
            throw exception::hdf::sizecorrect();
        if (!l_dataspace.isSimple())
            throw exception::hdf::simpletype();
        
        // read matrix size and create matrix
        // (first element is column size, second row size)
        hsize_t l_size[2];
        l_dataspace.getSimpleExtentDims( l_size );

        // create temp structur for reading data
        ublas::matrix<T> l_mat(l_size[1],l_size[0]);
        T l_data[l_size[0]][l_size[1]];
        l_dataset.read( l_data, p_datatype );
        
        //copy data to ublas matrix
        for(std::size_t i=0; i < l_mat.size1(); ++i)
            for(std::size_t j=0; j < l_mat.size2(); ++j)
                l_mat(i,j) = l_data[j][i];
    
        l_dataspace.close();
        l_dataset.close();
        return l_mat;
    }
    
    
    
    /** reads a vector with convert to blas matrix
     * @param p_path dataset path & name
     * @param p_datatype datatype for reading data
     * @return ublas matrix
    **/ 
    template<typename T> inline ublas::vector<T> hdf::readVector( const std::string& p_path, const H5::PredType& p_datatype ) const
    {
        H5::DataSet   l_dataset   = m_file.openDataSet( p_path.c_str() );
        H5::DataSpace l_dataspace = l_dataset.getSpace();
        
        // check datasetdimension
        if (l_dataspace.getSimpleExtentNdims() != 1)
            throw exception::hdf::sizecorrect();
        if (!l_dataspace.isSimple())
            throw exception::hdf::simpletype();
        
        // read matrix size and create vector
        hsize_t l_size[1];
        l_dataspace.getSimpleExtentDims( l_size );
        
        // create temp structur for reading data
        ublas::vector<T> l_vec(l_size[0]);
        T l_data[l_size[0]];
        l_dataset.read( l_data, p_datatype );
        
        //copy data to ublas matrix
        for(std::size_t i=0; i < l_vec.size(); ++i)
            l_vec(i) = l_data[i];
        
        l_dataspace.close();
        l_dataset.close();
        return l_vec;
    }
    
    
    
    /** write a blas matrix to hdf file
     * @param p_path dataset path & name
     * @param p_dataset matrixdata
     * @param p_datatype datatype for writing data (see http://www.hdfgroup.org/HDF5/doc/cpplus_RM/classH5_1_1PredType.html )
    **/
    template<typename T> inline void hdf::write( const std::string& p_path, ublas::matrix<T> p_dataset, const H5::PredType& p_datatype ) const
    {        
        // define Structurs
        H5::DSetCreatPropList l_defaultvalue;
        l_defaultvalue.setFillValue( p_datatype, 0 );     
        hsize_t l_size[2];
        l_size[0] = p_dataset.size2();
        l_size[1] = p_dataset.size1();
        H5::DataSpace l_dataspace( 2, l_size );
        std::vector<H5::Group> l_groups;

        // create Dataspace in path        
        H5::DataSet l_dataset;
        createPathWithDataset( p_path,  p_datatype, l_dataspace, l_groups, l_dataset ); 
        
        // copy matrix to dataset
        T l_data[p_dataset.size2()][p_dataset.size1()];
        for(std::size_t i=0; i < p_dataset.size1(); ++i)
            for(std::size_t j=0; j < p_dataset.size2(); ++j)
                l_data[j][i] = p_dataset(i,j);
        l_dataset.write( l_data, p_datatype, l_dataspace  );
        l_dataspace.close();
        
        // close groups
        for(std::vector<H5::Group>::reverse_iterator it = l_groups.rbegin(); it != l_groups.rend(); ++it)
            (*it).close();
    }
    
    
    /** write a blas vector to hdf file
     * @param p_path dataset path & name
     * @param p_dataset vectordata
     * @param p_datatype datatype for writing data (see http://www.hdfgroup.org/HDF5/doc/cpplus_RM/classH5_1_1PredType.html )
    **/
    template<typename T> inline void hdf::write( const std::string& p_path, ublas::vector<T> p_dataset, const H5::PredType& p_datatype ) const
    {        
        // define Structurs
        H5::DSetCreatPropList l_defaultvalue;
        l_defaultvalue.setFillValue( p_datatype, 0 );        
        hsize_t l_size[1];
        l_size[0] = p_dataset.size();       
        H5::DataSpace l_dataspace( 1, l_size );
        std::vector<H5::Group> l_groups;
        
        // create Dataspace in path
        H5::DataSet l_dataset;
        createPathWithDataset( p_path,  p_datatype, l_dataspace, l_groups, l_dataset );
        
        // copy matrix to dataset
        T l_data[p_dataset.size()];
        for(std::size_t i=0; i < p_dataset.size(); ++i)
            l_data[i] = p_dataset(i);
        l_dataset.write( l_data, p_datatype, l_dataspace  );
        l_dataspace.close();
        
        // close groups
        for(std::vector<H5::Group>::reverse_iterator it = l_groups.rbegin(); it != l_groups.rend(); ++it)
            (*it).close();
    }
    
    
    
    
};};};

#endif
