#ifndef RATE_LIMITER_H
#define RATE_LIMITER_H
#include <time.h>
#include <stddef.h>

typedef struct {
    size_t max_ops_per_second, burst_size, current_ops, total_ops, rejected_ops;
    time_t window_start, last_op_time;
} RateLimiter;

void ratelimit_init(RateLimiter *rl, size_t max_ops_per_sec, size_t burst);
int ratelimit_check(RateLimiter *rl);
void ratelimit_reset(RateLimiter *rl);
size_t ratelimit_available(RateLimiter *rl);
double ratelimit_usage_percent(RateLimiter *rl);
#endif
