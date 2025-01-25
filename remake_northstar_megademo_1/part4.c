/*
 *
 */

static uint32_t p4_scroller_inner_colors[] = {
	0xdd9999ff, 0xcc7799ff, 0xbb5599ff, 0xaa4499ff, 0x993399ff, 0x000000ff,
	0x993399ff, 0xaa4499ff, 0xbb5599ff, 0xcc7799ff, 0xdd9999ff, 0xeecc99ff,
	0xeeee99ff, 0xffff99ff, 0xffff99ff, 0xeeee99ff, 0xeecc99ff,
};

static uint32_t p4_scroller_border_colors[] = {
	0xdd9999ff, 0xcc7799ff, 0xbb5599ff, 0xaa4499ff, 0x993399ff, 0x993399ff, 0xaa4499ff, 0xbb5599ff,
	0xcc7799ff, 0xdd9999ff, 0xeecc99ff, 0xeeee99ff, 0xffff99ff, 0xffff99ff, 0xeeee99ff, 0xeecc99ff,
};

static uint8_t p4_real_inner_color_index;
static uint8_t p4_real_border_color_index;

static uint8_t p4_scroll_text_1[] = {
	"YAWN !! TIME:11:34:14   I'VE BEEN SLEEPING FOR A WHILE !!"
	"!   STARFIRE WAS IN GOTHENBURG ON A ROLE-PLAYING CONFERENCE, TH"
	"AT MEANS I HAVE TO WRITE ALL THESE SCROLLERS ON MY OWN, HE MUST"
	" HAVE PLANNED THIS !!!   MY PARENTS JUST LEFT FOR FOUR DAYS !!!"
	" THAT MEANS  P A R T Y  T I M E  !  !  !  PREZZ LEFT MOUSE BUTT"
	"ON TO CONTINUE !!!      COPYRIGHT NORHT STAR& 1988           !W"
	"ARP!     ^"
};
static uint8_t p4_scroll_text_2[] = {
	"WHY DID I DO THIS STUPID DEMO OR SLOULD I SAY ROUTINE TEST, IT'"
	"S PURE DEATH TO WRITE ALL THESE SCROLLERS.   SO I THINK I'L"
	"L LET STARFIRE DIE WITH ME BUT IT'S TO LATE TO CALL HIM N"
	"OW 02:22:02.   SO I THINK I'LL GO TO SLEEP NOW AND CALL S"
	"TARRY TOMORROW, DON'T YOU THINK THATS A GREAT IDEA ?!?!  "
	"   I THINK SO........             ZZZ  ZZ  Z   !ZZZARP!        "
	"          ^"
};
static uint8_t p4_scroll_text_3[] = {
	" ATOM OF NORTH STAR IS BACK AGAIN !!!! THIS TIME WIT"
	"H A NERVE KILLING SCROLLY DEMO !!!   TIME 02:14:21  I JUST WROT"
	"E THE SCROLLY BELOW STRANGE ORDER ???!??!?  ON THIS DEMO DISK A"
	"RE THERE MORE THAN 40K BYTES OF PURE SCROLL-MESSAGES !!! I THIN"
	"K THAT BEATS BOTH SAM-FOX AND ANTITRAX DEMO !!!   IF IT DOESN'"
	"T, WELL BIG DEAL !!   I'M STARTING TO GET TIRED SO EXCUSE"
	" ME IF I START MAKING SPELLING ERRORS !!!        !WARP!     ^"
};
static uint8_t p4_scroll_text_4[] = {
	"SHIIIIT !!!! WHICH ONE SHALL I READ !??!?!?!?!?  THATS A HARD CHO"
	"SE !!!  HOPE YOU MAKE THE RIGHT ONE !!! ANYWAY THIS NEAT PIECE "
	"OF CODING WAS DONE BY A*T*O*M 1987-03-30    02:04:32 (IN THE NI"
	"GHT)  THIS IS JUST A ROUTINE TEST SO PLEASE DON'T KILL ME"
	" IF YOU DON'T LIKE IT !!!  I DID THE-END PART AN HOUR AGO"
	", YOU MIGHT SPOT SOME SIMILARITIES(EH?) WITH THIS ONE !!! BUT W"
	"HO CARES !!!  HMM.... IT IS REALLY DIFFICULT TO WRITE EIGHT SCR"
	"OLLYS, AND I JUST WROTE THE FOUR SCROLLYS IN THE LAST PART.. SO"
	" I'M QIUTE EMPTY AT THE MOMENT..  TIME 02:09:41 ..... IT'"
	"S GETTING HOTTER EVERY DAY !!! SUMMER WILL SOON BE HERE !!!  TH"
	"AT WILL BE JUST GREAAAAAAAAAAAT !!!!!!  I MEAN TH SUMMER HOLIDA"
	"Y !!!!!!!!!!!!!!!!    !WARP!          ^"
};
static uint8_t p4_scroll_text_5[] = {
	"QUITE A SHOCK ISN'T IT !!!        THIS WASN'T MADE TO BE A DEMO, I WAS JUST "
	"CHECKING OUT HOW GOOD MY NEW SCROLL ROUTINE WAS, AND IT TURNED "
	"OUT TO WORK QUITE GOOD, DON'T YOU THINK. NO RASTER FLICKE"
	"RS HERE !!! I HATE DEMOS WITH MANY SCROLLYS ESPESH THIS ONE !!!"
	" CAUSE IT'S REALLY HARD TO READ ALL THE MESSAGES. I WILL "
	"PROBABLY USE THIS SCROLL-ROUTINE IN THE FUTURE, IT'S REAL"
	"LY FAST !!!   !WARP!       ^"
};
static uint8_t p4_scroll_text_6[] = {
	"THE MUSAX U ARE LISTENING TO WAS CO"
	"MPOSED BY OUR MUSIC-FREAK -* TITAN *-       LOOK OUT FOR OUR FI"
	"RST 64(@@@!) DEMO !!!    IT WILL BE CODED BY ME AND OUR 64-FREA"
	"K -* HEDDA *-    THERE WILL ALSO POP UP SOME CRACKED GAMES FROM"
	" US IN NEAR FUTURE (BOTH ON THE BIG ONE AND ON THE SMALL ONE !!"
	"!!)      [ STINKS !!! (WHY DO I OWN ONE ???!??)      MAYBE BECA"
	"USE WE ARE GOING TO MAKE OUR GAME ON BOTH $ AND [ !!!!  SO, LOO"
	"K OUT FOR OUR SOON COMING GAME !!!          !WARP!             "
	"   ^"
};
static uint8_t p4_scroll_text_7[] = {
	"HAVE YOU GOT IT YET ??!?!??!??!?!?!?!?!?!?!?    GOT WHAT !!"
	"!    OUR ANTI-VIRUS OF COURSE !!!!!!!!   TO CHECK IF IT IS INST"
	"ALLED IN YOUR AMIGA'S MEMORY, JUST PRESS RESET AND HOLD T"
	"HE @ BUTTON DOWN !!! IF THE POWER-LIGHT LAMP STARTS TO RAPP(?!)"
	"  THEN YOU CAN FEEL SAFE !!! NO SCA, NO BYTE BANDIT AND NO HMM,"
	" THERE ISN'T A NAME IN THIS VIRUS !!!    OUR VIRUS WORKS "
	"LIKE THIS,  IF THE DISK YOU INSERT ISN'T A NORMAL INTSALL"
	"ED DISK OR A VIRUS INFECTED ONE, LIKE FOR INSTANCE BARBARIAN OR"
	" A BOOT-INTRO, WILL THE VIRUS NOT PENETRATE THE DISK.  SO YOU C"
	"AN BE SAFE, NOTHING MORE THAN NORMAL INSTALLED DISKS AND VIRUS "
	"INFECTED ONES WILL BE THE TARGETS..... WHAT HAPPENEDS IF I INSE"
	"RTS A WRITE-PROTECTED VIRUS INFECTED DISK ?? I HERE YOU SAY !! "
	" WELL, A WARNING SIGN WILL SHOW UP !!!    THE VIRUS PROTECTS YO"
	"U FROM 3 DIFFERENT VIRUS, IF YOU DISCOVER A NEW ONE, CONTACT US"
	" SO WE CAN UPDATE OUR ANTI-VIRUS !!!!             BE SURE THAT "
	"YOUR MEMORY IS CLEAN (NO VIRUS!) BEFORE INSTALLING THE ANTI-VIR"
	"US  !!!       NOW YOU KNOW IT, OUR VIRUS IS A USER FRIENDLY PIE"
	"CE OF CODING AND WAS MADE JUST FOR DESTROYING SCA, BYTE BANDIT "
	"AND THE OTHER ONE !!!            - VIRUS IS A DISEASE. NORTH ST"
	"AR ANTI-VIRUS IS THE CURE !!!!!!!      !WARP!         ^"
};
static uint8_t p4_scroll_text_8[] = {
	"PHEW !!! THIS IS THE LAST ONE !!!!        I THINK A CALL THIS PART '"
	"SCROLL-READERS HEAVEN' OR MAYBE MORE FITTING 'SCROL"
	"L-READERS HELL' !!!    HEY !! BLACK SHADOW OF FAIRLIGHT, "
	"YOU SHOULD HAVE SENT THE DISKS 3 WEEKS AGO, WHAT'S UP ?!?"
	"! DON'T CARE TO SEND THE T.U.S SOUND DISK !!!      GALLEO"
	"N OF SCIENCE 451, OUR NAME ISN'T THE NORTH STAR (TNS) HED"
	"DA DID A LITTLE MISSTAKE !!!!  N O R T H  S T A R&  OK ?!?     "
	"       FIREBIRD, HURRY UP TO PUBLISH OUR GAME !!  I NEED THE MO"
	"NEY !!!!      MR SPARKY/CANADA OF CPU REX WANTS HIS DISKS   !!!"
	"!          OK, GUESS THATS ALL FOR NOW !!!!     !WARP!         "
	"    ^"
};

