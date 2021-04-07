#include "Window.h"

Window::Window()
	:window(nullptr), renderer(nullptr), font(nullptr), wHeight(0), wWidth(0), wFullScreened(false), wMinimized(false) { }

Window::~Window() {
	free();
}

void Window::free() {

	if (window != nullptr) {
		SDL_DestroyWindow(window);
		window = nullptr;

		SDL_DestroyRenderer(renderer);
		renderer = nullptr;

		TTF_CloseFont(font);
		font = nullptr;

		wHeight = 0;
		wWidth = 0;
		wFullScreened = false;
		wMinimized = false;
	}
}

bool Window::init() {

	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		std::cout << "Enable to initialize SDL, SDL_ERROR : " << SDL_GetError() << std::endl;
		success = false;
	}
	else {

		window = SDL_CreateWindow("HANGMAN", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

		if (window == nullptr) {

			std::cout << "Enable to create window, SDL_ERROR : " << SDL_GetError() << std::endl;
			success = false;
		}
		else {

			wHeight = SCREEN_HEIGHT;
			wWidth = SCREEN_WIDTH;

			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

			if (renderer == nullptr) {
				std::cout << "Enable to create renderer, SDL_ERROR : " << SDL_GetError() << std::endl;
				success = false;
			}
			else {

				SDL_SetRenderDrawColor(renderer, 0, 0, 0xAA, 0xFF);

				int imgFlag = 2;

				if (!(IMG_Init(imgFlag) & imgFlag)) {
					std::cout << "Failed to initialize IMG : " << IMG_GetError() << std::endl;
					success = false;
				}

				if (TTF_Init() == -1)
				{
					std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
					success = false;
				}
			}
		}
	}

	return success;
}

void Window::handle_event(SDL_Event& e) {

	if (e.type == SDL_WINDOWEVENT) {

		switch (e.window.event) {

		case SDL_WINDOWEVENT_RESIZED:
			wWidth = e.window.data1;
			wHeight = e.window.data2;
			SDL_RenderPresent(renderer);
			break;

		case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent(renderer);
			break;

			//Window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			wMinimized = true;
			break;

			//Window maxized
		case SDL_WINDOWEVENT_MAXIMIZED:
			wMinimized = false;
			break;

			//Window restored
		case SDL_WINDOWEVENT_RESTORED:
			wMinimized = false;
			break;
		}
	}
}


