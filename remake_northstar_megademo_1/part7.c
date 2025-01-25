/*
 *
 */

INCBIN(_part7_atom_logo, "data/p7/atom_logo.ugg");
INCBIN(_part7_foetus_logo, "data/p7/foetus_logo.ugg");
INCBIN(_part7_jazze_logo, "data/p7/jazze_logo.ugg");
INCBIN(_part7_northstar_logo, "data/p7/northstar_logo.ugg");
INCBIN(_part7_rex_logo, "data/p7/rex_logo.ugg");
INCBIN(_part7_objects, "data/p7/objects.ugg");
INCBIN(_part7_scroll_font, "data/p7/scroll_font.ugg");

static struct ugg *part7_objects_data = (struct ugg*)_part7_objects_data;
static struct ugg *part7_scroll_font_data = (struct ugg*)_part7_scroll_font_data;

static uint8_t p7_scroll_text[] = {
	"      e  DAVS FANS !!!! WIE GEHTS ?  PRESS RIGHT MOUSE BUTTON TO CHANGE THE "
	"SCROLL SPEED !!! PRESS LEFT TO EXIT THIS DEMO....  WE WANT MORE "
	"MEMBERS IN NORTH-STAR SO IF YOU ARE INTERESTED THEN HERE ARE SOME "
	"BRIEF INSTRUCTIONS: 1.PROGRAMMERS,SEND A DISK WITH A DEMO OR SOME ROUTINES "
	"TO:                    aLOOK BELOW!!  2.MUSICANS SEND SOME INTE RUPT-DRIVEN "
	"TUNES TO ME.    3.GRAPHICANS SEND SOME PICCYS OR A FONT TO ME. "
	"YOU WILL GET A 100 PERCENT ANSWER BACK !!! "
	"ALSO WRITE TO ME IF YOU ARE GOING TO HAVE A CONFERENCE OR SOMETHING "
	"LIKE THAT !!!!!     IF YOU WANT TO SWAP GAMES (ST AND AMIGA!) THEN WRITE OR CALL TO: "
	"b   OK!?          IF YOU ARE A 64-PROGRAMMER OR A SWAPPER THEN "
	"GET IN CONTACT WITHc THE GUYS BELOW !!!    IF YOU ARE GOING TO BUY A "
	"ORIGINAL THEN WAIT UNTIL OUR GAME WILL HIT THE SHOPWARE-SHELFS THE GAME "
	"IS PROGRAMMED BY SINE, GRAPHIX BY ME([ATOM[), "
	"MUZAX AND ALL THE BLIPP BLOPP BY ET IN OTHER WORDS, IT WILL BE GREAT !!!!! "
	"SO YOU'D BETTER BUY IT !!!! IT'S GONNA BE A COMMANDO TYPE OF GAME. "
	"FEATURING: TEAM WORK(TWO PLAYERS ON SCREEN), A LOAD OF PICK UP AND USE "
	"WEAPONS, MANY DIFFERENT LEVELS, EXELLENT GRAPHIX(32 COLORS,OF COURSE), "
	"THE BIGGEST HIGH-SCORE TABLE YOU EVER SEEN !, FAST(AND IMPOSSIBLE TO "
	"CRACK) DISK OPERATING, TANKS, FLYING OBJECTS, SOLDIERS TO KILL, SEAS, "
	"BRIDGES, TOWNS, DJUNGLES, SPACE, GRANATES, LAZERS, FLAME-THROWERS, "
	"MEGA-BLAZTERS, CARS, ALIENS, MINES, SUPER-BIG ALIENS AT THE END OF "
	"THE LEVEL   WELL WE HAVE ONLY PLANNED THIS GAME FOR A WEEK NOW "
	"SO THE FINAL VERSION WILL CONTAIN MUCH,MUCH MORE !!!!! "
	"   AND THIS WILL BE OUR FIRST AMIGA GAME, SO WATCH OUT, IT WILL BLOW "
	"YOUR BRAINS OUT  !!!! "
	"      WELL ENOGH OF THAT!   THIS DEMO WAS CODED BY [ ME UNDER A PERIOD "
	"OF THREE DAYS, IT'S GONNA BE A PART OF OUR FIRST DEMO DISK ! "
	"  WE REALLY NEED MORE MEMBERS TO CONTRIBUTE WITH THE DEMO PROGRAMMING, "
	"CAUSE I HAVE TO DO ALL DEMOS AT THE MOMENT. "
	" I MIGHT WRITE A GAME OF MY OWN LATER IN THE SUMMER-HOLIDAY, IF I'M "
	"NOT GOING TO SUN IN JOGOSLAVIA WITH MY FRIENDS. "
	"  IF THERE IS ANYONE OUT THERE WHO OWN A MONITOR THAT DISPLAYS THE "
	"HARDWARE LABELS THEN SEND IT TO ME !!!    THIS DEMO WAS FINISHED IN "
	"HASTE (THE SIGN SHOULD HAVE SCROLLED IN FROM THE LEFT!) BECAUSE "
	"IT WAS MUCH FUNNIER TO MAKE A KOTO RE-RE MIX, I DON'T KNOW YET "
	"IF THE KOTO MOX DEMO IS FINISHED IN TIME TO JOIN THE OTHER DEMOS ON THIS "
	"DISK, WELL YOU'LL SOON NOTICE !!   IF YOU HAVE ANY COMMENT ABOUT THESE "
	"DEMOS, GIVE ME A CALL! WHAT DO YOU GUYS THINK ABOUT JAZZE'S NORTH STAR SIGN....d"
	"  DID YOU LIKE IT ?? OR DO YOU THINK THIS ONE IS BETTER e, THATS MINE ! ANYWAY"
	" WHO GIVES A SHIT. d HAVE YOU NOTICED THAT THE SCROLL MAKES A LITTLE"
	" JUMP WHEN A SIGN IS BLITTERED OUT ?!? THAT'S REALLY DISCUSTING !!! "
	"BUT I'M TO LAZY TO SPLIT THE CLS AND THE DATA MOVER IN DIFFERENT "
	"FRAMES.   ALL OF THESE DEMOS WAS CODED IN THE METACOMCO ASSEMBLER. "
	"NEXT TIME I MIGHT USE SEKA INSTEAD, BECAUSE THE METACOMPCO IS "
	"QUITE UNREASONABLE WHEN YOU WANT TO JOIN THE PROGRAM WITH THE "
	"GRAPHIX OR DATA SAVED IN ANOTHER FILE!! "
	"   SOMEDAY WILL SINE MAKE A CROSS-INTERFACE BETWEEN MY ST AND "
	"AMIGA SO I CAN USE A CROSS-ASSEMBLER INSTEAD OF THE METACOMPCO "
	"OR THE LOUSY SEKA ASSEMBLER...... d       A SPECIAL HELLO TO GOOF "
	"OF THE ARNE BOYS, NICE PARTY !!! "
	"  BUT WE GOT A LITTLE BIT OF SHOCK WHEN WE ARRIVED. JUST WHEN "
	"WE WAS GOING TO ENTER A GROUP OF POLICES CAME OUT OF THE BUILDING "
	", JAZZE HAD A BAG FULL OF GUESS WHAT!, SO WE TOURNED AROUND AND WENT "
	"AWAY LOOKING VERY SUSPICIOUS. BUT THE POLICES LEFT,AND WE ARRIVED !!!! "
	"         I SAW A INTRO BY THE CHAMPS TODAY WHERE IT STOOD THAT THEY "
	"HAD A MEMBER CALLED ATOM, BUT WHAT THE HECK, THAT ONLY MEANS THAT IT "
	"IS A GOOD NAME, OR WHAT DO YOU THINK ATOM ???! "
	"       ANYWAY, IF YOU HAVE ANY NEW VIDEO FILMS THEN CONTACT REX.b  "
	"          WE WANT SOME SWAPPER PARTNERS IN DDR AND IN HOLLAND IF YOU "
	"THINK YOU ARE THE RIGHT ONE, CONTACT REX. "
	"WE ARE AT THE MOMENT 15 MEMBERS IN NORTH STAR THEY ARE: [ ATOM [ "
	"STARFIRE [ HEDDA [ TITAN [ REX [ FOETUS [ UFO [ RILLE [ MC [ "
	"JAZZE [ GIZMO [ ZOETROPE [ ELECTRO [ LIBERATOR AND CRAB . "
	"       IF YOU WANT TO JOIN OUR FAN-CLUB SEAL GIVE FOETUS A CALL !!!! "
	"   THE FAN CLUB IS GROWING !! OUR FAN-CLUB MEMBERS CONSIST "
	"AT THE MOMENT OF: MY DOG AND THE 'TORPEDO' QWERT !!!! "
	"      OUR VIRUS KILLER WILL SOON BE READY FOR YOUR DISKS !! IT WILL "
	"HANDLE THE SCA-VIRUS AND BYTE-BANDIT VIRUS (FUCK YOU!!) "
	"  IF YOU WANT A VERSION OF IT, SEND A DISK AND SOME STAMPS!! "
	" AND YOU'LL GET THE LATEST VERSION, IF IT'S FINISHED. "
	"IF YOU HAVE A NEW KIND OF VIRUS, SEND A DISK WITH IT TO MEa (AND WE "
	"WILL KILL IT HEHEH!!!!). "
	"THE SCA-VIRUS IS A JOKE BUT THE BYTE BANDIT-VIRUS IS A TOUGH SUCKER !!! "
	"BUT NOT TO TOUGH !!!  WE WILL SOON REALESE A VIRUS-VIRUS HUNTER. "
	"WICH WILL WORK LIKE A VIRUS BUT IT WILL ONLY SAVE ITSELF ON INFECTED AND "
	"INSTALLED DISK, SO IT WON'T HURT !!! "
	" IF YOU HAVE ANY GOOD USER PROGRAMS LIKE DISKTOOLS,MONITORS AND "
	"GRAPHIX PRG THEN SEND THEM TO ME (BUT ONLY IF IT'S NEW AND USEFULL STUFF!) "
	"    A BIG FUCK MUST GO TO LUN IN 'THE LIGHT' EH?d "
	" THAT BASTARD MADE THE MOST DISCUSTING 'DEMO' EVER SEEN ON A "
	"COMPUTER WHATSOEVER AND WROTE THAT HE WAS A MEMBER OF "
	"NORTH STAR !!! THAT WAS REALLY EMBARRASING, SO IF YOU SEE SOME "
	"UGLY LOOKING 'DEMOS' FROM 'US' WELL, THEN YOU KNOW !!!! "
	"     WELL NOW AM I OUT OF WORDS AND I HAVE SOME GRAPHIX TO DO FOR "
	"OUR SOON TO BE REALESED SUPER GAME SO I'D BETTER FINISH HERE !!!! "
	"JAHOOO!! THIS IS STARFIRE. ATOM JUST LEFT THE KEYBOARD ALL ALONE SO I'LL "
	"JUST WRITE A FEW LINES OF TEXT. OOOPS. DID I SAY FEW. I DID NOT MEAN THAT. "
	"I AM SOREEEE. WELL (COPYRIGHT 1986 ROBIN CANDY) HOW DO YOU LIKE THIS "
	"DEMODISK. SWELL AIN'T IT. ATOM MENTIONED THE VIRUS KILLER. I WILL EXPLAIN "
	"HOW IT WORKS (V1.2). IT CONSISTS OF TWO PROGRAMS, ONE IN THE BOOTBLOCK "
	"AND ONE IN THE STARTUP-SEQUENCE. IF THE BOOTBLOCK'S NOT PRESENT THEN A VIRUS "
	"IS ON THE DISK AND THE MAIN PROG WILL WRITE IT OVER. IF THERE IS A SCA-VIRUS "
	"IN THE COMPUTER THEN IT WILL KILL IT AND WRITE A NEW BOOTBLOCK. "
	"IF IT IS A BYTE BANDIT THEN IT WILL HALT THE COMPUTER AND URGE YOU "
	"TO SHUT DOWN. IT IS A TOUGH SUCKER((C) 1988 ATOM) AND I HAVEN'T FIGURED OUT "
	"A GOOD WAY TO KILL IT YET! BUT I WILL......... "
	"  HEJSAN I'M BACK !!  WELL, THE BYTE-BANDIT VIRUS WILL "
	"BE NOTHING MORE THAN HISTORY AFTER A COUPLE OF WEEKS !!! "
	"  I'M STILL OUT OF WORDS SO BYE,BYE !!!!!! "
	"                     ..............WRAPAROUND                    ^ "
};


