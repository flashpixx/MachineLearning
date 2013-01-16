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



#ifndef __MACHINELEARNING_DISTANCES_NCD_HPP
#define __MACHINELEARNING_DISTANCES_NCD_HPP

#include <omp.h>
#include <string>
#include <sstream>
#include <iostream>
#include <ostream>
#include <fstream>

#ifdef MACHINELEARNING_MPI
#include <boost/mpi.hpp>
#endif

#include <boost/static_assert.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/symmetric.hpp>

#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/bzip2.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/null.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/counter.hpp>

#include "../errorhandling/exception.hpp"




namespace machinelearning { namespace distances {   
    
    #ifndef SWIG
    namespace ublas = boost::numeric::ublas;
    namespace bio   = boost::iostreams;
    #ifdef MACHINELEARNING_MPI
    namespace mpi   = boost::mpi;
    #endif
    #endif
    
    
    /**
     * class for calculating the normalized compression distance (NCD)
     * with some different algorithms like gzip and bzip2
     **/
    template<typename T> class ncd
    {
        #ifndef SWIG
        BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
        #endif
        
        
        public:
            
            enum compresstype
            {
                gzip, 
                bzip2
            };
            
            enum compresslevel
            {
                defaultcompression,
                bestspeed,
                bestcompression
            };
            
            
            ncd ( void );
            ncd ( const compresstype& );
            ublas::matrix<T> unsquare ( const std::vector<std::string>&, const std::vector<std::string>&, const bool& = false ) const;
            ublas::matrix<T> unsymmetric ( const std::vector<std::string>&, const bool& = false ) const;
            ublas::symmetric_matrix<T, ublas::upper> symmetric ( const std::vector<std::string>&, const bool& = false ) const;
            T calculate ( const std::string&, const std::string&, const bool& = false ) const;
            void setCompressionLevel( const compresslevel& = defaultcompression );
            
            #ifdef MACHINELEARNING_MPI
            ublas::matrix<T> unsquare ( const mpi::communicator&, const std::vector<std::string>&, const bool& = false ) const;
            #endif
            
        private:
            
            /** type for internal compression state **/
            const compresstype m_compress;
            /** parameter for gzip **/
            bio::gzip_params m_gzipparam;
            /** parameter for bzip2 **/
            bio::bzip2_params m_bzip2param;
            
            std::size_t deflate ( const bool&, const std::string&, const std::string& = "" ) const;        
            std::multimap<std::size_t, std::pair<std::size_t,std::size_t> > getWavefrontIndex( const std::size_t&, const std::size_t& ) const;
    };
    
    
    
    
    /** default constructor **/
    template<typename T> inline ncd<T>::ncd( void ) :
        m_compress ( gzip ),
        m_gzipparam( bio::gzip::default_compression ),
        m_bzip2param( 6 )
    {}
    
    
    /** constructor with the compression parameter
     * @param p_compress enum value that is declared inside the class
     **/
    template<typename T> inline ncd<T>::ncd( const compresstype& p_compress ) :
        m_compress ( p_compress ),
        m_gzipparam( bio::gzip::default_compression ),
        m_bzip2param( 6 )
    {}
    
    
    /** creates wavefront index pairs for using on multithreaded systems
     * @param p_size number of rows or columns
     * @param p_threads number of threads
     * @return map with indices
     **/
    template<typename T> inline std::multimap<std::size_t, std::pair<std::size_t,std::size_t> > ncd<T>::getWavefrontIndex( const std::size_t& p_size, const std::size_t& p_threads ) const
    {
        std::multimap<std::size_t, std::pair<std::size_t,std::size_t> > l_wavefront;
        
        // create wavefront in this case: (0,n) = 0, (0,n-1) = 1, (1,n) = 2 ...
        // we must create only the upper or lower index position, the other one can be created of swapping the index position
        std::size_t n=0;
        for(std::size_t i=1; i < p_size; ++i)
            for(std::size_t j=0; (i+j) < p_size; ++j)
                l_wavefront.insert(  std::pair<std::size_t, std::pair<std::size_t,std::size_t> >(n++ % p_threads, std::pair<std::size_t,std::size_t>(j,i+j))  );
            
        return l_wavefront;
    }
    
      
    /** sets the compression level
     * @param  p_level compression level
     **/
    template<typename T> inline void ncd<T>::setCompressionLevel( const compresslevel& p_level )
    {
        switch (p_level) 
        {
            case defaultcompression :   
                m_gzipparam     = bio::gzip_params( bio::gzip::default_compression );
                m_bzip2param    = bio::bzip2_params( 6 );
                break;
                
            case bestspeed          :   
                m_gzipparam     = bio::gzip_params( bio::gzip::best_speed );
                m_bzip2param    = bio::bzip2_params( 1 );
                break;
                
            case bestcompression    :   
                m_gzipparam     = bio::gzip_params( bio::gzip::best_compression );
                m_bzip2param    = bio::bzip2_params( 9 );
                break;
        }
    }
    
    
    
