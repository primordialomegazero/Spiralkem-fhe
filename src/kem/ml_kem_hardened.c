#include "spiralkem.h"
#include "error_handler.h"
#include "rate_limiter.h"
#include <oqs/oqs.h>
#include <string.h>

static RateLimiter kem_limiter;
static int limiter_init = 0;

SpiralkemError spiralkem_keygen_hardened(unsigned char *pk, unsigned char *sk) {
    SPIRALKEM_CHECK_NULL(pk);
    SPIRALKEM_CHECK_NULL(sk);
    
    if (!limiter_init) {
        ratelimit_init(&kem_limiter, 1000, 100);  // 1000 ops/sec, 100 burst
        limiter_init = 1;
    }
    
    if (!ratelimit_check(&kem_limiter)) {
        return ERR_RATE_LIMITED;
    }
    
    OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_ml_kem_1024);
    if (!kem) return ERR_KEM_KEYGEN_FAILED;
    
    if (OQS_KEM_keypair(kem, pk, sk) != OQS_SUCCESS) {
        OQS_KEM_free(kem);
        return ERR_KEM_KEYGEN_FAILED;
    }
    
    OQS_KEM_free(kem);
    return ERR_NONE;
}

SpiralkemError spiralkem_encaps_hardened(unsigned char *ct, unsigned char *ss, 
                                          const unsigned char *pk) {
    SPIRALKEM_CHECK_NULL(ct);
    SPIRALKEM_CHECK_NULL(ss);
    SPIRALKEM_CHECK_NULL(pk);
    
    if (!ratelimit_check(&kem_limiter)) return ERR_RATE_LIMITED;
    
    OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_ml_kem_1024);
    if (!kem) return ERR_KEM_ENCAPS_FAILED;
    
    if (OQS_KEM_encaps(kem, ct, ss, pk) != OQS_SUCCESS) {
        OQS_KEM_free(kem);
        return ERR_KEM_ENCAPS_FAILED;
    }
    
    OQS_KEM_free(kem);
    return ERR_NONE;
}

SpiralkemError spiralkem_decaps_hardened(unsigned char *ss, const unsigned char *ct,
                                          const unsigned char *sk) {
    SPIRALKEM_CHECK_NULL(ss);
    SPIRALKEM_CHECK_NULL(ct);
    SPIRALKEM_CHECK_NULL(sk);
    
    if (!ratelimit_check(&kem_limiter)) return ERR_RATE_LIMITED;
    
    OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_ml_kem_1024);
    if (!kem) return ERR_KEM_DECAPS_FAILED;
    
    if (OQS_KEM_decaps(kem, ss, ct, sk) != OQS_SUCCESS) {
        OQS_KEM_free(kem);
        return ERR_KEM_DECAPS_FAILED;
    }
    
    OQS_KEM_free(kem);
    return ERR_NONE;
}
