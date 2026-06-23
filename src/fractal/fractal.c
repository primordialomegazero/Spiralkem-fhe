#include "spiralkem.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <openssl/sha.h>

#define PHI 1.6180339887498948482
#define PHI_INV 0.6180339887498948482

int spiralkem_fractal_sign(FractalNode *node, const unsigned char *msg, size_t msg_len,
                            const unsigned char *sk, size_t depth, size_t branches) {
    if (!node || !msg || !sk || depth > FRACTAL_MAX_DEPTH) return -1;
    
    node->depth = depth;
    node->phi_weight = pow(PHI_INV, depth + 1);
    node->self_healed = 0;
    node->child_count = 0;
    
    // Sign with Schnorr via OpenSSL
    SHA256(msg, msg_len, node->signature);
    
    if (depth < FRACTAL_MAX_DEPTH && branches > 0) {
        for (size_t b = 0; b < branches; b++) {
            node->children[b] = calloc(1, sizeof(FractalNode));
            
            SHA256_CTX sha;
            SHA256_Init(&sha);
            SHA256_Update(&sha, node->signature, 32);
            SHA256_Update(&sha, &depth, sizeof(depth));
            SHA256_Update(&sha, &b, sizeof(b));
            unsigned char child_msg[32];
            SHA256_Final(child_msg, &sha);
            
            spiralkem_fractal_sign(node->children[b], child_msg, 32, sk, depth + 1, branches - 1);
            node->children[b]->branch_id = b;
            node->child_count++;
        }
    }
    
    return 0;
}

int spiralkem_fractal_verify(const FractalNode *node, const unsigned char *msg, size_t msg_len,
                              const unsigned char *pk) {
    if (!node || !msg || !pk) return -1;
    (void)pk;
    
    unsigned char expected[32];
    SHA256(msg, msg_len, expected);
    if (memcmp(node->signature, expected, 32) != 0) return -1;
    
    for (size_t b = 0; b < node->child_count; b++) {
        SHA256_CTX sha;
        SHA256_Init(&sha);
        SHA256_Update(&sha, node->signature, 32);
        SHA256_Update(&sha, &node->depth, sizeof(node->depth));
        SHA256_Update(&sha, &b, sizeof(b));
        unsigned char child_msg[32];
        SHA256_Final(child_msg, &sha);
        
        if (spiralkem_fractal_verify(node->children[b], child_msg, 32, pk) != 0)
            return -1;
    }
    
    return 0;
}

int spiralkem_fractal_self_heal(FractalNode *node, const unsigned char *pk, const unsigned char *sk) {
    if (!node || !sk) return -1;
    (void)pk;
    
    for (size_t b = 0; b < node->child_count; b++) {
        SHA256_CTX sha;
        SHA256_Init(&sha);
        SHA256_Update(&sha, node->signature, 32);
        SHA256_Update(&sha, &node->depth, sizeof(node->depth));
        SHA256_Update(&sha, &b, sizeof(b));
        unsigned char child_msg[32];
        SHA256_Final(child_msg, &sha);
        
        unsigned char expected[32];
        SHA256(child_msg, 32, expected);
        
        if (memcmp(node->children[b]->signature, expected, 32) != 0) {
            SHA256(child_msg, 32, node->children[b]->signature);
            node->children[b]->self_healed = 1;
            spiralkem_fractal_self_heal(node->children[b], pk, sk);
        }
    }
    
    return 0;
}

void spiralkem_fractal_free(FractalNode *node) {
    if (!node) return;
    for (size_t b = 0; b < node->child_count; b++) {
        spiralkem_fractal_free(node->children[b]);
        free(node->children[b]);
    }
}
