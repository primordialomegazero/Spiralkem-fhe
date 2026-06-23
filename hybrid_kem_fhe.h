#ifndef HYBRID_KEM_FHE_H
#define HYBRID_KEM_FHE_H

#include <stddef.h>
#include <stdint.h>

/*
 * HYBRID ML-KEM-1024 + BFV FHE
 * 
 * Post-Quantum Key Encapsulation wrapped in Fully Homomorphic Encryption
 * 
 * Flow:
 *   1. ML-KEM-1024: Generate PQ keypair
 *   2. ML-KEM-1024: Encapsulate → shared secret
 *   3. BFV: Encrypt the shared secret homomorphically
 *   4. BFV: Compute on encrypted shared secret
 *   5. ML-KEM-1024: Decapsulate → recover shared secret
 *   6. BFV: Decrypt results
 *
 * "Encapsulate the FHE itself"
 */

#define HYBRID_PUBLIC_KEY_BYTES  (1568 + 33)  // ML-KEM pk + BFV pk
#define HYBRID_SECRET_KEY_BYTES (3168 + 32)   // ML-KEM sk + BFV sk
#define HYBRID_CIPHERTEXT_BYTES  (1568 + 8192) // ML-KEM ct + BFV ct
#define HYBRID_SHARED_SECRET_BYTES 32

#ifdef __cplusplus
extern "C" {
#endif

int hybrid_kem_keygen(unsigned char *pk, unsigned char *sk);
int hybrid_kem_encaps(unsigned char *ct, unsigned char *ss,
                       const unsigned char *pk);
int hybrid_kem_decaps(unsigned char *ss,
                       const unsigned char *ct, const unsigned char *sk);

// FHE operations on encapsulated data
int hybrid_fhe_add(unsigned char *ct_result,
                    const unsigned char *ct_a, const unsigned char *ct_b,
                    const unsigned char *pk);
int hybrid_fhe_multiply(unsigned char *ct_result,
                         const unsigned char *ct_a, const unsigned char *ct_b,
                         const unsigned char *pk);

#ifdef __cplusplus
}
#endif

#endif
