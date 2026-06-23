#include "spiralkem.h"
#include <oqs/oqs.h>
#include <string.h>

int spiralkem_keygen(unsigned char *pk, unsigned char *sk) {
    OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_ml_kem_1024);
    if (!kem) return -1;
    int ret = OQS_KEM_keypair(kem, pk, sk);
    OQS_KEM_free(kem);
    return (ret == OQS_SUCCESS) ? 0 : -1;
}

int spiralkem_encaps(unsigned char *ct, unsigned char *ss, const unsigned char *pk) {
    OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_ml_kem_1024);
    if (!kem) return -1;
    int ret = OQS_KEM_encaps(kem, ct, ss, pk);
    OQS_KEM_free(kem);
    return (ret == OQS_SUCCESS) ? 0 : -1;
}

int spiralkem_decaps(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
    OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_ml_kem_1024);
    if (!kem) return -1;
    int ret = OQS_KEM_decaps(kem, ss, ct, sk);
    OQS_KEM_free(kem);
    return (ret == OQS_SUCCESS) ? 0 : -1;
}
