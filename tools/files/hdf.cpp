/** 
 \verbatim
 #########################################################################
 # GPL License                                                           #
 #                                                                       #
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
 \endverbatim
 **/



#include "hdf.hpp"

namespace machinelearning { namespace tools { namespace files {
    
    namespace blas  = boost::numeric::ublas;
    
    
    /** constructor
     * @overload
     * @param p_file filename
     **/
    hdf::hdf( const std::string& p_file ) :
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
    hdf::hdf( const std::string& p_file, const bool& p_write ) :
        m_file( p_file.c_str(), (p_write ? H5F_ACC_TRUNC : H5F_ACC_RDWR) )
    {
        #ifdef NDEBUG
        H5::Exception::dontPrint();
        #endif
    }

    
    /** destructor for closing file **/
    hdf::~hdf( void )
    {
       flush();
       m_file.close();
    }
    
    /** flushs all data for the file **/
    void hdf::flush( void ) const
    {
        m_file.flush( H5F_SCOPE_GLOBAL );
    }

    
    /** returns the used filename
     * @return string with filename
     **/
    std::string hdf::getFilename( void ) const
    {
        return m_file.getFileName();
    }
    
    /** returns the filesize in bytes
     * @return filesize in bytes
    **/
    unsigned long hdf::getFilesize( void ) const
    {
        return m_file.getFileSize();
    }
    
    
    /** deletes an entry in the HDF file
     * @param p_path path to element
    **/
    void hdf::remove( const std::string& p_path ) const
    {
        m_file.unlink( p_path );
    }
   
    
	/** reads a string array from HDF file into a std::vector
	 * @param p_path path to element
	 * @return std::vector with std::string elements
	**/
	std::vector<std::string> hdf::readStringVector( const std::string& p_path ) const
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
    bool hdf::pathexists( const std::string& p_path ) const
    {
        return H5Lexists( m_file.getId(), p_path.c_str(), H5P_DEFAULT );
    }
    
    
    /** read a simple string from file
     * @param p_path path to dataset
     * @return string with data
    **/
    std::string hdf::readString( const std::string& p_path ) const
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
    void hdf::write( const std::string& p_path, const std::string& p_value, const H5::PredType& p_datatype ) const
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
    void hdf::createPathWithDataset( const std::string& p_path, const H5::PredType& p_datatype, const H5::DataSpace& p_dataspace, std::vector<H5::Group>& p_groups, H5::DataSet& p_dataset ) const
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
            throw exception::greaterthanzero("path");
        
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


    
    
};};};
