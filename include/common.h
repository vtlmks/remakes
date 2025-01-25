

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#if defined(__GNUC__) || defined(__clang__) || defined(__TINYC__)
#define ALIGNED(x) __attribute__((aligned(x)))
#elif defined(_MSC_VER)
#define ALIGNED(x) __declspec(align(x))
#else
#define ALIGNED(x) /* No alignment support */
#endif

#if defined(__GNUC__) || defined(__clang__)
#define ASSUME(condition) if (!(condition)) __builtin_unreachable()
#elif defined(_MSC_VER)
#define ASSUME(condition) __assume(condition)
#else
#define ASSUME(condition) ((void)0) /* Fallback: No-op */
#endif

