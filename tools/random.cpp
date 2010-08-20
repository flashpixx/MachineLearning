#include "random.hpp"

namespace machinelearning { namespace tools {

    #ifdef RANDOMDEVICE
        boost::random_device random::m_random;
    #else
        boost::mt19937 random::m_random(static_cast<std::time_t>(time(0)));
    #endif

};};