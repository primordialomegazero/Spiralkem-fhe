#ifndef SPIRALKEM_H
#define SPIRALKEM_H

#include <stddef.h>
#include <stdint.h>

/*
 * SPIRALKEM-FHE ENTERPRISE
 * 
 * Post-Quantum KEM + Multi-Scheme FHE + Recursive Fractal Trees
 *
 * KEM: ML-KEM-1024 (NIST FIPS 203)
 * FHE: BFV | CKKS | TFHE — All Available
 * Fractal: Multi-Recursive Self-Healing Signature Trees
 *
 * Dan Joseph M. Fernandez / ΦΩ0
 * IACR: 2026/110189
 */

// KEM
#define KEM_PUBLIC_KEY_BYTES  1568
#define KEM_SECRET_KEY_BYTES  3168
#define KEM_CIPHERTEXT_BYTES  1568
#define KEM_SHARED_SECRET_BYTES 32

// FHE Schemes
typedef enum {
    FHE_BFV,
    FHE_CKKS,
    FHE_TFHE,
    FHE_HYBRID
} FHEScheme;

// Fractal
#define FRACTAL_MAX_DEPTH 7
#define FRACTAL_MAX_BRANCHES 5

typedef struct FractalNode {
    unsigned char signature[65];
    size_t depth;
    size_t branch_id;
    double phi_weight;
    int self_healed;
    struct FractalNode *children[5];
    size_t child_count;
} FractalNode;

#ifdef __cplusplus
extern "C" {
#endif

// KEM API
int spiralkem_keygen(unsigned char *pk, unsigned char *sk);
int spiralkem_encaps(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int spiralkem_decaps(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

// FHE API
int spiralkem_fhe_encrypt(unsigned char *ct, size_t *ct_len,
                           const unsigned char *pt, size_t pt_len,
                           FHEScheme scheme);
int spiralkem_fhe_decrypt(unsigned char *pt, size_t *pt_len,
                           const unsigned char *ct, size_t ct_len,
                           FHEScheme scheme);
int spiralkem_fhe_add(unsigned char *result,
                       const unsigned char *a, const unsigned char *b,
                       FHEScheme scheme);
int spiralkem_fhe_multiply(unsigned char *result,
                            const unsigned char *a, const unsigned char *b,
                            FHEScheme scheme);

// Fractal API
int spiralkem_fractal_sign(FractalNode *root, const unsigned char *msg, size_t msg_len,
                            const unsigned char *sk, size_t depth, size_t branches);
int spiralkem_fractal_verify(const FractalNode *root, const unsigned char *msg, size_t msg_len,
                              const unsigned char *pk);
int spiralkem_fractal_self_heal(FractalNode *node, const unsigned char *pk, const unsigned char *sk);
void spiralkem_fractal_free(FractalNode *node);

#ifdef __cplusplus
}
#endif

#endif