static uint32_t p7_scroll_color[] = {
	0xff0000ff, 0xff1100ff, 0xff2200ff, 0xff3300ff, 0xff4400ff, 0xff5500ff, 0xff6600ff, 0xff7700ff,
	0xff8800ff, 0xff9900ff, 0xffaa00ff, 0xffbb00ff, 0xffcc00ff, 0xffdd00ff, 0xffee00ff, 0xffff00ff,
	0xeeff00ff, 0xddff00ff, 0xccff00ff, 0xbbff00ff, 0xaaff00ff, 0x99ff00ff, 0x88ff00ff, 0x77ff00ff,
	0x66ff00ff, 0x55ff00ff, 0x44ff00ff, 0x33ff00ff, 0x22ff00ff, 0x11ff00ff, 0x00ff00ff, 0x00ff11ff,
	0x00ff22ff, 0x00ff33ff, 0x00ff44ff, 0x00ff55ff, 0x00ff66ff, 0x00ff77ff, 0x00ff88ff, 0x00ff99ff,
	0x00ffaaff, 0x00ffbbff, 0x00ffccff, 0x00ffddff, 0x00ffeeff, 0x00ffffff, 0x00eeffff, 0x00ddffff,
};

static int32_t md1_p7_stars[188];

static struct scroller_state *p7_scroll;

