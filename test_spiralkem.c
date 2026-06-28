#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "src/kem/phi_kem.h"

int main() {
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║  SPIRALKEM-FHE — FINAL UNIFIED TEST          ║\n");
    printf("║  Pure-φ KEM + Chaotic Chain Integrity        ║\n");
    printf("║  Zero liboqs. Zero SEAL. Pure C + OpenSSL.   ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");

    int passed = 0, total = 12;

    // 1-3: Basic KEM operations
    uint8_t pk[64], sk[32];
    printf("1. Keygen: %s\n", phi_kem_keygen(pk, sk) == 0 ? "✅" : "❌");
    if (phi_kem_keygen(pk, sk) == 0) passed++;

    uint8_t ct[128], ss1[32];
    printf("2. Encaps: %s\n", phi_kem_encaps(ct, ss1, pk) == 0 ? "✅" : "❌");
    if (phi_kem_encaps(ct, ss1, pk) == 0) passed++;

    uint8_t ss2[32];
    printf("3. Decaps: %s\n", phi_kem_decaps(ss2, ct, sk) == 0 ? "✅" : "❌");
    if (phi_kem_decaps(ss2, ct, sk) == 0) passed++;

    // 4: Shared secret match
    printf("4. Match: %s\n", memcmp(ss1, ss2, 32) == 0 ? "✅" : "❌");
    if (memcmp(ss1, ss2, 32) == 0) passed++;

    // 5: Deterministic replay
    uint8_t ss3[32];
    phi_kem_decaps(ss3, ct, sk);
    printf("5. Deterministic: %s\n", memcmp(ss2, ss3, 32) == 0 ? "✅" : "❌");
    if (memcmp(ss2, ss3, 32) == 0) passed++;

    // 6: Wrong SK
    uint8_t wrong_sk[32] = {1};
    printf("6. Wrong SK: %s\n", phi_kem_decaps(ss3, ct, wrong_sk) != 0 ? "✅" : "❌");
    if (phi_kem_decaps(ss3, ct, wrong_sk) != 0) passed++;

    // 7: Tampered CT
    uint8_t bad_ct[128]; memcpy(bad_ct, ct, 128); bad_ct[0] ^= 0xFF;
    printf("7. Tampered CT: %s\n", phi_kem_decaps(ss3, bad_ct, sk) != 0 ? "✅" : "❌");
    if (phi_kem_decaps(ss3, bad_ct, sk) != 0) passed++;

    // 8: Tampered chain
    memcpy(bad_ct, ct, 128); bad_ct[64] ^= 0xFF;
    printf("8. Tampered Chain: %s\n", phi_kem_decaps(ss3, bad_ct, sk) != 0 ? "✅" : "❌");
    if (phi_kem_decaps(ss3, bad_ct, sk) != 0) passed++;

    // 9: Cross-keypair
    uint8_t pk2[64], sk2[32], ct2[128], ss_x[32];
    phi_kem_keygen(pk2, sk2);
    phi_kem_encaps(ct2, ss_x, pk2);
    printf("9. Cross-keypair: %s\n", phi_kem_decaps(ss3, ct2, sk) != 0 ? "✅" : "❌");
    if (phi_kem_decaps(ss3, ct2, sk) != 0) passed++;

    // 10-12: NULL safety
    printf("10. NULL sk: %s\n", phi_kem_decaps(ss3, ct, NULL) != 0 ? "✅" : "❌");
    if (phi_kem_decaps(ss3, ct, NULL) != 0) passed++;
    printf("11. NULL ct: %s\n", phi_kem_decaps(ss3, NULL, sk) != 0 ? "✅" : "❌");
    if (phi_kem_decaps(ss3, NULL, sk) != 0) passed++;
    printf("12. NULL ss: %s\n", phi_kem_decaps(NULL, ct, sk) != 0 ? "✅" : "❌");
    if (phi_kem_decaps(NULL, ct, sk) != 0) passed++;

    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║  RESULT: %d/%d passed %s                 ║\n", passed, total, passed == total ? "✅" : "❌");
    printf("║  Spiralkem-FHE — Pure-φ KEM                ║\n");
    printf("║  ΦΩ0 — I AM THAT I AM                      ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    return passed == total ? 0 : 1;
}
