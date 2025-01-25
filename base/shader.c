#include "shader.h"

//==============================================================
//
//                          CPU CODE
//
//==============================================================
//              TONAL CONTROL CONSTANT GENERATION
//--------------------------------------------------------------
// Make sure to use same CRTS_MASK_* defines on CPU and GPU!!!!!
//==============================================================
/*
 * dst - Output 4 float array.
 *
 * contrast - Increase contrast, ranges from,
 *              1.0 = no change
 *              2.0 = very strong contrast (over 2.0 for even more)
 *
 * saturation - Increase saturation, ranges from,
 *                0.0 = no change
 *                1.0 = increased saturation (over 1.0 for even more)
 *
 * thin, mask - Inputs shared between CrtsTone() and CrtsFilter()
 *
 */
static void CrtsTone(float * restrict dst, float contrast, float saturation, float thin, float mask) {
//--------------------------------------------------------------
#ifdef CRTS_MASK_NONE
	mask = 1.0f;
#endif
//--------------------------------------------------------------
#ifdef CRTS_MASK_GRILLE_LITE
	// Normal R mask is {1.0,mask,mask}
	// LITE   R mask is {mask,1.0,1.0}
	mask = 0.5f + mask * 0.5f;
#endif
//--------------------------------------------------------------
	float midOut = 0.18f / ((1.5f - thin) * (0.5f * mask + 0.5f));
	float pMidIn = powf(0.18f, contrast);
	dst[0] = contrast;
	dst[1] = ((-pMidIn) + midOut) / ((1.0f - pMidIn) * midOut);
	dst[2] = ((-pMidIn) * midOut + pMidIn) / (midOut * (-pMidIn) + midOut);
	dst[3] = contrast + saturation;
}