    /** calculate distances between two strings
     * @param p_str1 first string
     * @param p_str2 second string
     * @param p_isfile parameter for interpreting the string as a file with path
     * @return distance value
     **/   
    template<typename T> inline T ncd<T>::calculate( const std::string& p_str1, const std::string& p_str2, const bool& p_isfile ) const
    {
        const std::size_t l_first  = deflate(p_isfile, p_str1);
        const std::size_t l_second = deflate(p_isfile, p_str2);
        
        return std::min( static_cast<T>(1),     static_cast<T>(deflate(p_isfile, p_str1, p_str2) - std::min(l_first, l_second)) / std::max(l_first, l_second) );
    }
    
    
    
    /** calculate all distances of the string vector (first item in the vector is
     * first row and colum in the returning matrix)
     * @param p_strvec string vector
     * @param p_isfile parameter for interpreting the string as a file with path
     * @return dissimilarity matrix with std::vector x std::vector elements
     **/
    template<typename T> inline ublas::matrix<T> ncd<T>::unsymmetric( const std::vector<std::string>& p_strvec, const bool& p_isfile  ) const
    {
        if (p_strvec.size() == 0)
            throw exception::runtime(_("vector size must be greater than zero"), *this);
        
        // init data
        ublas::vector<std::size_t> l_cache(p_strvec.size(), 0);
        ublas::matrix<T> l_result(p_strvec.size(), p_strvec.size(), static_cast<T>(0));
        
        // the first and last element are calculated in a single run, because
        // the first two thread are used the data directly
        l_cache(0)                 = deflate(p_isfile, p_strvec[0]);
        l_cache(p_strvec.size()-1) = deflate(p_isfile, p_strvec[p_strvec.size()-1]);
        
        // create wavefront index position
        std::multimap<std::size_t, std::pair<std::size_t,std::size_t> > l_wavefront = getWavefrontIndex(p_strvec.size(), omp_get_max_threads());
        
        #pragma omp parallel shared(l_cache, l_result)
        {
            for(std::multimap<std::size_t, std::pair<std::size_t,std::size_t> >::iterator it=l_wavefront.lower_bound(omp_get_thread_num()); it != l_wavefront.upper_bound(omp_get_thread_num()); ++it) {
                
                // read cache data
                std::size_t l_first=0, l_second=0;
                
                #pragma omp critical
                {
                    l_first  = l_cache(it->second.first);
                    l_second = l_cache(it->second.second);
                }
                
                // check for both index positions the cache and adds the data
                if (l_first == 0) {
                    l_first = deflate(p_isfile, p_strvec[it->second.first]);
                    
                    #pragma omp critical
                    l_cache(it->second.first) = l_first;
                }
                
                if (l_second == 0) {
                    l_second = deflate(p_isfile, p_strvec[it->second.second]);
                    
                    #pragma omp critical 
                    l_cache(it->second.second) = l_second;
                }
                
                
                // determin min and max
                const std::size_t l_min = std::min(l_first, l_second);
                const std::size_t l_max = std::max(l_first, l_second);
                
                // calculate NCD
                l_result(it->second.first, it->second.second) = std::min( static_cast<T>(1), static_cast<T>(deflate(p_isfile, p_strvec[it->second.first], p_strvec[it->second.second]) - l_min) / l_max );
                l_result(it->second.second, it->second.first) = std::min( static_cast<T>(1), static_cast<T>(deflate(p_isfile, p_strvec[it->second.second], p_strvec[it->second.first]) - l_min) / l_max );
            }
        }
        
        return l_result;
    }
    
    
    /** calculate all distances of the string vector (first item in the vector is
     * first row and colum in the returning matrix)
     * @param p_strvec string vector
     * @param p_isfile parameter for interpreting the string as a file with path
     * @return dissimilarity matrix with std::vector x std::vector elements
     **/
    template<typename T> inline ublas::symmetric_matrix<T, ublas::upper> ncd<T>::symmetric( const std::vector<std::string>& p_strvec, const bool& p_isfile  ) const
    {
         if (p_strvec.size() == 0)
             throw exception::runtime(_("vector size must be greater than zero"), *this);
         
         // init data
         ublas::vector<std::size_t> l_cache(p_strvec.size(), 0);
         ublas::symmetric_matrix<T, ublas::upper> l_result(p_strvec.size(), p_strvec.size());
        
        // the first and last element are calculated in a single run, because
        // the first two thread are used the data directly
        l_cache(0)                 = deflate(p_isfile, p_strvec[0]);
        l_cache(p_strvec.size()-1) = deflate(p_isfile, p_strvec[p_strvec.size()-1]);
        
        // create wavefront index position
        std::multimap<std::size_t, std::pair<std::size_t,std::size_t> > l_wavefront = getWavefrontIndex(p_strvec.size(), omp_get_max_threads());
        
        #pragma omp parallel shared(l_cache, l_result)
        {
            for(std::multimap<std::size_t, std::pair<std::size_t,std::size_t> >::iterator it=l_wavefront.lower_bound(omp_get_thread_num()); it != l_wavefront.upper_bound(omp_get_thread_num()); ++it) {
                
                // read cache data
                std::size_t l_first=0, l_second=0;
                
                #pragma omp critical
                {
                    l_first  = l_cache(it->second.first);
                    l_second = l_cache(it->second.second);
                }
                
                // check for both index positions the cache and adds the data
                if (l_first == 0) {
                    l_first = deflate(p_isfile, p_strvec[it->second.first]);
                    
                    #pragma omp critical
                    l_cache(it->second.first) = l_first;
                }
                
                if (l_second == 0) {
                    l_second = deflate(p_isfile, p_strvec[it->second.second]);
                    
                    #pragma omp critical 
                    l_cache(it->second.second) = l_second;
                }
                
                
                // determin min and max
                const std::size_t l_min = std::min(l_first, l_second);
                const std::size_t l_max = std::max(l_first, l_second);
                
                // calculate NCD
                l_result(it->second.first, it->second.second) = std::min( static_cast<T>(1),
                             0.5 * (static_cast<T>(deflate(p_isfile, p_strvec[it->second.first], p_strvec[it->second.second]) - l_min) / l_max + 
                                    static_cast<T>(deflate(p_isfile, p_strvec[it->second.second], p_strvec[it->second.first]) - l_min) / l_max)
                             );

            }
        }
        
         // we set the diagonal elements to zero, because constructor has no parameter for initialization value
         #pragma omp for
         for(std::size_t i=0; i < l_result.size1(); ++i)
             l_result(i,i) = static_cast<T>(0);
         
         return l_result;
    }
    
    
    /** calculate all distances between each element of both string vectors
     * @param p_strvec1 string vector
     * @param p_strvec2 string vector
     * @param p_isfile parameter for interpreting the string as a file with path
     * @return dissimilarity matrix with std::vector1 x std::vector2 elements (deflating order: element first vector concat with element second vector )
     **/
    template<typename T> inline ublas::matrix<T> ncd<T>::unsquare( const std::vector<std::string>& p_strvec1, const std::vector<std::string>& p_strvec2, const bool& p_isfile ) const
    {
        if ( (p_strvec1.size() == 0) || (p_strvec2.size() == 0) )
            throw exception::runtime(_("vector size must be greater than zero"), *this);
        
        // init data
        ublas::vector<std::size_t> l_cache(p_strvec1.size() + p_strvec2.size(), 0);
        ublas::matrix<T> l_result( p_strvec1.size(), p_strvec2.size() );
    
        // we create the map for threads (on the contrary to wavefront we cut patches for each thread, because we must calculate each element without any topographical order)
        // The map element has in the std::pair within the first position the index of the first vector
        std::multimap<std::size_t, std::pair<std::size_t,std::size_t> > l_map;
        
        #pragma omp parallel shared(l_map, l_cache, l_result)
        {
            if (p_strvec1.size() < p_strvec2.size())
                
                #pragma omp for
                for(std::size_t j=0; j < p_strvec1.size(); ++j)
                    for(std::size_t i=0; i < p_strvec2.size(); ++i)
                        
                        #pragma omp critical
                        l_map.insert(  std::pair<std::size_t, std::pair<std::size_t,std::size_t> >(i % omp_get_max_threads(), std::pair<std::size_t,std::size_t>(j,i))  );
            
            else
                
                #pragma omp for
                for(std::size_t j=0; j < p_strvec2.size(); ++j)
                    for(std::size_t i=0; i < p_strvec1.size(); ++i)
                        
                        #pragma omp critical
                        l_map.insert(  std::pair<std::size_t, std::pair<std::size_t,std::size_t> >(i % omp_get_max_threads(), std::pair<std::size_t,std::size_t>(i,j))  );

            
            #pragma omp barrier
            
            for(std::multimap<std::size_t, std::pair<std::size_t,std::size_t> >::iterator it=l_map.lower_bound(omp_get_thread_num()); it != l_map.upper_bound(omp_get_thread_num()); ++it) {
        
                // read cache data
                std::size_t l_first=0, l_second=0;
                
                #pragma omp critical
                {
                    l_first  = l_cache(it->second.first);
                    l_second = l_cache(p_strvec1.size()+it->second.second);
                }
                
                // check for both index positions the cache and adds the data
                if (l_first == 0) {
                    l_first = deflate(p_isfile, p_strvec1[it->second.first]);
                    
                    #pragma omp critical
                    l_cache(it->second.first) = l_first;
                }
                
                if (l_second == 0) {
                    l_second = deflate(p_isfile, p_strvec2[it->second.second]);
                    
                    #pragma omp critical 
                    l_cache(p_strvec1.size()+it->second.second) = l_second;
                }
                
                
                // determin min and max
                const std::size_t l_min = std::min(l_first, l_second);
                const std::size_t l_max = std::max(l_first, l_second);
        
                // calculate NCD
                l_result(it->second.first, it->second.second) = std::min( static_cast<T>(1), static_cast<T>(deflate(p_isfile, p_strvec1[it->second.first], p_strvec2[it->second.second]) - l_min) / l_max );
            }
        }
                
        return l_result;
    }
    
    
    #ifdef MACHINELEARNING_MPI
    
