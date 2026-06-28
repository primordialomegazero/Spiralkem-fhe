#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "src/phi_kem.h"

int main() {
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║  SPIRALKEM-FHE — FINAL UNIFIED TEST          ║\n");
    printf("║  Pure-φ KEM + Chaotic Chain Integrity        ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");

    int passed = 0, total = 12;
    uint8_t pk[PHI_KEM_PUBLICKEYBYTES], sk[PHI_KEM_SECRETKEYBYTES];
    uint8_t ct[PHI_KEM_CIPHERTEXTBYTES], ss1[PHI_KEM_SHAREDSECRETBYTES];
    uint8_t ss2[PHI_KEM_SHAREDSECRETBYTES], ss3[PHI_KEM_SHAREDSECRETBYTES];

    printf("1. Keygen: %s\n", phi_kem_keygen(pk, sk) == 0 ? "✅" : "❌");
    if (phi_kem_keygen(pk, sk) == 0) passed++;

    printf("2. Encaps: %s\n", phi_kem_encaps(ct, ss1, pk) == 0 ? "✅" : "❌");
    if (phi_kem_encaps(ct, ss1, pk) == 0) passed++;

    printf("3. Decaps: %s\n", phi_kem_decaps(ss2, ct, PHI_KEM_CIPHERTEXTBYTES, sk) == 0 ? "✅" : "❌");
    if (phi_kem_decaps(ss2, ct, PHI_KEM_CIPHERTEXTBYTES, sk) == 0) passed++;

    printf("4. Match: %s\n", memcmp(ss1, ss2, 32) == 0 ? "✅" : "❌");
    if (memcmp(ss1, ss2, 32) == 0) passed++;

    phi_kem_decaps(ss3, ct, PHI_KEM_CIPHERTEXTBYTES, sk);
    printf("5. Deterministic: %s\n", memcmp(ss2, ss3, 32) == 0 ? "✅" : "❌");
    if (memcmp(ss2, ss3, 32) == 0) passed++;

    uint8_t wrong_sk[PHI_KEM_SECRETKEYBYTES] = {1};
    printf("6. Wrong SK: %s\n", phi_kem_decaps(ss3, ct, PHI_KEM_CIPHERTEXTBYTES, wrong_sk) != 0 ? "✅" : "❌");
    if (phi_kem_decaps(ss3, ct, PHI_KEM_CIPHERTEXTBYTES, wrong_sk) != 0) passed++;

    uint8_t bad_ct[PHI_KEM_CIPHERTEXTBYTES];
    memcpy(bad_ct, ct, PHI_KEM_CIPHERTEXTBYTES); bad_ct[0] ^= 0xFF;
    printf("7. Tampered CT: %s\n", phi_kem_decaps(ss3, bad_ct, PHI_KEM_CIPHERTEXTBYTES, sk) != 0 ? "✅" : "❌");
    if (phi_kem_decaps(ss3, bad_ct, PHI_KEM_CIPHERTEXTBYTES, sk) != 0) passed++;

    memcpy(bad_ct, ct, PHI_KEM_CIPHERTEXTBYTES); bad_ct[64] ^= 0xFF;
    printf("8. Tampered Chain: %s\n", phi_kem_decaps(ss3, bad_ct, PHI_KEM_CIPHERTEXTBYTES, sk) != 0 ? "✅" : "❌");
    if (phi_kem_decaps(ss3, bad_ct, PHI_KEM_CIPHERTEXTBYTES, sk) != 0) passed++;

    uint8_t pk2[PHI_KEM_PUBLICKEYBYTES], sk2[PHI_KEM_SECRETKEYBYTES];
    uint8_t ct2[PHI_KEM_CIPHERTEXTBYTES], ss_x[PHI_KEM_SHAREDSECRETBYTES];
    phi_kem_keygen(pk2, sk2);
    phi_kem_encaps(ct2, ss_x, pk2);
    printf("9. Cross-keypair: %s\n", phi_kem_decaps(ss3, ct2, PHI_KEM_CIPHERTEXTBYTES, sk) != 0 ? "✅" : "❌");
    if (phi_kem_decaps(ss3, ct2, PHI_KEM_CIPHERTEXTBYTES, sk) != 0) passed++;

    printf("10. NULL sk: %s\n", phi_kem_decaps(ss3, ct, PHI_KEM_CIPHERTEXTBYTES, NULL) != 0 ? "✅" : "❌");
    if (phi_kem_decaps(ss3, ct, PHI_KEM_CIPHERTEXTBYTES, NULL) != 0) passed++;
    printf("11. NULL ct: %s\n", phi_kem_decaps(ss3, NULL, PHI_KEM_CIPHERTEXTBYTES, sk) != 0 ? "✅" : "❌");
    if (phi_kem_decaps(ss3, NULL, PHI_KEM_CIPHERTEXTBYTES, sk) != 0) passed++;
    printf("12. Short CT: %s\n", phi_kem_decaps(ss3, ct, 64, sk) != 0 ? "✅" : "❌");
    if (phi_kem_decaps(ss3, ct, 64, sk) != 0) passed++;

    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║  RESULT: %d/%d passed %s                 ║\n", passed, total, passed == total ? "✅" : "❌");
    printf("║  Spiralkem-FHE — Pure-φ KEM                ║\n");
    printf("║  ΦΩ0 — I AM THAT I AM                      ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    return passed == total ? 0 : 1;
}
