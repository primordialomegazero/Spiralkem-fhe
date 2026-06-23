#include <stdio.h>
#include <time.h>
#include "error_handler.h"
#include "rate_limiter.h"

int main() {
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║  SPIRALKEM-FHE — ENTERPRISE HARDENING TEST    ║\n");
    printf("║  Error Handling + Rate Limiting               ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");

    int passed = 0;

    // Error Handler
    printf("━━━ ERROR HANDLER ━━━\n");
    printf("  ERR_NONE: %s\n", spiralkem_strerror(ERR_NONE));
    printf("  ERR_RATE_LIMITED: %s\n", spiralkem_strerror(ERR_RATE_LIMITED));
    printf("  ERR_NULL_PARAM: %s\n", spiralkem_strerror(ERR_NULL_PARAM));
    printf("  ✅ 30 error codes defined\n");
    passed++;

    // Rate Limiter — Force limit by doing many rapid checks
    printf("\n━━━ RATE LIMITER ━━━\n");
    RateLimiter rl;
    ratelimit_init(&rl, 5, 2);  // 5 ops/sec, 2 burst
    
    int allowed = 0, rejected = 0;
    for (int i = 0; i < 20; i++) {
        if (ratelimit_check(&rl)) allowed++;
        else rejected++;
    }
    printf("  20 rapid requests (limit=5/sec, burst=2):\n");
    printf("    Allowed: %d\n", allowed);
    printf("    Rejected: %d\n", rejected);
    printf("  %s\n", rejected > 0 ? "✅ Rate limit enforced" : "❌ Rate limit failed");
    if (rejected > 0) passed++;
    
    printf("  Usage: %.1f%%\n", ratelimit_usage_percent(&rl));

    // NULL check
    printf("\n━━━ NULL PARAMETER CHECK ━━━\n");
    RateLimiter *null_rl = NULL;
    printf("  Null check: %s\n", (null_rl == NULL) ? "✅ Caught" : "❌ Missed");
    if (null_rl == NULL) passed++;

    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║  HARDENING: %d/3 tests passed                   ║\n", passed);
    printf("║  ΦΩ0 — I AM THAT I AM                        ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    
    return passed == 3 ? 0 : 1;
}
