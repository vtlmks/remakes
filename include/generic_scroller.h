
#define SCROLL_BUFFER_WIDTH 512 // Fixed buffer width for wrapping

struct scroller_state {
	uint8_t (*process_char)(struct scroller_state *scr_state, uint8_t scroll_character); // Callback for custom behavior
	uint8_t *scroll_buffer;   // Pointer to the scroll buffer
	struct ugg *font;         // Font pointer
	uint8_t *text;               // Scroll text
	uint32_t dest_offset_y;   // Offset on screen
	uint32_t char_render_offset;  // Offset from start of buffer
	uint32_t char_next_render_offset; // Next time we render a character to buffer
	uint32_t next_render_offset_override;	// IF we need to override the width of the current character.
	uint32_t text_offset;     // Offset in scrolltext
	uint32_t pause_timer;     // If not 0, it will count down to zero
	uint32_t speed;           // The speed of the scroller
	uint32_t char_width;      // Width of each character
	uint32_t char_height;     // Height of each character
};

static struct scroller_state *scroller_new(uint32_t char_width, uint32_t char_height, uint32_t offset_y, uint32_t speed, uint8_t *text, struct ugg *font, uint8_t (*process_char)(struct scroller_state *, uint8_t));
static void scroller(struct scroller_state *scr_state);

// TODO(peter): Add #ifdef GENERIC_SCROLLER_IMPLEMENTATION

static uint8_t default_process_char(struct scroller_state *scr_state, uint8_t scroll_character) {
	if(scroll_character == '^') {		// Scrolltext end
		scr_state->text_offset = 0;
		return scr_state->text[scr_state->text_offset++];
	}
	return scroll_character;
}

__attribute__((cold, noinline, section(".init_section")))
static struct scroller_state *scroller_new(uint32_t char_width, uint32_t char_height, uint32_t offset_y, uint32_t speed, uint8_t *text, struct ugg *font, uint8_t (*process_char)(struct scroller_state *, uint8_t)) {
	struct scroller_state *scr_state = calloc(1, sizeof(struct scroller_state));
	scr_state->char_width = char_width;
	scr_state->next_render_offset_override = char_width;	// The default width
	scr_state->char_height = char_height;
	scr_state->scroll_buffer = calloc(SCROLL_BUFFER_WIDTH * char_height, sizeof(uint8_t));
	scr_state->speed = speed;
	scr_state->font = font;
	scr_state->text = text;
	scr_state->dest_offset_y = offset_y;
	scr_state->process_char = process_char ? process_char : default_process_char;
	return scr_state;
}

static void scroller(struct scroller_state *scr_state) {
	PROFILE_FUNCTION();
	if(scr_state->pause_timer) {
		scr_state->pause_timer--;
	} else {
		while(scr_state->char_render_offset >= scr_state->char_next_render_offset) {
			uint8_t char_index = scr_state->process_char(scr_state, scr_state->text[scr_state->text_offset++]);

			size_t font_offset = scr_state->char_width * scr_state->char_height * (char_index - ' ');
			uint8_t *font_src = scr_state->font->data + font_offset;

			uint8_t *dst = scr_state->scroll_buffer;
			size_t dst_offset = scr_state->char_next_render_offset;
			for(size_t i = 0; i < scr_state->char_height; ++i) {
				for(size_t j = 0; j < scr_state->char_width; ++j) {
					dst[(dst_offset + j) & (SCROLL_BUFFER_WIDTH - 1)] = *font_src++;
				}
				dst += 512;
			}
			scr_state->char_next_render_offset += scr_state->next_render_offset_override;
			scr_state->next_render_offset_override = scr_state->char_width;
		}
		scr_state->char_render_offset += scr_state->speed;
	}
}
