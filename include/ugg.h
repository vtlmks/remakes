#pragma once

// Uncomplicated Generic Graphics
struct ugg {
	uint32_t width;
	uint32_t height;
	uint32_t palette[256];
	uint8_t  data[];
};