static struct scroller_state *p4_scroll_1;
static struct scroller_state *p4_scroll_2;
static struct scroller_state *p4_scroll_3;
static struct scroller_state *p4_scroll_4;
static struct scroller_state *p4_scroll_5;
static struct scroller_state *p4_scroll_6;
static struct scroller_state *p4_scroll_7;
static struct scroller_state *p4_scroll_8;

__attribute__((constructor))
__attribute__((cold, noinline, section(".init_section")))
static void p4_init() {
	p4_scroll_1 = scroller_new(16, 16,  22, 4, p4_scroll_text_1, part1_small_font_data, 0);
	p4_scroll_2 = scroller_new(16, 16,  54, 3, p4_scroll_text_2, part1_small_font_data, 0);
	p4_scroll_3 = scroller_new(16, 16,  86, 2, p4_scroll_text_3, part1_small_font_data, 0);
	p4_scroll_4 = scroller_new(16, 16, 118, 1, p4_scroll_text_4, part1_small_font_data, 0);
	p4_scroll_5 = scroller_new(16, 16, 150, 1, p4_scroll_text_5, part1_small_font_data, 0);
	p4_scroll_6 = scroller_new(16, 16, 182, 2, p4_scroll_text_6, part1_small_font_data, 0);
	p4_scroll_7 = scroller_new(16, 16, 214, 3, p4_scroll_text_7, part1_small_font_data, 0);
	p4_scroll_8 = scroller_new(16, 16, 246, 4, p4_scroll_text_8, part1_small_font_data, 0);
}

