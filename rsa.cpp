#include "shifres.hpp"
#include <boost/integer/mod_inverse.hpp>    // mod_inverse
#include <boost/integer/common_factor_rt.hpp>  // gcd

using namespace boost::multiprecision;
using namespace boost::random;
/*
boost::random::mt11213b gen;
bool utilities::ferma(const uint1024_t &num) { // O(logN)
    boost::random::uniform_int_distribution<uint1024_t> rand(1, std::numeric_limits<uint1024_t>::max());
    if(num == 2)
		return true;
	for(int i=0;i<100;i++){
		uint1024_t a = (rand(utilities::gen) % (num - 2)) + 2;
		if (boost::integer::gcd_evaluator<uint1024_t>()(a, num) != 1)
			return false;
		if( utilities::pows(a, num-1, num) != 1)
			return false;
	}
	return true;
}
*/
typedef number<cpp_int_backend<2048, 2048, unsigned_magnitude, unchecked, void> > uint2048_t;
uint1024_t pows(const uint1024_t &g, const uint1024_t &x, const uint1024_t &p) {
    uint2048_t r = 1;
    uint1024_t xx = x;
    while (xx--) {

    }
    return static_cast<uint1024_t>(r);
}

RSA::RSA() {
    //boost::random::uniform_int_distribution<uint1024_t> dist(1, std::numeric_limits<uint1024_t>::max());
    cout << "pows: " << pows(2, std::numeric_limits<uint32_t>::max(), 30) << endl;
}

inline void RSA::decode(int &byte) {

}

inline void RSA::encode(int &byte) {

}

void RSA::recipient_protocol() {

}

void RSA::dispatcher_protocol() {

}

void RSA::takeSharedKey() {

}

void RSA::giveSharedKey() {

}

