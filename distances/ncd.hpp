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



#ifndef MACHINELEARNING_DISTANCES_NCD_HPP
#define MACHINELEARNING_DISTANCES_NCD_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include <boost/static_assert.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/symmetric.hpp>

#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/bzip2.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/null.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/counter.hpp>

#include <boost/thread.hpp>
#include <boost/ref.hpp>

#include "../exception/exception.h"



namespace machinelearning { namespace distances {   
    
    namespace ublas = boost::numeric::ublas;
    namespace bio   = boost::iostreams;
    
    
    /**
     * class for calculating the normalized compression distance (NCD)
     * with some different algorithms like gzip and bzip2
     * $LastChangedDate$
     **/
    template<typename T> class ncd {
        BOOST_STATIC_ASSERT( !boost::is_integral<T>::value );
        
        
    public:
        
        enum compresstype
        {
            gzip    = 0, 
            bzip2   = 1
        };
        
        enum compresslevel
        {
            defaultcompression  = 0,
            bestspeed           = 1,
            bestcompression     = 2
        };
        
        
        ncd ( void );
        ncd ( const compresstype& );
        ublas::matrix<T> unsymmetric ( const std::vector<std::string>&, const bool& = false );
        ublas::symmetric_matrix<T, ublas::upper> symmetric ( const std::vector<std::string>&, const bool& = false );
        T calculate ( const std::string&, const std::string&, const bool& = false );
        void setCompressionLevel( const compresslevel& = defaultcompression );
        
    private:
        
        /** type for internal compression state **/
        const compresstype m_compress;
        /** parameter for gzip **/
        bio::gzip_params m_gzipparam;
        /** parameter for bzip2 **/
        bio::bzip2_params m_bzip2param;
        /** map with index pairs for wavefront **/
        std::multimap<std::size_t, std::pair<std::size_t,std::size_t> > m_wavefront;    
        /** boost mutex for determine read/write access of the cache **/
        boost::shared_mutex m_cachelock;
        /** boost mutex for determine read/write access of the matrix **/
        boost::shared_mutex m_matrixlock;
        /** boost mutex for running **/
        boost::mutex m_running;
        
        
        std::size_t deflate ( const bool&, const std::string&, const std::string& = "" ) const;        
        void getWavefrontIndex( const std::size_t&, const std::size_t& );
        void multithread_deflate_symmetric( const std::vector<std::string>&, const std::size_t&, const bool&, ublas::symmetric_matrix<T, ublas::upper>&, ublas::vector<std::size_t>& );
        void multithread_deflate_unsymmetric( const std::vector<std::string>&, const std::size_t&, const bool&, ublas::matrix<T>&, ublas::vector<std::size_t>& );
        
        void setCache( ublas::vector<std::size_t>&, const std::size_t&, const std::size_t& );
        void getCache( const ublas::vector<std::size_t>&, const std::size_t&, const std::size_t&, std::size_t&, std::size_t& );
        void setUnsymmetric( ublas::matrix<T>&, const std::size_t&, const std::size_t&, const T&, const T& );
        void setSymmetric( ublas::symmetric_matrix<T, ublas::upper>&, const std::size_t&, const std::size_t&, const T& );
    };
    
    
    
    
    /** default constructor **/
    template<typename T> inline ncd<T>::ncd( void ) :
        m_compress ( gzip ),
        m_gzipparam( bio::gzip::default_compression ),
        m_bzip2param( 6 ),
        m_wavefront(),
        m_cachelock(),
        m_matrixlock(),
        m_running()
    {}
    
    
    /** constructor with the compression parameter
     * @param p_compress enum value that is declared inside the class
     **/
    template<typename T> inline ncd<T>::ncd( const compresstype& p_compress ) :
        m_compress ( p_compress ),
        m_gzipparam( bio::gzip::default_compression ),
        m_bzip2param( 6 ),
        m_wavefront(),
        m_cachelock(),
        m_matrixlock(),
        m_running()
    {}
    
    
    
    
    //--- multithread methods ---------------------------------------------------------------------------------------------------------------------------------
    
    
    /** creates wavefront index pairs for using on multithreaded systems
     * @param p_size number of rows or columns
     * @param p_threads number of threads
     **/
    template<typename T> inline void ncd<T>::getWavefrontIndex( const std::size_t& p_size, const std::size_t& p_threads )
    {
        m_wavefront.clear();
        
        // create wavefront in this case: (0,n) = 0, (0,n-1) = 1, (1,n) = 2 ...
        // we must create only the upper or lower index position, the other one can be created of swapping the index position
        std::size_t n=0;
        for(std::size_t i=p_size-1; i > 0; --i)
            for(std::size_t j=0; (i+j) < p_size; ++j)
                m_wavefront.insert(  std::pair<std::size_t, std::pair<std::size_t,std::size_t> >(n++ % p_threads, std::pair<std::size_t,std::size_t>(j, i+j))  );
    }
    
    
    /** writing cache data must be implementated thread-safe so we do writing-option in a own method
     * @param p_cache cache vector
     * @param p_id index position in the cache vector
     * @param p_data data which should be write
     **/
    template<typename T> inline void ncd<T>::setCache( ublas::vector<std::size_t>& p_cache, const std::size_t& p_id, const std::size_t& p_data )
    {
        // upgrade lock for writing
        boost::unique_lock<boost::shared_mutex> l_lock( m_cachelock ); 
        
        p_cache(p_id) = p_data;
    }
    
    
    /** reading cache data must be implementated thread-safe so we had created a own method for reading on one
     * access two data, because on the NCD calulation we need everytime two entries
     * @param p_cache cache vector
     * @param p_idfirst index position of the first element
     * @param p_idsecond index position of the second element
     * @param p_first reference for writing the first element
     * @param p_second reference for writing the second element
     **/
    template<typename T> inline void ncd<T>::getCache( const ublas::vector<std::size_t>& p_cache, const std::size_t& p_idfirst, const std::size_t& p_idsecond, std::size_t& p_first, std::size_t& p_second )
    {
        // shared access for reading
        boost::shared_lock<boost::shared_mutex> l_lock( m_cachelock );
        
        p_first  = p_cache(p_idfirst);
        p_second = p_cache(p_idsecond);
    }
    
    
    