// a - ATOM - address
// b - REX
// c - FOETUS
// d - NORTH STAR (Jazze logo) Yellow
// e - NORTH STAR  white logo

struct p7_logo {
	struct ugg *image;
	uint32_t logo_y;
};

static struct p7_logo p7_logos[] = {
	{ (struct ugg*)_part7_atom_logo_data, 84 },
	{ (struct ugg*)_part7_rex_logo_data, 105 },
	{ (struct ugg*)_part7_foetus_logo_data, 106 },
	{ (struct ugg*)_part7_jazze_logo_data, 110 },
	{ (struct ugg*)_part7_northstar_logo_data, 88},
};

static uint32_t p7_logo_index = -1;

static uint8_t p7_scroll_callback(struct scroller_state *scr_state, uint8_t scroll_character) {
	switch(scroll_character) {
		case 'a': {						// a - ATOM - address
			p7_logo_index = 0;
			scroll_character = scr_state->text[scr_state->text_offset++];
		} break;
		case 'b': {						// b - REX
			p7_logo_index = 1;
			scroll_character = scr_state->text[scr_state->text_offset++];
		} break;
		case 'c': {						// c - FOETUS
			p7_logo_index = 2;
			scroll_character = scr_state->text[scr_state->text_offset++];
		} break;
		case 'd': {						// d - NORTH STAR (Jazze logo) Yellow
			p7_logo_index = 3;
			scroll_character = scr_state->text[scr_state->text_offset++];
		} break;
		case 'e': {						// e - NORTH STAR  white logo
			p7_logo_index = 4;
			scroll_character = scr_state->text[scr_state->text_offset++];
		} break;

		case ' ':						// fallthrough
		case 'I': {
			scr_state->next_render_offset_override = 18;
		} break;
		case '^': {						// Scrolltext end
			scr_state->text_offset = 0;
			scroll_character = scr_state->text[scr_state->text_offset++];
		} break;
	}
	return scroll_character;
}

