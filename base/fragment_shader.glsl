// Input from vertex shader
in vec2 frag_texture_coord;

// Uniform variables
uniform vec2 resolution;
uniform vec2 src_image_size;
uniform float brightness;
uniform vec4 tone_data;
uniform int crt_emulation;
uniform sampler2D iChannel0;

out vec4 frag_color;

const float PI2 = 6.28318530717958;
const float HALF = 0.5;

#define CrtsRcpF1(x) (1.0 / (x))
#define CrtsSatF1(x) clamp((x), 0.0, 1.0)

vec3 CrtsFetch(vec2 uv) {
	return texture(iChannel0, uv).rgb;
}

float CrtsMax3F1(float a, float b, float c) {
	return max(a, max(b, c));
}

vec3 CrtsMask(vec2 pos, float dark) {
#ifdef CRTS_MASK_GRILLE
	vec3 m = vec3(dark);
	float x = fract(pos.x * (1.0 / 3.0));
	m.r = (x < (1.0 / 3.0)) ? 1.0 : dark;
	m.g = (x >= (1.0 / 3.0) && x < (2.0 / 3.0)) ? 1.0 : dark;
	m.b = (x >= (2.0 / 3.0)) ? 1.0 : dark;
	return m;
#endif

#ifdef CRTS_MASK_GRILLE_LITE
	vec3 m = vec3(1.0);
	float x = fract(pos.x * (1.0 / 3.0));
	m.r = (x < (1.0 / 3.0)) ? dark : 1.0;
	m.g = (x >= (1.0 / 3.0) && x < (2.0 / 3.0)) ? dark : 1.0;
	m.b = (x >= (2.0 / 3.0)) ? dark : 1.0;
	return m;
#endif

#ifdef CRTS_MASK_NONE
	return vec3(1.0);
#endif

#ifdef CRTS_MASK_SHADOW
	pos.x += pos.y * 3.0;
	vec3 m = vec3(dark);
	float x = fract(pos.x * (1.0 / 6.0));
	m.r = (x < (1.0 / 3.0)) ? 1.0 : dark;
	m.g = (x >= (1.0 / 3.0) && x < (2.0 / 3.0)) ? 1.0 : dark;
	m.b = (x >= (2.0 / 3.0)) ? 1.0 : dark;
	return m;
#endif

	// Default case if none of the above is defined
	return vec3(1.0);
}

vec3 CrtsFilter(vec2 ipos, vec2 inputSizeDivOutputSize, vec2 halfInputSize, vec2 rcpInputSize, vec2 rcpOutputSize, vec2 twoDivOutputSize, float inputHeight, vec2 warp, float thin, float blur, float mask, vec4 tone) {
	vec2 pos = ipos * twoDivOutputSize - vec2(1.0);
	pos *= vec2(1.0 + (pos.y * pos.y) * warp.x, 1.0 + (pos.x * pos.x) * warp.y);
	float vin = 1.0 - ((1.0 - CrtsSatF1(pos.x * pos.x)) * (1.0 - CrtsSatF1(pos.y * pos.y)));
	vin = CrtsSatF1((-vin) * inputHeight + inputHeight);
	pos = pos * halfInputSize + halfInputSize;

	float y0 = floor(pos.y - 0.5) + 0.5;
	float x0 = floor(pos.x - 1.5) + 0.5;
	vec2 p = vec2(x0 * rcpInputSize.x, y0 * rcpInputSize.y);

	vec3 colA[4], colB[4];
	for(int i = 0; i < 4; i++) {
		colA[i] = CrtsFetch(p);
		p.x += rcpInputSize.x;
	}
	p.y += rcpInputSize.y;
	for(int i = 3; i >= 0; i--) {
		p.x -= rcpInputSize.x;
		colB[i] = CrtsFetch(p);
	}

	float off = pos.y - y0;
	float scanA = cos(min(HALF, off * thin) * PI2) * HALF + HALF;
	float scanB = cos(min(HALF, (-off) * thin + thin) * PI2) * HALF + HALF;

	float off0 = pos.x - x0;
	float pix[4];
	for(int i = 0; i < 4; i++) {
		float diff = off0 - float(i);
		pix[i] = exp2(blur * diff * diff);
	}
	float pixT = CrtsRcpF1(pix[0] + pix[1] + pix[2] + pix[3]);

#ifdef CRTS_WARP
	pixT *= vin;
#endif

	scanA *= pixT;
	scanB *= pixT;

	vec3 color = (colA[0] * pix[0] + colA[1] * pix[1] + colA[2] * pix[2] + colA[3] * pix[3]) * scanA + (colB[0] * pix[0] + colB[1] * pix[1] + colB[2] * pix[2] + colB[3] * pix[3]) * scanB;
	color *= CrtsMask(ipos, mask);

#ifdef CRTS_TONE
	float peak = max(1.0 / (256.0 * 65536.0), CrtsMax3F1(color.r, color.g, color.b));
	vec3 ratio = color * CrtsRcpF1(peak);
#ifdef CRTS_CONTRAST
	peak = pow(peak, tone.x);
#endif
	peak = peak * CrtsRcpF1(peak * tone.y + tone.z);
#ifdef CRTS_SATURATION
	ratio = pow(ratio, vec3(tone.w));
#endif
	return ratio * peak;
#else
	return color;
#endif
}

void main() {
	vec2 fragCoord = vec2(frag_texture_coord.x, 1.0 - frag_texture_coord.y);
	fragCoord += 0.0001;
	if(crt_emulation != 0) {
		vec3 color = CrtsFilter(
			fragCoord * resolution,
			src_image_size / resolution,
			src_image_size * 0.5,
			1.0 / src_image_size,
			1.0 / resolution,
			2.0 / resolution,
			src_image_size.y,
			vec2(1.0 / 24.0, 1.0 / 16.0), // warp value
			INPUT_THIN,
			INPUT_BLUR,
			INPUT_MASK,
			tone_data
		);

		color *= brightness;

		// Remap the range [bias, 1.0] to [0.0, 1.0] linearly
		const float bias = 0.002;
		color = bias + color * (1.0 - bias);

		// Ensure no values fall below zero (important in case the subtraction went negative)
		color = max(color, vec3(0.0));

		frag_color = vec4(color, 1.0);
	} else {
		frag_color = vec4(texture(iChannel0, fragCoord).rgb, 1.0);
	}
}

