#include <seal/seal.h>
#include <string.h>
#include <sstream>

using namespace seal;

extern "C" {

static SEALContext* g_ctx = nullptr;
static Encryptor* g_enc = nullptr;
static Decryptor* g_dec = nullptr;
static Evaluator* g_eval = nullptr;
static BatchEncoder* g_encoder = nullptr;
static Ciphertext* g_enc_zero = nullptr;

int spiralkem_fhe_seal_init(void) {
    EncryptionParameters parms(scheme_type::bfv);
    parms.set_poly_modulus_degree(2048);
    parms.set_coeff_modulus(CoeffModulus::Create(2048, {60, 40, 40, 60}));
    parms.set_plain_modulus(PlainModulus::Batching(2048, 30));
    
    g_ctx = new SEALContext(parms, true, sec_level_type::none);
    KeyGenerator kg(*g_ctx);
    
    SecretKey sk = kg.secret_key();
    PublicKey pk;
    kg.create_public_key(pk);
    
    g_enc = new Encryptor(*g_ctx, pk);
    g_dec = new Decryptor(*g_ctx, sk);
    g_eval = new Evaluator(*g_ctx);
    g_encoder = new BatchEncoder(*g_ctx);
    
    std::vector<uint64_t> zeros(g_encoder->slot_count(), 0ULL);
    Plaintext pt;
    g_encoder->encode(zeros, pt);
    g_enc_zero = new Ciphertext();
    g_enc->encrypt(pt, *g_enc_zero);
    
    return 0;
}

int spiralkem_fhe_seal_encrypt(unsigned char *ct, size_t *ct_len,
                                const unsigned char *pt, size_t pt_len) {
    if (!g_ctx) spiralkem_fhe_seal_init();
    
    std::vector<uint64_t> vals(g_encoder->slot_count(), 0ULL);
    vals[0] = *(uint64_t*)pt;
    
    Plaintext plain;
    g_encoder->encode(vals, plain);
    
    Ciphertext encrypted;
    g_enc->encrypt(plain, encrypted);
    g_eval->add_inplace(encrypted, *g_enc_zero);
    
    std::stringstream ss;
    encrypted.save(ss);
    std::string s = ss.str();
    memcpy(ct, s.c_str(), s.size());
    *ct_len = s.size();
    
    return 0;
}

int spiralkem_fhe_seal_decrypt(unsigned char *pt, size_t *pt_len,
                                const unsigned char *ct, size_t ct_len) {
    if (!g_ctx) return -1;
    
    Ciphertext encrypted;
    std::stringstream ss(std::string((char*)ct, ct_len));
    encrypted.load(*g_ctx, ss);
    
    Plaintext plain;
    g_dec->decrypt(encrypted, plain);
    
    std::vector<uint64_t> vals;
    g_encoder->decode(plain, vals);
    *(uint64_t*)pt = vals[0];
    *pt_len = sizeof(uint64_t);
    
    return 0;
}

} // extern "C"