    /** creates a distance matrix with shared data
     * @param p_mpi MPI object
     * @param p_strvec local dataset
     * @param p_isfile parameter for interpreting the string as a file with path
     * @return part of distance matrix (all data size x local data size)
     **/
    template<typename T> inline ublas::matrix<T> ncd<T>::unsquare ( const mpi::communicator& p_mpi, const std::vector<std::string>& p_strvec, const bool& p_isfile ) const
    {
        if (p_strvec.size() == 0)
            throw exception::runtime(_("vector size must be greater than zero"), *this);
        
        // synchronize the isFile parameter
        const bool l_isfile = mpi::all_reduce(p_mpi, p_isfile, std::multiplies<bool>());
        
        // we detect the matrix row size (sum over each CPU data - we don't use the all_reduce,
        // because the different datasizes of each CPU data is needed later)
        std::vector<std::size_t> l_datasize;
        mpi::all_gather(p_mpi, p_strvec.size(), l_datasize );
        const std::size_t l_rowsize = std::accumulate( l_datasize.begin(), l_datasize.end(), 0 );
        
        // create the target matrix (rows = all data size, column local data size)
        ublas::matrix<T> l_result( l_rowsize, p_strvec.size() );
        
        
        // create the local distances
        const std::size_t l_localrow = std::accumulate( l_datasize.begin(), l_datasize.begin() + static_cast<std::size_t>(p_mpi.rank()), 0 );
        ublas::matrix_range< ublas::matrix<T> > l_rangelocal(l_result, 
                                                                  ublas::range( l_localrow, l_localrow + p_strvec.size() ), 
                                                                  ublas::range( 0, l_result.size2() )
                                                                  );
        l_rangelocal.assign( unsquare(p_strvec, p_strvec, l_isfile) );
        
        // create distance to the local articles and the articless of the neighborhood CPU
        for(std::size_t i=1; i < static_cast<std::size_t>(p_mpi.size()); ++i)
        {
            // get start index on the matrix
            const std::size_t l_successor   = (static_cast<std::size_t>(p_mpi.rank())+i) % static_cast<std::size_t>(p_mpi.size());
            const std::size_t l_predecessor = (static_cast<std::size_t>(p_mpi.rank())+p_mpi.size()-i) % static_cast<std::size_t>(p_mpi.size());
            
            // send and receive with non-blocking operation and wait for both request
            mpi::request l_req[2];
            std::vector<std::string> l_neighbourdata;
            l_req[0] = p_mpi.isend(l_successor, 0, p_strvec);
            l_req[1] = p_mpi.irecv(l_predecessor, 0, l_neighbourdata);
            mpi::wait_all(l_req, l_req+2);
            
            // get position within the matrix and create distance values
            const std::size_t l_startrow = std::accumulate( l_datasize.begin(), l_datasize.begin() + l_predecessor, 0 );
            ublas::matrix_range< ublas::matrix<T> > l_range( l_result, 
                                                             ublas::range( l_startrow, l_startrow+l_neighbourdata.size() ), 
                                                             ublas::range( 0, l_result.size2() )
                                                           );
            l_range.assign( unsquare(l_neighbourdata, p_strvec, l_isfile) );
        }
        
        // set the main diagonal zero values
        #pragma omp parallel for shared(l_result)
        for(std::size_t i = 0; i < l_result.size2(); ++i)
            l_result(l_localrow+i,i) = 0;
        
        return l_result;
    }
    #endif
    
    
    /** deflate a string or file with the algorithm
     * @param p_isfile bool for interpret input string like filenames
     * @param p_str1 first string to compress
     * @param p_str2 optional second string to compress
     * @return number of bytes 
     **/    
    template<typename T> inline std::size_t ncd<T>::deflate( const bool& p_isfile, const std::string& p_str1, const std::string& p_str2 ) const
    {
        if (p_str1.empty())
            throw exception::runtime(_("string size must be greater than zero"), *this);

        // for each compress algorithm we removed the header & footer size of the resulting count
        // @see http://en.wikipedia.org/wiki/Bzip2#File_format
        // @see http://en.wikipedia.org/wiki/Gzip#File_format
        std::size_t l_removesize = 0;
        
        // create output null stream, compressor and counter structure
        bio::counter l_counter;
        bio::basic_null_sink<char> l_null;
        bio::filtering_streambuf< bio::output > l_deflate;
        
        switch (m_compress) {
            case gzip   : l_deflate.push( bio::gzip_compressor( m_gzipparam ) );      l_removesize = 18; break;
            case bzip2  : l_deflate.push( bio::bzip2_compressor( m_bzip2param ) );    l_removesize = 8; break;
        }
        l_deflate.push( boost::ref(l_counter) );        
        l_deflate.push( l_null );
        
        // create stream for data and copy streamdata to output
        if (p_isfile) {
            // for filestreams copy data manually to deflate stream, cause boost::iostreams::copy close
            // source and destination stream after copy is finished, so we copy only data from first
            // stream into deflate and than from second stream
            
            std::ifstream l_file1(p_str1.c_str(), std::ifstream::binary);
            if (!l_file1.is_open())
                throw exception::runtime(_("file can not be opened"), *this);
            std::copy( std::istream_iterator<char>(l_file1), std::istream_iterator<char>(), std::ostreambuf_iterator<char>(&l_deflate) );
            
            if (!p_str2.empty()) {
                std::ifstream l_file2(p_str2.c_str(), std::ifstream::binary);
                if (!l_file2.is_open())
                    throw exception::runtime(_("file can not be opened"), *this);
                std::copy( std::istream_iterator<char>(l_file2), std::istream_iterator<char>(), std::ostreambuf_iterator<char>(&l_deflate) );
            }
            
        } else {
          
            // we read binary data and don't skip any whitespaces
            std::stringstream l_sstream( std::stringstream::in | std::stringstream::out | std::stringstream::binary );
            l_sstream << std::noskipws << p_str1 << p_str2;
            std::copy( std::istream_iterator<char>(l_sstream), std::istream_iterator<char>(), std::ostreambuf_iterator<char>(&l_deflate) );
        }

        // close deflate stream, cause only than counter returns correct value
        bio::close(l_deflate);
        
        return (static_cast<std::size_t>(l_counter.characters()) >= l_removesize) ? l_counter.characters()-l_removesize*sizeof(char) : l_counter.characters();
    }
    
    
}}
#endif
