#include "machinelearning.h"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/numeric/ublas/io.hpp>

#include "boost/multi_array.hpp"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>

#include <boost/numeric/bindings/ublas/matrix.hpp>
#include <boost/numeric/bindings/ublas/vector.hpp>


int main(int argc, char *argv[]) {
    #ifdef CLUSTER
    namespace mpi	= boost::mpi;
    mpi::environment loMPIenv(argc, argv);
    mpi::communicator loMPICom;
    #endif
    
    
    namespace tl        = machinelearning::tools;
    namespace dist      = machinelearning::distances;
    namespace ndim      = machinelearning::dimensionreduce::nonsupervised;
    namespace dim       = machinelearning::dimensionreduce::supervised;
    namespace sl        = machinelearning::clustering::supervised;
    namespace nsl       = machinelearning::clustering::nonsupervised;
    namespace nd        = machinelearning::neighborhood;
    namespace cl        = machinelearning::classifier;
    namespace func      = machinelearning::functionaloptimization;
    namespace linalg    = boost::numeric::bindings::lapack;
    namespace ublas     = boost::numeric::ublas;
    
    #ifdef MULTILANGUAGE
    tl::language::bind("ml", "./tools/language/");
    #endif
    
    #ifdef FILES
    //tl::files::hdf o("blub.hdf5");
    /*tl::files::hdf o("string.hdf5");
    std::vector<std::string> x = o.readStringVector("/array");
    for(std::size_t i=0; i < x.size(); ++i)
        std::cout << x[i] << std::endl;*/
    #endif
      
    
    // ===== MDS ======
    /*
    ublas::matrix<double> data = o.readMatrix<double>("/mds2", H5::PredType::NATIVE_DOUBLE); 
    ndim::mds<double> l(2, ndim::mds<double>::sammon, 1);
     
    tl::files::hdf f("mds.hdf5", true);
    f.write<double>( "/data",  l.map(data), H5::PredType::NATIVE_DOUBLE );  
    */
   
    // ===== Wikipedia ======
    /*
    #ifdef SOURCES
    tl::sources::wikipedia w;
    //w.getArticle("schäfer");
    //w.getArticle("RMI");
    w.getRandomArticle();
    
    std::vector<std::string> k;
    if (w.isArticle()) {
        std::cout << w.getArticleContent() << "\n==========================================================" << std::endl;
        k = w.getArticleLabel();
    } else
        k = w.getArticleAcronym();
    
    
    for(std::size_t i=0; i < k.size(); ++i)
        std::cout << k[i] << std::endl;
    
    #endif
    */
    
    // ===== Newsgroup ======
    /*
    // dt. große Gruppen: de.soc.politik.misc, de.rec.fotografie, de.talk.tagesgeschehen, de.etc.sprache.deutsch, de.comp.os.unix.linux.misc, de.sci.electronics, de.etc.fahrzeug.auto, de.comp.sys.mac.misc, de.comp.lang.java, de.soc.weltanschauung.christentum
    
    #ifdef SOURCES    
    tl::sources::nntp x("news.online.de");
    //tl::sources::nntp x("news.tu-clausthal.de");
    //tl::sources::nntp x("news.t-online.de");
    //tl::sources::nntp x("news.gmane.org");
    
    
    // sort with size
    std::map<std::string, std::size_t> groups = x.getGroupList();
    std::multimap<std::size_t, std::string> newsgroups;
    for (std::map<std::string, std::size_t>::iterator it = groups.begin(); it != groups.end(); ++it)
        if (it->first.substr(0,2) == "de")
            newsgroups.insert( std::pair<std::size_t, std::string>(it->second, it->first) );
    for (std::multimap<std::size_t, std::string>::iterator it = newsgroups.begin(); it != newsgroups.end(); ++it)
        std::cout << it->second << "\t\t\t" << it->first << std::endl;
    
    
    x.setContent( tl::sources::nntp::full );
    x.setGroup("1und1.announce");
    //x.setGroup("tuc.misc");
    //x.setGroup("de.comp.text.misc");
    //x.setGroup("gmane.linux.gentoo.user");
    for(tl::sources::nntp::iterator it=x.begin(); it != x.end(); ++it) {
        std::string header, body;
        tl::sources::nntp::separateHeaderBody( *it, header, body );
        
        std::cout << header << "\n-------------------------------------------------\n" << body << "\n>=================================================================================<" << std::endl;
    }
    #endif
    */
     
    
    // ===== Gradient ======
    /*
    boost::multi_array<double, 2> x;
    boost::multi_array<double, 2> y;
    boost::multi_array<double, 2> target;
    
    func::gradientdescent<double, 2> gd("a*x^3 + b*y^4");
    
    
    gd.setErrorFunction("a, b");
    
    gd.setOptimizeVar("a", 9);
    gd.setOptimizeVar("b", 2, 5);
    
    gd.setStaticVar("x", x);
    gd.setStaticVar("y", y);
    gd.setStaticVar("target", target);
    
    gd.optimize(1, 0.1);
    */
    
    
    // ===== Lazy Learner ======
    /*
    ublas::matrix<double> data       = o.readMatrix<double>("/iris/data", H5::PredType::NATIVE_DOUBLE); 
    ublas::vector<std::size_t> label = o.readVector<std::size_t>("/iris/label", H5::PredType::NATIVE_ULONG); 

    
    ublas::indirect_array<> x(10);
    tl::random l_rand;
    for(std::size_t i=0; i < x.size(); ++i)
        x(i) = static_cast<std::size_t>(l_rand.get<double>( tl::random::uniform, 0, data.size1()));

    ublas::indirect_array<> y(data.size2());
    for(std::size_t i=0; i < y.size(); ++i)
        y(i) = i;
    

    ublas::matrix<double>      blub1 = ublas::project(data, x, y);
    ublas::vector<std::size_t> blub2 = ublas::project(label, x);
    
    dist::euclid<double> d;
    nd::knn<double> k(d, 4);
    cl::lazylearner<double, std::size_t> ll(k, cl::lazylearner<double, std::size_t>::inversedistance);
    
    ll.setDatabase( blub1,  tl::vector::copy(blub2) );
    
    
    tl::files::hdf f("lazy.hdf5", true);
    f.write<double>( "/data",  data, H5::PredType::NATIVE_DOUBLE );
    f.write<double>( "/label",  tl::vector::copy( ll.use(data) ), H5::PredType::NATIVE_DOUBLE ); 
    f.write<double>( "/basedata",  blub1, H5::PredType::NATIVE_DOUBLE );
    f.write<double>( "/baselabel",  blub2, H5::PredType::NATIVE_DOUBLE );
    */
    
    // ==== Spectral Clustering ====
    /*
    ublas::matrix<double> data = o.readMatrix<double>("/spectral2", H5::PredType::NATIVE_DOUBLE); 
    nsl::spectralclustering<double> sp(3, data.size1());
    sp.train(data, 15);
    
    tl::files::hdf f("spectral.hdf5", true);
    f.write<double>( "/data",  tl::matrix::setNumericalZero(sp.getPrototypes()), H5::PredType::NATIVE_DOUBLE );  
    */
    
    // ==== LLE ====
    /*
    ublas::matrix<double> data = o.readMatrix<double>("/pcadata", H5::PredType::NATIVE_DOUBLE); 
    
    dist::euclid<double> d;
    nd::knn<double> k(d, 12);
    ndim::lle<double> l(k,2);
    
    tl::files::hdf f("lle.hdf5", true);
    f.write<double>( "/data",  l.map(data), H5::PredType::NATIVE_DOUBLE );  
    */
    
	
	// ==== LDA ====
    /*
    ublas::matrix<double> data = o.readMatrix<double>("/ldadata", H5::PredType::NATIVE_DOUBLE);
    std::vector<unsigned int> labels = tl::vector::copy( o.readVector<unsigned int>("/ldalabel", H5::PredType::NATIVE_UINT) );
    
    dim::lda<double, unsigned int> lx(2);
    tl::files::hdf f("lda.hdf5", true);
    f.write<double>( "/data",  lx.map(data, labels), H5::PredType::NATIVE_DOUBLE );    
    */
    
    
    // ==== PCA ====
    /*
    ublas::matrix<double> data = o.readMatrix<double>("/pcadata", H5::PredType::NATIVE_DOUBLE); 
    ndim::pca<double> p(2);
    
    tl::files::hdf f("pca.hdf5", true);
    f.write<double>( "/data",  p.map(data), H5::PredType::NATIVE_DOUBLE );
    */
    
    
    // ===== kmeans ===== 
    /*
    ublas::matrix<double> data = o.readMatrix<double>("/ngdata", H5::PredType::NATIVE_DOUBLE);
    
    dist::euclid<double> d;
    nsl::kmeans<double> kmeans(d, 11, data.size2());
    kmeans.setLogging(true);
    kmeans.train(data, 15);
    
    tl::files::hdf f("kmeans.hdf5", true);
    f.write<double>( "/protos",  kmeans.getPrototypes(), H5::PredType::NATIVE_DOUBLE );
    
    if (kmeans.getLogging()) {
        f.write<double>( "/error",  tl::vector::copy(kmeans.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
        std::vector< ublas::matrix<double> > p = kmeans.getLoggedPrototypes();
        for(std::size_t i=0; i < p.size(); ++i)
            f.write<double>("/log" + boost::lexical_cast<std::string>( i ), p[i], H5::PredType::NATIVE_DOUBLE );
    }
    */
    
    
    
    // ===== RNG =====
    /*
    ublas::matrix<double> data = o.readMatrix<double>("/relational/cortex/data2", H5::PredType::NATIVE_DOUBLE);
    
    const std::size_t ngit      = 25;
    const std::size_t numproto  = 3;
    
    #ifdef CLUSTER
    // extract the data for the process (every process has load the whole data and shuffel them with the broadcasted shuffle vector)
    std::size_t nums     		= data.size1() / loMPICom.size();
    std::size_t protonum 		= numproto / loMPICom.size();
    std::size_t add      		= 0;
    
    if (loMPICom.rank() == loMPICom.size()-1) {
        protonum += numproto % loMPICom.size();
        add       = data.size1() % loMPICom.size();
    }
    
    ublas::matrix_range< ublas::matrix<double> > datarange(data, ublas::range(loMPICom.rank()*nums, (loMPICom.rank()+1)*nums + add), ublas::range(0, data.size2()));
    
    
    nsl::relationalneuralgas<double> ng(d, protonum, data.size2());
    ng.setLogging(true);
    mpi::timer t;
    ng.train(loMPICom, datarange, ngit);
    if (loMPICom.rank() == 0)
        std::cout << "number of process: " << loMPICom.size() << " Time: " << t.elapsed() << std::endl;
    #else    
    nsl::relationalneuralgas<double> ng(d, numproto, data.size2());
    ng.setLogging(true);
    ng.train(data, ngit);
    #endif
    
    
    
    #ifdef CLUSTER
	ublas::matrix<double> proto                   = tl::matrix::setNumericalZero(ng.getPrototypes(loMPICom));
    ublas::vector<double> qerror                  = tl::vector::copy(ng.getLoggedQuantizationError(loMPICom));
    std::vector< ublas::matrix<double> > logproto = ng.getLoggedPrototypes(loMPICom);
    
    if (loMPICom.rank() == 0) {
        tl::files::hdf f("rng.hdf5", true);
        f.write<double>( "/numprotos",  numproto, H5::PredType::NATIVE_DOUBLE );
        f.write<double>( "/protos",  proto, H5::PredType::NATIVE_DOUBLE );
        f.write<std::size_t>( "/iteration",  ngit, H5::PredType::NATIVE_ULONG );
        
        if (ng.getLogging()) {
            f.write<double>( "/error",  qerror, H5::PredType::NATIVE_DOUBLE );
            
            for(std::size_t i=0; i < logproto.size(); ++i)
                f.write<double>("/log" + boost::lexical_cast<std::string>( i ), tl::matrix::setNumericalZero(logproto[i]), H5::PredType::NATIVE_DOUBLE );
        }
    }
    
    #else
    tl::files::hdf f("rng.hdf5", true);
    f.write<double>( "/numprotos",  numproto, H5::PredType::NATIVE_DOUBLE );
    f.write<double>( "/protos",  tl::matrix::setNumericalZero(ng.getPrototypes()), H5::PredType::NATIVE_DOUBLE );    
    f.write<std::size_t>( "/iteration",  ngit, H5::PredType::NATIVE_ULONG );
    
    if (ng.getLogging()) {
        f.write<double>( "/error",  tl::vector::copy(ng.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
        
        
        std::vector< ublas::matrix<double> > logproto = ng.getLoggedPrototypes();
        for(std::size_t i=0; i < logproto.size(); ++i)
            f.write<double>("/log" + boost::lexical_cast<std::string>( i ), tl::matrix::setNumericalZero(logproto[i]), H5::PredType::NATIVE_DOUBLE );
    }
    #endif    
    */
    
   
    // ===== NG =====    
    /*
    //ublas::matrix<double> data = o.readMatrix<double>("/ngdata", H5::PredType::NATIVE_DOUBLE);
    //ublas::matrix<double> data = o.readMatrix<double>("/ngmini", H5::PredType::NATIVE_DOUBLE);
    ublas::matrix<double> data = o.readMatrix<double>("/ngbigdata", H5::PredType::NATIVE_DOUBLE);
    	
    
    dist::euclid<double> d;
    
    const std::size_t ngit      = 25;
    const std::size_t numproto  = 22;

    
    #ifdef CLUSTER
    // we must create a shuffle vector for disjoint datasets and shuffle the matrix - it's not nice, but it works
    ublas::vector<std::size_t> disjoint = static_cast< ublas::vector<std::size_t> >( tl::vector::random<double>(data.size1(), tl::random::uniform, 0, data.size1()) );
    mpi::broadcast(loMPICom, disjoint, 0);
    for(std::size_t i=0; i < disjoint.size(); ++i) {
        ublas::vector<double> x         = ublas::row(data, i);
        ublas::row(data, i)             = ublas::row(data, disjoint(i));
        ublas::row(data, disjoint(i))   = x;
    }
    
    // extract the data for the process (every process has load the whole data and shuffel them with the broadcasted shuffle vector)
    std::size_t nums     		= data.size1() / loMPICom.size();
    std::size_t protonum 		= numproto / loMPICom.size();
    std::size_t add      		= 0;
    
    if (loMPICom.rank() == loMPICom.size()-1) {
        protonum += numproto % loMPICom.size();
        add       = data.size1() % loMPICom.size();
    }

    ublas::matrix_range< ublas::matrix<double> > datarange(data, ublas::range(loMPICom.rank()*nums, (loMPICom.rank()+1)*nums + add), ublas::range(0, data.size2()));
	    
    nsl::neuralgas<double> ng(d, protonum, data.size2());
    ng.setLogging(true);
    mpi::timer t;
    ng.train(loMPICom, datarange, ngit);
    if (loMPICom.rank() == 0)
        std::cout << "number of process: " << loMPICom.size() << " Time: " << t.elapsed() << std::endl;
    
    
    #else    
    nsl::neuralgas<double> ng(d, numproto, data.size2());
    ng.setLogging(true);
    ng.train(data, ngit);
    #endif
    
    
    
    #ifdef CLUSTER
	ublas::matrix<double> proto                   = tl::matrix::setNumericalZero(ng.getPrototypes(loMPICom));
    ublas::vector<double> qerror                  = tl::vector::copy(ng.getLoggedQuantizationError(loMPICom));
    std::vector< ublas::matrix<double> > logproto = ng.getLoggedPrototypes(loMPICom);
    
    if (loMPICom.rank() == 0) {
        tl::files::hdf f("ng.hdf5", true);
        f.write<double>( "/numprotos",  numproto, H5::PredType::NATIVE_DOUBLE );
        f.write<double>( "/protos",  proto, H5::PredType::NATIVE_DOUBLE );
        f.write<std::size_t>( "/iteration",  ngit, H5::PredType::NATIVE_ULONG );
    
        if (ng.getLogging()) {
            f.write<double>( "/error",  qerror, H5::PredType::NATIVE_DOUBLE );
            
            for(std::size_t i=0; i < logproto.size(); ++i)
                f.write<double>("/log" + boost::lexical_cast<std::string>( i ), tl::matrix::setNumericalZero(logproto[i]), H5::PredType::NATIVE_DOUBLE );
        }
    }
    
    #else
    tl::files::hdf f("ng.hdf5", true);
    f.write<double>( "/numprotos",  numproto, H5::PredType::NATIVE_DOUBLE );
    f.write<double>( "/protos",  tl::matrix::setNumericalZero(ng.getPrototypes()), H5::PredType::NATIVE_DOUBLE );    
    f.write<std::size_t>( "/iteration",  ngit, H5::PredType::NATIVE_ULONG );
    
    if (ng.getLogging()) {
        f.write<double>( "/error",  tl::vector::copy(ng.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
        
        
        std::vector< ublas::matrix<double> > logproto = ng.getLoggedPrototypes();
        for(std::size_t i=0; i < logproto.size(); ++i)
            f.write<double>("/log" + boost::lexical_cast<std::string>( i ), tl::matrix::setNumericalZero(logproto[i]), H5::PredType::NATIVE_DOUBLE );
    }
    #endif
    */
    // ===== RLVQ ======
    /*
    ublas::matrix<double> data          = o.readMatrix<double>("/rlvqdata", H5::PredType::NATIVE_DOUBLE);
    ublas::vector<unsigned int> label   = o.readVector<unsigned int>("/rlvqlabel", H5::PredType::NATIVE_UINT);
    std::vector<unsigned int> datalabel = tl::vector::copy<unsigned int>( label );   
    
    std::vector<unsigned int> protolabel;
    for (std::size_t i=1; i <= 10; ++i)
        protolabel.push_back(i);
   
    
    dist::euclid<double> d;
    sl::rlvq<double, unsigned int> vq(d, protolabel, data.size2());
    vq.setLogging(true);
    vq.train(data, datalabel, 10, 0.001, 0.0001);
    
    tl::files::hdf f("rlvq.hdf5", true);
    f.write<double>( "/protos",  vq.getPrototypes(), H5::PredType::NATIVE_DOUBLE );
    
    if (vq.getLogging()) {
        f.write<double>( "/error",  tl::vector::copy(vq.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
        std::vector< ublas::matrix<double> > p = vq.getLoggedPrototypes();
        for(std::size_t i=0; i < p.size(); ++i)
            f.write<double>("/log" + boost::lexical_cast<std::string>( i ), p[i], H5::PredType::NATIVE_DOUBLE );
    }
    */
     
    //=============== NCD ================
    /*
    std::vector< std:: string > val;
    val.push_back("main.cpp");
    val.push_back("tools/files/hdf.hpp");
    val.push_back("distances/ncd.hpp");
    val.push_back("clustering/supervised/rlvq.hpp");
    
    val.push_back("tools/lapack.hpp");
    val.push_back("tools/random.hpp");
    val.push_back("tools/matrix.hpp");
    val.push_back("tools/vector.hpp");
    //val.push_back("blub.hdf5");
    
    dist::ncd<double> ncd(dist::ncd<double>::bzip2);
    //ncd.setCompressionLevel( dist::ncd::bestspeed );
    //std::cout << "unsymmetric: " << ncd.unsymmetric(val, true) << std::endl;
    std::cout << "\nsymmetric: " << ncd.symmetric(val, true) << std::endl;
    */
    
    return EXIT_SUCCESS;
}