    /** write method for unsymmetric matrix data, which sets two values into the positions 
     * @param p_matrix matrix with data
     * @param p_row row index
     * @param p_col col index
     * @param p_val1 value for (p_row/p_col)
     * @param p_val2 value for (p_col/p_row)
     **/
    template<typename T> inline void ncd<T>::setUnsymmetric( ublas::matrix<T>& p_matrix, const std::size_t& p_row, const std::size_t& p_col, const T& p_val1, const T& p_val2 )
    {
        // upgrade lock for writing
        boost::unique_lock<boost::shared_mutex> l_lock( m_matrixlock ); 
        
        // because of deflate algorithms there can be values greater than 1, so we check and set it to 1
        p_matrix(p_row, p_col) = std::min( static_cast<T>(1), p_val1 );
        p_matrix(p_col, p_row) = std::min( static_cast<T>(1), p_val2 );
    }
    
    
    /** write method for symmetric matrix data, which sets the values into the position
     * @param p_matrix matrix with data
     * @param p_row row index
     * @param p_col col index
     * @param p_val value for (p_row/p_col)
     **/
     template<typename T> inline void ncd<T>::setSymmetric( ublas::symmetric_matrix<T, ublas::upper>& p_matrix, const std::size_t& p_row, const std::size_t& p_col, const T& p_val )
     {
         // upgrade lock for writing
         boost::unique_lock<boost::shared_mutex> l_lock( m_matrixlock ); 
     
         p_matrix(p_row, p_col) = p_val;
     }
    
    
    
