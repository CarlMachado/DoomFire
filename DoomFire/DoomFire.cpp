#include <iostream>
#include <time.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_FONT *mainFont = NULL;
ALLEGRO_FONT *secFont = NULL;

#define FPS 60

#define FIRE_WIDTH 40
#define FIRE_HEIGHT 40

#define TILE 20

int *firePixelArray;
bool exitProgram = false;
bool debug = false;
bool proceed = false;

using namespace std;

void init() {
	al_init();

	display = al_create_display(TILE * FIRE_WIDTH, TILE * FIRE_HEIGHT);
	queue = al_create_event_queue();
	timer = al_create_timer(0.05);

	al_install_keyboard();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_keyboard_event_source());

	mainFont = al_load_font("font.ttf", TILE * 0.50, NULL);
	secFont = al_load_font("font.ttf", TILE * 0.20, NULL);

	al_start_timer(timer);
}

void finalize() {
	al_destroy_display(display);
	al_destroy_event_queue(queue);
	al_destroy_timer(timer);
	al_destroy_font(mainFont);
	al_destroy_font(secFont);
}

void input() {
	if (!al_is_event_queue_empty(queue)) {
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			exitProgram = true;
		}
		else if (event.type == ALLEGRO_EVENT_TIMER) {
			proceed = true;
		}
		else if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
			switch (event.keyboard.keycode)
			{
			case ALLEGRO_KEY_RIGHT:
				break;
			case ALLEGRO_KEY_LEFT:
				break;
			case ALLEGRO_KEY_UP:
				break;
			case ALLEGRO_KEY_DOWN:
				break;
			case ALLEGRO_KEY_D:
				if (!debug) debug = true;
				else debug = false;
				break;
			default:
				break;
			}
		}
	}
}

void createFireDataStructure() {
	int numOfPixels = FIRE_WIDTH * FIRE_HEIGHT;
	
	firePixelArray = new int[numOfPixels];

	for (size_t i = 0; i < numOfPixels; i++) {
		firePixelArray[i] = 0;
	}
}

void createFireSource() {
	for (size_t colum = 0; colum < FIRE_WIDTH; colum++) {
		int overflowPixelIndex = FIRE_WIDTH * FIRE_HEIGHT;
		int pixelIndex = (overflowPixelIndex - FIRE_WIDTH) + colum;

		firePixelArray[pixelIndex] = 36;
	}
}

void updateFireIntensityPerPixel(int currentPixelIndex) {
	int belowPixelIndex = currentPixelIndex + FIRE_WIDTH;

	if (belowPixelIndex >= FIRE_WIDTH * FIRE_HEIGHT) return;

	int decay = rand() % 3;
	int belowPixelFireIntensity = firePixelArray[belowPixelIndex];
	int newFireIntensyty = belowPixelFireIntensity - decay >= 0? belowPixelFireIntensity - decay : 0;

	firePixelArray[currentPixelIndex - decay] = newFireIntensyty;
}

void calculateFirePropagation() {
	for (size_t colum = 0; colum < FIRE_HEIGHT; colum++) {
		for (size_t row = 0; row < FIRE_WIDTH; row++) {
			int pixelIndex = colum + (FIRE_WIDTH * row);
			updateFireIntensityPerPixel(pixelIndex);
		}
	}
}

void renderFire() {
	int x = 0, y = 0;

	al_clear_to_color(al_map_rgb(0, 0, 0));
	for (size_t row = 0; row < FIRE_HEIGHT; row++) {
		for (size_t colum = 0; colum < FIRE_WIDTH; colum++) {

			int pixelIndex = colum + (FIRE_WIDTH * row);
			int fireIntensity = firePixelArray[pixelIndex];

			if (debug) {
				int txWidth = al_get_text_width(mainFont, "0");
				al_draw_filled_rectangle(x, y, x + TILE, y + TILE, al_map_rgb(255, 255, 255));
				al_draw_textf(mainFont, al_map_rgb(0, 0, 0), x + TILE / 2 - txWidth / 2, y + TILE * 0.25, NULL, "%d", fireIntensity);
				al_draw_textf(secFont, al_map_rgb(150, 150, 150), x + TILE * 0.7, y, NULL, "%d", pixelIndex);
			}
			else {
				ALLEGRO_COLOR fireColorsPalette[] =
				{
					al_map_rgb(7, 7, 7),
					al_map_rgb(31, 7, 7),
					al_map_rgb(47, 15, 7),
					al_map_rgb(71, 15, 7),
					al_map_rgb(87, 23, 7),
					al_map_rgb(103, 31, 7),
					al_map_rgb(119, 31, 7),
					al_map_rgb(143, 39, 7),
					al_map_rgb(159, 47, 7),
					al_map_rgb(175, 63, 7),
					al_map_rgb(191, 71, 7),
					al_map_rgb(199, 71, 7),
					al_map_rgb(223, 79, 7),
					al_map_rgb(223, 87, 7),
					al_map_rgb(223, 87, 7),
					al_map_rgb(215, 95, 7),
					al_map_rgb(215, 95, 7),
					al_map_rgb(215, 103, 15),
					al_map_rgb(207, 111, 15),
					al_map_rgb(207, 119, 15),
					al_map_rgb(207, 127, 15),
					al_map_rgb(207, 135, 23),
					al_map_rgb(199, 135, 23),
					al_map_rgb(199, 143, 23),
					al_map_rgb(199, 151, 31),
					al_map_rgb(191, 159, 31),
					al_map_rgb(191, 159, 31),
					al_map_rgb(191, 167, 39),
					al_map_rgb(191, 167, 39),
					al_map_rgb(191, 175, 47),
					al_map_rgb(183, 175, 47),
					al_map_rgb(183, 183, 47),
					al_map_rgb(183, 183, 55),
					al_map_rgb(207, 207, 111),
					al_map_rgb(223, 223, 159),
					al_map_rgb(239, 239, 199),
					al_map_rgb(255, 255, 255)
				};
				ALLEGRO_COLOR color = fireColorsPalette[fireIntensity];
				al_draw_filled_rectangle(x, y, x + TILE, y + TILE, color);
			}
			x += TILE;
		}
		y += TILE;
		x = 0;
	}
	al_flip_display();
}

int main() {
	srand(time(NULL));
	init();
	createFireDataStructure();
	while (!exitProgram) {
		input();
		if (proceed) {
			createFireSource();
			calculateFirePropagation();
			renderFire();
			proceed = false;
		}
	}
	finalize();
}