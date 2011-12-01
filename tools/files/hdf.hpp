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

#if defined(MACHINELEARNING_FILES) && defined(MACHINELEARNING_FILES_HDF)

#ifndef __MACHINELEARNING_TOOLS_FILES_HDF_HPP
#define __MACHINELEARNING_TOOLS_FILES_HDF_HPP

#include <string>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/storage.hpp>
#include <boost/algorithm/string.hpp> 
#include <H5Cpp.h>


#include <iostream>
#include <boost/numeric/ublas/io.hpp>

#include "../../exception/exception.h"



namespace machinelearning { namespace tools { namespace files {
    
    namespace ublas  = boost::numeric::ublas;
    
    
    
    /** class for reading and writing the HDF data
     * $LastChangedDate$
     * @see http://www.hdfgroup.org
     * @note hdf uses their own datatypes http://www.hdfgroup.org/HDF5/doc/cpplus_RM/classH5_1_1PredType.html 
     * @todo add ndim cube support
     * @todo iterate over groups
     * @todo adding moving objects
     * @todo adding MPI support
     **/
    class hdf
    {
        
        public :
        
            hdf( const std::string& );
            hdf( const std::string&, const bool& );
            ~hdf( void );
            
            void open( const std::string&, const bool& = false );
            void flush( void ) const;
            std::string getFilename( void ) const;
            std::size_t getFilesize( void ) const;
            void remove( const std::string& ) const;
            bool pathexists( const std::string& ) const;
            
            
            template<typename T> ublas::matrix<T> readBlasMatrix( const std::string&, const H5::PredType& ) const;
            template<typename T> ublas::vector<T> readBlasVector( const std::string&, const H5::PredType& ) const;
            template<typename T> T readValue( const std::string&, const H5::PredType& ) const;
            
            std::string readString( const std::string& ) const;
            std::vector<std::string> readStringVector( const std::string& ) const;
            
            
            template<typename T> void writeBlasMatrix( const std::string&, const ublas::matrix<T>&, const H5::PredType& ) const;
            template<typename T> void writeBlasVector( const std::string&, const ublas::vector<T>&, const H5::PredType& ) const;
            template<typename T> void writeValue( const std::string&, const T&, const H5::PredType& ) const;
            
            void writeString( const std::string&, const std::string&, const bool& = true ) const;
            void writeStringVector( const std::string&, const std::vector<std::string>&, const bool& = true ) const;
        
        
        private :
        
            /** file handler **/
            H5::H5File m_file;
            
