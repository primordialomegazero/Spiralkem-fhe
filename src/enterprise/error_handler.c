#include "error_handler.h"
#include <string.h>

const char* spiralkem_strerror(SpiralkemError err) {
    switch (err) {
        case ERR_NONE: return "Success";
        case ERR_KEM_KEYGEN_FAILED: return "KEM key generation failed";
        case ERR_KEM_ENCAPS_FAILED: return "KEM encapsulation failed";
        case ERR_KEM_DECAPS_FAILED: return "KEM decapsulation failed";
        case ERR_FHE_ENCRYPT_FAILED: return "FHE encryption failed";
        case ERR_FHE_DECRYPT_FAILED: return "FHE decryption failed";
        case ERR_FHE_COMPUTE_FAILED: return "FHE computation failed";
        case ERR_FRACTAL_SIGN_FAILED: return "Fractal signing failed";
        case ERR_FRACTAL_VERIFY_FAILED: return "Fractal verification failed";
        case ERR_FRACTAL_HEAL_FAILED: return "Fractal self-healing failed";
        case ERR_NULL_PARAM: return "Null parameter";
        case ERR_BUFFER_TOO_SMALL: return "Buffer too small";
        case ERR_RATE_LIMITED: return "Rate limit exceeded";
        case ERR_TIMEOUT: return "Operation timeout";
        case ERR_INTERNAL: return "Internal error";
        default: return "Unknown error";
    }
}

int spiralkem_error_recoverable(SpiralkemError err) {
    return (err >= -500 && err <= -1) ? 1 : 0;
}
