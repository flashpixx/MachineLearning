/** 
 @cond
 ############################################################################
 # LGPL License                                                             #
 #                                                                          #
 # This file is part of the Machine Learning Framework.                     #
 # Copyright (c) 2010-2012, Philipp Kraus, <philipp.kraus@flashpixx.de>     #
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

extern "C" {
#include <hdf5_hl.h>
}


#include "../language/language.h"
#include "../../errorhandling/exception.hpp"



namespace machinelearning { namespace tools { namespace files {
    
    #ifndef SWIG
    namespace ublas  = boost::numeric::ublas;
    #endif
    
    
    /** class for reading and writing the HDF data
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
        
            /** datatype for read / write options (encodes the static constants in the HDF datatyp) **/
            enum datatype
            {
                STD_I8BE, 
                STD_I8LE,
                STD_I16BE,
                STD_I16LE,
                STD_I32BE,
                STD_I32LE,
                STD_I64BE,
                STD_I64LE,
                STD_U8BE,
                STD_U8LE,
                STD_U16BE,
                STD_U16LE,
                STD_U32BE,
                STD_U32LE,
                STD_U64BE,
                STD_U64LE,
                STD_B8BE,
                STD_B8LE,
                STD_B16BE,
                STD_B16LE,
                STD_B32BE,
                STD_B32LE,
                STD_B64BE,
                STD_B64LE,
                STD_REF_OBJ,
                STD_REF_DSETREG,
                
                C_S1,
                FORTRAN_S1,
                
                IEEE_F32BE,
                IEEE_F32LE,
                IEEE_F64BE,
                IEEE_F64LE,
                
                UNIX_D32BE,
                UNIX_D32LE,
                UNIX_D64BE,
                UNIX_D64LE,
                
                INTEL_I8,
                INTEL_I16,
                INTEL_I32,
                INTEL_I64,
                INTEL_U8,
                INTEL_U16,
                INTEL_U32,
                INTEL_U64,
                INTEL_B8,
                INTEL_B16,
                INTEL_B32,
                INTEL_B64,
                INTEL_F32,
                INTEL_F64,
                
                ALPHA_I8,
                ALPHA_I16,
                ALPHA_I32,
                ALPHA_I64,
                ALPHA_U8,
                ALPHA_U16,
                ALPHA_U32,
                ALPHA_U64,
                ALPHA_B8,
                ALPHA_B16,
                ALPHA_B32,
                ALPHA_B64,
                ALPHA_F32,
                ALPHA_F64,
                
                MIPS_I8,
                MIPS_I16,
                MIPS_I32,
                MIPS_I64,
                MIPS_U8,
                MIPS_U16,
                MIPS_U32,
                MIPS_U64,
                MIPS_B8,
                MIPS_B16,
                MIPS_B32,
                MIPS_B64,
                MIPS_F32,
                MIPS_F64,
                
                NATIVE_CHAR,
                NATIVE_SCHAR,
                NATIVE_UCHAR,
                NATIVE_SHORT,
                NATIVE_USHORT,
                NATIVE_INT,
                NATIVE_UINT,
                NATIVE_LONG,
                NATIVE_ULONG,
                NATIVE_LLONG,
                NATIVE_ULLONG,
                NATIVE_FLOAT,
                NATIVE_DOUBLE,
                NATIVE_LDOUBLE,
                NATIVE_B8,
                NATIVE_B16,
                NATIVE_B32,
                NATIVE_B64,
                NATIVE_OPAQUE,
                NATIVE_HSIZE,
                NATIVE_HSSIZE,
                NATIVE_HERR,
                NATIVE_HBOOL,
                
                NATIVE_INT8,
                NATIVE_UINT8,
                NATIVE_INT16,
                NATIVE_UINT16,
                NATIVE_INT32,
                NATIVE_UINT32,
                NATIVE_INT64,
                NATIVE_UINT64
            };
            
             
        
            hdf( const std::string& );
            hdf( const std::string&, const bool& );
            ~hdf( void );
            
            void open( const std::string&, const bool& = false );
            void flush( void ) const;
            std::string getFilename( void ) const;
            std::size_t getFilesize( void ) const;
            void remove( const std::string& ) const;
            bool pathexists( const std::string&, const bool& = true ) const;
            
            
            template<typename T> ublas::matrix<T> readBlasMatrix( const std::string&, const datatype& ) const;
            template<typename T> ublas::vector<T> readBlasVector( const std::string&, const datatype& ) const;
            template<typename T> std::vector<T> readStdVector( const std::string&, const datatype& ) const;
            template<typename T> T readValue( const std::string&, const datatype& ) const;
            
            std::string readString( const std::string& ) const;
            std::vector<std::string> readStringVector( const std::string& ) const;
            
            
            template<typename T> void writeBlasMatrix( const std::string&, const ublas::matrix<T>&, const datatype& ) const;
            template<typename T> void writeBlasVector( const std::string&, const ublas::vector<T>&, const datatype& ) const;
            template<typename T> void writeStdVector( const std::string&, const std::vector<T>&, const datatype& ) const;
            template<typename T> void writeValue( const std::string&, const T&, const datatype& ) const;
            
            void writeString( const std::string&, const std::string& ) const;
            void writeStringVector( const std::string&, const std::vector<std::string>& ) const;
        
        
        private :
        
            /** file handler **/
            H5::H5File m_file;
            
            bool isAbsolutePath( const std::string& p_path ) const;
            std::string createPath( const std::string&, std::vector<H5::Group>& ) const;
            void createDataSpace( const std::string&, const H5::PredType&, const ublas::vector<std::size_t>&, H5::DataSpace&, H5::DataSet&, std::vector<H5::Group>& ) const;
            void createStringSpace( const std::string&, const ublas::vector<std::size_t>&, const std::size_t&, H5::DataSpace&, H5::DataSet&, H5::StrType&, std::vector<H5::Group>& ) const;
            void closeSpace( std::vector<H5::Group>&, H5::DataSet&, H5::DataSpace& ) const;
            H5::PredType getHDFType( const datatype& ) const;
        
        
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
        if (!isAbsolutePath(p_path))
            throw exception::runtime(_("path is not an absolute path"));
        
        m_file.unlink( p_path );
    }
    
    
    /** returns the bool if path exists
     * @note we use the C function, because this is not portet to C++
     * @see http://www.hdfgroup.org/HDF5/doc/HL/RM_H5LT.html#H5LTpath_valid
     * @param p_path path
     * @param p_checkend checks if the last component exists (default true)
     * @return boolean
     **/
    inline bool hdf::pathexists( const std::string& p_path, const bool& p_checkend ) const
    {
        if (!isAbsolutePath(p_path))
            throw exception::runtime(_("path is not an absolute path"));
        
        return H5LTpath_valid(m_file.getId(), p_path.c_str(), p_checkend);
    }
    
    
    /** reads a matrix with convert to blas matrix
     * @param p_path dataset name
     * @param p_datatype datatype for reading data
     * @return ublas matrix
     **/ 
    template<typename T> inline ublas::matrix<T> hdf::readBlasMatrix( const std::string& p_path, const datatype& p_datatype ) const
    {
        if (!isAbsolutePath(p_path))
            throw exception::runtime(_("path is not an absolute path"));
        
        H5::DataSet   l_dataset   = m_file.openDataSet( p_path.c_str() );
        H5::DataSpace l_dataspace = l_dataset.getSpace();
        
        // check datasetdimension
        if (l_dataspace.getSimpleExtentNdims() != 2)
            throw exception::runtime(_("dataset must be two-dimensional"));
        if (!l_dataspace.isSimple())
            throw exception::runtime(_("dataset must be a simple datatype"));
        
        // read matrix size and create matrix
        // (first element is column size, second row size)
        hsize_t l_size[2];
        l_dataspace.getSimpleExtentDims( l_size );
        
        if ((!l_size[1]) || (!l_size[0]))
            throw exception::runtime(_("dimension need not be zero"));
        
        // read data (read column oriantated, because data order is changed)
        ublas::matrix<T, ublas::column_major> l_mat(l_size[1],l_size[0]);
        l_dataset.read( &(l_mat.data()[0]), getHDFType(p_datatype) );
        
        l_dataspace.close();
        l_dataset.close();
        return l_mat;
    }
    
    
    
    /** reads a vector with convert to blas vector
     * @param p_path dataset path & name
     * @param p_datatype datatype for reading data
     * @return ublas vector
     **/ 
    template<typename T> inline ublas::vector<T> hdf::readBlasVector( const std::string& p_path, const datatype& p_datatype ) const
    {
        if (!isAbsolutePath(p_path))
            throw exception::runtime(_("path is not an absolute path"));
        
        H5::DataSet   l_dataset   = m_file.openDataSet( p_path.c_str() );
        H5::DataSpace l_dataspace = l_dataset.getSpace();
        
        // check datasetdimension
        if (l_dataspace.getSimpleExtentNdims() != 1)
            throw exception::runtime(_("dataset must be one-dimensional"));
        if (!l_dataspace.isSimple())
            throw exception::runtime(_("dataset must be a simple datatype"));
        
        // read vector size and create vector
        hsize_t l_size[1];
        l_dataspace.getSimpleExtentDims( l_size );
        
        if (!l_size[0])
            throw exception::runtime(_("dimension need not be zero"));
        
        // create temp structur for reading data
        ublas::vector<T> l_vec(l_size[0]);
        l_dataset.read( &(l_vec.data()[0]), getHDFType(p_datatype) );
        
        l_dataspace.close();
        l_dataset.close();
        return l_vec;
    }
    
    
    /** reads a vector wto a std::vector
     * @param p_path dataset path & name
     * @param p_datatype datatype for reading data
     * @return std::vector
     **/ 
    template<typename T> inline std::vector<T> hdf::readStdVector( const std::string& p_path, const datatype& p_datatype ) const
    {
        if (!isAbsolutePath(p_path))
            throw exception::runtime(_("path is not an absolute path"));
        
        H5::DataSet   l_dataset   = m_file.openDataSet( p_path.c_str() );
        H5::DataSpace l_dataspace = l_dataset.getSpace();
        
        // check datasetdimension
        if (l_dataspace.getSimpleExtentNdims() != 1)
            throw exception::runtime(_("dataset must be one-dimensional"));
        if (!l_dataspace.isSimple())
            throw exception::runtime(_("dataset must be a simple datatype"));
        
        // read vector size and create vector
        hsize_t l_size[1];
        l_dataspace.getSimpleExtentDims( l_size );
        
        if (!l_size[0])
            throw exception::runtime(_("dimension need not be zero"));
        
        // create temp structur for reading data
        std::vector<T> l_vec(l_size[0]);
        l_dataset.read( &(l_vec[0]), getHDFType(p_datatype) );
        
        l_dataspace.close();
        l_dataset.close();
        return l_vec;
    }
    
    
    /** reads a single value
     * @param p_path dataset path & name
     * @param p_datatype datatype for reading data
     * @return single value
     **/ 
    template<typename T> inline T hdf::readValue( const std::string& p_path, const datatype& p_datatype ) const
    {
        if (!isAbsolutePath(p_path))
            throw exception::runtime(_("path is not an absolute path"));
        
        H5::DataSet   l_dataset   = m_file.openDataSet( p_path.c_str() );
        H5::DataSpace l_dataspace = l_dataset.getSpace();
        
        // check datasetdimension
        if (l_dataspace.getSimpleExtentNdims() != 1)
            throw exception::runtime(_("dataset must be one-dimensional"));
        if (!l_dataspace.isSimple())
            throw exception::runtime(_("dataset must be a simple datatype"));
        
        // read dataset size
        hsize_t l_size[1];
        l_dataspace.getSimpleExtentDims( l_size );
        if (l_size[0] != 1)
            throw exception::runtime(_("element is not a single value"));
        
        // create temp structur for reading data
        T l_value;
        l_dataset.read( &l_value, getHDFType(p_datatype) );
        
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
        if (!isAbsolutePath(p_path))
            throw exception::runtime(_("path is not an absolute path"));
        
        H5::DataSet   l_dataset   = m_file.openDataSet( p_path.c_str() );
        H5::DataSpace l_dataspace = l_dataset.getSpace();
        
        // check datasetdimension
        if (l_dataspace.getSimpleExtentNdims() != 1)
            throw exception::runtime(_("dataset must be one-dimensional"));
        if (!l_dataspace.isSimple())
            throw exception::runtime(_("dataset must be a simple datatype"));
        
        // the storage size is the whole string array data
        char* l_data = new char[l_dataset.getStorageSize()];
        
        // create a string type of dataset for getting string length of each vector element
        H5::StrType l_str(l_dataset);
        l_dataset.read(l_data, l_str);
        
        // check correct size for avoid wrong memcopy
        if (!(l_dataset.getStorageSize() % l_str.getSize()))
        {
            delete[] l_data;
            throw exception::runtime(_("block size can not seperated by the string length"));
        }
        
        // each element has max l_str.getSize() chars (with \0), so the array will be cut on
        // each l_str.getSize()-th element ( l_dataset.getStorageSize() = l_str.getSize * number of elements)
        std::vector<std::string> l_vec;
        char* l_cut = new char[l_str.getSize()];
        for(std::size_t i=0; i < l_dataset.getStorageSize(); i += l_str.getSize()) {
            memcpy( l_cut, &l_data[i], l_str.getSize() * sizeof(char) );
            l_vec.push_back( std::string(l_cut) );
        }
        delete[] l_cut;
        
        l_dataspace.close();
        l_dataset.close();
        delete[] l_data;
        
        return l_vec;
    }
    
    
    /** read a simple string from file
     * @param p_path path to dataset
     * @return string with data
     **/
    inline std::string hdf::readString( const std::string& p_path ) const
    {
        if (!isAbsolutePath(p_path))
            throw exception::runtime(_("path is not an absolute path"));
        
        H5::DataSet   l_dataset   = m_file.openDataSet( p_path.c_str() );
        H5::DataSpace l_dataspace = l_dataset.getSpace();
        
        // check datasetdimension
        if (l_dataspace.getSimpleExtentNdims() != 1)
            throw exception::runtime(_("dataset must be one-dimensional"));
        if (!l_dataspace.isSimple())
            throw exception::runtime(_("dataset must be a simple datatype"));       
        
        // read size of chars and create dataspace
        char* l_chars = new char[l_dataset.getStorageSize()];
        
        l_dataset.read(l_chars, H5::StrType(l_dataset) );
        const std::string l_ret(l_chars);
        
        delete[] l_chars;
        l_dataspace.close();
        l_dataset.close();
        
        return l_ret;
    }
    
    
    
    /** writes a simple string to hdf
     * @param p_path path to dataset
     * @param p_value string value
     **/
    inline void hdf::writeString( const std::string& p_path, const std::string& p_value ) const
    {
        if (p_value.empty())
            throw exception::runtime(_("can not write empty data"));
        
        if (!isAbsolutePath(p_path))
            throw exception::runtime(_("path is not an absolute path"));
        
        H5::DataSpace l_dataspace;
        H5::DataSet l_dataset;
        H5::StrType l_str;
        std::vector<H5::Group> l_groups;
        
        // create string space and write the data
        createStringSpace(p_path, ublas::vector<std::size_t>(1,1), p_value.size(), l_dataspace, l_dataset, l_str, l_groups);
        l_dataset.write( p_value.c_str(), l_str, l_dataspace  );
        
        closeSpace(l_groups, l_dataset, l_dataspace);
    }
    
    
    /** writes a string vector to hdf
     * @param p_path path to dataset
     * @param p_value string vector
     **/
    inline void hdf::writeStringVector( const std::string& p_path, const std::vector<std::string>& p_value ) const
    {
        if (!p_value.size())
            throw exception::runtime(_("can not write empty data"));
        
        if (!isAbsolutePath(p_path))
            throw exception::runtime(_("path is not an absolute path"));
        
        H5::DataSpace l_dataspace;
        H5::DataSet l_dataset;
        H5::StrType l_str;
        std::vector<H5::Group> l_groups;
        
        // at first we need the max length of the string for creating data string object
        std::size_t l_maxstrlen = 0;
        for(std::size_t i=0; i < p_value.size(); ++i)
            l_maxstrlen  = std::max( l_maxstrlen, p_value[i].size() );
        
        if (!l_maxstrlen)
            throw exception::runtime(_("can not write empty data"));
        
		// at second, we create the block size for the full vector
		const std::size_t l_blocksize = (l_maxstrlen+1)*p_value.size();
        
        
        // create char array for the string data, each vector element ist seperated with \0, so we fill the block with 0
        char* l_data = new char[l_blocksize];
		memset(l_data, 0, l_blocksize );
        
        std::size_t l_offset = 0;
        for(std::size_t i=0; i < p_value.size(); ++i) {
            memcpy( l_data+l_offset, p_value[i].c_str(), p_value[i].size() * sizeof(char) );
            l_offset += (l_maxstrlen+1) * sizeof(char);
        }
        
        // create string vector data and write it
        try {
            createStringSpace(p_path, ublas::vector<std::size_t>(1, p_value.size()), l_maxstrlen, l_dataspace, l_dataset, l_str, l_groups);
            l_dataset.write( l_data, l_str, l_dataspace  );
        } catch (...) {
            delete[] l_data;
            throw;
        }
        
		delete[] l_data;
        closeSpace(l_groups, l_dataset, l_dataspace);
    }
    
    
    /** write a blas matrix to hdf file
     * @param p_path dataset path & name
     * @param p_dataset matrixdata
     * @param p_datatype datatype for writing data 
     * @todo remove ublas::transpose
     **/
    template<typename T> inline void hdf::writeBlasMatrix( const std::string& p_path, const ublas::matrix<T>& p_dataset, const datatype& p_datatype ) const
    {        
        if ((!p_dataset.size1()) || (!p_dataset.size2()))
            throw exception::runtime(_("can not write empty data"));
        
        if (!isAbsolutePath(p_path))
            throw exception::runtime(_("path is not an absolute path"));
        
        H5::DataSet l_dataset;
        H5::DataSpace l_dataspace;
        std::vector<H5::Group> l_groups;
        
        ublas::vector<std::size_t> l_dim(2);
        l_dim(0) = p_dataset.size2();
        l_dim(1) = p_dataset.size1();
        
        createDataSpace(p_path,  getHDFType(p_datatype), l_dim, l_dataspace, l_dataset, l_groups);
        ublas::matrix<T> l_matrix = ublas::trans(p_dataset);
        l_dataset.write( &(l_matrix.data()[0]), getHDFType(p_datatype), l_dataspace  );        
        closeSpace(l_groups, l_dataset, l_dataspace);
    }
    
    
    /** write a blas vector to hdf file
     * @param p_path dataset path & name
     * @param p_dataset vectordata
     * @param p_datatype datatype for writing data
     **/
    template<typename T> inline void hdf::writeBlasVector( const std::string& p_path, const ublas::vector<T>& p_dataset, const datatype& p_datatype ) const
    {     
        if (!p_dataset.size())
            throw exception::runtime(_("can not write empty data"));
        
        if (!isAbsolutePath(p_path))
            throw exception::runtime(_("path is not an absolute path"));
        
        H5::DataSet l_dataset;
        H5::DataSpace l_dataspace;
        std::vector<H5::Group> l_groups;
        
        createDataSpace(p_path, getHDFType(p_datatype), ublas::vector<std::size_t>(1,p_dataset.size()), l_dataspace, l_dataset, l_groups);
        l_dataset.write( &p_dataset.data(), getHDFType(p_datatype), l_dataspace  );
        closeSpace(l_groups, l_dataset, l_dataspace);
    }
    
    
    /** writes a std::vector to the hdf file
     * @param p_path dataset path & name
     * @param p_dataset vectordata
     * @param p_datatype datatype for writing data
     **/
    template<typename T> void hdf::writeStdVector( const std::string& p_path, const std::vector<T>& p_dataset, const datatype& p_datatype ) const
    {
        if (!p_dataset.size())
            throw exception::runtime(_("can not write empty data"));
        
        if (!isAbsolutePath(p_path))
            throw exception::runtime(_("path is not an absolute path"));
        
        H5::DataSet l_dataset;
        H5::DataSpace l_dataspace;
        std::vector<H5::Group> l_groups;
        
        createDataSpace(p_path, getHDFType(p_datatype), ublas::vector<std::size_t>(1,p_dataset.size()), l_dataspace, l_dataset, l_groups);
        l_dataset.write( &(p_dataset[0]), getHDFType(p_datatype), l_dataspace  );
        closeSpace(l_groups, l_dataset, l_dataspace);
    }
    
    
    /** write a value to hdf file
     * @param p_path dataset path & name
     * @param p_dataset value
     * @param p_datatype datatype for writing data
     **/
    template<typename T> inline void hdf::writeValue( const std::string& p_path, const T& p_dataset, const datatype& p_datatype ) const
    {        
        if (!isAbsolutePath(p_path))
            throw exception::runtime(_("path is not an absolute path"));
        
        H5::DataSet l_dataset;
        H5::DataSpace l_dataspace;
        std::vector<H5::Group> l_groups;
        
        createDataSpace(p_path, getHDFType(p_datatype), ublas::vector<std::size_t>(1,1), l_dataspace, l_dataset, l_groups);
        l_dataset.write( &p_dataset, getHDFType(p_datatype), l_dataspace  );
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
        if (!p_dim.size())
            throw exception::runtime(_("one dimension is required"));
        
        hsize_t* l_size = new hsize_t[p_dim.size()];
        for(std::size_t i=0; i < p_dim.size(); ++i)
            l_size[i] = p_dim(i);
        
        p_dataspace = H5::DataSpace( p_dim.size(), l_size );
        p_groups = std::vector<H5::Group>();
        const std::string l_path = createPath( p_path, p_groups );
        delete[] l_size;
        
        if (l_path.empty())
            throw exception::runtime(_("empty path is forbidden"));
        
        if (!p_groups.size())
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
            throw exception::runtime(_("one dimension is required"));
        
        hsize_t* l_size = new hsize_t[p_dim.size()];
        for(std::size_t i=0; i < p_dim.size(); ++i)
            l_size[i] = p_dim(i);
        
        p_dataspace = H5::DataSpace( p_dim.size(), l_size );
        p_groups = std::vector<H5::Group>();
        const std::string l_path = createPath( p_path, p_groups );
        delete[] l_size;
        
        if (l_path.empty())
            throw exception::runtime(_("empty path is forbidden"));
        
        p_str = H5::StrType(0, p_strlen+1);
        if (!p_groups.size())
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
        if (!l_path.size())
            throw exception::runtime(_("empty path is forbidden"));
        
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
        
        throw exception::runtime(_("can not create path structure"));
    }
    
    
    /** checks if a path is an absolute path
     * @param p_path string with path
     * @return boolean is absolute
     **/
    inline bool hdf::isAbsolutePath( const std::string& p_path ) const
    {
        if (p_path.empty())
            return false;
        
        return p_path[0] == '/';
    }
    
    
    /** maps the HDF datatype
     * @param p_in input datatype
     * @return HDF datatype
     **/
    inline H5::PredType hdf::getHDFType( const datatype& p_in ) const
    {
        switch (p_in)
        {
            case STD_I8BE 				: return H5::PredType::STD_I8BE;
            case STD_I8LE 				: return H5::PredType::STD_I8LE;
            case STD_I16BE 				: return H5::PredType::STD_I16BE;
            case STD_I16LE 				: return H5::PredType::STD_I16LE;
            case STD_I32BE 				: return H5::PredType::STD_I32BE;
            case STD_I32LE 				: return H5::PredType::STD_I32LE;
            case STD_I64BE 				: return H5::PredType::STD_I64BE;
            case STD_I64LE 				: return H5::PredType::STD_I64LE;
            case STD_U8BE 				: return H5::PredType::STD_U8BE;
            case STD_U8LE 				: return H5::PredType::STD_U8LE;
            case STD_U16BE 				: return H5::PredType::STD_U16BE;
            case STD_U16LE 				: return H5::PredType::STD_U16LE;
            case STD_U32BE 				: return H5::PredType::STD_U32BE;
            case STD_U32LE 				: return H5::PredType::STD_U32LE;
            case STD_U64BE 				: return H5::PredType::STD_U64BE;
            case STD_U64LE 				: return H5::PredType::STD_U64LE;
            case STD_B8BE				: return H5::PredType::STD_B8BE;
            case STD_B8LE				: return H5::PredType::STD_B8LE;
            case STD_B16BE 				: return H5::PredType::STD_B16BE;
            case STD_B16LE 				: return H5::PredType::STD_B16LE;
            case STD_B32BE 				: return H5::PredType::STD_B32BE;
            case STD_B32LE 				: return H5::PredType::STD_B32LE;
            case STD_B64BE 				: return H5::PredType::STD_B64BE;
            case STD_B64LE 				: return H5::PredType::STD_B64LE;
            case STD_REF_OBJ 			: return H5::PredType::STD_REF_OBJ;
            case STD_REF_DSETREG 		: return H5::PredType::STD_REF_DSETREG;
                
            case C_S1 					: return H5::PredType::C_S1;
            case FORTRAN_S1 			: return H5::PredType::FORTRAN_S1;
                
            case IEEE_F32BE 			: return H5::PredType::IEEE_F32BE;
            case IEEE_F32LE 			: return H5::PredType::IEEE_F32LE;
            case IEEE_F64BE 			: return H5::PredType::IEEE_F64BE;
            case IEEE_F64LE 			: return H5::PredType::IEEE_F64LE;
                
            case UNIX_D32BE 			: return H5::PredType::UNIX_D32BE;
            case UNIX_D32LE 			: return H5::PredType::UNIX_D32LE;
            case UNIX_D64BE 			: return H5::PredType::UNIX_D64BE;
            case UNIX_D64LE 			: return H5::PredType::UNIX_D64LE;
                
            case INTEL_I8 				: return H5::PredType::INTEL_I8;
            case INTEL_I16 				: return H5::PredType::INTEL_I16;
            case INTEL_I32 				: return H5::PredType::INTEL_I32;
            case INTEL_I64 				: return H5::PredType::INTEL_I64;
            case INTEL_U8 				: return H5::PredType::INTEL_U8;
            case INTEL_U16 				: return H5::PredType::INTEL_U16;
            case INTEL_U32 				: return H5::PredType::INTEL_U32;
            case INTEL_U64 				: return H5::PredType::INTEL_U64;
            case INTEL_B8 				: return H5::PredType::INTEL_B8;
            case INTEL_B16 				: return H5::PredType::INTEL_B16;
            case INTEL_B32 				: return H5::PredType::INTEL_B32;
            case INTEL_B64 				: return H5::PredType::INTEL_B64;
            case INTEL_F32 				: return H5::PredType::INTEL_F32;
            case INTEL_F64 				: return H5::PredType::INTEL_F64;
                
            case ALPHA_I8 				: return H5::PredType::ALPHA_I8;
            case ALPHA_I16 				: return H5::PredType::ALPHA_I16;
            case ALPHA_I32 				: return H5::PredType::ALPHA_I32;
            case ALPHA_I64 				: return H5::PredType::ALPHA_I64;
            case ALPHA_U8 				: return H5::PredType::ALPHA_U8;
            case ALPHA_U16 				: return H5::PredType::ALPHA_U16;
            case ALPHA_U32 				: return H5::PredType::ALPHA_U32;
            case ALPHA_U64 				: return H5::PredType::ALPHA_U64;
            case ALPHA_B8 				: return H5::PredType::ALPHA_B8;
            case ALPHA_B16 				: return H5::PredType::ALPHA_B16;
            case ALPHA_B32 				: return H5::PredType::ALPHA_B32;
            case ALPHA_B64 				: return H5::PredType::ALPHA_B64;
            case ALPHA_F32 				: return H5::PredType::ALPHA_F32;
            case ALPHA_F64 				: return H5::PredType::ALPHA_F64;
                
            case MIPS_I8 				: return H5::PredType::MIPS_I8;
            case MIPS_I16 				: return H5::PredType::MIPS_I16;
            case MIPS_I32 				: return H5::PredType::MIPS_I32;
            case MIPS_I64 				: return H5::PredType::MIPS_I64;
            case MIPS_U8 				: return H5::PredType::MIPS_U8;
            case MIPS_U16 				: return H5::PredType::MIPS_U16;
            case MIPS_U32 				: return H5::PredType::MIPS_U32;
            case MIPS_U64 				: return H5::PredType::MIPS_U64;
            case MIPS_B8 				: return H5::PredType::MIPS_B8;
            case MIPS_B16 				: return H5::PredType::MIPS_B16;
            case MIPS_B32 				: return H5::PredType::MIPS_B32;
            case MIPS_B64 				: return H5::PredType::MIPS_B64;
            case MIPS_F32 				: return H5::PredType::MIPS_F32;
            case MIPS_F64 				: return H5::PredType::MIPS_F64;
                
            case NATIVE_CHAR 			: return H5::PredType::NATIVE_CHAR;
            case NATIVE_SCHAR           : return H5::PredType::NATIVE_SCHAR;
            case NATIVE_UCHAR 			: return H5::PredType::NATIVE_UCHAR;
            case NATIVE_SHORT 			: return H5::PredType::NATIVE_SHORT;
            case NATIVE_USHORT 			: return H5::PredType::NATIVE_USHORT;
            case NATIVE_INT 			: return H5::PredType::NATIVE_INT;
            case NATIVE_UINT 			: return H5::PredType::NATIVE_UINT;
            case NATIVE_LONG 			: return H5::PredType::NATIVE_LONG;
            case NATIVE_ULONG			: return H5::PredType::NATIVE_ULONG;
            case NATIVE_LLONG			: return H5::PredType::NATIVE_LLONG;
            case NATIVE_ULLONG			: return H5::PredType::NATIVE_ULLONG;
            case NATIVE_FLOAT			: return H5::PredType::NATIVE_FLOAT;
            case NATIVE_DOUBLE			: return H5::PredType::NATIVE_DOUBLE;
            case NATIVE_LDOUBLE			: return H5::PredType::NATIVE_LDOUBLE;
            case NATIVE_B8				: return H5::PredType::NATIVE_B8;
            case NATIVE_B16				: return H5::PredType::NATIVE_B16;
            case NATIVE_B32				: return H5::PredType::NATIVE_B32;
            case NATIVE_B64				: return H5::PredType::NATIVE_B64;
            case NATIVE_OPAQUE			: return H5::PredType::NATIVE_OPAQUE;
            case NATIVE_HSIZE			: return H5::PredType::NATIVE_HSIZE;
            case NATIVE_HSSIZE			: return H5::PredType::NATIVE_HSSIZE;
            case NATIVE_HERR			: return H5::PredType::NATIVE_HERR;
            case NATIVE_HBOOL			: return H5::PredType::NATIVE_HBOOL;
                
            case NATIVE_INT8			: return H5::PredType::NATIVE_INT8;
            case NATIVE_UINT8			: return H5::PredType::NATIVE_UINT8;
            case NATIVE_INT16			: return H5::PredType::NATIVE_INT16;
            case NATIVE_UINT16			: return H5::PredType::NATIVE_UINT16;
            case NATIVE_INT32			: return H5::PredType::NATIVE_INT32;
            case NATIVE_UINT32			: return H5::PredType::NATIVE_UINT32;
            case NATIVE_INT64			: return H5::PredType::NATIVE_INT64;
            case NATIVE_UINT64			: return H5::PredType::NATIVE_UINT64;
        }
        throw exception::runtime(_("HDF datatype unkown"));
    }
    
    
}}}
#endif
#endif
