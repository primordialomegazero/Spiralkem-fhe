#include "spiralkem.h"
#include <openssl/ec.h>
#include <openssl/bn.h>
#include <openssl/obj_mac.h>
#include <openssl/sha.h>
#include <string.h>
#include <stdlib.h>

// True Schnorr Σ-Protocol on secp256k1: s·G == R + c·Y

int spiralkem_schnorr_keygen(unsigned char *pk, unsigned char *sk) {
    EC_KEY *ec = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (!ec) return -1;
    EC_KEY_generate_key(ec);
    
    const BIGNUM *priv = EC_KEY_get0_private_key(ec);
    const EC_POINT *pub = EC_KEY_get0_public_key(ec);
    const EC_GROUP *g = EC_KEY_get0_group(ec);
    
    BN_bn2binpad(priv, sk, 32);
    EC_POINT_point2oct(g, pub, POINT_CONVERSION_COMPRESSED, pk, 33, NULL);
    
    EC_KEY_free(ec);
    return 0;
}

int spiralkem_schnorr_sign(unsigned char *sig, size_t *siglen,
                            const unsigned char *msg, size_t msg_len,
                            const unsigned char *sk) {
    EC_GROUP *g = EC_GROUP_new_by_curve_name(NID_secp256k1);
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *order = BN_new();
    EC_GROUP_get_order(g, order, ctx);
    
    BIGNUM *priv = BN_new();
    BN_bin2bn(sk, 32, priv);
    
    BIGNUM *k = BN_new();
    BN_rand_range(k, order);
    
    EC_POINT *R = EC_POINT_new(g);
    EC_POINT_mul(g, R, k, NULL, NULL, ctx);
    EC_POINT_point2oct(g, R, POINT_CONVERSION_COMPRESSED, sig, 33, ctx);
    
    EC_POINT *Y = EC_POINT_new(g);
    EC_POINT_mul(g, Y, priv, NULL, NULL, ctx);
    unsigned char Y_bytes[33];
    EC_POINT_point2oct(g, Y, POINT_CONVERSION_COMPRESSED, Y_bytes, 33, ctx);
    
    SHA256_CTX sha;
    SHA256_Init(&sha);
    SHA256_Update(&sha, sig, 33);
    SHA256_Update(&sha, Y_bytes, 33);
    SHA256_Update(&sha, msg, msg_len);
    unsigned char c_hash[32];
    SHA256_Final(c_hash, &sha);
    
    BIGNUM *c = BN_new();
    BN_bin2bn(c_hash, 32, c);
    BN_mod(c, c, order, ctx);
    
    BIGNUM *s = BN_new();
    BIGNUM *cx = BN_new();
    BN_mod_mul(cx, c, priv, order, ctx);
    BN_mod_add(s, k, cx, order, ctx);
    BN_bn2binpad(s, sig + 33, 32);
    *siglen = 65;
    
    BN_free(priv); BN_free(k); BN_free(c); BN_free(s); BN_free(cx);
    BN_free(order); EC_POINT_free(R); EC_POINT_free(Y);
    BN_CTX_free(ctx); EC_GROUP_free(g);
    return 0;
}

int spiralkem_schnorr_verify(const unsigned char *sig, size_t siglen,
                              const unsigned char *msg, size_t msg_len,
                              const unsigned char *pk) {
    if (siglen < 65) return -1;
    
    EC_GROUP *g = EC_GROUP_new_by_curve_name(NID_secp256k1);
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *order = BN_new();
    EC_GROUP_get_order(g, order, ctx);
    
    EC_POINT *R = EC_POINT_new(g);
    EC_POINT_oct2point(g, R, sig, 33, ctx);
    
    BIGNUM *s = BN_new();
    BN_bin2bn(sig + 33, 32, s);
    
    EC_POINT *Y = EC_POINT_new(g);
    EC_POINT_oct2point(g, Y, pk, 33, ctx);
    
    SHA256_CTX sha;
    SHA256_Init(&sha);
    SHA256_Update(&sha, sig, 33);
    SHA256_Update(&sha, pk, 33);
    SHA256_Update(&sha, msg, msg_len);
    unsigned char c_hash[32];
    SHA256_Final(c_hash, &sha);
    
    BIGNUM *c = BN_new();
    BN_bin2bn(c_hash, 32, c);
    BN_mod(c, c, order, ctx);
    
    EC_POINT *sG = EC_POINT_new(g);
    EC_POINT_mul(g, sG, s, NULL, NULL, ctx);
    
    EC_POINT *cY = EC_POINT_new(g);
    EC_POINT_mul(g, cY, NULL, Y, c, ctx);
    
    EC_POINT *RcY = EC_POINT_new(g);
    EC_POINT_add(g, RcY, R, cY, ctx);
    
    int result = (EC_POINT_cmp(g, sG, RcY, ctx) == 0) ? 0 : -1;
    
    BN_free(s); BN_free(c); BN_free(order);
    EC_POINT_free(R); EC_POINT_free(Y); EC_POINT_free(sG);
    EC_POINT_free(cY); EC_POINT_free(RcY);
    BN_CTX_free(ctx); EC_GROUP_free(g);
    return result;
}
