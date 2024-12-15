#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;

bool initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	window_width = display_mode.w;
	window_height = display_mode.h;

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS);

	if (!window) {
		fprintf(stderr, "Error creating SDL window.\n");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error creating SDL render.\n");
		return false;
	}
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void paint_color_buffer(uint32_t color) {
	for (int y_axis = 0; y_axis < window_height; y_axis++) {
		for (int x_axis = 0; x_axis < window_width; x_axis++) {
			color_buffer[(window_width * y_axis) + x_axis] = color;
		}
	}
}

void draw_grid(void) {
	for (int y_axis = 0; y_axis < window_height; y_axis += 10) {
		for (int x_axis = 0; x_axis < window_width; x_axis += 10) {
			color_buffer[(window_width * y_axis) + x_axis] = 0xFFFFFFFF;
		}
	}
}

void draw_rect(int x_start, int y_start, int width, int height, uint32_t color) {
	for (int y_axis = y_start; y_axis < height; y_axis++) {
		for (int x_axis = x_start; x_axis < width; x_axis++) {
			color_buffer[(window_width * y_axis) + x_axis] = color;
		}
	}
}

void render_color_buffer() {
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)(window_width * sizeof(uint32_t))
	);
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void draw_pixel(int x_pos, int y_pos, uint32_t pixel_color) {
	if (x_pos < window_width && y_pos < window_height) {
		color_buffer[(window_width * y_pos) + x_pos] = pixel_color;
	}
	else {
		fprintf(stderr, "Not valid position informed for draw_pixel!\n");
	}
}

void destroy_window(void) {
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