    /** creates the thread and calculates the NCD symmetric 
     * @param p_sources vector with source data
     * @param p_id thread id for reading pairs in wavefront
     * @πaram p_isfile bool if data is interpretated as filenames
     * @param p_matrix result matrix
     * @param p_cache cache vector
     **/
     template<typename T> inline void ncd<T>::multithread_deflate_symmetric( const std::vector<std::string>& p_sources, const std::size_t& p_id, const bool& p_isfile, ublas::symmetric_matrix<T, ublas::upper>& p_matrix, ublas::vector<std::size_t>& p_cache )
     {
         for(std::multimap<std::size_t, std::pair<std::size_t,std::size_t> >::iterator it=m_wavefront.lower_bound(p_id); it != m_wavefront.upper_bound(p_id); ++it) {
             // read cache data
             std::size_t l_first=0, l_second=0;
             getCache( p_cache, it->second.first, it->second.second, l_first, l_second);

             // check for both index positions the cache and adds the data
             if (l_first == 0) {
                 l_first = deflate(p_isfile, p_sources[it->second.first]);
                 setCache( p_cache, it->second.first, l_first );
             }
             
             if (l_second == 0) {
                 l_second = deflate(p_isfile, p_sources[it->second.second]);
                 setCache( p_cache, it->second.second, l_second );
             }
             
             
             // determin min and max
             const std::size_t l_min = std::min(l_first, l_second);
             const std::size_t l_max = std::max(l_first, l_second);
             
             // calculate NCD for both parts and create the arithmetic mean
             setSymmetric( p_matrix, it->second.first, it->second.second, 
                           0.5 * (static_cast<T>(deflate(p_isfile, p_sources[it->second.first], p_sources[it->second.second]) - l_min) / l_max + 
                                  static_cast<T>(deflate(p_isfile, p_sources[it->second.second], p_sources[it->second.first]) - l_min) / l_max)
                         );
         }
     }
    
    
    
    /** creates the thread and calculates the NCD unsymmetric 
     * @param p_sources vector with source data
     * @param p_id thread id for reading pairs in wavefront
     * @πaram p_isfile bool if data is interpretated as filenames
     * @param p_matrix result matrix
     * @param p_cache cache vector
     **/
    template<typename T> inline void ncd<T>::multithread_deflate_unsymmetric( const std::vector<std::string>& p_sources, const std::size_t& p_id, const bool& p_isfile, ublas::matrix<T>& p_matrix, ublas::vector<std::size_t>& p_cache )
    {
        for(std::multimap<std::size_t, std::pair<std::size_t,std::size_t> >::iterator it=m_wavefront.lower_bound(p_id); it != m_wavefront.upper_bound(p_id); ++it) {
            
            // read cache data
            std::size_t l_first=0, l_second=0;
            getCache( p_cache, it->second.first, it->second.second, l_first, l_second );
            
            // check for both index positions the cache and adds the data
            if (l_first == 0) {
                l_first = deflate(p_isfile, p_sources[it->second.first]);
                setCache( p_cache, it->second.first, l_first );
            }
            
            if (l_second == 0) {
                l_second = deflate(p_isfile, p_sources[it->second.second]);
                setCache( p_cache, it->second.second, l_second );
            }
            
            
            // determin min and max
            const std::size_t l_min = std::min(l_first, l_second);
            const std::size_t l_max = std::max(l_first, l_second);
            
            // calculate NCD
            setUnsymmetric( p_matrix, it->second.first, it->second.second, 
                            static_cast<T>(deflate(p_isfile, p_sources[it->second.first], p_sources[it->second.second]) - l_min) / l_max,
                            static_cast<T>(deflate(p_isfile, p_sources[it->second.second], p_sources[it->second.first]) - l_min) / l_max
                          );
        }
    }
    
