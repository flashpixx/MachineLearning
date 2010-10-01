#include "machinelearning.h"

#include <iostream>
#include <algorithm>
#include <fstream>

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
    namespace nn        = machinelearning::neuronalnetwork;

    
    namespace linalg    = boost::numeric::bindings::lapack;
    namespace ublas     = boost::numeric::ublas;
    
    #ifdef MULTILANGUAGE
    tl::language::bind("ml", "./tools/language/");
    #endif
    
    //nn::nnet<double> net(3);
    
    
    
    
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
    
        
    tl::files::hdf o("blub.hdf5");
    //std::cout << o.readString("/string") << std::endl;
	//std::vector<std::string> x = o.readStringVector("/stringarray");
    //ublas::vector<double> x = o.readVector<double>("/vector", H5::PredType::NATIVE_DOUBLE);
    //std::cout << x << std::endl;
    
    
    // ===== Lazy Learner ======
    /*
    ublas::matrix<double> data       = o.readMatrix<double>("/iris/data", H5::PredType::NATIVE_DOUBLE); 
    ublas::vector<std::size_t> label = o.readVector<std::size_t>("/iris/label", H5::PredType::NATIVE_ULONG); 

    
    ublas::indirect_array<> x(10);
    for(std::size_t i=0; i < x.size(); ++i)
        x(i) = static_cast<std::size_t>(tl::random::get<double>( tl::random::uniform, 0, data.size1()));

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
    
   
    // ===== NG =====     
    ublas::matrix<double> data = o.readMatrix<double>("/ngdata", H5::PredType::NATIVE_DOUBLE);
     
    dist::euclid<double> d;
    #ifdef CLUSTER
    
    // we must create a shuffle vector for disjoint datasets and shuffle the matrix - it's not nice, but it works
    ublas::vector<std::size_t> disjoint = static_cast< ublas::vector<std::size_t> >( tl::vector::random<double>(data.size1(), tl::random::uniform, 0, data.size1()) );
    mpi::broadcast(loMPICom, disjoint, 0);
    for(std::size_t i=0; i < disjoint.size(); ++i) {
        ublas::vector<double> x              = ublas::row(data, i);
        ublas::row(data, i)             = ublas::row(data, disjoint(i));
        ublas::row(data, disjoint(i))   = x;
    }
    
    // extract the data for the process (every process has load the whole data and shuffel them with the broadcasted shuffle vector)
    std::size_t nums     = data.size1() / loMPICom.size();
    std::size_t protonum = 11 / loMPICom.size();
    
    if (loMPICom.rank() == loMPICom.size()-1)
        protonum += 11 % loMPICom.size();
    
    
    ublas::matrix_range< ublas::matrix<double> > datarange(data, ublas::range(loMPICom.rank()*nums, (loMPICom.rank()+1)*nums), ublas::range(0, data.size2()));
    std::cout << datarange.size1() << " " << datarange.size2() << std::endl;
                                                                                     
    return 0;
    
    nsl::neuralgas<double> ng(d, protonum, data.size2());
    //seperates the data for every process (disjoint sets)
    //std::size_t numpoints = data.size1() / loMPICom.size();
    //for(std::size_t i=loMPICom.rank()*numpoints; (i <  (loMPICom.rank()+1)*numpoints) && (i < data.size1()); ++i)
    //ng.train(loMPICom, datarange, 15);
    #else    
    nsl::neuralgas<double> ng(d, 11, data.size2());
    ng.train(data, 15);
    #endif
    
    ng.setLogging(true);
    
    
    #ifndef CLUSTER
    tl::files::hdf f("ng.hdf5", true);
    f.write<double>( "/protos",  ng.getPrototypes(), H5::PredType::NATIVE_DOUBLE );
    
    if (ng.getLogging()) {
        f.write<double>( "/error",  tl::vector::copy(ng.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
        std::vector< ublas::matrix<double> > p = ng.getLoggedPrototypes();
        for(std::size_t i=0; i < p.size(); ++i)
            f.write<double>("/log" + boost::lexical_cast<std::string>( i ), p[i], H5::PredType::NATIVE_DOUBLE );
    }
    #else
    std::cout << ng.getPrototypes(loMPICom) << std::endl;
    #endif
    
    
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
    }*/
    
     
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
