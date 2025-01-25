#include <cpuid.h>

// Bit position definitions for CPUID feature detection
#define CPUID_FEATURE_SSE3        0  // ECX bit 0  (EAX=1)
#define CPUID_FEATURE_SSSE3       9  // ECX bit 9  (EAX=1)
#define CPUID_FEATURE_SSE4_1     19  // ECX bit 19 (EAX=1)
#define CPUID_FEATURE_SSE4_2     20  // ECX bit 20 (EAX=1)
#define CPUID_FEATURE_AES        25  // ECX bit 25 (EAX=1)
#define CPUID_FEATURE_AVX        28  // ECX bit 28 (EAX=1)
#define CPUID_FEATURE_FMA        12  // ECX bit 12 (EAX=1)

#define CPUID_FEATURE_BMI1        3  // EBX bit 3  (EAX=7, ECX=0)
#define CPUID_FEATURE_BMI2        8  // EBX bit 8  (EAX=7, ECX=0)
#define CPUID_FEATURE_SHA        29  // EBX bit 29 (EAX=7, ECX=0)
#define CPUID_FEATURE_AVX2        5  // EBX bit 5  (EAX=7, ECX=0)
#define CPUID_FEATURE_AVX512F    16  // EBX bit 16 (EAX=7, ECX=0)

// Macro for feature detection
#define CHECK_FEATURE(reg, bit, feature) if((reg) & (1 << (bit))) { cpu_features |= (feature); }

__attribute__((target("avx")))
static int check_xgetbv_ymm() {
	unsigned long long xcr0 = _xgetbv(0);
	return (xcr0 & 0x6) == 0x6;
}

static void check_cpu_features() {
	unsigned eax = 0;
	unsigned ebx = 0;
	unsigned ecx = 0;
	unsigned edx = 0;

	uint32_t cpu_features = 0;

	// Basic CPUID Information
	__get_cpuid(1, &eax, &ebx, &ecx, &edx);
	CHECK_FEATURE(ecx, CPUID_FEATURE_SSE3, REMAKE_CPU_SSE3);				// SSE3
	CHECK_FEATURE(ecx, CPUID_FEATURE_SSSE3, REMAKE_CPU_SSSE3);			// SSSE3
	CHECK_FEATURE(ecx, CPUID_FEATURE_SSE4_1, REMAKE_CPU_SSE4_1);		// SSE4.1
	CHECK_FEATURE(ecx, CPUID_FEATURE_SSE4_2, REMAKE_CPU_SSE4_2);		// SSE4.2
	CHECK_FEATURE(ecx, CPUID_FEATURE_AES, REMAKE_CPU_AES);				// AES-NI
	CHECK_FEATURE(ecx, CPUID_FEATURE_FMA, REMAKE_CPU_FMA);				// FMA3

	// Check for AVX support
	if(ecx & (1 << CPUID_FEATURE_AVX) && check_xgetbv_ymm()) {
		cpu_features |= REMAKE_CPU_AVX; // AVX
	}

	// Extended Features Information (EAX = 7, ECX = 0)
	eax = 7;
	ecx = 0;
	__get_cpuid_count(7, 0, &eax, &ebx, &ecx, &edx);
	CHECK_FEATURE(ebx, CPUID_FEATURE_BMI1, REMAKE_CPU_BMI1);				// BMI1
	CHECK_FEATURE(ebx, CPUID_FEATURE_BMI2, REMAKE_CPU_BMI2);				// BMI2
	CHECK_FEATURE(ebx, CPUID_FEATURE_SHA, REMAKE_CPU_SHA);				// SHA Extensions
	CHECK_FEATURE(ebx, CPUID_FEATURE_AVX2, REMAKE_CPU_AVX2);				// AVX2
	CHECK_FEATURE(ebx, CPUID_FEATURE_AVX512F, REMAKE_CPU_AVX512F);		// AVX-512 Foundation

	// Check for AVX2
	if(ebx & (1 << CPUID_FEATURE_AVX2) && check_xgetbv_ymm()) {
		cpu_features |= REMAKE_CPU_AVX2; // AVX2
	}

	state.cpu_features = cpu_features;
}
