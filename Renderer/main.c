#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

bool is_running = false;

SDL_Window* window;
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

void setup(void) {
	color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
	if (!color_buffer) {
		is_running = false;
	}
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);
}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				is_running = false;
			break;
		default:
			break;
	}
}

void update(void) {
	// TODO:
}

void paint_color_buffer(uint32_t color) {
	for (int y_axis = 0; y_axis < window_height; y_axis++) {
		for (int x_axis = 0; x_axis < window_width; x_axis++) {
			color_buffer[(window_width * y_axis) + x_axis] = color;
		}
	}
}

void draw_grid(void) {
	for (int y_axis = 0; y_axis < window_height; y_axis++) {
		for (int x_axis = 0; x_axis < window_width; x_axis++) {
			if (x_axis % 10 == 0 || y_axis % 10 == 0) {
				color_buffer[(window_width * y_axis) + x_axis] = 0xFFFFFFFF;
			}
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

void render(void) {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	draw_grid();
	render_color_buffer();
	paint_color_buffer(0x00000000);

	SDL_RenderPresent(renderer);
}

void destroy_window(void) {
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(void) {
	
	is_running = initialize_window();

	setup();

	while (is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}