#include "display.h"
#include "vector.h"
#define N_POINTS (9 * 9 * 9)

vec3_t cube_points[N_POINTS]; // 9*9*9 cube
vec2_t projected_points[N_POINTS];

float fov_factor = 128;

bool is_running = false;

void setup(void) {
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);
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

	int point_count = 0;

	for (float x = -1; x <= 1; x += 0.25) {
		for (float y = -1; y <= 1; y += 0.25) {
			for (float z = -1; z <= 1; z += 0.25) {
				vec3_t new_point = { .x_pos = x, .y_pos = y, .z_pos = z };
				cube_points[point_count++] = new_point;
			}
		}
	}
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

vec2_t project(vec3_t point) {
	vec2_t projected_point = {
		.x_pos = (fov_factor * point.x_pos),
		.y_pos = (fov_factor * point.y_pos)
	};

	return projected_point;
}

void update(void) {
	for (int i = 0; i < N_POINTS; i++) {
		vec3_t point = cube_points[i];

		vec2_t projected_point = project(point);

		projected_points[i] = projected_point;
	}
}

void render(void) {
	draw_grid();
	
	for (int i = 0; i < N_POINTS; i++) {
		vec2_t projected_point = projected_points[i];
		draw_rect(
			projected_point.x_pos + (window_width / 2), 
			projected_point.y_pos + (window_height / 2),
			4,
			4,
			0xFFFF0066);
	}
	
	render_color_buffer();
	paint_color_buffer(0x00000000);

	SDL_RenderPresent(renderer);
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