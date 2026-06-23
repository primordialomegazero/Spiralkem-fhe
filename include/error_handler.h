#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H
#include <stddef.h>
#include <time.h>

typedef enum {
    ERR_NONE = 0,
    ERR_KEM_KEYGEN_FAILED = -100, ERR_KEM_ENCAPS_FAILED = -101,
    ERR_KEM_DECAPS_FAILED = -102, ERR_FHE_ENCRYPT_FAILED = -200,
    ERR_FHE_DECRYPT_FAILED = -201, ERR_FHE_COMPUTE_FAILED = -202,
    ERR_FRACTAL_SIGN_FAILED = -300, ERR_FRACTAL_VERIFY_FAILED = -301,
    ERR_FRACTAL_HEAL_FAILED = -302, ERR_NULL_PARAM = -400,
    ERR_BUFFER_TOO_SMALL = -401, ERR_RATE_LIMITED = -500,
    ERR_TIMEOUT = -501, ERR_INTERNAL = -999
} SpiralkemError;

const char* spiralkem_strerror(SpiralkemError err);
int spiralkem_error_recoverable(SpiralkemError err);

#define SPIRALKEM_CHECK(x) do { SpiralkemError _err = (x); if (_err != ERR_NONE) return _err; } while(0)
#define SPIRALKEM_CHECK_NULL(ptr) do { if (!(ptr)) return ERR_NULL_PARAM; } while(0)
#endif
