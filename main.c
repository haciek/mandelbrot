#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_COLOR 255

int iter_max = 100;
int width = 640;
int height = 480;
double magnify = 10;

void mandelbrot(SDL_Renderer *renderer);
void SDL_read_input();
void color(int red, int green, int blue);

int main(void) {

	int status = SDL_Init(SDL_INIT_EVERYTHING);
	if (status != 0) {
		perror("SDL_Init\n");
		exit(EXIT_FAILURE);
	}
	SDL_Window *window =
	    SDL_CreateWindow("Mandelbrot", SDL_WINDOWPOS_CENTERED,
			     SDL_WINDOWPOS_CENTERED, 1024, 1024, 0);
	SDL_Renderer *renderer =
	    SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	while (1) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		SDL_read_input();
		mandelbrot(renderer);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}

void mandelbrot(SDL_Renderer *renderer) {
	for (int row = 1; row < height; ++row) {
		/* c_imag = (row - height / 2.0) * 4.0 / height * magnify; */
		double c_imag = ((row / (double)height) - 0.5) / 3.0 * magnify;

		for (int col = 1; col < width; ++col) {
			/* c_real = (col - width / 2.0) * 4.0 / width * magnify;
			 */
			double c_real =
			    ((col / (double)width) - 0.5) / 3.0 * magnify - 0.7;

			double x = 0.0;
			double y = 0.0;
			double x_2 = 0.0;
			double y_2 = 0.0;
			int iter = 1;

			while (x_2 + y_2 <= 100.0 && iter < iter_max) {
				x_2 = pow(x, 2);
				y_2 = pow(y, 2);
				double x_n = x_2 - y_2 + c_real;

				y = 2 * x * y + c_imag;
				x = x_n;
				iter++;
			}

			int color = iter % 255;
			SDL_SetRenderDrawColor(renderer, color, color, color,
					       SDL_ALPHA_OPAQUE);
			SDL_RenderDrawPoint(renderer, col, row);
		}
	}
}

void SDL_read_input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		/* Look for a keypress */
		case SDL_KEYUP:
			/* Check the SDLKey values and move change the coords */
			switch (event.key.keysym.sym) {
			case SDLK_LEFT:
				magnify -= 0.2;
				break;
			case SDLK_RIGHT:
				magnify += 0.2;
				break;
			case SDLK_UP:
				iter_max *= 2;
				break;
			case SDLK_DOWN:
				iter_max /= 2;
				break;
			default:
				SDL_Quit();
				break;
			}
		}
	}
}