            std::string createPath( const std::string&, std::vector<H5::Group>& ) const;
            void createDataSpace( const std::string&, const H5::PredType&, const ublas::vector<std::size_t>&, H5::DataSpace&, H5::DataSet&, std::vector<H5::Group>& ) const;
            void createStringSpace( const std::string&, const ublas::vector<std::size_t>&, const std::size_t&, H5::DataSpace&, H5::DataSet&, H5::StrType&, std::vector<H5::Group>& ) const;
            void closeSpace( std::vector<H5::Group>&, H5::DataSet&, H5::DataSpace& ) const;
        
    };
    
    
    
    
    /** constructor
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
    
    
    /** open a file
     * @param p_file filename
     * @param p_write bool for clear/create file
     **/
    inline void hdf::open( const std::string& p_file, const bool& p_write )
    {
        flush();
        m_file.close();
        
        m_file = H5::H5File( p_file.c_str(), (p_write ? H5F_ACC_TRUNC : H5F_ACC_RDWR) );
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
    
    
    /** returns the bool if path exists
     * @note we use the C function, because this is not portet to C++
     * @param p_path path
     * @return boolean
     **/
    inline bool hdf::pathexists( const std::string& p_path ) const
    {
        return H5Lexists( m_file.getId(), p_path.c_str(), H5P_DEFAULT );
    }
    
    
    /** reads a matrix with convert to blas matrix
     * @param p_path dataset name
     * @param p_datatype datatype for reading data (see http://www.hdfgroup.org/HDF5/doc/cpplus_RM/classH5_1_1PredType.html )
     * @return ublas matrix
     **/ 
    template<typename T> inline ublas::matrix<T> hdf::readBlasMatrix( const std::string& p_path, const H5::PredType& p_datatype ) const
    {
        H5::DataSet   l_dataset   = m_file.openDataSet( p_path.c_str() );
        H5::DataSpace l_dataspace = l_dataset.getSpace();
        
        // check datasetdimension
        if (l_dataspace.getSimpleExtentNdims() != 2)
            throw exception::runtime(_("dataset must be two-dimensional"), *this);
        if (!l_dataspace.isSimple())
            throw exception::runtime(_("dataset must be a simple datatype"), *this);
        
        // read matrix size and create matrix
        // (first element is column size, second row size)
        hsize_t l_size[2];
        l_dataspace.getSimpleExtentDims( l_size );
        
        if ((l_size[1]==0) || (l_size[0]==0))
            throw exception::runtime(_("dimension need not be zero"), *this);
        
        // read data (read column oriantated, because data order is changed)
        ublas::matrix<T, ublas::column_major> l_mat(l_size[1],l_size[0]);
        l_dataset.read( &(l_mat.data()[0]), p_datatype );
        
        l_dataspace.close();
        l_dataset.close();
        return l_mat;
    }
    
    
    
    /** reads a vector with convert to blas vector
     * @param p_path dataset path & name
     * @param p_datatype datatype for reading data
     * @return ublas matrix
     **/ 
    template<typename T> inline ublas::vector<T> hdf::readBlasVector( const std::string& p_path, const H5::PredType& p_datatype ) const
    {
        H5::DataSet   l_dataset   = m_file.openDataSet( p_path.c_str() );
        H5::DataSpace l_dataspace = l_dataset.getSpace();
        
        // check datasetdimension
        if (l_dataspace.getSimpleExtentNdims() != 1)
            throw exception::runtime(_("dataset must be one-dimensional"), *this);
        if (!l_dataspace.isSimple())
            throw exception::runtime(_("dataset must be a simple datatype"), *this);
        
        // read vector size and create vector
        hsize_t l_size[1];
        l_dataspace.getSimpleExtentDims( l_size );
        
        if (l_size[0]==0)
            throw exception::runtime(_("dimension need not be zero"), *this);
        
        // create temp structur for reading data
        ublas::vector<T> l_vec(l_size[0]);
        l_dataset.read( &(l_vec.data()[0]), p_datatype );
        
        l_dataspace.close();
        l_dataset.close();
        return l_vec;
    }
    
    
    /** reads a single value
     * @param p_path dataset path & name
     * @param p_datatype datatype for reading data
     * @return single value
     **/ 
    template<typename T> inline T hdf::readValue( const std::string& p_path, const H5::PredType& p_datatype ) const
    {
        H5::DataSet   l_dataset   = m_file.openDataSet( p_path.c_str() );
        H5::DataSpace l_dataspace = l_dataset.getSpace();
        
        // check datasetdimension
        if (l_dataspace.getSimpleExtentNdims() != 1)
            throw exception::runtime(_("dataset must be one-dimensional"), *this);
        if (!l_dataspace.isSimple())
            throw exception::runtime(_("dataset must be a simple datatype"), *this);
        
        // read dataset size
        hsize_t l_size[1];
        l_dataspace.getSimpleExtentDims( l_size );
        if (l_size[0] != 1)
            throw exception::runtime(_("element is not a single value"), *this);
        
        // create temp structur for reading data
        T l_value;
        l_dataset.read( &l_value, p_datatype );
        
        l_dataspace.close();
        l_dataset.close();
        return l_value;
    }
    
    
    /** reads a string array from HDF file into a std::vector
     * @todo switch array to a vector, because it creates pedantic errors
     * @param p_path path to element
     * @return std::vector with std::string elements
     **/
    inline std::vector<std::string> hdf::readStringVector( const std::string& p_path ) const
    {
        H5::DataSet   l_dataset   = m_file.openDataSet( p_path.c_str() );
        H5::DataSpace l_dataspace = l_dataset.getSpace();
        
        // check datasetdimension
        if (l_dataspace.getSimpleExtentNdims() != 1)
            throw exception::runtime(_("dataset must be one-dimensional"), *this);
        if (!l_dataspace.isSimple())
            throw exception::runtime(_("dataset must be a simple datatype"), *this);
          
        // the storage size is the whole string array data
        char l_data[l_dataset.getStorageSize()];
        
        // create a string type of dataset for getting string length of each vector element
        H5::StrType l_str(l_dataset);
        l_dataset.read(l_data, l_str);
        
        // check correct size for avoid wrong memcopy
        if (l_dataset.getStorageSize() % l_str.getSize() != 0)
            throw exception::runtime(_("block size can not seperated by the string length"), *this);
        
        // each element has max l_str.getSize() chars (with \0), so the array will be cut on
        // each l_str.getSize()-th element ( l_dataset.getStorageSize() = l_str.getSize * number of elements)
        std::vector<std::string> l_vec;
        for(std::size_t i=0; i < l_dataset.getStorageSize(); i += l_str.getSize()) {
            char l_cut[l_str.getSize()];
            memcpy( l_cut, &l_data[i], l_str.getSize() * sizeof(char) );
            
            l_vec.push_back( l_cut );
        }

        l_dataspace.close();
        l_dataset.close();
        
        return l_vec;
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
            throw exception::runtime(_("dataset must be one-dimensional"), *this);
        if (!l_dataspace.isSimple())
            throw exception::runtime(_("dataset must be a simple datatype"), *this);       
        
        // read size of chars and create dataspace
        char l_chars[l_dataset.getStorageSize()];
        l_dataset.read(l_chars, H5::StrType(l_dataset) );
        
        l_dataspace.close();
        l_dataset.close();
        
        return std::string(l_chars);
    }
    
    
    
    /** writes a simple string to hdf
     * @param p_path path to dataset
     * @param p_value string value
     * @param p_utf8 write the string value with UTF-8 encoding
     **/
    inline void hdf::writeString( const std::string& p_path, const std::string& p_value, const bool& p_utf8 ) const
    {
        if (p_value.empty())
            throw exception::runtime(_("can not write empty data"), *this);
        
        H5::DataSpace l_dataspace;
        H5::DataSet l_dataset;
        H5::StrType l_str;
        std::vector<H5::Group> l_groups;
        
        createStringSpace(p_path, ublas::vector<std::size_t>(1,1), p_value.size(), l_dataspace, l_dataset, l_str, l_groups);

        if (p_utf8)
            l_str.setCset( H5T_CSET_UTF8 );
        
        // write string
        l_dataset.write( p_value.c_str(), l_str, l_dataspace  );
        
        closeSpace(l_groups, l_dataset, l_dataspace);
    }
    

    /** writes a string vector to hdf
     * @param p_path path to dataset
     * @param p_value string vector
     * @param p_utf8 write the string value with UTF-8 encoding
     **/
    inline void hdf::writeStringVector( const std::string& p_path, const std::vector<std::string>& p_value, const bool& p_utf8 ) const
    {
        if (p_value.size() == 0)
            throw exception::runtime(_("can not write empty data"), *this);
            
        H5::DataSpace l_dataspace;
        H5::DataSet l_dataset;
        H5::StrType l_str;
        std::vector<H5::Group> l_groups;
        
        // at first we need the max length of the string for creating data string object
        std::size_t l_maxstrlen = 0;
        for(std::size_t i=0; i < p_value.size(); ++i)
            l_maxstrlen  = std::max( l_maxstrlen, p_value[i].size() );
        
        if (l_maxstrlen == 0)
            throw exception::runtime(_("can not write empty data"), *this);
        
        // create char array for the string data, each vector element ist seperated with \0
        char l_data[(l_maxstrlen+1)*p_value.size()];
        std::size_t l_start = 0;
        for(std::size_t i=0; i < p_value.size(); ++i) {
            memcpy( &l_data[l_start], p_value[i].c_str(), (p_value[i].size()+1) * sizeof(char) );
            l_start += (l_maxstrlen+1) * sizeof(char);
        }
       
        // create string vector data and write it
        createStringSpace(p_path, ublas::vector<std::size_t>(1, p_value.size()), l_maxstrlen, l_dataspace, l_dataset, l_str, l_groups);
        
        if (p_utf8)
            l_str.setCset( H5T_CSET_UTF8 );
        
        l_dataset.write( l_data, l_str, l_dataspace  );
        closeSpace(l_groups, l_dataset, l_dataspace);
    }
    
    
    /** write a blas matrix to hdf file
     * @param p_path dataset path & name
     * @param p_dataset matrixdata
     * @param p_datatype datatype for writing data (see http://www.hdfgroup.org/HDF5/doc/cpplus_RM/classH5_1_1PredType.html )
     * @todo remove ublas::transpose
     **/
    template<typename T> inline void hdf::writeBlasMatrix( const std::string& p_path, const ublas::matrix<T>& p_dataset, const H5::PredType& p_datatype ) const
    {        
        if ((p_dataset.size1() == 0) || (p_dataset.size2() == 0))
            throw exception::runtime(_("can not write empty data"), *this);
        
        H5::DataSet l_dataset;
        H5::DataSpace l_dataspace;
        std::vector<H5::Group> l_groups;
        
        ublas::vector<std::size_t> l_dim(2);
        l_dim(0) = p_dataset.size2();
        l_dim(1) = p_dataset.size1();
        
        createDataSpace(p_path,  p_datatype, l_dim, l_dataspace, l_dataset, l_groups);
        ublas::matrix<T> l_matrix = ublas::trans(p_dataset);
        l_dataset.write( &(l_matrix.data()[0]), p_datatype, l_dataspace  );        
        closeSpace(l_groups, l_dataset, l_dataspace);
    }
    
    
    /** write a blas vector to hdf file
     * @param p_path dataset path & name
     * @param p_dataset vectordata
     * @param p_datatype datatype for writing data (see http://www.hdfgroup.org/HDF5/doc/cpplus_RM/classH5_1_1PredType.html )
     **/
    template<typename T> inline void hdf::writeBlasVector( const std::string& p_path, const ublas::vector<T>& p_dataset, const H5::PredType& p_datatype ) const
    {     
        if (p_dataset.size() == 0)
            throw exception::runtime(_("can not write empty data"), *this);
        
        H5::DataSet l_dataset;
        H5::DataSpace l_dataspace;
        std::vector<H5::Group> l_groups;
        
        createDataSpace(p_path,  p_datatype, ublas::vector<std::size_t>(1,p_dataset.size()), l_dataspace, l_dataset, l_groups);
        l_dataset.write( &(p_dataset.data()[0]), p_datatype, l_dataspace  );
        closeSpace(l_groups, l_dataset, l_dataspace);
    }
    
    
    /** write a value to hdf file
     * @param p_path dataset path & name
     * @param p_dataset value
     * @param p_datatype datatype for writing data (see http://www.hdfgroup.org/HDF5/doc/cpplus_RM/classH5_1_1PredType.html )
     **/
    template<typename T> inline void hdf::writeValue( const std::string& p_path, const T& p_dataset, const H5::PredType& p_datatype ) const
    {        
        H5::DataSet l_dataset;
        H5::DataSpace l_dataspace;
        std::vector<H5::Group> l_groups;
        
        createDataSpace(p_path,  p_datatype, ublas::vector<std::size_t>(1,1), l_dataspace, l_dataset, l_groups);
        l_dataset.write( &p_dataset, p_datatype, l_dataspace  );
        closeSpace(l_groups, l_dataset, l_dataspace);
    }
    
    
    /** close the dataspace and the groups in the right order
     * @param p_groups vector with group information
     * @param p_dataset dataset
     * @param p_dataspace dataspace
     **/
    inline void hdf::closeSpace( std::vector<H5::Group>& p_groups, H5::DataSet& p_dataset, H5::DataSpace& p_dataspace ) const
    {
        p_dataset.close();
        p_dataspace.close();
        
        // close groups
        for(std::vector<H5::Group>::reverse_iterator it = p_groups.rbegin(); it != p_groups.rend(); ++it)
            (*it).close();
        
        p_groups.clear();
    }
    
    
    /** create a dataspace in the file content
     * @param p_path path for dataspace
     * @param p_datatype datatype for writing data (see http://www.hdfgroup.org/HDF5/doc/cpplus_RM/classH5_1_1PredType.html )
     * @param p_dim dimension for dataspace and every dimension
     * @param p_dataspace refernce of the dataspace
     * @param p_dataset refernce for the dataset
     * @param p_groups groups for closing
     **/
    inline void hdf::createDataSpace( const std::string& p_path, const H5::PredType& p_datatype, const ublas::vector<std::size_t>& p_dim, H5::DataSpace& p_dataspace, H5::DataSet& p_dataset, std::vector<H5::Group>& p_groups ) const
    {
        if (p_dim.size() == 0)
            throw exception::runtime(_("it is at least one dimension requires"), *this);
        
        hsize_t l_size[p_dim.size()];
        for(std::size_t i=0; i < p_dim.size(); ++i)
            l_size[i] = p_dim(i);
        
        p_dataspace              = H5::DataSpace( p_dim.size(), l_size );
        p_groups                 = std::vector<H5::Group>();
        const std::string l_path = createPath( p_path, p_groups );
        
        if (l_path.empty())
            throw exception::runtime(_("empty path is forbidden"), *this);
        
        if (p_groups.size() == 0)
            p_dataset = m_file.createDataSet( l_path.c_str(), p_datatype, p_dataspace );
        else
            p_dataset = p_groups[p_groups.size()-1].createDataSet( l_path.c_str(), p_datatype, p_dataspace );
    }
    
    
    /** create a dataspace in the file content for string types
     * @param p_path path for dataspace
     * @param p_dim dimension for dataspace and every dimension
     * @param p_strlen max. string length
     * @param p_dataspace refernce of the dataspace
     * @param p_dataset refernce for the dataset
     * @param p_str string type (@see http://www.hdfgroup.org/HDF5/doc/cpplus_RM/classH5_1_1DataType.html)
     * @param p_groups groups for closing
     **/
    inline void hdf::createStringSpace( const std::string& p_path, const ublas::vector<std::size_t>& p_dim, const std::size_t& p_strlen, H5::DataSpace& p_dataspace, H5::DataSet& p_dataset, H5::StrType& p_str, std::vector<H5::Group>& p_groups ) const
    {
        if (p_dim.size() == 0)
            throw exception::runtime(_("it is at least one dimension requires"), *this);
        
        hsize_t l_size[p_dim.size()];
        for(std::size_t i=0; i < p_dim.size(); ++i)
            l_size[i] = p_dim(i);
        
        p_dataspace              = H5::DataSpace( p_dim.size(), l_size );
        p_groups                 = std::vector<H5::Group>();
        const std::string l_path = createPath( p_path, p_groups );
        
        if (l_path.empty())
            throw exception::runtime(_("empty path is forbidden"), *this);
        
        p_str     = H5::StrType(0, p_strlen+1);
        if (p_groups.size() == 0)
            p_dataset = m_file.createDataSet( l_path.c_str(), p_str, p_dataspace );
        else
            p_dataset = p_groups[p_groups.size()-1].createDataSet( l_path.c_str(), p_str, p_dataspace );
    }
    
    
    /** create a path with dataset (path separator is "/")
     * @param p_path full path (last element dataset name)
     * @param p_groups std::vector with opened/created groups for closing
     * @return string with last path entry
     **/
    inline std::string hdf::createPath( const std::string& p_path, std::vector<H5::Group>& p_groups ) const
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
            throw exception::runtime(_("empty path is forbidden"), *this);
        
        // if only one element then create dataset directly
        if (l_path.size() == 1)
            return l_path[0];
        else {
            
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
            return *l_path.end();
        }
        
        throw exception::runtime(_("can not create path structure"), *this);
    }
    
    
}}}
#endif
#endif