__attribute__((constructor))
__attribute__((cold, noinline, section(".init_section")))
static void p7_init() {
	for(uint32_t i = 0; i < 188; ++i) {
		md1_p7_stars[i] = xor_generate_random(&base_rand) & 0x7ff;
	}
	p7_scroll = scroller_new(48, 48, 8, 1, p7_scroll_text, part7_scroll_font_data, p7_scroll_callback);
}

static void p7_render_scroll_buffer(struct scroller_state *scr_state) {
	PROFILE_FUNCTION();
	uint32_t *scroll_dest = buffer + (scr_state->dest_offset_y * MAIN_BUFFER_WIDTH);
	uint8_t *scroll_src = scr_state->scroll_buffer;
	for(size_t i = 0; i < scr_state->char_height; ++i) {
		for(size_t j = 0; j < MAIN_BUFFER_WIDTH; ++j) {
			uint8_t color_index = scroll_src[(scr_state->char_render_offset - 370 + j) & (SCROLL_BUFFER_WIDTH - 1)];
			switch(color_index) {
				case 1: { scroll_dest[j] = p7_scroll_color[i]; } break;
				default: { scroll_dest[j] = scr_state->font->palette[color_index]; } break;
			}
		}
		scroll_dest += MAIN_BUFFER_WIDTH;
		scroll_src += SCROLL_BUFFER_WIDTH;
	}
}

