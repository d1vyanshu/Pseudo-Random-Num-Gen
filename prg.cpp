#include "prg.h"

PseudoRandomGenerator :: PseudoRandomGenerator() {

#ifndef AESNI
    uint32_t eax, ebx, ecx, edx;

    eax = ebx = ecx = edx = 0;
    __get_cpuid(1, &eax, &ebx, &ecx, &edx);
#endif


    _aes_keys = (AES_KEY*) malloc(sizeof(AES_KEY)*prf_len);

    for(int i=0; i < prf_len; i++) {
        unsigned char rand_bytes[16];
        if(!RAND_bytes(rand_bytes, 16)) {
            printf("Random bytes failed.\n");
        }

#ifndef AESNI
        if ((ecx & bit_AES) > 0) {
            aesni_set_encrypt_key(rand_bytes, 128, &(_aes_keys[i]));
        } else {
            AES_set_encrypt_key(rand_bytes, 128, &(_aes_keys[i]));
        }
#else
        aesni_set_encrypt_key(rand_bytes, 128, &(aes_keys[i]));
#endif    

    }

}

void PseudoRandomGenerator::generate_random_number(unsigned char* out,
                            unsigned char* key, uint64_t output_size,
                            uint32_t num_keys) {
#ifndef AESNI
    // check if there is aes-ni instruction
    uint32_t eax, ebx, ecx, edx;

    eax = ebx = ecx = edx = 0;
    __get_cpuid(1, &eax, &ebx, &ecx, &edx);
#endif
    
    AES_KEY* temp_keys = _aes_keys;
    // Do Matyas–Meyer–Oseas one-way compression function using different AES keys to get desired
    // output length
    uint32_t required_num_keys = output_size/16;
    if (required_num_keys> prf_len) {
        free(temp_keys);
        temp_keys = (AES_KEY*) malloc(sizeof(AES_KEY)*required_num_keys); 
        for (int i = 0; i < required_num_keys; i++) {
            unsigned char rand_bytes[16];
            if (!RAND_bytes(rand_bytes, 16)) {
                printf("Random bytes failed.\n");
            }
#ifndef AESNI
            if ((ecx & bit_AES) > 0) {
                aesni_set_encrypt_key(rand_bytes, 128, &(temp_keys[i]));
            } else {
                AES_set_encrypt_key(rand_bytes, 128, &(temp_keys[i]));
            }
#else
            aesni_set_encrypt_key(rand_bytes, 128, &(temp_keys[i]));
#endif
        }
    }
    for (int i = 0; i < required_num_keys; i++) {
#ifndef AESNI
        if ((ecx & bit_AES) > 0) {
            aesni_encrypt(key, out + (i*16), &temp_keys[i]);
        } else {
            AES_encrypt(key, out + (i*16), &temp_keys[i]);
        }
#else
        aesni_encrypt(key, out + (i*16), &temp_keys[i]);
#endif
    }
    for (int i = 0; i < output_size; i++) {
        out[i] = out[i] ^ key[i%16];
    }
    _aes_keys = temp_keys;
}
