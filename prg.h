#include "openssl-aes.h"
#include<openssl/aes.h>
#include<openssl/evp.h>
#include<openssl/rand.h>
#include <cpuid.h>

//To use pseudo random generator, make an object of this class which will automatically set the AES Keys and then we can generate random bits.
//prf(prev code) = generate_random_numeber(this code)
class PseudoRandomGenerator {
    public:
        PseudoRandomGenerator();

        void generate_random_number(unsigned char* out,
                            unsigned char* key, uint64_t output_size,
                            uint32_t num_keys);
        int prf_len = 4;
    private:
        
        AES_KEY* _aes_keys;

};