#define P7_OBJECT_WIDTH 48
#define P7_OBJECT_HEIGHT 160
#define P7_OBJECT_COLORS 8

static uint8_t p7_objects_buffer[16*(MAIN_BUFFER_WIDTH+P7_OBJECT_WIDTH)];
static uint8_t p7_item_type = 0;
static uint8_t p7_item_count = 0;
static uint8_t p7_item_scroll_offset = 47;

static void p7_bar_scrollers() {
	PROFILE_FUNCTION();
// 20 objects before change
	if(p7_item_scroll_offset++ == 47) {
		p7_item_scroll_offset = 0;
		uint8_t *src = part7_objects_data->data + (p7_item_type * 16 * 48);
		uint8_t *dst = p7_objects_buffer;
		for(uint32_t y = 0; y < 16; ++y) {
			for(uint32_t x = 0; x < P7_OBJECT_WIDTH; ++x) {
				dst[x] = *src++;
			}
			dst += (MAIN_BUFFER_WIDTH + P7_OBJECT_WIDTH);
		}

		if(p7_item_count++ == 19) {
			p7_item_count = 0;
			if(p7_item_type++ == 9) {
				p7_item_type = 0;
			}
		}
	}
	memmove(p7_objects_buffer + 1, p7_objects_buffer, sizeof(p7_objects_buffer)-1);

// both bars render
	uint32_t *dst0 = buffer + 58 * MAIN_BUFFER_WIDTH;
	uint32_t *dst1 = buffer + 230 * MAIN_BUFFER_WIDTH;
	uint8_t *src = p7_objects_buffer + P7_OBJECT_WIDTH;
	for(uint32_t y = 0; y < 16; ++y) {
		for(uint32_t x = 0; x < MAIN_BUFFER_WIDTH; ++x) {
			uint8_t color_index = src[x];
			if(color_index) {
				dst0[x] = part7_objects_data->palette[color_index];
				dst1[x] = part7_objects_data->palette[color_index];
			}
		}
		src += MAIN_BUFFER_WIDTH + P7_OBJECT_WIDTH;
		dst0 += MAIN_BUFFER_WIDTH;
		dst1 += MAIN_BUFFER_WIDTH;
	}
}

static void p7_show_logos() {
	PROFILE_FUNCTION();
// logo order:
// a - ATOM - address
// b - REX - address
// c - FOETUS - address
// d - NORTH STAR (Jazze logo) Yellow
// e - NORTH STAR (large white logo)
	if(p7_logo_index != -1) {
		uint32_t logo_x = (MAIN_BUFFER_WIDTH - p7_logos[p7_logo_index].image->width) >> 1;
		blit_full(p7_logos[p7_logo_index].image, logo_x, p7_logos[p7_logo_index].logo_y);
	}
}

static void p7_render_stars() {
	uint32_t *dst = buffer + 58 * MAIN_BUFFER_WIDTH + 8;
	uint32_t star_add = (state.frame_number & 0x1);
	for(uint32_t i = 0; i < 188; ++i) {
		int32_t x = md1_p7_stars[i] & 0x7ff;
		if((x >= 0) && (x < 336)) {
			dst[x] = 0xffffffff;
		}
		dst += MAIN_BUFFER_WIDTH;
		md1_p7_stars[i] += star_add;
	}
}

static bool p7_update()  {
	PROFILE_NAMED("part7 all");

	if(state.mousebuttonstate[1] & !state.mousebuttonprevstate[1]) {
		p7_scroll->speed++;
		if(p7_scroll->speed == 8) {
			p7_scroll->speed = 1;
		}
	}

	p7_render_stars();
	scroller(p7_scroll);
	p7_render_scroll_buffer(p7_scroll);
	p7_bar_scrollers();
	p7_show_logos();

	return (state.mousebuttonstate[0] & !state.mousebuttonprevstate[0]) ? true : false;
}