    //---------------------------------------------------------------------------------------------------------------------------------------------------------
    
    
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
    template<typename T> inline T ncd<T>::calculate( const std::string& p_str1, const std::string& p_str2, const bool& p_isfile )
    {
        const std::size_t l_first  = deflate(p_isfile, p_str1);
        const std::size_t l_second = deflate(p_isfile, p_str2);
        
        return std::min( static_cast<T>(1),     static_cast<T>(deflate(p_isfile, p_str1, p_str2) - std::min(l_first, l_second)) / std::max(l_first, l_second) );
    }
    
    
    
    /** calculate all distances of the string vector (first item in the vector is
     * first row and colum in the returning matrix
     * @param p_strvec string vector
     * @param p_isfile parameter for interpreting the string as a file with path
     * @return dissimilarity matrix with std::vector x std::vector elements
     **/
    template<typename T> inline ublas::matrix<T> ncd<T>::unsymmetric( const std::vector<std::string>& p_strvec, const bool& p_isfile  )
    {
        if (p_strvec.size() == 0)
            throw exception::runtime(_("vector size must be greater than zero"));
        
        // init data
        ublas::vector<std::size_t> l_cache(p_strvec.size(), 0);
        ublas::matrix<T> l_result(p_strvec.size(), p_strvec.size(), static_cast<T>(0));
        
        // if we can use multithreads 
        if (boost::thread::hardware_concurrency() > 1) {
            
            // we set a lock during calculation, because the object can't handle different local states
            boost::unique_lock<boost::mutex> l_lock( m_running );
            
            // the first and last element are calculated in a single run, because
            // the first two thread are used the data directly
            l_cache(0) = deflate(p_isfile, p_strvec[0]);
            l_cache(p_strvec.size()-1) = deflate(p_isfile, p_strvec[p_strvec.size()-1]);
            
            // create wavefront index position
            getWavefrontIndex(p_strvec.size(), boost::thread::hardware_concurrency());
            
            // create threads
            boost::thread_group l_threadgroup;
            for(std::size_t i=0; i < boost::thread::hardware_concurrency(); ++i)
                l_threadgroup.create_thread(  boost::bind( &ncd<T>::multithread_deflate_unsymmetric, this, boost::ref(p_strvec), i, p_isfile, boost::ref(l_result), boost::ref(l_cache) )  );
            
            // run threads and wait during all finished
            l_threadgroup.join_all();
            
        } else // otherwise (we can't use threads)
            for (std::size_t i = 0; i < p_strvec.size(); ++i) {
                // create deflate to cache
                if (i==0)
                    l_cache(i) = deflate(p_isfile, p_strvec[i]);
                
                for (std::size_t j = i+1; j < p_strvec.size(); ++j) {
                    if (i==0)
                        l_cache(j) = deflate(p_isfile, p_strvec[j]);
                    
                    const std::size_t l_min = std::min(l_cache(i), l_cache(j));
                    const std::size_t l_max = std::max(l_cache(i), l_cache(j));
                    
                    setUnsymmetric( l_result, i, j,  
                                    static_cast<T>(deflate(p_isfile, p_strvec[i], p_strvec[j]) - l_min) / l_max, 
                                    static_cast<T>(deflate(p_isfile, p_strvec[j], p_strvec[i]) - l_min) / l_max
                                  );

                }
            }
        
        return l_result;
    }
    
    
    /** calculate all distances of the string vector (first item in the vector is
     * first row and colum in the returning matrix
     * @param p_strvec string vector
     * @param p_isfile parameter for interpreting the string as a file with path
     * @return dissimilarity matrix with std::vector x std::vector elements
     **/
     template<typename T> inline ublas::symmetric_matrix<T, ublas::upper> ncd<T>::symmetric( const std::vector<std::string>& p_strvec, const bool& p_isfile  )
     {
         if (p_strvec.size() == 0)
             throw exception::runtime(_("vector size must be greater than zero"));
         
         // init data
         ublas::vector<std::size_t> l_cache(p_strvec.size(), 0);
         ublas::symmetric_matrix<T, ublas::upper> l_result(p_strvec.size(), p_strvec.size());
         
         // if we can use multithreads 
         if (boost::thread::hardware_concurrency() > 1) {
             
             // we set a lock during calculation, because the object can't handle different local states
             boost::unique_lock<boost::mutex> l_lock( m_running );
             
             // the first and last element are calculated in a single run, because
             // the first two thread are used the data directly
             l_cache(0) = deflate(p_isfile, p_strvec[0]);
             l_cache(p_strvec.size()-1) = deflate(p_isfile, p_strvec[p_strvec.size()-1]);
             
             // create wavefront index position
             getWavefrontIndex(p_strvec.size(), boost::thread::hardware_concurrency());
             
             // create threads
             boost::thread_group l_threadgroup;
             for(std::size_t i=0; i < boost::thread::hardware_concurrency(); ++i)
                 l_threadgroup.create_thread(  boost::bind( &ncd<T>::multithread_deflate_symmetric, this, boost::ref(p_strvec), i, p_isfile, boost::ref(l_result), boost::ref(l_cache) )  );
             
             // run threads and wait during all finished
             l_threadgroup.join_all();
             
         } else // otherwise (we can't use threads)
             for (std::size_t i = 0; i < p_strvec.size(); ++i) {
                 // create deflate to cache
                 if (i==0)
                     l_cache(i) = deflate(p_isfile, p_strvec[i]);
                 
                 for (std::size_t j = i+1; j < p_strvec.size(); ++j) {
                     if (i==0)
                         l_cache(j) = deflate(p_isfile, p_strvec[j]);
                     
                     const std::size_t l_min = std::min(l_cache(i), l_cache(j));
                     const std::size_t l_max = std::max(l_cache(i), l_cache(j));
                     
                     setSymmetric( l_result, i, j,  
                                   0.5 * (static_cast<T>(deflate(p_isfile, p_strvec[i], p_strvec[j]) - l_min) / l_max + 
                                          static_cast<T>(deflate(p_isfile, p_strvec[j], p_strvec[i]) - l_min) / l_max)
                                 );
                     
                 }
             }
         
         // we set the diagonal elements to zero
         for(std::size_t i=0; i < l_result.size1(); ++i)
             l_result(i,i) = static_cast<T>(0);
         
         return l_result;
     }
    
    
    /** deflate a string or file with the algorithm
     * @param p_file bool for interpret input string like filenames
     * @param p_str1 first string to compress
     * @param p_str2 optional second string to compress
     * @return number of bytes 
     **/    
    template<typename T> inline std::size_t ncd<T>::deflate( const bool& p_isfile, const std::string& p_str1, const std::string& p_str2 ) const
    {
        if (p_str1.empty())
            throw exception::runtime(_("string size must be greater than zero"));
        
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
            
            std::ifstream l_file;
            
            l_file.open(p_str1.c_str(), std::ifstream::binary);
            if (!l_file.is_open())
                throw exception::runtime(_("file can not be opened"));
            std::copy( std::istream_iterator<char>(l_file), std::istream_iterator<char>(), std::ostreambuf_iterator<char>(&l_deflate) );
            l_file.close();
            
            if (!p_str2.empty()) {
                l_file.open(p_str2.c_str(), std::ifstream::binary);
                if (!l_file.is_open())
                    throw exception::runtime(_("file can not be opened"));
                std::copy( std::istream_iterator<char>(l_file), std::istream_iterator<char>(), std::ostreambuf_iterator<char>(&l_deflate) );
                l_file.close();                
            }
            
        } else {
            std::istringstream l_sstream( p_str1+p_str2, std::stringstream::binary );
            std::copy( std::istream_iterator<char>(l_sstream), std::istream_iterator<char>(), std::ostreambuf_iterator<char>(&l_deflate) );
        }

        // close deflate stream, cause only than counter returns correct value
        bio::close(l_deflate);
        
        
        return (static_cast<std::size_t>(l_counter.characters()) >= l_removesize) ? l_counter.characters()-l_removesize*sizeof(char) : l_counter.characters();
    }
    
    
};};
#endif
