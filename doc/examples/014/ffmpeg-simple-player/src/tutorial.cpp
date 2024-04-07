#include <SDL2/SDL.h>

#include <iostream>

int main() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window * w = SDL_CreateWindow("tutorial", 100, 100, 800, 700, SDL_WINDOW_SHOWN);
	SDL_Surface * surface = SDL_GetWindowSurface(w);
	SDL_Surface * cat = SDL_LoadBMP("a.bmp");
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;

	SDL_Event e;
	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (SDL_QUIT == e.type  ) {
				quit = true;
			} else if (e.type == SDL_MOUSEMOTION) {
				rect.x = e.motion.x - cat->w / 2;
				rect.y = e.motion.y - cat->h / 2;

				printf("%d, %d\n", rect.x, rect.y);

				SDL_FillRect(surface, NULL, 0);
				SDL_BlitSurface(cat, NULL, surface, &rect);
				SDL_UpdateWindowSurface(w);
			}
		}
	}

	return 0;
}