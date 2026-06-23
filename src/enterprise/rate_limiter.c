#include "rate_limiter.h"
#include <string.h>

void ratelimit_init(RateLimiter *rl, size_t max_ops_per_sec, size_t burst) {
    memset(rl, 0, sizeof(*rl));
    rl->max_ops_per_second = max_ops_per_sec;
    rl->burst_size = burst;
    rl->window_start = time(NULL);
}

int ratelimit_check(RateLimiter *rl) {
    time_t now = time(NULL);
    if (now > rl->window_start) {
        rl->current_ops = 0;
        rl->window_start = now;
    }
    if (rl->current_ops < rl->burst_size) {
        rl->current_ops++; rl->total_ops++; rl->last_op_time = now;
        return 1;
    }
    if (rl->current_ops < rl->max_ops_per_second) {
        rl->current_ops++; rl->total_ops++; rl->last_op_time = now;
        return 1;
    }
    rl->rejected_ops++;
    return 0;
}

void ratelimit_reset(RateLimiter *rl) { rl->current_ops = 0; rl->window_start = time(NULL); }
size_t ratelimit_available(RateLimiter *rl) { return rl->max_ops_per_second - rl->current_ops; }
double ratelimit_usage_percent(RateLimiter *rl) {
    if (rl->max_ops_per_second == 0) return 100.0;
    return (double)rl->current_ops / (double)rl->max_ops_per_second * 100.0;
}