static void p4_render_scroll_buffer(struct scroller_state *scr_state) {
	PROFILE_FUNCTION();
	uint32_t *scroll_dest = buffer + scr_state->dest_offset_y * MAIN_BUFFER_WIDTH;
	uint8_t *scroll_src = scr_state->scroll_buffer;
	uint32_t inner_color_count = ARRAY_SIZE(p4_scroller_inner_colors);
	uint32_t border_color_count = ARRAY_SIZE(p4_scroller_border_colors);

	for(size_t i = 0; i < scr_state->char_height; ++i) {
		uint8_t real_inner_color_index = (p4_real_inner_color_index + i) % inner_color_count;
		uint8_t real_border_color_index = (p4_real_border_color_index + i) % border_color_count;
		uint32_t inner_color = p4_scroller_inner_colors[real_inner_color_index];
		uint32_t border_color = p4_scroller_border_colors[real_border_color_index];

		for(size_t j = 0; j < MAIN_BUFFER_WIDTH; ++j) {
			size_t src_index = (scr_state->char_render_offset - 370 + j) & (SCROLL_BUFFER_WIDTH - 1);
			uint8_t color_index = scroll_src[src_index];
			if(color_index == 0) continue;

			switch(color_index) {
				case 1: { scroll_dest[j] = inner_color; } break;
				case 2: { scroll_dest[j] = 0xffffffff; } break;
				case 3: { scroll_dest[j] = border_color; } break;
			}
		}
		scroll_dest += MAIN_BUFFER_WIDTH;
		scroll_src += SCROLL_BUFFER_WIDTH;
	}
}

static bool p4_update()  {
	PROFILE_NAMED("part4 all");

// NOTE(peter): Background logo
	uint32_t atom_offset_x = (MAIN_BUFFER_WIDTH - part4_atom_logo_bg_data->width) >> 1;
	blit_full(part4_atom_logo_bg_data, atom_offset_x, 37);
// NOTE(peter): Scrollers
	scroller(p4_scroll_1);
	scroller(p4_scroll_2);
	scroller(p4_scroll_3);
	scroller(p4_scroll_4);
	scroller(p4_scroll_5);
	scroller(p4_scroll_6);
	scroller(p4_scroll_7);
	scroller(p4_scroll_8);
	p4_render_scroll_buffer(p4_scroll_1);
	p4_render_scroll_buffer(p4_scroll_2);
	p4_render_scroll_buffer(p4_scroll_3);
	p4_render_scroll_buffer(p4_scroll_4);
	p4_render_scroll_buffer(p4_scroll_5);
	p4_render_scroll_buffer(p4_scroll_6);
	p4_render_scroll_buffer(p4_scroll_7);
	p4_render_scroll_buffer(p4_scroll_8);

	p4_real_inner_color_index++;
	p4_real_border_color_index--;

	return (state.mousebuttonstate[0] & !state.mousebuttonprevstate[0]) ? true : false;
}
