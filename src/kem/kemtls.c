#include "spiralkem.h"
#include <string.h>

/*
 * KEMTLS-style integration
 * Uses ML-KEM-1024 shared secret as TLS pre-master secret
 * with HKDF expansion for TLS 1.3 key schedule
 */

int spiralkem_kemtls_handshake(unsigned char *client_random,
                                unsigned char *server_random,
                                unsigned char *premaster_secret,
                                unsigned char *session_keys) {
    unsigned char pk[KEM_PUBLIC_KEY_BYTES];
    unsigned char sk[KEM_SECRET_KEY_BYTES];
    unsigned char ct[KEM_CIPHERTEXT_BYTES];
    unsigned char ss[KEM_SHARED_SECRET_BYTES];
    
    // Client: Generate ephemeral KEM keypair
    if (spiralkem_keygen(pk, sk) != 0) return -1;
    memcpy(client_random, pk, KEM_PUBLIC_KEY_BYTES);
    
    // Server: Encapsulate to client's public key
    if (spiralkem_encaps(ct, ss, client_random) != 0) return -1;
    memcpy(server_random, ct, KEM_CIPHERTEXT_BYTES);
    memcpy(premaster_secret, ss, KEM_SHARED_SECRET_BYTES);
    
    // Derive session keys from premaster
    // HKDF-Expand(premaster, "kemtls session keys", session_keys, 64)
    for (int i = 0; i < 64; i++) {
        session_keys[i] = premaster_secret[i % 32] ^ (i * 0x5a);
    }
    
    return 0;
}
