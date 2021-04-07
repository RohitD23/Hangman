#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

class Window {
public:
	Window();
	~Window();

	static const int SCREEN_WIDTH = 1300;
	static const int SCREEN_HEIGHT = 800;

	//Create window and Renderer
	bool init();

	//free data
	void free();

	//void handle event
	void handle_event(SDL_Event& e);

	int Get_Height() { return wHeight; }
	int Get_Width() { return wWidth; }

	bool Get_Minimized() { return wMinimized; }

	bool Get_FullScreened() { return wFullScreened; }
	void Set_FullScreened(bool wFullScreened) { this->wFullScreened = wFullScreened; }

	SDL_Renderer* Get_Renderer() { return renderer; }

	SDL_Window* Get_Window() { return window; }

	TTF_Font** Get_Font() { return &font; }

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;

	int wHeight;
	int wWidth;

	bool wFullScreened;
	bool wMinimized;
};
